
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
    uint16_t color_buffer[SCREEN_HEIGHT][SCREEN_HEIGHT];
    uint8_t depth_buffer[SCREEN_HEIGHT][SCREEN_WIDTH];
    uint8_t stencil_buffer[SCREEN_HEIGHT][SCREEN_WIDTH/8];
    mat4f view;
    mat4f projection;
    mat4f viewport;
    float near;
    float far;
    float fov;
    float depth_coef;
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

// --------------------------------------------------- RASTERIZATION ----------------------------------------------------------- // 

static void pgl_horizontal_line(const vec3i* v0, const vec3i* v1, uint16_t color) {
    const int dx = v1->x - v0->x;
    const int dz = v1->z - v0->z;
    const int y = v0->y;

    for (int x = v0->x; x <= v1->x; x++) {
        int z = dz * (x - v0->x) / dx + v0->z;
        if (z < pgl.depth_buffer[y][x]) {
            pgl.color_buffer[y][x] = color;
            pgl.depth_buffer[y][x] = z;
        }
    }
}

static void pgl_vertical_line(const vec3i* v0, const vec3i* v1, uint16_t color) {
    const int dy = v1->y - v0->y;
    const int dz = v1->z - v0->z;
    const int x = v0->x;

    for (int y = v0->y; y <= v1->y; y++) {
        int z = dz * (y - v0->y) / dy + v0->z;
        if (z < pgl.depth_buffer[y][x]) {
            pgl.color_buffer[y][x] = color;
            pgl.depth_buffer[y][x] = z;
        }
    }
}

