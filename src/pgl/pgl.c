
#include "pgl.h"
#include "pgl_queue.h"

#define AMBIENT_COEF 0.2f
#define DIFFUSE_COEF 0.8f
#define SHADE_2_POWER 8u

// Stencil test is disabled.
// Stencil write mask is enabled. 
// Stencil function is PGL_NEVER.
// Stencil op sfail is PGL_KEEP.
// Stencil op dpfail is PGL_KEEP.
// Stencil op dppass is PGL_KEEP.
// Depth test disabled.
// Depth write mask is enabled.
// Depth test function is PGL_LESS.
// Cull face is PGL_CULL_NO.
// Cull winding order is PGL_CCW.
#define PGL_DEFAULT_STATE (0 << 0) |            \
                          (1 << 1) |            \
                          (PGL_NEVER << 2) |    \
                          (PGL_KEEP << 5) |     \
                          (PGL_KEEP << 6) |     \
                          (PGL_KEEP << 7) |     \
                          (0 << 8) |            \
                          (1 << 9) |            \
                          (PGL_LESS << 10) |    \
                          (PGL_CULL_NO << 13) | \
                          (PGL_CCW << 15)

typedef struct {
    // Buffers
    uint16_t color_buffer[SCREEN_HEIGHT][SCREEN_HEIGHT];
    uint8_t depth_buffer[SCREEN_HEIGHT][SCREEN_WIDTH];
    uint8_t stencil_buffer[SCREEN_HEIGHT][SCREEN_WIDTH/8];
    // Transformation matrices
    mat4f view;
    mat4f projection;
    mat4f viewport;
    // Cache values for fast execution
    float near;
    float far;
    float sin_half_fovw;
    float cos_half_fovw;
    float sin_half_fovh;
    float cos_half_fovh;
    float depth_coef;
    // Internal state 
    pgl_enum_t state;
    union {
        uint16_t color;     // used for wired rendering 
        uint16_t tex_index; // used for filled rendering
    };
    uint16_t clear_color;
    uint32_t shade_multp;
} pgl_t;

pgl_t pgl = {
    .color_buffer = {{0x0000u}},
    .depth_buffer = {{0x00u}},
    .stencil_buffer = {{0x00u}},
    .state = PGL_DEFAULT_STATE,
    .clear_color = PGLM_RGB565_BLACK,
};

typedef struct {
    vec2i position;
    vec2f tex_coord;
    float depth_inv;
} fragment_t;

// --------------------------------------------------- RASTERIZATION --------------------------------------------------- // 

// static void pgl_horizontal_line(fragment_t f0, fragment_t f1) {
//     const vec3i p0 = f0.position;
//     const vec3i p1 = f1.position;

//     const int dx = abs(p1.x - p0.x);
//     const int sx = (p0.x < p1.x) ? 1 : -1;
//     const int dz = -abs(p1.z - p0.z);
//     const int sz = (p0.z < p1.z) ? 1 : -1;
//     const int y  = p0.y;
    
//     int ez = dx + dz;
//     int x = p0.x;
//     int z = p0.z;

//     while (true) {
//         if (z < pgl.depth_buffer[y][x]) {
//             const float alpha = (float)(x - p0.x) / (float)(p1.x - p0.x);
//             const vec2f tex_coord = interp_vec2f(f1.tex_coord, f0.tex_coord, alpha);
//             const uint16_t color = sample_texture_vec2f(tex_coord, pgl.tex_index);

//             const uint32_t red   = (pgl.shade_multp * (color & (31 << 11))) >> SHADE_2_POWER;
//             const uint32_t green = (pgl.shade_multp * (color & (63 << 5))) >> SHADE_2_POWER;
//             const uint32_t blue  = (pgl.shade_multp * (color & 31)) >> SHADE_2_POWER;
//             const uint16_t shaded_color = (uint16_t)((red & (31 << 11)) | (green & (63 << 5)) | blue);

//             pgl.color_buffer[y][x] = shaded_color;
//             pgl.depth_buffer[y][x] = z;
//         }

//         if (x == p1.x) {
//             break;
//         }

//         const int two_ez = 2 * ez;
//         if (two_ez >= dz) {
//             ez += dz;
//             x += sx;
//         }
//         if (two_ez <= dx) {
//             ez += dx;
//             z += sz;
//         }
//     }
// }

