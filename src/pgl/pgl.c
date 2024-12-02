
#include "pgl.h"
#include "pgl_queue.h"

#define AMBIENT_COEF 0.2f
#define DIFFUSE_COEF 0.8f
#define SHADE_2_POWER 8u

#define PGL_DEPTH_NEAR 0x00u
#define PGL_DEPTH_FAR  0xFFu

#define PGL_DEPTH_CLEAR_TO_NEAR ((PGL_DEPTH_NEAR << 24) | (PGL_DEPTH_NEAR << 16) | (PGL_DEPTH_NEAR << 8) | PGL_DEPTH_NEAR)
#define PGL_DEPTH_CLEAR_TO_FAR  ((PGL_DEPTH_FAR  << 24) | (PGL_DEPTH_FAR  << 16) | (PGL_DEPTH_FAR  << 8) | PGL_DEPTH_FAR)

typedef struct {
    // Buffers
    u16  color_buffer[DEVICE_SCREEN_HEIGHT][DEVICE_SCREEN_WIDTH];       // 16 bits per pixel
    u8   depth_buffer[DEVICE_SCREEN_HEIGHT][DEVICE_SCREEN_WIDTH];       //  8 bits per pixel
    u8 stencil_buffer[DEVICE_SCREEN_HEIGHT][DEVICE_SCREEN_WIDTH / 8];   //  1 bit  per pixel

    // Transformation matrices
    mat4f view;
    mat4f projection;
    mat4f viewport;

    // Cache values for fast execution
    f32 near;
    f32 far;
    f32 sin_half_fovw;
    f32 cos_half_fovw;
    f32 sin_half_fovh;
    f32 cos_half_fovh;
    f32 depth_coef;

    // Internal state
    u32 shade_multp; 
    u16 wired_color;
    u16 clear_color;
    union {
        u16 fill_color; // used for painting the mesh when 'mesh_t.filled_render' == FILLED_RENDER_SINGLE_COLOR or 'mesh_t.filled_render' == FILLED_RENDER_COLORS
        u16 tex_index;  // used for painting the mesh when 'mesh_t.filled_render' == FILLED_RENDER_TEX_COORDS
    };

    pgl_enable_t    depth_test_enabled          : 1;
    pgl_mask_t      depth_test_mask             : 1;
    pgl_test_func_t depth_test_func             : 3;

    pgl_enable_t     stencil_test_enabled       : 1;
    pgl_mask_t       stencil_test_mask          : 1;
    pgl_test_func_t  stencil_test_func          : 3;
    pgl_stencil_op_t stencil_test_op_sfail      : 1;
    pgl_stencil_op_t stencil_test_op_dpfail     : 1;
    pgl_stencil_op_t stencil_test_op_dppass     : 1;

    pgl_cull_face_t          cull_face          : 2;
    pgl_cull_winding_order_t cull_winding_order : 1;
} pgl_t;

static pgl_t pgl = {
    .color_buffer   = {{PGLM_RGB565_BLACK}},
    .depth_buffer   = {{PGL_DEPTH_FAR}},
    .stencil_buffer = {{0x00u}},
    .clear_color    = PGLM_RGB565_BLACK,

    .depth_test_enabled = PGL_TEST_DISABLED,
    .depth_test_mask    = PGL_MASKED,
    .depth_test_func    = PGL_LESS,

    .stencil_test_enabled   = PGL_TEST_DISABLED,
    .stencil_test_mask      = PGL_MASKED,
    .stencil_test_func      = PGL_NEVER,
    .stencil_test_op_sfail  = PGL_KEEP,
    .stencil_test_op_dpfail = PGL_KEEP,
    .stencil_test_op_dppass = PGL_KEEP,

    .cull_face          = PGL_CULL_NO,
    .cull_winding_order = PGL_CCW,
};

typedef struct {
    vec2i position;
    vec2f tex_coord;
    f32 depth_inv;
} fragment_t;

// --------------------------------------------------- INTERNAL STATE --------------------------------------------------- // 

void pgl_clear_color(u16 color) {
    pgl.clear_color = color;
}

