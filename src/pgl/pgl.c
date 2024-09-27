
#include "pgl.h"
#include "pgl_queue.h"

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
    uint16_t clear_color;
} pgl_t;

pgl_t pgl = {
    .color_buffer = {{0x0000u}},
    .depth_buffer = {{0x00u}},
    .stencil_buffer = {{0x00u}},
    .state = PGL_DEFAULT_STATE,
    .clear_color = PGLM_RGB565_BLACK,
};

// --------------------------------------------------- RASTERIZATION --------------------------------------------------- // 

static void pgl_horizontal_line(vec3i p0, vec3i p1, uint16_t color) {
    const int dx = p1.x - p0.x;
    const int dz = p1.z - p0.z;
    const int y = p0.y;

    for (int x = p0.x; x <= p1.x; x++) {
        const int z = dz * (x - p0.x) / dx + p0.z;
        if (z < pgl.depth_buffer[y][x]) {
            pgl.color_buffer[y][x] = color;
            pgl.depth_buffer[y][x] = z;
        }
    }
}

static void pgl_vertical_line(vec3i p0, vec3i p1, uint16_t color) {
    const int dy = p1.y - p0.y;
    const int dz = p1.z - p0.z;
    const int x = p0.x;

    for (int y = p0.y; y <= p1.y; y++) {
        const int z = dz * (y - p0.y) / dy + p0.z;
        if (z < pgl.depth_buffer[y][x]) {
            pgl.color_buffer[y][x] = color;
            pgl.depth_buffer[y][x] = z;
        }
    }
}

static void pgl_line(vec3i p0, vec3i p1, uint16_t color) {
    int dx = abs(p1.x - p0.x);
    int sx = (p0.x < p1.x) ? 1 : -1;
    int dy = -abs(p1.y - p0.y);
    int sy = (p0.y < p1.y) ? 1 : -1;
    int dz = -abs(p1.z - p0.z);
    int sz = (p0.z < p1.z) ? 1 : -1;

    int ey = dx + dy;
    int ez = dx + dz;

    int x = p0.x;
    int y = p0.y;
    int z = p0.z;

    while (true) {
        if (z < pgl.depth_buffer[y][x]) {
            pgl.color_buffer[y][x] = color;
            pgl.depth_buffer[y][x] = z;
        }

        if (x == p1.x && y == p1.y) {
            break;
        }

        const int two_ey = 2 * ey;
        if (two_ey >= dy) {
            ey += dy;
            x += sx;

            const int two_ez = 2 * ez;
            if (two_ez >= dz) {
                ez += dz;
            }
            if (two_ez <= dx) {
                ez += dx;
                z += sz;
            }
        }
        if (two_ey <= dx) {
            ey += dx;
            y += sy;
        }
    }
}

static void pgl_clamp_coordinates(vec3i* v0, vec3i* v1, vec3i* v2) {
    v0->x = clamp(v0->x, 0, SCREEN_WIDTH-1);
    v0->y = clamp(v0->y, 0, SCREEN_HEIGHT-1);
    v1->x = clamp(v1->x, 0, SCREEN_WIDTH-1);
    v1->y = clamp(v1->y, 0, SCREEN_HEIGHT-1);
    v2->x = clamp(v2->x, 0, SCREEN_WIDTH-1);
    v2->y = clamp(v2->y, 0, SCREEN_HEIGHT-1);
}

static void pgl_wired_triangle(vec3i v0, vec3i v1, vec3i v2, uint16_t color) {
    pgl_clamp_coordinates(&v0, &v1, &v2);
    pgl_line(v0, v1, color);
    pgl_line(v0, v2, color);
    pgl_line(v1, v2, color);
}