// TODO: may not be working, check and fix.
static void pgl_line(const vec3i* v0, const vec3i* v1, uint16_t color) {
    int dx = abs(v1->x - v0->x);
    int sx = (v0->x < v1->x) ? 1 : -1;

    int dy = -abs(v1->y - v0->y);
    int sy = (v0->y < v1->y) ? 1 : -1;

    int dz = -abs(v1->z - v0->z);
    int sz = (v0->z < v1->z) ? 1 : -1;

    int ey = dx + dy;
    int ez = dx + dz;

    int x = v0->x;
    int y = v0->y;
    int z = v0->z;

    while (true) {
        if (z < pgl.depth_buffer[y][x]) {
            pgl.color_buffer[y][x] = color;
            pgl.depth_buffer[y][x] = z;
        }

        if (x == v1->x && y == v1->y) {
            break;
        }

        int ey2 = 2 * ey;
        if (ey2 >= dy) {
            ey += dy;
            x += sx;
        }
        if (ey2 <= dx) {
            ey += dx;
            y += sy;
        }

        int ez2 = 2 * ez;
        if (ez2 >= dz) {
            ez += dz;
        }
        if (ez2 <= dx) {
            ez += dx;
            z += sz;
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

static void pgl_wired_triangle(vec3i* v0, vec3i* v1, vec3i* v2, uint16_t color) {
    pgl_clamp_coordinates(v0, v1, v2);

    pgl_line(v0, v1, color);
    pgl_line(v0, v2, color);
    pgl_line(v1, v2, color);
}

static void pgl_filled_triangle(vec3i* v0, vec3i* v1, vec3i* v2, uint16_t color) {
    pgl_clamp_coordinates(v0, v1, v2);

    if (v1->y < v0->y) {
        swap_vec3i(v0, v1);
    }
    if (v2->y < v1->y) {
        swap_vec3i(v1, v2);
    }
    if (v1->y < v0->y) {
        swap_vec3i(v0, v1);
    }

    for (int y = v0->y; y <= v2->y; y++) {
        vec3i left  = {.y = y};
        vec3i right = {.y = y};
                              
        if (y >= v1->y) {
            const int dxl = (v2->y != v1->y) ? (y - v1->y) * (v2->x - v1->x) / (v2->y - v1->y) : 0;
            const int dxr = (v2->y != v0->y) ? (y - v0->y) * (v2->x - v0->x) / (v2->y - v0->y) : 0;
            left.x  = v1->x + dxl;
            right.x = v0->x + dxr;

            const int dzl = (v2->y != v1->y) ? (y - v1->y) * (v2->z - v1->z) / (v2->y - v1->y) : 0;
            const int dzr = (v2->y != v0->y) ? (y - v0->y) * (v2->z - v0->z) / (v2->y - v0->y) : 0;
            left.z = v1->z + dzl;
            right.z = v0->z + dzr;
        } 
        else {
            const int dxl = (v1->y != v0->y) ? (y - v0->y) * (v1->x - v0->x) / (v1->y - v0->y) : 0;
            const int dxr = (v2->y != v0->y) ? (y - v0->y) * (v2->x - v0->x) / (v2->y - v0->y) : 0;
            left.x  = v0->x + dxl;
            right.x = v0->x + dxr;

            const int dzl = (v1->y != v0->y) ? (y - v0->y) * (v1->z - v0->z) / (v1->y - v0->y) : 0;
            const int dzr = (v2->y != v0->y) ? (y - v0->y) * (v2->z - v0->z) / (v2->y - v0->y) : 0;
            left.z  = v0->z + dzl;
            right.z = v0->z + dzr;
        }

        if (left.x > right.x) {
            swap_vec3i(&left, &right);
        }

        pgl_horizontal_line(&left, &right, color);
    }
}

// -------------------------------------------- TRANSFORMATION MATRICES -------------------------------------------------------- // 

void pgl_view(const vec3f position, const vec3f right, const vec3f up, const vec3f forward) {
    pgl.view = view(position, right, up, forward);
}

void pgl_projection(float near, float far, float fov) {
    pgl.near = near;
    pgl.far = far;
    pgl.fov = fov;
    pgl.depth_coef = 255.0f / (far - near);
    pgl.projection = perspective(fov, ASPECT_RATIO, near, far);
}

void pgl_viewport(int x, int y, uint32_t width, uint32_t height) {
    pgl.viewport = viewport(x, y, width, height);
}

// ------------------------------------------------- INTERNAL STATE ------------------------------------------------------------- // 

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
            ptr[i] = 0xFFFFFFFFu;
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

// ---------------------------------------------------------- DRAW -------------------------------------------------------------- // 

static void pgl_triangle_clip_plane_intersection(const pgl_queue_triangle_t* t, const vec4f clip_plane_vector, vec4f* c10, vec4f* c20) {
    const float d0 = dot_vec4f(t->c0, clip_plane_vector);
    const float d1 = dot_vec4f(t->c1, clip_plane_vector);
    const float d2 = dot_vec4f(t->c2, clip_plane_vector);

    const float a = d0 / (d0 - d1);
    const float b = d0 / (d0 - d2);

    *c10 = add_vec4f(scale_vec4f(t->c0, 1.0f - a), scale_vec4f(t->c1, a));
    *c20 = add_vec4f(scale_vec4f(t->c0, 1.0f - b), scale_vec4f(t->c2, b));
}

// Returns true if the mesh may be visible.
// Returns false if there is no chance that the mesh is visible. 
static bool pgl_broad_phase_clipping(const mesh_t* mesh, const mat4f view_model) {
    // TODO: it is assumed that the window is square, fix it.
    // TODO: it is better to use lazy_from_homogeneous if view_model is guaranteed to have w = 1.0f.
    const vec3f center = from_homogeneous(mul_mat4f_vec4f(view_model, to_homogeneous(mesh->bounding_volume.center)));
    const float minus_s = -sinf(pgl.fov * 0.5f);
    const float c       =  cosf(pgl.fov * 0.5f);

    const plane_t left   = {{   c, 0.0f, minus_s}, 0.0f};
    const plane_t right  = {{  -c, 0.0f, minus_s}, 0.0f};
    const plane_t bottom = {{0.0f,    c, minus_s}, 0.0f};
    const plane_t top    = {{0.0f,   -c, minus_s}, 0.0f};

    return (plane_signed_distance(left,   center) >= -mesh->bounding_volume.radius &&
            plane_signed_distance(right,  center) >= -mesh->bounding_volume.radius &&
            plane_signed_distance(bottom, center) >= -mesh->bounding_volume.radius &&
            plane_signed_distance(top,    center) >= -mesh->bounding_volume.radius);
}

// Clips the triangle with respect to near, far, left, right, bottom, and top planes.
static int pgl_triangle_clipping(pgl_queue_triangle_t* triangle, pgl_queue_triangle_t* subtriangles) {
    pgl_queue_t queue;
    triangle_queue_init(&queue);
    triangle_queue_push(&queue, triangle);

    int subtriangle_index = -1;
    vec4f c10;
    vec4f c20;
    
    // Near Clip: Z + W > 0
    while (!queue.empty) {
        pgl_queue_triangle_t* t = triangle_queue_pop(&queue);
        const int in0 = t->c0.z > -t->c0.w;
        const int in1 = t->c1.z > -t->c1.w;
        const int in2 = t->c2.z > -t->c2.w;
        const int in_number = in0 + in1 + in2;

        if (in_number == 3) {
            subtriangles[++subtriangle_index] = *t;
        }
        else if (in_number == 2) {
            // Ensure that c0 is the vertex that is not in
            if      (!in1) { swap_vec4f(&t->c0, &t->c1); }
            else if (!in2) { swap_vec4f(&t->c0, &t->c2); }

            const vec4f v = (vec4f){0.0f, 0.0f, 1.0f, 1.0f};
            pgl_triangle_clip_plane_intersection(t, v, &c10, &c20);
            subtriangles[++subtriangle_index] = (pgl_queue_triangle_t){c10, t->c1, t->c2};
            subtriangles[++subtriangle_index] = (pgl_queue_triangle_t){c10,   c20, t->c2};
        }
        else if (in_number == 1) {
            // Ensure that c0 is the vertex that is in
            if      (in1) { swap_vec4f(&t->c0, &t->c1); }
            else if (in2) { swap_vec4f(&t->c0, &t->c2); }

            const vec4f v = (vec4f){0.0f, 0.0f, 1.0f, 1.0f};
            pgl_triangle_clip_plane_intersection(t, v, &c10, &c20);
            subtriangles[++subtriangle_index] = (pgl_queue_triangle_t){t->c0, c10, c20};
        }
    }

    while (subtriangle_index >= 0) {
        triangle_queue_push(&queue, subtriangles + subtriangle_index);
        subtriangle_index--;
    }

    // Far Clip: Z - W < 0
    while (!queue.empty) {
        pgl_queue_triangle_t* t = triangle_queue_pop(&queue);
        const int in0 = t->c0.z < t->c0.w;
        const int in1 = t->c1.z < t->c1.w;
        const int in2 = t->c2.z < t->c2.w;
        const int in_number = in0 + in1 + in2;

        if (in_number == 3) {
            subtriangles[++subtriangle_index] = *t;
        }
        else if (in_number == 2) {
            // Ensure that c0 is the vertex that is not in
            if      (!in1) { swap_vec4f(&t->c0, &t->c1); }
            else if (!in2) { swap_vec4f(&t->c0, &t->c2); }

            const vec4f v = (vec4f){0.0f, 0.0f, 1.0f, -1.0f};
            pgl_triangle_clip_plane_intersection(t, v, &c10, &c20);
            subtriangles[++subtriangle_index] = (pgl_queue_triangle_t){c10, t->c1, t->c2};
            subtriangles[++subtriangle_index] = (pgl_queue_triangle_t){c10,   c20, t->c2};
        }
        else if (in_number == 1) {
            // Ensure that c0 is the vertex that is in
            if      (in1) { swap_vec4f(&t->c0, &t->c1); }
            else if (in2) { swap_vec4f(&t->c0, &t->c2); }

            const vec4f v = (vec4f){0.0f, 0.0f, 1.0f, -1.0f};
            pgl_triangle_clip_plane_intersection(t, v, &c10, &c20);
            subtriangles[++subtriangle_index] = (pgl_queue_triangle_t){t->c0, c10, c20};
        }
    }

    while (subtriangle_index >= 0) {
        triangle_queue_push(&queue, subtriangles + subtriangle_index);
        subtriangle_index--;
    }

    // Left Clip: X + W > 0
    while (!queue.empty) {
        pgl_queue_triangle_t* t = triangle_queue_pop(&queue);
        const int in0 = t->c0.x > -t->c0.w;
        const int in1 = t->c1.x > -t->c1.w;
        const int in2 = t->c2.x > -t->c2.w;
        const int in_number = in0 + in1 + in2;

        if (in_number == 3) {
            subtriangles[++subtriangle_index] = *t;
        }
        else if (in_number == 2) {
            // Ensure that c0 is the vertex that is not in
            if      (!in1) { swap_vec4f(&t->c0, &t->c1); }
            else if (!in2) { swap_vec4f(&t->c0, &t->c2); }

            const vec4f v = (vec4f){1.0f, 0.0f, 0.0f, 1.0f};
            pgl_triangle_clip_plane_intersection(t, v, &c10, &c20);
            subtriangles[++subtriangle_index] = (pgl_queue_triangle_t){c10, t->c1, t->c2};
            subtriangles[++subtriangle_index] = (pgl_queue_triangle_t){c10,   c20, t->c2};
        }
        else if (in_number == 1) {
            // Ensure that c0 is the vertex that is in
            if      (in1) { swap_vec4f(&t->c0, &t->c1); }
            else if (in2) { swap_vec4f(&t->c0, &t->c2); }

            const vec4f v = (vec4f){1.0f, 0.0f, 0.0f, 1.0f};
            pgl_triangle_clip_plane_intersection(t, v, &c10, &c20);
            subtriangles[++subtriangle_index] = (pgl_queue_triangle_t){t->c0, c10, c20};
        }
    }

    while (subtriangle_index >= 0) {
        triangle_queue_push(&queue, subtriangles + subtriangle_index);
        subtriangle_index--;
    }

    // Right Clip: X - W < 0
    while (!queue.empty) {
        pgl_queue_triangle_t* t = triangle_queue_pop(&queue);
        const int in0 = t->c0.x < t->c0.w;
        const int in1 = t->c1.x < t->c1.w;
        const int in2 = t->c2.x < t->c2.w;
        const int in_number = in0 + in1 + in2;

        if (in_number == 3) {
            subtriangles[++subtriangle_index] = *t;
        }
        else if (in_number == 2) {
            // Ensure that c0 is the vertex that is not in
            if      (!in1) { swap_vec4f(&t->c0, &t->c1); }
            else if (!in2) { swap_vec4f(&t->c0, &t->c2); }

            const vec4f v = (vec4f){1.0f, 0.0f, 0.0f, -1.0f};
            pgl_triangle_clip_plane_intersection(t, v, &c10, &c20);
            subtriangles[++subtriangle_index] = (pgl_queue_triangle_t){c10, t->c1, t->c2};
            subtriangles[++subtriangle_index] = (pgl_queue_triangle_t){c10,   c20, t->c2};
        }
        else if (in_number == 1) {
            // Ensure that c0 is the vertex that is in
            if      (in1) { swap_vec4f(&t->c0, &t->c1); }
            else if (in2) { swap_vec4f(&t->c0, &t->c2); }

            const vec4f v = (vec4f){1.0f, 0.0f, 0.0f, -1.0f};
            pgl_triangle_clip_plane_intersection(t, v, &c10, &c20);
            subtriangles[++subtriangle_index] = (pgl_queue_triangle_t){t->c0, c10, c20};
        }
    }

    while (subtriangle_index >= 0) {
        triangle_queue_push(&queue, subtriangles + subtriangle_index);
        subtriangle_index--;
    }

    // Bottom Clip: Y + W > 0
    while (!queue.empty) {
        pgl_queue_triangle_t* t = triangle_queue_pop(&queue);
        const int in0 = t->c0.y > -t->c0.w;
        const int in1 = t->c1.y > -t->c1.w;
        const int in2 = t->c2.y > -t->c2.w;
        const int in_number = in0 + in1 + in2;

        if (in_number == 3) {
            subtriangles[++subtriangle_index] = *t;
        }
        else if (in_number == 2) {
            // Ensure that c0 is the vertex that is not in
            if      (!in1) { swap_vec4f(&t->c0, &t->c1); }
            else if (!in2) { swap_vec4f(&t->c0, &t->c2); }

            const vec4f v = (vec4f){0.0f, 1.0f, 0.0f, 1.0f};
            pgl_triangle_clip_plane_intersection(t, v, &c10, &c20);
            subtriangles[++subtriangle_index] = (pgl_queue_triangle_t){c10, t->c1, t->c2};
            subtriangles[++subtriangle_index] = (pgl_queue_triangle_t){c10,   c20, t->c2};
        }
        else if (in_number == 1) {
            // Ensure that c0 is the vertex that is in
            if      (in1) { swap_vec4f(&t->c0, &t->c1); }
            else if (in2) { swap_vec4f(&t->c0, &t->c2); }

            const vec4f v = (vec4f){0.0f, 1.0f, 0.0f, 1.0f};
            pgl_triangle_clip_plane_intersection(t, v, &c10, &c20);
            subtriangles[++subtriangle_index] = (pgl_queue_triangle_t){t->c0, c10, c20};
        }
    }

    while (subtriangle_index >= 0) {
        triangle_queue_push(&queue, subtriangles + subtriangle_index);
        subtriangle_index--;
    }

    // Top Clip: Y - W < 0
    while (!queue.empty) {
        pgl_queue_triangle_t* t = triangle_queue_pop(&queue);
        const int in0 = t->c0.y < t->c0.w;
        const int in1 = t->c1.y < t->c1.w;
        const int in2 = t->c2.y < t->c2.w;
        const int in_number = in0 + in1 + in2;

        if (in_number == 3) {
            subtriangles[++subtriangle_index] = *t;
        }
        else if (in_number == 2) {
            // Ensure that c0 is the vertex that is not in
            if      (!in1) { swap_vec4f(&t->c0, &t->c1); }
            else if (!in2) { swap_vec4f(&t->c0, &t->c2); }

            const vec4f v = (vec4f){0.0f, 1.0f, 0.0f, -1.0f};
            pgl_triangle_clip_plane_intersection(t, v, &c10, &c20);
            subtriangles[++subtriangle_index] = (pgl_queue_triangle_t){c10, t->c1, t->c2};
            subtriangles[++subtriangle_index] = (pgl_queue_triangle_t){c10,   c20, t->c2};
        }
        else if (in_number == 1) {
            // Ensure that c0 is the vertex that is in
            if      (in1) { swap_vec4f(&t->c0, &t->c1); }
            else if (in2) { swap_vec4f(&t->c0, &t->c2); }

            const vec4f v = (vec4f){0.0f, 1.0f, 0.0f, -1.0f};
            pgl_triangle_clip_plane_intersection(t, v, &c10, &c20);
            subtriangles[++subtriangle_index] = (pgl_queue_triangle_t){t->c0, c10, c20};
        }
    }
    
    return subtriangle_index;
}

void pgl_draw(const mesh_t* mesh) {
    const mat4f view_model = mul_mat4f_mat4f(pgl.view, mesh->model);
    const mat4f projection_view_model = mul_mat4f_mat4f(pgl.projection, view_model);

    // if (!pgl_broad_phase_clipping(mesh, view_model)) { return; }
    
    pgl_queue_triangle_t subtriangles[16];

    for (uint32_t i = 0; i < mesh->index_number; i += 3) {
        // Homogeneous local space coordinates
        const vec4f p0 = to_homogeneous(mesh->vertices[mesh->indices[i]].position);
        const vec4f p1 = to_homogeneous(mesh->vertices[mesh->indices[i+1]].position);
        const vec4f p2 = to_homogeneous(mesh->vertices[mesh->indices[i+2]].position);

        // Clip space coordinates
        pgl_queue_triangle_t triangle = {
            .c0 = mul_mat4f_vec4f(projection_view_model, p0),
            .c1 = mul_mat4f_vec4f(projection_view_model, p1),
            .c2 = mul_mat4f_vec4f(projection_view_model, p2),
        };

        int subtriangle_index = pgl_triangle_clipping(&triangle, subtriangles);

        while (subtriangle_index >= 0) {
            pgl_queue_triangle_t* subt = subtriangles + subtriangle_index;
            subtriangle_index--;

            // Screen coordinates
            const vec4f sc0 = mul_mat4f_vec4f(pgl.viewport, normalize_homogeneous(subt->c0));
            const vec4f sc1 = mul_mat4f_vec4f(pgl.viewport, normalize_homogeneous(subt->c1));
            const vec4f sc2 = mul_mat4f_vec4f(pgl.viewport, normalize_homogeneous(subt->c2));

            vec3i v0 = {(int)sc0.x, (int)sc0.y, (int)(pgl.depth_coef * (subt->c0.w - pgl.near))};
            vec3i v1 = {(int)sc1.x, (int)sc1.y, (int)(pgl.depth_coef * (subt->c1.w - pgl.near))};
            vec3i v2 = {(int)sc2.x, (int)sc2.y, (int)(pgl.depth_coef * (subt->c2.w - pgl.near))};
        
            // Rasterize
            if ((mesh->mesh_enum ^ MESH_RENDER_WIRED) == 0) {
                pgl_wired_triangle(&v0, &v1, &v2, vec3f_to_rgb565(mesh->vertices[0].color));
            }
            else if ((mesh->mesh_enum ^ MESH_RENDER_FILLED) == 0) {
                pgl_filled_triangle(&v0, &v1, &v2, vec3f_to_rgb565(mesh->vertices[0].color));
            }
        }
    }
}

void pgl_display() {
    lcd_display((uint16_t*)pgl.color_buffer);
}