void pgl_clear(pgl_buffer_bit_t buffer_bits) {
    if (buffer_bits & PGL_COLOR_BUFFER_BIT) {
        const u32 clear_value = (pgl.clear_color << 16) | pgl.clear_color;
        u32* ptr = (u32*)pgl.color_buffer;
        for (u32 i = 0; i < (DEVICE_SCREEN_HEIGHT * DEVICE_SCREEN_WIDTH / 2); i++) {
            ptr[i] = clear_value;
        }
    }
    if (buffer_bits & PGL_DEPTH_BUFFER_BIT) {
        const u32 clear_value = (pgl.depth_test_func & PGL_LESS) ? PGL_DEPTH_CLEAR_TO_FAR : PGL_DEPTH_CLEAR_TO_NEAR;
        u32* ptr = (u32*)pgl.depth_buffer;
        for (u32 i = 0; i < (DEVICE_SCREEN_HEIGHT * DEVICE_SCREEN_WIDTH / 4); i++) {
            ptr[i] = clear_value;
        }
    }
    if (buffer_bits & PGL_STENCIL_BUFFER_BIT) {
        const u32 clear_value = 0x00000000u;
        u32* ptr = (u32*)pgl.stencil_buffer;
        for (u32 i = 0; i < (DEVICE_SCREEN_HEIGHT * DEVICE_SCREEN_WIDTH / 32); i++) {
            ptr[i] = clear_value;
        }
    }
}

void pgl_set_tests(pgl_test_t tests, pgl_enable_t enabled) {
    if (tests ^ PGL_DEPTH_TEST   == 0) { pgl.depth_test_enabled   = enabled; }
    if (tests ^ PGL_STENCIL_TEST == 0) { pgl.stencil_test_enabled = enabled; }
}

void pgl_depth_mask(pgl_mask_t mask) {
    pgl.depth_test_mask = mask;
}

void pgl_depth_func(pgl_test_func_t test_func) {
    pgl.depth_test_func = test_func;
}

static bool passed_depth_test(u32 x, u32 y, u8 depth) {
    if (!pgl.depth_test_enabled) { return true; }
    const u8 depth_in_buffer = pgl.depth_buffer[y][x];

    switch (pgl.depth_test_func) {
        case PGL_NEVER    : return false;  
        case PGL_EQUAL    : return depth == depth_in_buffer;   
        case PGL_LESS     : return depth  < depth_in_buffer;    
        case PGL_LEQUAL   : return depth <= depth_in_buffer;  
        case PGL_GREATER  : return depth  > depth_in_buffer;
        case PGL_GEQUAL   : return depth >= depth_in_buffer;  
        case PGL_NOTEQUAL : return depth != depth_in_buffer;
        case PGL_ALWAYS   : return true;  
    }
}

static void update_depth_buffer(u32 x, u32 y, u8 depth) {
    if (pgl.depth_test_enabled && pgl.depth_test_mask) {
        pgl.depth_buffer[y][x] = depth;
    }
}

void pgl_stencil_mask(pgl_mask_t mask) {
    pgl.stencil_test_mask = mask;
}

void pgl_stencil_func(pgl_test_func_t test_func) {
    pgl.stencil_test_func = test_func;
}

void pgl_stencil_op(pgl_stencil_op_t sfail, pgl_stencil_op_t dpfail, pgl_stencil_op_t dppass) {
    pgl.stencil_test_op_sfail  = sfail;
    pgl.stencil_test_op_dpfail = dpfail;
    pgl.stencil_test_op_dppass = dppass;
}

void pgl_cull_face(pgl_cull_face_t cull_face) {
    pgl.cull_face = cull_face;
}

void pgl_cull_winding_order(pgl_cull_winding_order_t cull_winding_order) {
    pgl.cull_winding_order = cull_winding_order;
}

static bool face_is_culled(vec3f face_normal_in_camera_space, vec3f point_on_face_in_camera_space) {
    const f32 dot = vec3f_dot(face_normal_in_camera_space, point_on_face_in_camera_space);
    return (pgl.cull_winding_order == PGL_CCW && pgl.cull_face & PGL_CULL_BACK  && dot >= 0.0f) || 
           (pgl.cull_winding_order == PGL_CW  && pgl.cull_face & PGL_CULL_BACK  && dot <= 0.0f) ||
           (pgl.cull_winding_order == PGL_CCW && pgl.cull_face & PGL_CULL_FRONT && dot <= 0.0f) || 
           (pgl.cull_winding_order == PGL_CW  && pgl.cull_face & PGL_CULL_FRONT && dot >= 0.0f);
}

// --------------------------------------------------- RASTERIZATION --------------------------------------------------- // 