static void pgl_filled_triangle(vec3i v0, vec3i v1, vec3i v2, uint16_t color) {
    pgl_clamp_coordinates(&v0, &v1, &v2);
    if (v1.y < v0.y) { swap(&v0, &v1); }
    if (v2.y < v1.y) { swap(&v1, &v2); }
    if (v1.y < v0.y) { swap(&v0, &v1); }

    for (int y = v0.y; y <= v2.y; y++) {
        vec3i left  = {.y = y};
        vec3i right = {.y = y};
                              
        if (y >= v1.y) {
            const int dxl = (v2.y != v1.y) ? (y - v1.y) * (v2.x - v1.x) / (v2.y - v1.y) : 0;
            const int dxr = (v2.y != v0.y) ? (y - v0.y) * (v2.x - v0.x) / (v2.y - v0.y) : 0;
            left.x  = v1.x + dxl;
            right.x = v0.x + dxr;

            const int dzl = (v2.y != v1.y) ? (y - v1.y) * (v2.z - v1.z) / (v2.y - v1.y) : 0;
            const int dzr = (v2.y != v0.y) ? (y - v0.y) * (v2.z - v0.z) / (v2.y - v0.y) : 0;
            left.z  = v1.z + dzl;
            right.z = v0.z + dzr;
        } 
        else {
            const int dxl = (v1.y != v0.y) ? (y - v0.y) * (v1.x - v0.x) / (v1.y - v0.y) : 0;
            const int dxr = (v2.y != v0.y) ? (y - v0.y) * (v2.x - v0.x) / (v2.y - v0.y) : 0;
            left.x  = v0.x + dxl;
            right.x = v0.x + dxr;

            const int dzl = (v1.y != v0.y) ? (y - v0.y) * (v1.z - v0.z) / (v1.y - v0.y) : 0;
            const int dzr = (v2.y != v0.y) ? (y - v0.y) * (v2.z - v0.z) / (v2.y - v0.y) : 0;
            left.z  = v0.z + dzl;
            right.z = v0.z + dzr;
        }

        if (left.x > right.x) {
            swap(&left, &right);
        }
        pgl_horizontal_line(left, right, color);
    }
}

// --------------------------------------------------- TRANSFORMATION MATRICES --------------------------------------------------- // 

void pgl_view(vec3f position, vec3f right, vec3f up, vec3f forward) {
    pgl.view = view(position, right, up, forward);
}