static void pgl_line(fragment_t f0, fragment_t f1) {
    const int dx = abs(f1.position.x - f0.position.x);
    const int sx = (f0.position.x < f1.position.x) ? 1 : -1;
    const int dy = -abs(f1.position.y - f0.position.y);
    const int sy = (f0.position.y < f1.position.y) ? 1 : -1;

    const int depth0 = pgl.depth_coef * ((1.0f / f0.depth_inv) - pgl.near);
    const int depth1 = pgl.depth_coef * ((1.0f / f1.depth_inv) - pgl.near);

    const int d_depth = -abs(depth1 - depth0);
    const int s_depth = (depth0 < depth1) ? 1 : -1;

    int ey = dx + dy;
    int e_depth = dx + d_depth;
    int x = f0.position.x;
    int y = f0.position.y;
    int depth = depth0;

    while (true) {
        if (depth < pgl.depth_buffer[y][x]) {
            pgl.color_buffer[y][x] = pgl.color;
            pgl.depth_buffer[y][x] = depth;
        }

        if (x == f1.position.x && y == f1.position.y) {
            break;
        }

        const int two_ey = 2 * ey;
        if (two_ey >= dy) {
            ey += dy;
            x += sx;

            const int two_e_depth = 2 * e_depth;
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

static void pgl_filled_triangle2(fragment_t f0, fragment_t f1, fragment_t f2) {
    // Sort fragments with respect to their y coordinates
    if (f0.position.y > f1.position.y) { swap(&f0, &f1); }
    if (f1.position.y > f2.position.y) { swap(&f1, &f2); }
    if (f0.position.y > f1.position.y) { swap(&f0, &f1); }

	int   dx1 = f1.position.x  - f0.position.x;
	int   dy1 = f1.position.y  - f0.position.y;
	float du1 = f1.tex_coord.u - f0.tex_coord.u;
	float dv1 = f1.tex_coord.v - f0.tex_coord.v;
    float dw1 = f1.depth_inv   - f0.depth_inv;

	int   dx2 = f2.position.x  - f0.position.x;
	int   dy2 = f2.position.y  - f0.position.y;
	float du2 = f2.tex_coord.u - f0.tex_coord.u;
	float dv2 = f2.tex_coord.v - f0.tex_coord.v;
    float dw2 = f2.depth_inv   - f0.depth_inv;

	float dax_step = (dy1) ? dx1 / (float)abs(dy1) : 0.0f;
	float du1_step = (dy1) ? du1 / (float)abs(dy1) : 0.0f;
	float dv1_step = (dy1) ? dv1 / (float)abs(dy1) : 0.0f;
    float dw1_step = (dy1) ? dw1 / (float)abs(dy1) : 0.0f;

	float dbx_step = (dy2) ? dx2 / (float)abs(dy2) : 0.0f;
	float du2_step = (dy2) ? du2 / (float)abs(dy2) : 0.0f;
	float dv2_step = (dy2) ? dv2 / (float)abs(dy2) : 0.0f;
    float dw2_step = (dy2) ? dw2 / (float)abs(dy2) : 0.0f;

	if (dy1) {
		for (int y = f0.position.y; y <= f1.position.y; y++) {
			int ax = f0.position.x + (float)(y - f0.position.y) * dax_step;
			int bx = f0.position.x + (float)(y - f0.position.y) * dbx_step;

			float tex_su = f0.tex_coord.u + (float)(y - f0.position.y) * du1_step;
			float tex_sv = f0.tex_coord.v + (float)(y - f0.position.y) * dv1_step;
            float tex_sw = f0.depth_inv   + (float)(y - f0.position.y) * dw1_step;

			float tex_eu = f0.tex_coord.u + (float)(y - f0.position.y) * du2_step;
			float tex_ev = f0.tex_coord.v + (float)(y - f0.position.y) * dv2_step;
            float tex_ew = f0.depth_inv   + (float)(y - f0.position.y) * dw2_step;

			if (ax > bx) {
				swap(&ax, &bx);
				swap(&tex_su, &tex_eu);
				swap(&tex_sv, &tex_ev);
                swap(&tex_sw, &tex_ew);
			}

			float t_step = 1.0f / ((float)(bx - ax));
			float t = 0.0f;

			for (int x = ax; x < bx; x++) {
				float tex_u = tex_su + t * (tex_eu - tex_su);
				float tex_v = tex_sv + t * (tex_ev - tex_sv);
                float tex_w_inv = 1.0f / (tex_sw + t * (tex_ew - tex_sw));
                uint8_t depth = pgl.depth_coef * (tex_w_inv - pgl.near);

				if (depth < pgl.depth_buffer[y][x]) {
                    const uint16_t color = sample_texture_vec2f((vec2f){tex_u * tex_w_inv, tex_v * tex_w_inv}, pgl.tex_index);
                    const uint32_t red   = (pgl.shade_multp * (color & (31 << 11))) >> SHADE_2_POWER;
                    const uint32_t green = (pgl.shade_multp * (color & (63 << 5))) >> SHADE_2_POWER;
                    const uint32_t blue  = (pgl.shade_multp * (color & 31)) >> SHADE_2_POWER;
                    const uint16_t shaded_color = (uint16_t)((red & (31 << 11)) | (green & (63 << 5)) | blue);

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

    dax_step = (dy1) ? dx1 / (float)abs(dy1) : 0.0f;
	du1_step = (dy1) ? du1 / (float)abs(dy1) : 0.0f;
	dv1_step = (dy1) ? dv1 / (float)abs(dy1) : 0.0f;
    dw1_step = (dy1) ? dw1 / (float)abs(dy1) : 0.0f;

	if (dy1) {
		for (int y = f1.position.y; y <= f2.position.y; y++) {
			int ax = f1.position.x + (float)(y - f1.position.y) * dax_step;
			int bx = f0.position.x + (float)(y - f0.position.y) * dbx_step;

			float tex_su = f1.tex_coord.u + (float)(y - f1.position.y) * du1_step;
			float tex_sv = f1.tex_coord.v + (float)(y - f1.position.y) * dv1_step;
            float tex_sw = f1.depth_inv   + (float)(y - f1.position.y) * dw1_step;

			float tex_eu = f0.tex_coord.u + (float)(y - f0.position.y) * du2_step;
			float tex_ev = f0.tex_coord.v + (float)(y - f0.position.y) * dv2_step;
            float tex_ew = f0.depth_inv   + (float)(y - f0.position.y) * dw2_step;

			if (ax > bx) {
				swap(&ax, &bx);
				swap(&tex_su, &tex_eu);
				swap(&tex_sv, &tex_ev);
                swap(&tex_sw, &tex_ew);
			}

			float t_step = 1.0f / ((float)(bx - ax));
			float t = 0.0f;

			for (int x = ax; x < bx; x++) {
				float tex_u = tex_su + t * (tex_eu - tex_su);
				float tex_v = tex_sv + t * (tex_ev - tex_sv);
                float tex_w_inv = 1.0f / (tex_sw + t * (tex_ew - tex_sw));
                uint8_t depth = pgl.depth_coef * (tex_w_inv - pgl.near);

				if (depth < pgl.depth_buffer[y][x]) {
                    const uint16_t color = sample_texture_vec2f((vec2f){tex_u * tex_w_inv, tex_v * tex_w_inv}, pgl.tex_index);
                    const uint32_t red   = (pgl.shade_multp * (color & (31 << 11))) >> SHADE_2_POWER;
                    const uint32_t green = (pgl.shade_multp * (color & (63 << 5))) >> SHADE_2_POWER;
                    const uint32_t blue  = (pgl.shade_multp * (color & 31)) >> SHADE_2_POWER;
                    const uint16_t shaded_color = (uint16_t)((red & (31 << 11)) | (green & (63 << 5)) | blue);

                    pgl.color_buffer[y][x] = shaded_color;
                    pgl.depth_buffer[y][x] = depth;
				}
				t += t_step;
			}
		}	
	}		
}

// static void pgl_filled_triangle(fragment_t f0, fragment_t f1, fragment_t f2) {
//     // Sort fragments with respect to their y coordinates
//     if (f1.position.y < f0.position.y) { swap(&f0, &f1); }
//     if (f2.position.y < f1.position.y) { swap(&f1, &f2); }
//     if (f1.position.y < f0.position.y) { swap(&f0, &f1); }

//     const vec3i p0 = f0.position;    
//     const vec3i p1 = f1.position;    
//     const vec3i p2 = f2.position;

//     for (int y = p0.y; y <= p2.y; y++) {
//         fragment_t fleft  = {.position = {.y = y}};
//         fragment_t fright = {.position = {.y = y}};
                              
//         if (y >= p1.y) {
//             const int dxl = (p2.y != p1.y) ? (y - p1.y) * (p2.x - p1.x) / (p2.y - p1.y) : 0;
//             const int dxr = (p2.y != p0.y) ? (y - p0.y) * (p2.x - p0.x) / (p2.y - p0.y) : 0;
//             fleft.position.x  = p1.x + dxl;
//             fright.position.x = p0.x + dxr;

//             const int dzl = (p2.y != p1.y) ? (y - p1.y) * (p2.z - p1.z) / (p2.y - p1.y) : 0;
//             const int dzr = (p2.y != p0.y) ? (y - p0.y) * (p2.z - p0.z) / (p2.y - p0.y) : 0;
//             fleft.position.z  = p1.z + dzl;
//             fright.position.z = p0.z + dzr;

//             const float alpha_left  = (float)(y - p1.y) / (float)(p2.y - p1.y);
//             const float alpha_right = (float)(y - p0.y) / (float)(p2.y - p0.y);
//             fleft.tex_coord  = interp_vec2f(f2.tex_coord, f1.tex_coord, alpha_left);
//             fright.tex_coord = interp_vec2f(f2.tex_coord, f0.tex_coord, alpha_right);
//         } 
//         else {
//             const int dxl = (p1.y != p0.y) ? (y - p0.y) * (p1.x - p0.x) / (p1.y - p0.y) : 0;
//             const int dxr = (p2.y != p0.y) ? (y - p0.y) * (p2.x - p0.x) / (p2.y - p0.y) : 0;
//             fleft.position.x  = p0.x + dxl;
//             fright.position.x = p0.x + dxr;

//             const int dzl = (p1.y != p0.y) ? (y - p0.y) * (p1.z - p0.z) / (p1.y - p0.y) : 0;
//             const int dzr = (p2.y != p0.y) ? (y - p0.y) * (p2.z - p0.z) / (p2.y - p0.y) : 0;
//             fleft.position.z  = p0.z + dzl;
//             fright.position.z = p0.z + dzr;

//             const float alpha_left  = (float)(y - p0.y) / (float)(p1.y - p0.y);
//             const float alpha_right = (float)(y - p0.y) / (float)(p2.y - p0.y);
//             fleft.tex_coord  = interp_vec2f(f1.tex_coord, f0.tex_coord, alpha_left);
//             fright.tex_coord = interp_vec2f(f2.tex_coord, f0.tex_coord, alpha_right);
//         }

//         if (fleft.position.x > fright.position.x) {
//             swap(&fleft, &fright);
//         }
//         pgl_horizontal_line(fleft, fright);
//     }
// }

// --------------------------------------------------- TRANSFORMATIONS --------------------------------------------------- // 

void pgl_view(vec3f position, vec3f right, vec3f up, vec3f forward) {
    pgl.view = view(position, right, up, forward);
}

void pgl_projection(float near, float far, float fovw) {
    const float sin_half_fovw = sinf(fovw * 0.5f);
    const float cos_half_fovw = cosf(fovw * 0.5f);
    const float tan_half_fovh = sin_half_fovw / (cos_half_fovw * SCREEN_ASPECT_RATIO);
    const float cos_half_fovh = 1.0f / sqrtf(tan_half_fovh * tan_half_fovh + 1.0f); 
    const float sin_half_fovh = tan_half_fovh * cos_half_fovh;

    pgl.sin_half_fovw = sin_half_fovw;
    pgl.cos_half_fovw = cos_half_fovw;
    pgl.sin_half_fovh = sin_half_fovh;
    pgl.cos_half_fovh = cos_half_fovh;
    pgl.depth_coef = 255.0f / (far - near);
    pgl.projection = perspective(fovw, SCREEN_ASPECT_RATIO, near, far);
    pgl.near = near;
    pgl.far = far;
}

void pgl_viewport(int x, int y, uint32_t width, uint32_t height) {
    pgl.viewport = viewport(x, y, width, height);
}

// --------------------------------------------------- INTERNAL STATE --------------------------------------------------- // 

void pgl_enable(pgl_enum_t test) {
    pgl.state |= test;
}

void pgl_disable(pgl_enum_t test) {
    pgl.state &= ~test;
}

void pgl_clear_color(uint16_t color) {
    pgl.clear_color = color;
}

void pgl_clear(pgl_enum_t buffer_bits) {
    const uint32_t double_color = (pgl.clear_color << 16) | pgl.clear_color;
    if (buffer_bits & PGL_COLOR_BUFFER_BIT) {
        uint32_t* ptr = (uint32_t*)pgl.color_buffer;
        for (uint32_t i = 0; i < (SCREEN_HEIGHT * SCREEN_WIDTH / 2); i++) {
            ptr[i] = double_color;
        }
    }
    if (buffer_bits & PGL_DEPTH_BUFFER_BIT) {
        uint32_t* ptr = (uint32_t*)pgl.depth_buffer;
        for (uint32_t i = 0; i < (SCREEN_HEIGHT * SCREEN_WIDTH / 4); i++) {
            ptr[i] = 0xFFFFFFFFu; // reset to far = 0xFF
        }
    }
    if (buffer_bits & PGL_STENCIL_BUFFER_BIT) {
        uint32_t* ptr = (uint32_t*)pgl.stencil_buffer;
        for (uint32_t i = 0; i < (SCREEN_HEIGHT * SCREEN_WIDTH / 32); i++) {
            ptr[i] = 0x00000000u;
        }
    }
}

void pgl_depth_mask(bool enable_write) {
    pgl.state &= ~(0b1 << 9);
    pgl.state |= (enable_write << 9);
}

void pgl_depth_func(pgl_enum_t test_func) {
    pgl.state &= ~(0b111 << 10);
    pgl.state |= (test_func << 10);
}

void pgl_stencil_mask(bool enable_write) {
    pgl.state &= ~(0b1 << 1);
    pgl.state |= (enable_write << 1);
}

void pgl_stencil_func(pgl_enum_t test_func) {
    pgl.state &= ~(0b111 << 2);
    pgl.state |= (test_func << 2);
}

void pgl_stencil_op(pgl_enum_t sfail, pgl_enum_t dpfail, pgl_enum_t dppass) {
    pgl.state &= ~(0b111 << 5);
    pgl.state |= ((sfail << 5) | (dpfail << 6) | (dppass << 7));
}

void pgl_cull_face(pgl_enum_t face) {
    pgl.state &= ~(0b11 << 13);
    pgl.state |= face;
}

void pgl_front_face(pgl_enum_t winding_order) {
    pgl.state &= ~(0b1 << 15);
    pgl.state |= winding_order;
}

// --------------------------------------------------- CLIPPING --------------------------------------------------- // 

// Clips the triangle with respect to a plane in the clipping space.
// Intersection vertices are outputted as v10 and v20.
static void pgl_triangle_clip_plane_intersection(const pgl_queue_triangle_t* t, vec4f clip_plane_vector, pgl_vertex_t* restrict v10, pgl_vertex_t* restrict v20) {
    const float d0 = dot_vec4f(t->v0.position, clip_plane_vector);
    const float d1 = dot_vec4f(t->v1.position, clip_plane_vector);
    const float d2 = dot_vec4f(t->v2.position, clip_plane_vector);

    const float alpha10 = d0 / (d0 - d1);
    const float alpha20 = d0 / (d0 - d2);

    v10->position  = interp_vec4f(t->v1.position,  t->v0.position,  alpha10);
    v10->tex_coord = interp_vec2f(t->v1.tex_coord, t->v0.tex_coord, alpha10);
    v20->position  = interp_vec4f(t->v2.position,  t->v0.position,  alpha20);
    v20->tex_coord = interp_vec2f(t->v2.tex_coord, t->v0.tex_coord, alpha20);
}

// Returns true if the mesh may be visible.
// Returns false if there is no chance that the mesh is visible. 
static bool pgl_broad_phase_clipping(const mesh_t* mesh, mat4f view_model) {
    const vec3f center = mul_mat3f_vec3f(cast_mat4f_to_mat3f(view_model), mesh->bounding_volume.center);
    const float minus_sin_half_fovw = -pgl.sin_half_fovw;
    const float cos_half_fovw       =  pgl.cos_half_fovw;
    const float minus_sin_half_fovh = -pgl.sin_half_fovh;
    const float cos_half_fovh       =  pgl.cos_half_fovh;

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
static int pgl_narrow_phase_clipping(pgl_queue_triangle_t* restrict triangle, pgl_queue_triangle_t* restrict subtriangles) {
    pgl_queue_t queue;
    triangle_queue_init(&queue);
    triangle_queue_push(&queue, triangle);

    int index = -1;
    pgl_vertex_t v10;
    pgl_vertex_t v20;
    
    // Near Clip: Z + W > 0
    while (!queue.empty) {
        pgl_queue_triangle_t* t = triangle_queue_pop(&queue);
        const int in0 = t->v0.position.z > -t->v0.position.w;
        const int in1 = t->v1.position.z > -t->v1.position.w;
        const int in2 = t->v2.position.z > -t->v2.position.w;
        const int in_number = in0 + in1 + in2;

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
        const int in0 = t->v0.position.z < t->v0.position.w;
        const int in1 = t->v1.position.z < t->v1.position.w;
        const int in2 = t->v2.position.z < t->v2.position.w;
        const int in_number = in0 + in1 + in2;

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
        const int in0 = t->v0.position.x > -t->v0.position.w;
        const int in1 = t->v1.position.x > -t->v1.position.w;
        const int in2 = t->v2.position.x > -t->v2.position.w;
        const int in_number = in0 + in1 + in2;

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
        const int in0 = t->v0.position.x < t->v0.position.w;
        const int in1 = t->v1.position.x < t->v1.position.w;
        const int in2 = t->v2.position.x < t->v2.position.w;
        const int in_number = in0 + in1 + in2;

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
        const int in0 = t->v0.position.y > -t->v0.position.w;
        const int in1 = t->v1.position.y > -t->v1.position.w;
        const int in2 = t->v2.position.y > -t->v2.position.w;
        const int in_number = in0 + in1 + in2;

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
        const int in0 = t->v0.position.y < t->v0.position.w;
        const int in1 = t->v1.position.y < t->v1.position.w;
        const int in2 = t->v2.position.y < t->v2.position.w;
        const int in_number = in0 + in1 + in2;

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
    pgl.tex_index = mesh->tex_index;

    for (uint32_t i = 0; i < mesh->index_number; i += 6) {
        // Camera space coordinates
        const vec4f c0 = mul_mat4f_vec4f(view_model, to_homogeneous_point(mesh->vertices[mesh->indices[i+0]]));
        const vec4f c1 = mul_mat4f_vec4f(view_model, to_homogeneous_point(mesh->vertices[mesh->indices[i+2]]));
        const vec4f c2 = mul_mat4f_vec4f(view_model, to_homogeneous_point(mesh->vertices[mesh->indices[i+4]]));
        
        // Face culling
        const vec3f normal = cross_vec3f(
            (vec3f){c1.x - c0.x, c1.y - c0.y, c1.z - c0.z}, 
            (vec3f){c2.x - c0.x, c2.y - c0.y, c2.z - c0.z}
        );
        if (dot_vec3f(normal, (vec3f){c0.x, c0.y, c0.z}) >= 0.0f) { continue; }

        // Flat shading
        const float shade = dl->intensity * (AMBIENT_COEF + DIFFUSE_COEF * greater(-dot_vec3f(light_dir, normalize_vec3f(normal)), 0.0f));
        pgl.shade_multp = (uint32_t)(shade * (1 << SHADE_2_POWER));

        // Clip space coordinates
        pgl_queue_triangle_t triangle = {
            {mul_mat4f_vec4f(pgl.projection, c0), tex_coords[mesh->indices[i+1]]},
            {mul_mat4f_vec4f(pgl.projection, c1), tex_coords[mesh->indices[i+3]]},
            {mul_mat4f_vec4f(pgl.projection, c2), tex_coords[mesh->indices[i+5]]},
        };

        pgl_queue_triangle_t subtriangles[PGL_QUEUE_CAPACITY];
        int subtriangle_index = pgl_narrow_phase_clipping(&triangle, subtriangles);

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
            if ((mesh->mesh_enum ^ MESH_RENDER_WIRED) == 0) {
                pgl_wired_triangle(f0, f1, f2);
            }
            else if ((mesh->mesh_enum ^ MESH_RENDER_FILLED) == 0) {
                pgl_filled_triangle2(f0, f1, f2);
            }
        }
    }
}

void pgl_display() {
    lcd_display((uint16_t*)pgl.color_buffer);
}
