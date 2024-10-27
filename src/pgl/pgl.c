
#include "pgl.h"
#include "pgl_queue.h"

#define AMBIENT_COEF 0.2f
#define DIFFUSE_COEF 0.8f
#define SHADE_2_POWER 8u

typedef struct {
    // Buffers
    u16  color_buffer[DEVICE_SCREEN_HEIGHT][DEVICE_SCREEN_WIDTH];
    u8   depth_buffer[DEVICE_SCREEN_HEIGHT][DEVICE_SCREEN_WIDTH];
    u8 stencil_buffer[DEVICE_SCREEN_HEIGHT][DEVICE_SCREEN_WIDTH / 8];

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
        u16 fill_color; // used to pai32 the mesh when 'mesh_t.filled_render' == FILLED_RENDER_SINGLE_COLOR or 'mesh_t.filled_render' == FILLED_RENDER_COLORS
        u16 tex_index;  // used to pai32 the mesh when 'mesh_t.filled_render' == FILLED_RENDER_TEX_COORDS
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

pgl_t pgl = {
    .color_buffer   = {{0x0000u}},
    .depth_buffer   = {{0x00u}},
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
        if (depth < pgl.depth_buffer[y][x]) {
            pgl.color_buffer[y][x] = pgl.wired_color;
            pgl.depth_buffer[y][x] = depth;
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
    const u32 red   = (pgl.shade_multp * (color & (31 << 11))) >> SHADE_2_POWER;
    const u32 green = (pgl.shade_multp * (color & (63 <<  5))) >> SHADE_2_POWER;
    const u32 blue  = (pgl.shade_multp * (color &  31))        >> SHADE_2_POWER;
    const u16 shaded_color = (u16)((red & (31 << 11)) | (green & (63 << 5)) | blue);
    return shaded_color;
}

static void pgl_filled_triangle(fragment_t f0, fragment_t f1, fragment_t f2, filled_render_t filled_render) {
    // Sort fragments with respect to their y coordinates
    if (f0.position.y > f1.position.y) { swap(&f0, &f1); }
    if (f1.position.y > f2.position.y) { swap(&f1, &f2); }
    if (f0.position.y > f1.position.y) { swap(&f0, &f1); }

	i32 dx1 = f1.position.x  - f0.position.x;
	i32 dy1 = f1.position.y  - f0.position.y;
	f32 du1 = f1.tex_coord.u - f0.tex_coord.u;
	f32 dv1 = f1.tex_coord.v - f0.tex_coord.v;
    f32 dw1 = f1.depth_inv   - f0.depth_inv;

	i32 dx2 = f2.position.x  - f0.position.x;
	i32 dy2 = f2.position.y  - f0.position.y;
	f32 du2 = f2.tex_coord.u - f0.tex_coord.u;
	f32 dv2 = f2.tex_coord.v - f0.tex_coord.v;
    f32 dw2 = f2.depth_inv   - f0.depth_inv;

	f32 dax_step = (dy1) ? dx1 / (f32)abs(dy1) : 0.0f;
	f32 du1_step = (dy1) ? du1 / (f32)abs(dy1) : 0.0f;
	f32 dv1_step = (dy1) ? dv1 / (f32)abs(dy1) : 0.0f;
    f32 dw1_step = (dy1) ? dw1 / (f32)abs(dy1) : 0.0f;

	f32 dbx_step = (dy2) ? dx2 / (f32)abs(dy2) : 0.0f;
	f32 du2_step = (dy2) ? du2 / (f32)abs(dy2) : 0.0f;
	f32 dv2_step = (dy2) ? dv2 / (f32)abs(dy2) : 0.0f;
    f32 dw2_step = (dy2) ? dw2 / (f32)abs(dy2) : 0.0f;

    u16 shaded_color;
    if (filled_render == FILLED_RENDER_SINGLE_COLOR || filled_render == FILLED_RENDER_COLORS) {
        shaded_color = pgl_shader(pgl.fill_color);
    }

	if (dy1) {
		for (i32 y = f0.position.y; y <= f1.position.y; y++) {
			i32 ax = f0.position.x + (f32)(y - f0.position.y) * dax_step;
			i32 bx = f0.position.x + (f32)(y - f0.position.y) * dbx_step;

			f32 tex_su = f0.tex_coord.u + (f32)(y - f0.position.y) * du1_step;
			f32 tex_sv = f0.tex_coord.v + (f32)(y - f0.position.y) * dv1_step;
            f32 tex_sw = f0.depth_inv   + (f32)(y - f0.position.y) * dw1_step;

			f32 tex_eu = f0.tex_coord.u + (f32)(y - f0.position.y) * du2_step;
			f32 tex_ev = f0.tex_coord.v + (f32)(y - f0.position.y) * dv2_step;
            f32 tex_ew = f0.depth_inv   + (f32)(y - f0.position.y) * dw2_step;

			if (ax > bx) {
				swap(&ax, &bx);
				swap(&tex_su, &tex_eu);
				swap(&tex_sv, &tex_ev);
                swap(&tex_sw, &tex_ew);
			}

			f32 t_step = 1.0f / ((f32)(bx - ax));
			f32 t = 0.0f;

			for (i32 x = ax; x < bx; x++) {
				f32 tex_u = tex_su + t * (tex_eu - tex_su);
				f32 tex_v = tex_sv + t * (tex_ev - tex_sv);
                f32 tex_w_inv = 1.0f / (tex_sw + t * (tex_ew - tex_sw));
                u8 depth = pgl.depth_coef * (tex_w_inv - pgl.near);

				if (depth < pgl.depth_buffer[y][x]) {
                    if (filled_render == FILLED_RENDER_TEX_COORDS) {
                        const u16 color = texture_sample_vec2f((vec2f){tex_u * tex_w_inv, tex_v * tex_w_inv}, pgl.tex_index);
                        shaded_color = pgl_shader(color);
                    }
                    pgl.color_buffer[y][x] = shaded_color;
                    pgl.depth_buffer[y][x] = depth;
				}
				t += t_step;
			}
		}
	}

	dx1 = f2.position.x  - f1.position.x;
	dy1 = f2.position.y  - f1.position.y;
	du1 = f2.tex_coord.u - f1.tex_coord.u;
	dv1 = f2.tex_coord.v - f1.tex_coord.v;
    dw1 = f2.depth_inv   - f1.depth_inv;

    dax_step = (dy1) ? dx1 / (f32)abs(dy1) : 0.0f;
	du1_step = (dy1) ? du1 / (f32)abs(dy1) : 0.0f;
	dv1_step = (dy1) ? dv1 / (f32)abs(dy1) : 0.0f;
    dw1_step = (dy1) ? dw1 / (f32)abs(dy1) : 0.0f;

	if (dy1) {
		for (i32 y = f1.position.y; y <= f2.position.y; y++) {
			i32 ax = f1.position.x + (f32)(y - f1.position.y) * dax_step;
			i32 bx = f0.position.x + (f32)(y - f0.position.y) * dbx_step;

			f32 tex_su = f1.tex_coord.u + (f32)(y - f1.position.y) * du1_step;
			f32 tex_sv = f1.tex_coord.v + (f32)(y - f1.position.y) * dv1_step;
            f32 tex_sw = f1.depth_inv   + (f32)(y - f1.position.y) * dw1_step;

			f32 tex_eu = f0.tex_coord.u + (f32)(y - f0.position.y) * du2_step;
			f32 tex_ev = f0.tex_coord.v + (f32)(y - f0.position.y) * dv2_step;
            f32 tex_ew = f0.depth_inv   + (f32)(y - f0.position.y) * dw2_step;

			if (ax > bx) {
				swap(&ax, &bx);
				swap(&tex_su, &tex_eu);
				swap(&tex_sv, &tex_ev);
                swap(&tex_sw, &tex_ew);
			}

			f32 t_step = 1.0f / ((f32)(bx - ax));
			f32 t = 0.0f;

			for (i32 x = ax; x < bx; x++) {
				f32 tex_u = tex_su + t * (tex_eu - tex_su);
				f32 tex_v = tex_sv + t * (tex_ev - tex_sv);
                f32 tex_w_inv = 1.0f / (tex_sw + t * (tex_ew - tex_sw));
                u8 depth = pgl.depth_coef * (tex_w_inv - pgl.near);

				if (depth < pgl.depth_buffer[y][x]) {
                    if (filled_render == FILLED_RENDER_TEX_COORDS) {
                        const u16 color = texture_sample_vec2f((vec2f){tex_u * tex_w_inv, tex_v * tex_w_inv}, pgl.tex_index);
                        shaded_color = pgl_shader(color);
                    }
                    pgl.color_buffer[y][x] = shaded_color;
                    pgl.depth_buffer[y][x] = depth;
				}
				t += t_step;
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

// --------------------------------------------------- INTERNAL STATE --------------------------------------------------- // 

void pgl_clear_color(u16 color) {
    pgl.clear_color = color;
}

void pgl_clear(pgl_buffer_bit_t buffer_bits) {
    const u32 double_color = (pgl.clear_color << 16) | pgl.clear_color;
    if (buffer_bits & PGL_COLOR_BUFFER_BIT) {
        u32* ptr = (u32*)pgl.color_buffer;
        for (u32 i = 0; i < (DEVICE_SCREEN_HEIGHT * DEVICE_SCREEN_WIDTH / 2); i++) {
            ptr[i] = double_color;
        }
    }
    if (buffer_bits & PGL_DEPTH_BUFFER_BIT) {
        u32* ptr = (u32*)pgl.depth_buffer;
        for (u32 i = 0; i < (DEVICE_SCREEN_HEIGHT * DEVICE_SCREEN_WIDTH / 4); i++) {
            ptr[i] = 0xFFFFFFFFu; // reset to far = 0xFF
        }
    }
    if (buffer_bits & PGL_STENCIL_BUFFER_BIT) {
        u32* ptr = (u32*)pgl.stencil_buffer;
        for (u32 i = 0; i < (DEVICE_SCREEN_HEIGHT * DEVICE_SCREEN_WIDTH / 32); i++) {
            ptr[i] = 0x00000000u;
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

void pgl_cull_face(pgl_cull_face_t face) {
    pgl.cull_face = face;
}

void pgl_cull_winding_order(pgl_cull_winding_order_t cull_winding_order) {
    pgl.cull_winding_order = cull_winding_order;
}

// --------------------------------------------------- CLIPPING --------------------------------------------------- // 

// Clips the triangle with respect to a plane in the clipping space.
// i32ersection vertices are outputted as v10 and v20.
static void pgl_triangle_clip_plane_intersection(const pgl_queue_triangle_t* t, vec4f clip_plane_vector, pgl_vertex_t* restrict v10, pgl_vertex_t* restrict v20) {
    const f32 d0 = dot_vec4f(t->v0.position, clip_plane_vector);
    const f32 d1 = dot_vec4f(t->v1.position, clip_plane_vector);
    const f32 d2 = dot_vec4f(t->v2.position, clip_plane_vector);

    const f32 alpha10 = d0 / (d0 - d1);
    const f32 alpha20 = d0 / (d0 - d2);

    v10->position  = interp_vec4f(t->v1.position,  t->v0.position,  alpha10);
    v10->tex_coord = interp_vec2f(t->v1.tex_coord, t->v0.tex_coord, alpha10);
    v20->position  = interp_vec4f(t->v2.position,  t->v0.position,  alpha20);
    v20->tex_coord = interp_vec2f(t->v2.tex_coord, t->v0.tex_coord, alpha20);
}

// Returns true if the mesh may be visible.
// Returns false if there is no chance that the mesh is visible. 
static bool pgl_broad_phase_clipping(const mesh_t* mesh, mat4f view_model) {
    const vec3f center = mul_mat3f_vec3f(cast_mat4f_to_mat3f(view_model), mesh->bounding_volume.center);
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
    while (!queue.empty) {
        pgl_queue_triangle_t* t = triangle_queue_pop(&queue);
        const i32 in0 = t->v0.position.z > -t->v0.position.w;
        const i32 in1 = t->v1.position.z > -t->v1.position.w;
        const i32 in2 = t->v2.position.z > -t->v2.position.w;
        const i32 in_number = in0 + in1 + in2;

        if (in_number == 3) {
            subtriangles[++index] = *t;
        }
        else if (in_number == 2) {
            // Ensure that c0 is the vertex that is not in
            if      (!in1) { swap(&t->v0, &t->v1); }
            else if (!in2) { swap(&t->v0, &t->v2); }

            const vec4f v = (vec4f){0.0f, 0.0f, 1.0f, 1.0f};
            pgl_triangle_clip_plane_intersection(t, v, &v10, &v20);
            subtriangles[++index] = (pgl_queue_triangle_t){v10, t->v1, t->v2};
            subtriangles[++index] = (pgl_queue_triangle_t){v10,   v20, t->v2};
        }
        else if (in_number == 1) {
            // Ensure that c0 is the vertex that is in
            if      (in1) { swap(&t->v0, &t->v1); }
            else if (in2) { swap(&t->v0, &t->v2); }

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
    while (!queue.empty) {
        pgl_queue_triangle_t* t = triangle_queue_pop(&queue);
        const i32 in0 = t->v0.position.z < t->v0.position.w;
        const i32 in1 = t->v1.position.z < t->v1.position.w;
        const i32 in2 = t->v2.position.z < t->v2.position.w;
        const i32 in_number = in0 + in1 + in2;

        if (in_number == 3) {
            subtriangles[++index] = *t;
        }
        else if (in_number == 2) {
            // Ensure that c0 is the vertex that is not in
            if      (!in1) { swap(&t->v0, &t->v1); }
            else if (!in2) { swap(&t->v0, &t->v2); }

            const vec4f v = (vec4f){0.0f, 0.0f, 1.0f, -1.0f};
            pgl_triangle_clip_plane_intersection(t, v, &v10, &v20);
            subtriangles[++index] = (pgl_queue_triangle_t){v10, t->v1, t->v2};
            subtriangles[++index] = (pgl_queue_triangle_t){v10,   v20, t->v2};
        }
        else if (in_number == 1) {
            // Ensure that c0 is the vertex that is in
            if      (in1) { swap(&t->v0, &t->v1); }
            else if (in2) { swap(&t->v0, &t->v2); }

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
    while (!queue.empty) {
        pgl_queue_triangle_t* t = triangle_queue_pop(&queue);
        const i32 in0 = t->v0.position.x > -t->v0.position.w;
        const i32 in1 = t->v1.position.x > -t->v1.position.w;
        const i32 in2 = t->v2.position.x > -t->v2.position.w;
        const i32 in_number = in0 + in1 + in2;

        if (in_number == 3) {
            subtriangles[++index] = *t;
        }
        else if (in_number == 2) {
            // Ensure that c0 is the vertex that is not in
            if      (!in1) { swap(&t->v0, &t->v1); }
            else if (!in2) { swap(&t->v0, &t->v2); }

            const vec4f v = (vec4f){1.0f, 0.0f, 0.0f, 1.0f};
            pgl_triangle_clip_plane_intersection(t, v, &v10, &v20);
            subtriangles[++index] = (pgl_queue_triangle_t){v10, t->v1, t->v2};
            subtriangles[++index] = (pgl_queue_triangle_t){v10,   v20, t->v2};
        }
        else if (in_number == 1) {
            // Ensure that c0 is the vertex that is in
            if      (in1) { swap(&t->v0, &t->v1); }
            else if (in2) { swap(&t->v0, &t->v2); }

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
    while (!queue.empty) {
        pgl_queue_triangle_t* t = triangle_queue_pop(&queue);
        const i32 in0 = t->v0.position.x < t->v0.position.w;
        const i32 in1 = t->v1.position.x < t->v1.position.w;
        const i32 in2 = t->v2.position.x < t->v2.position.w;
        const i32 in_number = in0 + in1 + in2;

        if (in_number == 3) {
            subtriangles[++index] = *t;
        }
        else if (in_number == 2) {
            // Ensure that c0 is the vertex that is not in
            if      (!in1) { swap(&t->v0, &t->v1); }
            else if (!in2) { swap(&t->v0, &t->v2); }

            const vec4f v = (vec4f){1.0f, 0.0f, 0.0f, -1.0f};
            pgl_triangle_clip_plane_intersection(t, v, &v10, &v20);
            subtriangles[++index] = (pgl_queue_triangle_t){v10, t->v1, t->v2};
            subtriangles[++index] = (pgl_queue_triangle_t){v10,   v20, t->v2};
        }
        else if (in_number == 1) {
            // Ensure that c0 is the vertex that is in
            if      (in1) { swap(&t->v0, &t->v1); }
            else if (in2) { swap(&t->v0, &t->v2); }

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
    while (!queue.empty) {
        pgl_queue_triangle_t* t = triangle_queue_pop(&queue);
        const i32 in0 = t->v0.position.y > -t->v0.position.w;
        const i32 in1 = t->v1.position.y > -t->v1.position.w;
        const i32 in2 = t->v2.position.y > -t->v2.position.w;
        const i32 in_number = in0 + in1 + in2;

        if (in_number == 3) {
            subtriangles[++index] = *t;
        }
        else if (in_number == 2) {
            // Ensure that c0 is the vertex that is not in
            if      (!in1) { swap(&t->v0, &t->v1); }
            else if (!in2) { swap(&t->v0, &t->v2); }

            const vec4f v = (vec4f){0.0f, 1.0f, 0.0f, 1.0f};
            pgl_triangle_clip_plane_intersection(t, v, &v10, &v20);
            subtriangles[++index] = (pgl_queue_triangle_t){v10, t->v1, t->v2};
            subtriangles[++index] = (pgl_queue_triangle_t){v10,   v20, t->v2};
        }
        else if (in_number == 1) {
            // Ensure that c0 is the vertex that is in
            if      (in1) { swap(&t->v0, &t->v1); }
            else if (in2) { swap(&t->v0, &t->v2); }

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
    while (!queue.empty) {
        pgl_queue_triangle_t* t = triangle_queue_pop(&queue);
        const i32 in0 = t->v0.position.y < t->v0.position.w;
        const i32 in1 = t->v1.position.y < t->v1.position.w;
        const i32 in2 = t->v2.position.y < t->v2.position.w;
        const i32 in_number = in0 + in1 + in2;

        if (in_number == 3) {
            subtriangles[++index] = *t;
        }
        else if (in_number == 2) {
            // Ensure that c0 is the vertex that is not in
            if      (!in1) { swap(&t->v0, &t->v1); }
            else if (!in2) { swap(&t->v0, &t->v2); }

            const vec4f v = (vec4f){0.0f, 1.0f, 0.0f, -1.0f};
            pgl_triangle_clip_plane_intersection(t, v, &v10, &v20);
            subtriangles[++index] = (pgl_queue_triangle_t){v10, t->v1, t->v2};
            subtriangles[++index] = (pgl_queue_triangle_t){v10,   v20, t->v2};
        }
        else if (in_number == 1) {
            // Ensure that c0 is the vertex that is in
            if      (in1) { swap(&t->v0, &t->v1); }
            else if (in2) { swap(&t->v0, &t->v2); }

            const vec4f v = (vec4f){0.0f, 1.0f, 0.0f, -1.0f};
            pgl_triangle_clip_plane_intersection(t, v, &v10, &v20);
            subtriangles[++index] = (pgl_queue_triangle_t){t->v0, v10, v20};
        }
    }

    return index;
}

// --------------------------------------------------- DRAWING --------------------------------------------------- // 

void pgl_draw(const mesh_t* mesh, const directional_light_t* dl) {
    const vec3f light_dir = mul_mat3f_vec3f(cast_mat4f_to_mat3f(pgl.view), dl->direction);
    const mat4f view_model = mul_mat4f_mat4f(pgl.view, mesh->transform.model);
    pgl.fill_color  = mesh->fill_color; // or tex_coords
    pgl.wired_color = mesh->wired_color;

    const u32 incr   = (mesh->filled_render == FILLED_RENDER_TEX_COORDS) ? 6 : (mesh->filled_render == FILLED_RENDER_COLORS) ? 4 : 3;
    const u32 offset = (mesh->filled_render == FILLED_RENDER_TEX_COORDS) ? 2 : 1;

    for (u32 i = 0; i < mesh->index_number; i += incr) {
        // Camera space coordinates
        const vec4f c0 = mul_mat4f_vec4f(view_model, to_homogeneous_point(mesh->vertices[mesh->indices[i + 0 * offset]]));
        const vec4f c1 = mul_mat4f_vec4f(view_model, to_homogeneous_point(mesh->vertices[mesh->indices[i + 1 * offset]]));
        const vec4f c2 = mul_mat4f_vec4f(view_model, to_homogeneous_point(mesh->vertices[mesh->indices[i + 2 * offset]]));
        if (mesh->filled_render == FILLED_RENDER_COLORS) {
            pgl.fill_color = mesh->indices[i+3]; // Color of the face
        }
        
        // Face culling
        const vec3f normal = cross_vec3f(
            (vec3f){c1.x - c0.x, c1.y - c0.y, c1.z - c0.z}, 
            (vec3f){c2.x - c0.x, c2.y - c0.y, c2.z - c0.z}
        );
        if (dot_vec3f(normal, (vec3f){c0.x, c0.y, c0.z}) >= 0.0f) { continue; }

        // Flat shading
        const f32 shade = dl->intensity * (AMBIENT_COEF + DIFFUSE_COEF * greater(-dot_vec3f(light_dir, normalize_vec3f(normal)), 0.0f));
        pgl.shade_multp = (u32)(shade * (1 << SHADE_2_POWER));

        // Clip space coordinates
        pgl_queue_triangle_t triangle = { // New typedef can be defined to simplify
            {.position = mul_mat4f_vec4f(pgl.projection, c0)},
            {.position = mul_mat4f_vec4f(pgl.projection, c1)},
            {.position = mul_mat4f_vec4f(pgl.projection, c2)},
        };

        if (mesh->filled_render == FILLED_RENDER_TEX_COORDS) {
            triangle.v0.tex_coord = tex_coords[mesh->indices[i+1]];
            triangle.v1.tex_coord = tex_coords[mesh->indices[i+3]];
            triangle.v2.tex_coord = tex_coords[mesh->indices[i+5]];
        }
        else {
            triangle.v0.tex_coord = zero_vec2f;
            triangle.v1.tex_coord = zero_vec2f;
            triangle.v2.tex_coord = zero_vec2f;
        }

        pgl_queue_triangle_t subtriangles[PGL_QUEUE_CAPACITY];
        i32 subtriangle_index = pgl_narrow_phase_clipping(&triangle, subtriangles); // tex coords should not be i32erpolated if 'mesh->filled_render' != FILLED_RENDER_TEX_COORDS

        while (subtriangle_index >= 0) {
            const pgl_queue_triangle_t* subt = subtriangles + subtriangle_index;
            subtriangle_index--;

            // Screen space coordinates
            const vec4f sc0 = mul_mat4f_vec4f(pgl.viewport, normalize_homogeneous_point(subt->v0.position));
            const vec4f sc1 = mul_mat4f_vec4f(pgl.viewport, normalize_homogeneous_point(subt->v1.position));
            const vec4f sc2 = mul_mat4f_vec4f(pgl.viewport, normalize_homogeneous_point(subt->v2.position));

            const fragment_t f0 = {{sc0.x, sc0.y}, scale_vec2f(subt->v0.tex_coord, 1.0f / subt->v0.position.w), 1.0f / subt->v0.position.w};
            const fragment_t f1 = {{sc1.x, sc1.y}, scale_vec2f(subt->v1.tex_coord, 1.0f / subt->v1.position.w), 1.0f / subt->v1.position.w};
            const fragment_t f2 = {{sc2.x, sc2.y}, scale_vec2f(subt->v2.tex_coord, 1.0f / subt->v2.position.w), 1.0f / subt->v2.position.w};
        
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