static void pgl_line(fragment_t f0, fragment_t f1) {
    const i32 dx = abs(f1.position.x - f0.position.x);
    const i32 sx = (f0.position.x < f1.position.x) ? 1 : -1;
    const i32 dy = -abs(f1.position.y - f0.position.y);
    const i32 sy = (f0.position.y < f1.position.y) ? 1 : -1;

    const i32 depth0 = pgl.depth_coef * ((1.0f / f0.depth_inv) - pgl.near);
    const i32 depth1 = pgl.depth_coef * ((1.0f / f1.depth_inv) - pgl.near);

    const i32 d_depth = -abs(depth1 - depth0);
    const i32 s_depth = (depth0 < depth1) ? 1 : -1;

    i32 ey = dx + dy;
    i32 e_depth = dx + d_depth;
    i32 x = f0.position.x;
    i32 y = f0.position.y;
    i32 depth = depth0;

    while (true) {
        if (passed_depth_test(x, y, depth)) {
            pgl.color_buffer[y][x] = pgl.wired_color;
            update_depth_buffer(x, y, depth);
        }

        if (x == f1.position.x && y == f1.position.y) {
            break;
        }

        const i32 two_ey = 2 * ey;
        if (two_ey >= dy) {
            ey += dy;
            x += sx;

            const i32 two_e_depth = 2 * e_depth;
            if (two_e_depth >= d_depth) {
                e_depth += d_depth;
            }
            if (two_e_depth <= dx) {
                e_depth += dx;
                depth += s_depth;
            }
        }
        
        if (two_ey <= dx) {
            ey += dx;
            y += sy;
        }
    }
}

static void pgl_wired_triangle(fragment_t f0, fragment_t f1, fragment_t f2) {
    pgl_line(f0, f1);
    pgl_line(f0, f2);
    pgl_line(f1, f2);
}

static u16 pgl_shader(u16 color) {
    const u16 red   = (pgl.shade_multp * (color & (31 << 11))) >> SHADE_2_POWER;
    const u16 green = (pgl.shade_multp * (color & (63 <<  5))) >> SHADE_2_POWER;
    const u16 blue  = (pgl.shade_multp * (color &  31))        >> SHADE_2_POWER;
    const u16 shaded_color = (red & (31 << 11)) | (green & (63 << 5)) | blue;
    return shaded_color;
}