void pgl_projection(float near, float far, float fovw) {
    const float sin_half_fovw = sinf(fovw * 0.5f);
    const float cos_half_fovw = cosf(fovw * 0.5f);
    const float tan_half_fovh = sin_half_fovw / (cos_half_fovw * ASPECT_RATIO);
    const float cos_half_fovh = 1.0f / sqrtf(tan_half_fovh * tan_half_fovh + 1.0f); 
    const float sin_half_fovh = tan_half_fovh * cos_half_fovh;

    pgl.sin_half_fovw = sin_half_fovw;
    pgl.cos_half_fovw = cos_half_fovw;
    pgl.sin_half_fovh = sin_half_fovh;
    pgl.cos_half_fovh = cos_half_fovh;
    pgl.depth_coef = 255.0f / (far - near);
    pgl.projection = perspective(fovw, ASPECT_RATIO, near, far);
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
        for (uint32_t i = 0; i < (SCREEN_HEIGHT*SCREEN_WIDTH/2); i++) {
            ptr[i] = double_color;
        }
    }
    if (buffer_bits & PGL_DEPTH_BUFFER_BIT) {
        uint32_t* ptr = (uint32_t*)pgl.depth_buffer;
        for (uint32_t i = 0; i < (SCREEN_HEIGHT*SCREEN_WIDTH/4); i++) {
            ptr[i] = 0xFFFFFFFFu; // reset to far = 0xFF
        }
    }
    if (buffer_bits & PGL_STENCIL_BUFFER_BIT) {
        uint32_t* ptr = (uint32_t*)pgl.stencil_buffer;
        for (uint32_t i = 0; i < (SCREEN_HEIGHT*SCREEN_WIDTH/32); i++) {
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

// --------------------------------------------------- DRAW --------------------------------------------------- // 

static void pgl_triangle_clip_plane_intersection(const pgl_queue_triangle_t* t, vec4f clip_plane_vector, pgl_vertex_t* v10, pgl_vertex_t* v20) {
    const float d0 = dot_vec4f(t->v0.position, clip_plane_vector);
    const float d1 = dot_vec4f(t->v1.position, clip_plane_vector);
    const float d2 = dot_vec4f(t->v2.position, clip_plane_vector);

    const float a = d0 / (d0 - d1);
    const float b = d0 / (d0 - d2);

    v10->position  = add_vec4f(scale_vec4f(t->v0.position,  1.0f - a), scale_vec4f(t->v1.position,  a));
    v20->position  = add_vec4f(scale_vec4f(t->v0.position,  1.0f - b), scale_vec4f(t->v2.position,  b));
    v10->tex_coord = add_vec2f(scale_vec2f(t->v0.tex_coord, 1.0f - a), scale_vec2f(t->v1.tex_coord, a));
    v20->tex_coord = add_vec2f(scale_vec2f(t->v0.tex_coord, 1.0f - b), scale_vec2f(t->v2.tex_coord, a));
}

// Returns true if the mesh may be visible.
// Returns false if there is no chance that the mesh is visible. 
static bool pgl_broad_phase_clipping(const mesh_t* mesh, mat4f view_model) {
    const vec3f center = lazy_from_homogeneous(mul_mat4f_vec4f(view_model, to_homogeneous(mesh->bounding_volume.center)));
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
static int pgl_narrow_phase_clipping(pgl_queue_triangle_t* triangle, pgl_queue_triangle_t* subtriangles) {
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

void pgl_draw(const mesh_t* mesh) {
    const mat4f view_model = mul_mat4f_mat4f(pgl.view, mesh->transform.model);
    const mat4f projection_view_model = mul_mat4f_mat4f(pgl.projection, view_model);

    if (!pgl_broad_phase_clipping(mesh, view_model)) { return; }

    for (uint32_t i = 0; i < mesh->index_number; i += 3) {
        const vertex_t vertex0 = mesh->vertices[mesh->indices[i]];
        const vertex_t vertex1 = mesh->vertices[mesh->indices[i+1]];
        const vertex_t vertex2 = mesh->vertices[mesh->indices[i+2]];

        // Clip space coordinates
        pgl_queue_triangle_t triangle = {
            {mul_mat4f_vec4f(projection_view_model, to_homogeneous(vertex0.position)), vertex0.tex_coord},
            {mul_mat4f_vec4f(projection_view_model, to_homogeneous(vertex1.position)), vertex1.tex_coord},
            {mul_mat4f_vec4f(projection_view_model, to_homogeneous(vertex2.position)), vertex2.tex_coord},
        };

        pgl_queue_triangle_t subtriangles[16];
        int subtriangle_index = pgl_narrow_phase_clipping(&triangle, subtriangles);

        while (subtriangle_index >= 0) {
            const pgl_queue_triangle_t* subt = subtriangles + subtriangle_index;
            subtriangle_index--;

            // Screen coordinates
            const vec4f sc0 = mul_mat4f_vec4f(pgl.viewport, normalize_homogeneous(subt->v0.position));
            const vec4f sc1 = mul_mat4f_vec4f(pgl.viewport, normalize_homogeneous(subt->v1.position));
            const vec4f sc2 = mul_mat4f_vec4f(pgl.viewport, normalize_homogeneous(subt->v2.position));

            const vec3i v0 = {sc0.x, sc0.y, (pgl.depth_coef * (subt->v0.position.w - pgl.near))};
            const vec3i v1 = {sc1.x, sc1.y, (pgl.depth_coef * (subt->v1.position.w - pgl.near))};
            const vec3i v2 = {sc2.x, sc2.y, (pgl.depth_coef * (subt->v2.position.w - pgl.near))};
        
            // Rasterize
            const uint16_t color = textures[mesh->tex_index][0][0];
            if ((mesh->mesh_enum ^ MESH_RENDER_WIRED) == 0) {
                pgl_wired_triangle(v0, v1, v2, color);
            }
            else if ((mesh->mesh_enum ^ MESH_RENDER_FILLED) == 0) {
                pgl_filled_triangle(v0, v1, v2, color);
            }
        }
    }
}

void pgl_display() {
    lcd_display((uint16_t*)pgl.color_buffer);
}