static void pgl_filled_triangle(fragment_t f0, fragment_t f1, fragment_t f2, filled_render_t filled_render) {
    // Sort fragments with respect to their y coordinates
    if (f0.position.y > f1.position.y) { swap(&f0, &f1); }
    if (f1.position.y > f2.position.y) { swap(&f1, &f2); }
    if (f0.position.y > f1.position.y) { swap(&f0, &f1); }

    vec2i dp1 = vec2i_sub(f1.position,  f0.position);
    vec2f dt1 = vec2f_sub(f1.tex_coord, f0.tex_coord);
    f32   dw1 = f1.depth_inv - f0.depth_inv;

    f32   dx1_step = (dp1.y != 0) ? dp1.x / (f32)abs(dp1.y) : 0.0f;
    vec2f dt1_step = (dp1.y != 0) ? vec2f_scale(dt1, 1.0f / abs(dp1.y)) : vec2f_zero;
    f32   dw1_step = (dp1.y != 0) ? dw1 / (f32)abs(dp1.y) : 0.0f;

    vec2i dp2 = vec2i_sub(f2.position,  f0.position);
    vec2f dt2 = vec2f_sub(f2.tex_coord, f0.tex_coord);
    f32   dw2 = f2.depth_inv - f0.depth_inv;

	f32   dx2_step = (dp2.y != 0) ? dp2.x / (f32)abs(dp2.y) : 0.0f;
    vec2f dt2_step = (dp2.y != 0) ? vec2f_scale(dt2, 1.0f / abs(dp2.y)) : vec2f_zero;
    f32   dw2_step = (dp2.y != 0) ? dw2 / (f32)abs(dp2.y) : 0.0f;

    u16 shaded_color;
    if (filled_render == FILLED_RENDER_SINGLE_COLOR || filled_render == FILLED_RENDER_COLORS) {
        shaded_color = pgl_shader(pgl.fill_color);
    }

	if (dp1.y != 0) {
		for (i32 y = f0.position.y; y <= f1.position.y; y++) {
            const f32 y0_diff = (f32)(y - f0.position.y);

			i32   start_x = f0.position.x + y0_diff * dx1_step;
            vec2f start_t = vec2f_add(f0.tex_coord, vec2f_scale(dt1_step, y0_diff));
            f32   start_w = f0.depth_inv + y0_diff * dw1_step;

            i32   end_x = f0.position.x + y0_diff * dx2_step;
            vec2f end_t = vec2f_add(f0.tex_coord, vec2f_scale(dt2_step, y0_diff));
            f32   end_w = f0.depth_inv + y0_diff * dw2_step;

			if (start_x > end_x) {
				swap(&start_x, &end_x);
				swap(&start_t, &end_t);
                swap(&start_w, &end_w);
			}

			const f32 alpha_step = 1.0f / ((f32)(end_x - start_x));
			f32 alpha = 0.0f;

			for (i32 x = start_x; x < end_x; x++) {
                const f32 w_inv = 1.0f / interp(end_w, start_w, alpha);
                const u8 depth = pgl.depth_coef * (w_inv - pgl.near);

				if (passed_depth_test(x, y, depth)) {
                    if (filled_render == FILLED_RENDER_TEX_COORDS) {
                        const vec2f tex_coord = vec2f_scale(vec2f_interp(end_t, start_t, alpha), w_inv); 
                        const u16 color = texture_sample_vec2f(tex_coord, pgl.tex_index);
                        shaded_color = pgl_shader(color);
                    }
                    pgl.color_buffer[y][x] = shaded_color;
                    update_depth_buffer(x, y, depth);
				}
				alpha += alpha_step;
			}
		}
	}

    dp1 = vec2i_sub(f2.position,  f1.position);
    dt1 = vec2f_sub(f2.tex_coord, f1.tex_coord);
    dw1 = f2.depth_inv - f1.depth_inv;

    dx1_step = (dp1.y != 0) ? dp1.x / (f32)abs(dp1.y) : 0.0f;
    dt1_step = (dp1.y != 0) ? vec2f_scale(dt1, 1.0f / abs(dp1.y)) : vec2f_zero;
    dw1_step = (dp1.y != 0) ? dw1 / (f32)abs(dp1.y) : 0.0f;

	if (dp1.y != 0) {
		for (i32 y = f1.position.y; y <= f2.position.y; y++) {
            const f32 y0_diff = (f32)(y - f0.position.y);
            const f32 y1_diff = (f32)(y - f1.position.y);

			i32   start_x = f1.position.x + y1_diff * dx1_step;
            vec2f start_t = vec2f_add(f1.tex_coord, vec2f_scale(dt1_step, y1_diff));
            f32   start_w = f1.depth_inv + y1_diff * dw1_step;

            i32   end_x = f0.position.x + y0_diff * dx2_step;
            vec2f end_t = vec2f_add(f0.tex_coord, vec2f_scale(dt2_step, y0_diff));
            f32   end_w = f0.depth_inv + y0_diff * dw2_step;

			if (start_x > end_x) {
				swap(&start_x, &end_x);
				swap(&start_t, &end_t);
                swap(&start_w, &end_w);
			}

			const f32 alpha_step = 1.0f / ((f32)(end_x - start_x));
			f32 alpha = 0.0f;

			for (i32 x = start_x; x < end_x; x++) {
				const f32 w_inv = 1.0f / interp(end_w, start_w, alpha);
                const u8 depth = pgl.depth_coef * (w_inv - pgl.near);

				if (passed_depth_test(x, y, depth)) {
                    if (filled_render == FILLED_RENDER_TEX_COORDS) {
                        const vec2f tex_coord = vec2f_scale(vec2f_interp(end_t, start_t, alpha), w_inv); 
                        const u16 color = texture_sample_vec2f(tex_coord, pgl.tex_index);
                        shaded_color = pgl_shader(color);
                    }
                    pgl.color_buffer[y][x] = shaded_color;
                    update_depth_buffer(x, y, depth);
				}
				alpha += alpha_step;
			}
		}	
	}		
}

// --------------------------------------------------- TRANSFORMATIONS --------------------------------------------------- // 

void pgl_view(vec3f position, vec3f right, vec3f up, vec3f forward) {
    pgl.view = view(position, right, up, forward);
}

void pgl_projection(f32 near, f32 far, f32 fovw) {
    const f32 sin_half_fovw = sinf(fovw * 0.5f);
    const f32 cos_half_fovw = cosf(fovw * 0.5f);
    const f32 tan_half_fovh = sin_half_fovw / (cos_half_fovw * DEVICE_SCREEN_ASPECT_RATIO);
    const f32 cos_half_fovh = 1.0f / sqrtf(tan_half_fovh * tan_half_fovh + 1.0f); 
    const f32 sin_half_fovh = tan_half_fovh * cos_half_fovh;

    pgl.sin_half_fovw = sin_half_fovw;
    pgl.cos_half_fovw = cos_half_fovw;
    pgl.sin_half_fovh = sin_half_fovh;
    pgl.cos_half_fovh = cos_half_fovh;
    pgl.depth_coef = 255.0f / (far - near);
    pgl.projection = perspective(fovw, DEVICE_SCREEN_ASPECT_RATIO, near, far);
    pgl.near = near;
    pgl.far = far;
}

void pgl_viewport(i32 x, i32 y, u32 width, u32 height) {
    pgl.viewport = viewport(x, y, width, height);
}

// --------------------------------------------------- CLIPPING --------------------------------------------------- // 

// Clips the triangle with respect to a plane in the clipping space.
// intersection vertices are outputted as v10 and v20.
static void pgl_triangle_clip_plane_intersection(const pgl_queue_triangle_t* t, vec4f clip_plane_vector, pgl_vertex_t* restrict v10, pgl_vertex_t* restrict v20) {
    const f32 d0 = vec4f_dot(t->v0.position, clip_plane_vector);
    const f32 d1 = vec4f_dot(t->v1.position, clip_plane_vector);
    const f32 d2 = vec4f_dot(t->v2.position, clip_plane_vector);

    const f32 alpha10 = d0 / (d0 - d1);
    const f32 alpha20 = d0 / (d0 - d2);

    v10->position  = vec4f_interp(t->v1.position,  t->v0.position,  alpha10);
    v10->tex_coord = vec2f_interp(t->v1.tex_coord, t->v0.tex_coord, alpha10);
    v20->position  = vec4f_interp(t->v2.position,  t->v0.position,  alpha20);
    v20->tex_coord = vec2f_interp(t->v2.tex_coord, t->v0.tex_coord, alpha20);
}

// Returns true if the mesh may be visible.
// Returns false if there is no chance that the mesh is visible. 
static bool pgl_passed_broad_phase_clipping(const mesh_t* mesh, mat4f view_model) {
    const vec3f center = mat3f_mul_vec3f(cast_mat4f_to_mat3f(view_model), mesh->bounding_volume.center);
    const f32 minus_sin_half_fovw = -pgl.sin_half_fovw;
    const f32 cos_half_fovw       =  pgl.cos_half_fovw;
    const f32 minus_sin_half_fovh = -pgl.sin_half_fovh;
    const f32 cos_half_fovh       =  pgl.cos_half_fovh;

    const plane_t left   = {{ cos_half_fovw,           0.0f, minus_sin_half_fovw}, 0.0f};
    const plane_t right  = {{-cos_half_fovw,           0.0f, minus_sin_half_fovw}, 0.0f};
    const plane_t bottom = {{          0.0f,  cos_half_fovh, minus_sin_half_fovh}, 0.0f};
    const plane_t top    = {{          0.0f, -cos_half_fovh, minus_sin_half_fovh}, 0.0f};

    return (plane_signed_distance(left,   center) >= -mesh->bounding_volume.radius &&
            plane_signed_distance(right,  center) >= -mesh->bounding_volume.radius &&
            plane_signed_distance(bottom, center) >= -mesh->bounding_volume.radius &&
            plane_signed_distance(top,    center) >= -mesh->bounding_volume.radius);
}

// Clips the triangle with respect to near, far, left, right, bottom, and top planes.
static i32 pgl_narrow_phase_clipping(pgl_queue_triangle_t* restrict triangle, pgl_queue_triangle_t* restrict subtriangles) {
    pgl_queue_t queue;
    triangle_queue_init(&queue);
    triangle_queue_push(&queue, triangle);

    i32 index = -1;
    pgl_vertex_t v10;
    pgl_vertex_t v20;
    
    // Near Clip: Z + W > 0
    while (!queue.is_empty) {
        pgl_queue_triangle_t* t = triangle_queue_pop(&queue);
        const i32 inside0 = t->v0.position.z > -t->v0.position.w;
        const i32 inside1 = t->v1.position.z > -t->v1.position.w;
        const i32 inside2 = t->v2.position.z > -t->v2.position.w;
        const i32 inside_number = inside0 + inside1 + inside2;

        if (inside_number == 3) {
            subtriangles[++index] = *t;
        }
        else if (inside_number == 2) {
            // Ensure that c0 is the vertex that is not inside
            if      (!inside1) { swap(&t->v0, &t->v1); }
            else if (!inside2) { swap(&t->v0, &t->v2); }

            const vec4f v = (vec4f){0.0f, 0.0f, 1.0f, 1.0f};
            pgl_triangle_clip_plane_intersection(t, v, &v10, &v20);
            subtriangles[++index] = (pgl_queue_triangle_t){v10, t->v1, t->v2};
            subtriangles[++index] = (pgl_queue_triangle_t){v10,   v20, t->v2};
        }
        else if (inside_number == 1) {
            // Ensure that c0 is the vertex that is inside
            if      (inside1) { swap(&t->v0, &t->v1); }
            else if (inside2) { swap(&t->v0, &t->v2); }

            const vec4f v = (vec4f){0.0f, 0.0f, 1.0f, 1.0f};
            pgl_triangle_clip_plane_intersection(t, v, &v10, &v20);
            subtriangles[++index] = (pgl_queue_triangle_t){t->v0, v10, v20};
        }
    }

    while (index >= 0) {
        triangle_queue_push(&queue, subtriangles + index);
        index--;
    }

    // Far Clip: Z - W < 0
    while (!queue.is_empty) {
        pgl_queue_triangle_t* t = triangle_queue_pop(&queue);
        const i32 inside0 = t->v0.position.z < t->v0.position.w;
        const i32 inside1 = t->v1.position.z < t->v1.position.w;
        const i32 inside2 = t->v2.position.z < t->v2.position.w;
        const i32 inside_number = inside0 + inside1 + inside2;

        if (inside_number == 3) {
            subtriangles[++index] = *t;
        }
        else if (inside_number == 2) {
            // Ensure that c0 is the vertex that is not inside
            if      (!inside1) { swap(&t->v0, &t->v1); }
            else if (!inside2) { swap(&t->v0, &t->v2); }

            const vec4f v = (vec4f){0.0f, 0.0f, 1.0f, -1.0f};
            pgl_triangle_clip_plane_intersection(t, v, &v10, &v20);
            subtriangles[++index] = (pgl_queue_triangle_t){v10, t->v1, t->v2};
            subtriangles[++index] = (pgl_queue_triangle_t){v10,   v20, t->v2};
        }
        else if (inside_number == 1) {
            // Ensure that c0 is the vertex that is inside
            if      (inside1) { swap(&t->v0, &t->v1); }
            else if (inside2) { swap(&t->v0, &t->v2); }

            const vec4f v = (vec4f){0.0f, 0.0f, 1.0f, -1.0f};
            pgl_triangle_clip_plane_intersection(t, v, &v10, &v20);
            subtriangles[++index] = (pgl_queue_triangle_t){t->v0, v10, v20};
        }
    }

    while (index >= 0) {
        triangle_queue_push(&queue, subtriangles + index);
        index--;
    }

    // Left Clip: X + W > 0
    while (!queue.is_empty) {
        pgl_queue_triangle_t* t = triangle_queue_pop(&queue);
        const i32 inside0 = t->v0.position.x > -t->v0.position.w;
        const i32 inside1 = t->v1.position.x > -t->v1.position.w;
        const i32 inside2 = t->v2.position.x > -t->v2.position.w;
        const i32 inside_number = inside0 + inside1 + inside2;

        if (inside_number == 3) {
            subtriangles[++index] = *t;
        }
        else if (inside_number == 2) {
            // Ensure that c0 is the vertex that is not inside
            if      (!inside1) { swap(&t->v0, &t->v1); }
            else if (!inside2) { swap(&t->v0, &t->v2); }

            const vec4f v = (vec4f){1.0f, 0.0f, 0.0f, 1.0f};
            pgl_triangle_clip_plane_intersection(t, v, &v10, &v20);
            subtriangles[++index] = (pgl_queue_triangle_t){v10, t->v1, t->v2};
            subtriangles[++index] = (pgl_queue_triangle_t){v10,   v20, t->v2};
        }
        else if (inside_number == 1) {
            // Ensure that c0 is the vertex that is inside
            if      (inside1) { swap(&t->v0, &t->v1); }
            else if (inside2) { swap(&t->v0, &t->v2); }

            const vec4f v = (vec4f){1.0f, 0.0f, 0.0f, 1.0f};
            pgl_triangle_clip_plane_intersection(t, v, &v10, &v20);
            subtriangles[++index] = (pgl_queue_triangle_t){t->v0, v10, v20};
        }
    }

    while (index >= 0) {
        triangle_queue_push(&queue, subtriangles + index);
        index--;
    }

    // Right Clip: X - W < 0
    while (!queue.is_empty) {
        pgl_queue_triangle_t* t = triangle_queue_pop(&queue);
        const i32 inside0 = t->v0.position.x < t->v0.position.w;
        const i32 inside1 = t->v1.position.x < t->v1.position.w;
        const i32 inside2 = t->v2.position.x < t->v2.position.w;
        const i32 inside_number = inside0 + inside1 + inside2;

        if (inside_number == 3) {
            subtriangles[++index] = *t;
        }
        else if (inside_number == 2) {
            // Ensure that c0 is the vertex that is not inside
            if      (!inside1) { swap(&t->v0, &t->v1); }
            else if (!inside2) { swap(&t->v0, &t->v2); }

            const vec4f v = (vec4f){1.0f, 0.0f, 0.0f, -1.0f};
            pgl_triangle_clip_plane_intersection(t, v, &v10, &v20);
            subtriangles[++index] = (pgl_queue_triangle_t){v10, t->v1, t->v2};
            subtriangles[++index] = (pgl_queue_triangle_t){v10,   v20, t->v2};
        }
        else if (inside_number == 1) {
            // Ensure that c0 is the vertex that is inside
            if      (inside1) { swap(&t->v0, &t->v1); }
            else if (inside2) { swap(&t->v0, &t->v2); }

            const vec4f v = (vec4f){1.0f, 0.0f, 0.0f, -1.0f};
            pgl_triangle_clip_plane_intersection(t, v, &v10, &v20);
            subtriangles[++index] = (pgl_queue_triangle_t){t->v0, v10, v20};
        }
    }

    while (index >= 0) {
        triangle_queue_push(&queue, subtriangles + index);
        index--;
    }

    // Bottom Clip: Y + W > 0
    while (!queue.is_empty) {
        pgl_queue_triangle_t* t = triangle_queue_pop(&queue);
        const i32 inside0 = t->v0.position.y > -t->v0.position.w;
        const i32 inside1 = t->v1.position.y > -t->v1.position.w;
        const i32 inside2 = t->v2.position.y > -t->v2.position.w;
        const i32 inside_number = inside0 + inside1 + inside2;

        if (inside_number == 3) {
            subtriangles[++index] = *t;
        }
        else if (inside_number == 2) {
            // Ensure that c0 is the vertex that is not inside
            if      (!inside1) { swap(&t->v0, &t->v1); }
            else if (!inside2) { swap(&t->v0, &t->v2); }

            const vec4f v = (vec4f){0.0f, 1.0f, 0.0f, 1.0f};
            pgl_triangle_clip_plane_intersection(t, v, &v10, &v20);
            subtriangles[++index] = (pgl_queue_triangle_t){v10, t->v1, t->v2};
            subtriangles[++index] = (pgl_queue_triangle_t){v10,   v20, t->v2};
        }
        else if (inside_number == 1) {
            // Ensure that c0 is the vertex that is inside
            if      (inside1) { swap(&t->v0, &t->v1); }
            else if (inside2) { swap(&t->v0, &t->v2); }

            const vec4f v = (vec4f){0.0f, 1.0f, 0.0f, 1.0f};
            pgl_triangle_clip_plane_intersection(t, v, &v10, &v20);
            subtriangles[++index] = (pgl_queue_triangle_t){t->v0, v10, v20};
        }
    }

    while (index >= 0) {
        triangle_queue_push(&queue, subtriangles + index);
        index--;
    }

    // Top Clip: Y - W < 0
    while (!queue.is_empty) {
        pgl_queue_triangle_t* t = triangle_queue_pop(&queue);
        const i32 inside0 = t->v0.position.y < t->v0.position.w;
        const i32 inside1 = t->v1.position.y < t->v1.position.w;
        const i32 inside2 = t->v2.position.y < t->v2.position.w;
        const i32 inside_number = inside0 + inside1 + inside2;

        if (inside_number == 3) {
            subtriangles[++index] = *t;
        }
        else if (inside_number == 2) {
            // Ensure that c0 is the vertex that is not inside
            if      (!inside1) { swap(&t->v0, &t->v1); }
            else if (!inside2) { swap(&t->v0, &t->v2); }

            const vec4f v = (vec4f){0.0f, 1.0f, 0.0f, -1.0f};
            pgl_triangle_clip_plane_intersection(t, v, &v10, &v20);
            subtriangles[++index] = (pgl_queue_triangle_t){v10, t->v1, t->v2};
            subtriangles[++index] = (pgl_queue_triangle_t){v10,   v20, t->v2};
        }
        else if (inside_number == 1) {
            // Ensure that c0 is the vertex that is inside
            if      (inside1) { swap(&t->v0, &t->v1); }
            else if (inside2) { swap(&t->v0, &t->v2); }

            const vec4f v = (vec4f){0.0f, 1.0f, 0.0f, -1.0f};
            pgl_triangle_clip_plane_intersection(t, v, &v10, &v20);
            subtriangles[++index] = (pgl_queue_triangle_t){t->v0, v10, v20};
        }
    }

    return index;
}

// --------------------------------------------------- DRAWING --------------------------------------------------- // 

void pgl_draw(const mesh_t* mesh, const directional_light_t* dl) {
    const vec3f light_direction = mat3f_mul_vec3f(cast_mat4f_to_mat3f(pgl.view), dl->direction);
    const mat4f view_model = mat4f_mul_mat4f(pgl.view, mesh->transform.model);
    pgl.fill_color  = mesh->fill_color; // or tex_coords
    pgl.wired_color = mesh->wired_color;

    const u32 increment = (mesh->filled_render == FILLED_RENDER_TEX_COORDS) ? 6 : (mesh->filled_render == FILLED_RENDER_COLORS) ? 4 : 3;
    const u32 offset    = (mesh->filled_render == FILLED_RENDER_TEX_COORDS) ? 2 : 1;

    for (u32 i = 0; i < mesh->index_number; i += increment) {
        // Camera space coordinates
        const vec4f c0 = mat4f_mul_vec4f(view_model, homogeneous_point(mesh->vertices[mesh->indices[i + 0 * offset]]));
        const vec4f c1 = mat4f_mul_vec4f(view_model, homogeneous_point(mesh->vertices[mesh->indices[i + 1 * offset]]));
        const vec4f c2 = mat4f_mul_vec4f(view_model, homogeneous_point(mesh->vertices[mesh->indices[i + 2 * offset]]));
        
        if (mesh->filled_render == FILLED_RENDER_COLORS) {
            pgl.fill_color = mesh->indices[i+3]; // Color of the whole face
        }
        
        const vec3f face_normal_in_camera_space = vec3f_cross(
            (vec3f){c1.x - c0.x, c1.y - c0.y, c1.z - c0.z}, 
            (vec3f){c2.x - c0.x, c2.y - c0.y, c2.z - c0.z}
        );
        if (face_is_culled(face_normal_in_camera_space, (vec3f){c0.x, c0.y, c0.z})) { continue; }

        // Flat shading
        const f32 cos_angle = -vec3f_dot(light_direction, vec3f_normalize(face_normal_in_camera_space));
        const f32 shade = dl->intensity * (AMBIENT_COEF + DIFFUSE_COEF * greater(cos_angle, 0.0f));
        pgl.shade_multp = (u32)(shade * (1 << SHADE_2_POWER));

        // Clip space coordinates
        pgl_queue_triangle_t triangle = {
            {mat4f_mul_vec4f(pgl.projection, c0), (mesh->filled_render == FILLED_RENDER_TEX_COORDS) ? tex_coords[mesh->indices[i+1]] : vec2f_zero},
            {mat4f_mul_vec4f(pgl.projection, c1), (mesh->filled_render == FILLED_RENDER_TEX_COORDS) ? tex_coords[mesh->indices[i+3]] : vec2f_zero},
            {mat4f_mul_vec4f(pgl.projection, c2), (mesh->filled_render == FILLED_RENDER_TEX_COORDS) ? tex_coords[mesh->indices[i+5]] : vec2f_zero},
        };

        pgl_queue_triangle_t subtriangles[PGL_QUEUE_CAPACITY];
        i32 subtriangle_index = pgl_narrow_phase_clipping(&triangle, subtriangles); // tex coords should not be interpolated if 'mesh->filled_render' != FILLED_RENDER_TEX_COORDS

        while (subtriangle_index >= 0) {
            const pgl_queue_triangle_t* subt = subtriangles + subtriangle_index;
            subtriangle_index--;

            // Screen space coordinates
            const vec4f sc0 = mat4f_mul_vec4f(pgl.viewport, homogeneous_point_normalize(subt->v0.position));
            const vec4f sc1 = mat4f_mul_vec4f(pgl.viewport, homogeneous_point_normalize(subt->v1.position));
            const vec4f sc2 = mat4f_mul_vec4f(pgl.viewport, homogeneous_point_normalize(subt->v2.position));

            const fragment_t f0 = {{sc0.x, sc0.y}, vec2f_scale(subt->v0.tex_coord, 1.0f / subt->v0.position.w), 1.0f / subt->v0.position.w};
            const fragment_t f1 = {{sc1.x, sc1.y}, vec2f_scale(subt->v1.tex_coord, 1.0f / subt->v1.position.w), 1.0f / subt->v1.position.w};
            const fragment_t f2 = {{sc2.x, sc2.y}, vec2f_scale(subt->v2.tex_coord, 1.0f / subt->v2.position.w), 1.0f / subt->v2.position.w};
        
            // Rasterize
            if (mesh->filled_render != FILLED_RENDER_NO) {
                pgl_filled_triangle(f0, f1, f2, mesh->filled_render);
            }
            if (mesh->wired_render != WIRED_RENDER_NO) {
                pgl_wired_triangle(f0, f1, f2);
            }
        }
    }
}

void pgl_display() {
    device_display((u16*)pgl.color_buffer); // a function with prototype "void device_display(u16* image)" must be defined
}
