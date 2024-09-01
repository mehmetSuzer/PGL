
#include "pgl.h"
#include "triangle_queue.h"

typedef struct {
    uint16_t color_buffer[SCREEN_HEIGHT][SCREEN_HEIGHT];
    uint8_t depth_buffer[SCREEN_HEIGHT][SCREEN_WIDTH];
    triangle_queue_t queue;
    mat4f view;
    mat4f projection;
    mat4f viewport;
    float near;
    float far;
    uint16_t clear_color;
} pgl_t;

pgl_t pgl = {
    .color_buffer = {{0x0000u}},
    .depth_buffer = {{0x00u}},
    .clear_color = 0x0000u,
    .near = 0.1f,
    .far = 30.0f,
};

// ------------------------------------------------ RASTERIZATION -------------------------------------------------------------- // 

static void pgl_horizontal_line(int x0, int x1, int y, uint16_t color) {
    for (int x = x0; x <= x1; x++) {
        pgl.color_buffer[y][x] = color;
    }
}

static void pgl_vertical_line(int y0, int y1, int x, uint16_t color) {
    for (int y = y0; y <= y1; y++) {
        pgl.color_buffer[y][x] = color;
    }
}

// Draws a line where x0 < x1 and the slope is in [-1, 1]
static void pgl_line_low(vec3i* v0, vec3i* v1, uint16_t color) {    
    int dx = v1->x - v0->x;
    int dy = v1->y - v0->y;
    int yi = 1;

    if (dy < 0) {
        yi = -1;
        dy = -dy;
    }

    int dyy = 2 * dy - dx;
    int y = v0->y;

    for (int x = v0->x; x <= v1->x; x++) {
        pgl.color_buffer[y][x] = color;
        if (dyy > 0) {
            y += yi;
            dyy += 2 * (dy - dx);
        }
        else {
            dyy += 2 * dy;
        }
    }
}

// Draws a line where x0 < x1 and the slope is in [-inf, -1] U [1, inf]
static void pgl_line_high(vec3i* v0, vec3i* v1, uint16_t color) {
    int dx = v1->x - v0->x;
    int dy = v1->y - v0->y;
    int xi = 1;

    if (dx < 0) {
        xi = -1;
        dx = -dx;
    }

    int dxx = 2 * dx - dy;
    int x = v0->x;

    for (int y = v0->y; y <= v1->y; y++) {
        pgl.color_buffer[y][x] = color;
        if (dxx > 0) {
            x += xi;
            dxx += 2 * (dx - dy);
        }
        else {
            dxx += 2 * dx;
        }
    }
}

// Draws a line and handles all cases
static void pgl_line(vec3i* v0, vec3i* v1, uint16_t color) {
    if (abs(v1->y - v0->y) < abs(v1->x - v0->x)) {
        if (v0->x > v1->x) {
            pgl_line_low(v1, v0, color);
        }
        else {
            pgl_line_low(v0, v1, color);
        }
    }
    else {
        if (v0->y > v1->y) {
            pgl_line_high(v1, v0, color);
        }
        else {
            pgl_line_high(v0, v1, color);
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
        int x_left;
        int x_right;

        if (y >= v1->y) {
            const int dxl = (v2->y != v1->y) ? (y - v1->y) * (v2->x - v1->x) / (v2->y - v1->y) : 0;
            const int dxr = (v2->y != v0->y) ? (y - v0->y) * (v2->x - v0->x) / (v2->y - v0->y) : 0;
            x_left  = v1->x + dxl;
            x_right = v0->x + dxr;
        } 
        else {
            const int dxl = (v1->y != v0->y) ? (y - v0->y) * (v1->x - v0->x) / (v1->y - v0->x) : 0;
            const int dxr = (v2->y != v0->y) ? (y - v0->y) * (v2->x - v0->x) / (v2->y - v0->x) : 0;
            x_left  = v0->x + dxl;
            x_right = v0->x + dxr;
        }

        if (x_left > x_right) {
            const int temp = x_left;
            x_left = x_right;
            x_right = temp;
        }

        for (int x = x_left; x <= x_right; x++) {
            pgl.color_buffer[y][x] = color;
        }
    }
}

// -------------------------------------------- TRANSFORMATION MATRICES --------------------------------------------------------- // 

void pgl_view(const vec3f position, const vec3f right, const vec3f up, const vec3f forward) {
    pgl.view = view(position, right, up, forward);
}

void pgl_projection(float near, float far, float fov) {
    pgl.near = near;
    pgl.far = far;
    pgl.projection = perspective(fov, ASPECT_RATIO, near, far);
}

void pgl_viewport(int x, int y, uint32_t width, uint32_t height) {
    pgl.viewport = viewport(x, y, width, height);
}

void pgl_clear_color(uint16_t color) {
    pgl.clear_color = color;
}

void pgl_clear(pgl_buffer_bit_t buffer_bits) {
    const uint32_t double_color = (pgl.clear_color << 16) | pgl.clear_color;
    if (buffer_bits & COLOR_BUFFER_BIT) {
        uint32_t* ptr = (uint32_t*)pgl.color_buffer;
        for (uint32_t i = 0; i < (SCREEN_HEIGHT*SCREEN_WIDTH/2); i++) {
            ptr[i] = double_color;
        }
    }
    if (buffer_bits & DEPTH_BUFFER_BIT) {
        uint32_t* ptr = (uint32_t*)pgl.depth_buffer;
        for (uint32_t i = 0; i < (SCREEN_HEIGHT*SCREEN_WIDTH/4); i++) {
            ptr[i] = 0xFFFFFFFFu; // reset to far = 0xFF
        }
    }
}

// ----------------------------------------------- DRAW ------------------------------------------------------------ // 

static void pgl_triangle_clip_plane_intersection(triangle_t* t, const vec4f clip_plane_vector, vec4f* c10, vec4f* c20) {
    const float d0 = dot_vec4f(t->c0, clip_plane_vector); // TODO: clip plane vector can be sent with a pointer
    const float d1 = dot_vec4f(t->c1, clip_plane_vector);
    const float d2 = dot_vec4f(t->c2, clip_plane_vector);

    const float a = d0 / (d0 - d1);
    const float b = d0 / (d0 - d2);

    *c10 = add_vec4f(scale_vec4f(t->c0, 1.0f - a), scale_vec4f(t->c1, a));
    *c20 = add_vec4f(scale_vec4f(t->c0, 1.0f - b), scale_vec4f(t->c2, b));
}

// Maps non-linear [0, 1] screen space depth range to linear [0, 255] range.
static int pgl_depth_value(float screen_space_depth) {
    return (int) ((255.0f * pgl.near * screen_space_depth) / (pgl.far - (pgl.far - pgl.near) * screen_space_depth));
}

void pgl_draw(const mesh_t* mesh) {
    const mat4f projection_view_model = mul_mat4f_mat4f(pgl.projection, mul_mat4f_mat4f(pgl.view, mesh->model));

    // TODO: Broad Phase Clipping

    triangle_t subtriangles[16];
    int subtriangle_index;
    vec4f c10;
    vec4f c20;

    for (uint32_t i = 0; i < mesh->index_number; i += 3) {
        // Homogeneous local space coordinates
        const vec4f p0 = to_homogeneous(mesh->vertices[mesh->indices[i]].position);
        const vec4f p1 = to_homogeneous(mesh->vertices[mesh->indices[i+1]].position);
        const vec4f p2 = to_homogeneous(mesh->vertices[mesh->indices[i+2]].position);

        // Clip space coordinates
        triangle_t triangle = {
            .c0 = mul_mat4f_vec4f(projection_view_model, p0),
            .c1 = mul_mat4f_vec4f(projection_view_model, p1),
            .c2 = mul_mat4f_vec4f(projection_view_model, p2),
        };

        triangle_queue_init(&pgl.queue);
        triangle_queue_push(&pgl.queue, &triangle);
        subtriangle_index = -1;

        // Near Clip: Z + W > 0
        while (!pgl.queue.empty) {
            triangle_t t = *triangle_queue_pop(&pgl.queue);
            const int in0 = t.c0.z > -t.c0.w; // TODO: can be written as dot(t.c0, clip_plane_vector)
            const int in1 = t.c1.z > -t.c1.w;
            const int in2 = t.c2.z > -t.c2.w;
            const int in_number = in0 + in1 + in2;

            if (in_number == 3) {
                subtriangles[++subtriangle_index] = t;
            }
            else if (in_number == 2) {
                // Ensure that c0 is the vertex that is not in
                if      (!in1) { swap_vec4f(&t.c0, &t.c1); }
                else if (!in2) { swap_vec4f(&t.c0, &t.c2); }

                const vec4f v = (vec4f) {0.0f, 0.0f, 1.0f, 1.0f};
                pgl_triangle_clip_plane_intersection(&t, v, &c10, &c20);
                subtriangles[++subtriangle_index] = (triangle_t) {c10, t.c1, t.c2};
                subtriangles[++subtriangle_index] = (triangle_t) {c10,  c20, t.c2};
            }
            else if (in_number == 1) {
                // Ensure that c0 is the vertex that is in
                if      (in1) { swap_vec4f(&t.c0, &t.c1); }
                else if (in2) { swap_vec4f(&t.c0, &t.c2); }

                const vec4f v = (vec4f) {0.0f, 0.0f, 1.0f, 1.0f};
                pgl_triangle_clip_plane_intersection(&t, v, &c10, &c20);
                subtriangles[++subtriangle_index] = (triangle_t) {t.c0, c10, c20};
            }
        }

        while (subtriangle_index >= 0) {
            triangle_queue_push(&pgl.queue, subtriangles + subtriangle_index);
            subtriangle_index--;
        }

        // Far Clip: Z - W < 0
        while (!pgl.queue.empty) {
            triangle_t t = *triangle_queue_pop(&pgl.queue);
            const int in0 = t.c0.z < t.c0.w;
            const int in1 = t.c1.z < t.c1.w;
            const int in2 = t.c2.z < t.c2.w;
            const int in_number = in0 + in1 + in2;

            if (in_number == 3) {
                subtriangles[++subtriangle_index] = t;
            }
            else if (in_number == 2) {
                // Ensure that c0 is the vertex that is not in
                if      (!in1) { swap_vec4f(&t.c0, &t.c1); }
                else if (!in2) { swap_vec4f(&t.c0, &t.c2); }

                const vec4f v = (vec4f) {0.0f, 0.0f, 1.0f, -1.0f};
                pgl_triangle_clip_plane_intersection(&t, v, &c10, &c20);
                subtriangles[++subtriangle_index] = (triangle_t) {c10, t.c1, t.c2};
                subtriangles[++subtriangle_index] = (triangle_t) {c10,  c20, t.c2};
            }
            else if (in_number == 1) {
                // Ensure that c0 is the vertex that is in
                if      (in1) { swap_vec4f(&t.c0, &t.c1); }
                else if (in2) { swap_vec4f(&t.c0, &t.c2); }

                const vec4f v = (vec4f) {0.0f, 0.0f, 1.0f, -1.0f};
                pgl_triangle_clip_plane_intersection(&t, v, &c10, &c20);
                subtriangles[++subtriangle_index] = (triangle_t) {t.c0, c10, c20};
            }
        }

        while (subtriangle_index >= 0) {
            triangle_queue_push(&pgl.queue, subtriangles + subtriangle_index);
            subtriangle_index--;
        }

        // Left Clip: X + W > 0
        while (!pgl.queue.empty) {
            triangle_t t = *triangle_queue_pop(&pgl.queue);
            const int in0 = t.c0.x > -t.c0.w;
            const int in1 = t.c1.x > -t.c1.w;
            const int in2 = t.c2.x > -t.c2.w;
            const int in_number = in0 + in1 + in2;

            if (in_number == 3) {
                subtriangles[++subtriangle_index] = t;
            }
            else if (in_number == 2) {
                // Ensure that c0 is the vertex that is not in
                if      (!in1) { swap_vec4f(&t.c0, &t.c1); }
                else if (!in2) { swap_vec4f(&t.c0, &t.c2); }

                const vec4f v = (vec4f) {1.0f, 0.0f, 0.0f, 1.0f};
                pgl_triangle_clip_plane_intersection(&t, v, &c10, &c20);
                subtriangles[++subtriangle_index] = (triangle_t) {c10, t.c1, t.c2};
                subtriangles[++subtriangle_index] = (triangle_t) {c10,  c20, t.c2};
            }
            else if (in_number == 1) {
                // Ensure that c0 is the vertex that is in
                if      (in1) { swap_vec4f(&t.c0, &t.c1); }
                else if (in2) { swap_vec4f(&t.c0, &t.c2); }

                const vec4f v = (vec4f) {1.0f, 0.0f, 0.0f, 1.0f};
                pgl_triangle_clip_plane_intersection(&t, v, &c10, &c20);
                subtriangles[++subtriangle_index] = (triangle_t) {t.c0, c10, c20};
            }
        }

        while (subtriangle_index >= 0) {
            triangle_queue_push(&pgl.queue, subtriangles + subtriangle_index);
            subtriangle_index--;
        }

        // Right Clip: X - W < 0
        while (!pgl.queue.empty) {
            triangle_t t = *triangle_queue_pop(&pgl.queue);
            const int in0 = t.c0.x < t.c0.w;
            const int in1 = t.c1.x < t.c1.w;
            const int in2 = t.c2.x < t.c2.w;
            const int in_number = in0 + in1 + in2;

            if (in_number == 3) {
                subtriangles[++subtriangle_index] = t;
            }
            else if (in_number == 2) {
                // Ensure that c0 is the vertex that is not in
                if      (!in1) { swap_vec4f(&t.c0, &t.c1); }
                else if (!in2) { swap_vec4f(&t.c0, &t.c2); }

                const vec4f v = (vec4f) {1.0f, 0.0f, 0.0f, -1.0f};
                pgl_triangle_clip_plane_intersection(&t, v, &c10, &c20);
                subtriangles[++subtriangle_index] = (triangle_t) {c10, t.c1, t.c2};
                subtriangles[++subtriangle_index] = (triangle_t) {c10,  c20, t.c2};
            }
            else if (in_number == 1) {
                // Ensure that c0 is the vertex that is in
                if      (in1) { swap_vec4f(&t.c0, &t.c1); }
                else if (in2) { swap_vec4f(&t.c0, &t.c2); }

                const vec4f v = (vec4f) {1.0f, 0.0f, 0.0f, -1.0f};
                pgl_triangle_clip_plane_intersection(&t, v, &c10, &c20);
                subtriangles[++subtriangle_index] = (triangle_t) {t.c0, c10, c20};
            }
        }

        while (subtriangle_index >= 0) {
            triangle_queue_push(&pgl.queue, subtriangles + subtriangle_index);
            subtriangle_index--;
        }

        // Bottom Clip: Y + W > 0
        while (!pgl.queue.empty) {
            triangle_t t = *triangle_queue_pop(&pgl.queue);
            const int in0 = t.c0.y > -t.c0.w;
            const int in1 = t.c1.y > -t.c1.w;
            const int in2 = t.c2.y > -t.c2.w;
            const int in_number = in0 + in1 + in2;

            if (in_number == 3) {
                subtriangles[++subtriangle_index] = t;
            }
            else if (in_number == 2) {
                // Ensure that c0 is the vertex that is not in
                if      (!in1) { swap_vec4f(&t.c0, &t.c1); }
                else if (!in2) { swap_vec4f(&t.c0, &t.c2); }

                const vec4f v = (vec4f) {0.0f, 1.0f, 0.0f, 1.0f};
                pgl_triangle_clip_plane_intersection(&t, v, &c10, &c20);
                subtriangles[++subtriangle_index] = (triangle_t) {c10, t.c1, t.c2};
                subtriangles[++subtriangle_index] = (triangle_t) {c10,  c20, t.c2};
            }
            else if (in_number == 1) {
                // Ensure that c0 is the vertex that is in
                if      (in1) { swap_vec4f(&t.c0, &t.c1); }
                else if (in2) { swap_vec4f(&t.c0, &t.c2); }

                const vec4f v = (vec4f) {0.0f, 1.0f, 0.0f, 1.0f};
                pgl_triangle_clip_plane_intersection(&t, v, &c10, &c20);
                subtriangles[++subtriangle_index] = (triangle_t) {t.c0, c10, c20};
            }
        }

        while (subtriangle_index >= 0) {
            triangle_queue_push(&pgl.queue, subtriangles + subtriangle_index);
            subtriangle_index--;
        }

        // Top Clip: Y - W < 0
        while (!pgl.queue.empty) {
            triangle_t t = *triangle_queue_pop(&pgl.queue);
            const int in0 = t.c0.y < t.c0.w;
            const int in1 = t.c1.y < t.c1.w;
            const int in2 = t.c2.y < t.c2.w;
            const int in_number = in0 + in1 + in2;

            if (in_number == 3) {
                subtriangles[++subtriangle_index] = t;
            }
            else if (in_number == 2) {
                // Ensure that c0 is the vertex that is not in
                if      (!in1) { swap_vec4f(&t.c0, &t.c1); }
                else if (!in2) { swap_vec4f(&t.c0, &t.c2); }

                const vec4f v = (vec4f) {0.0f, 1.0f, 0.0f, -1.0f};
                pgl_triangle_clip_plane_intersection(&t, v, &c10, &c20);
                subtriangles[++subtriangle_index] = (triangle_t) {c10, t.c1, t.c2};
                subtriangles[++subtriangle_index] = (triangle_t) {c10,  c20, t.c2};
            }
            else if (in_number == 1) {
                // Ensure that c0 is the vertex that is in
                if      (in1) { swap_vec4f(&t.c0, &t.c1); }
                else if (in2) { swap_vec4f(&t.c0, &t.c2); }

                const vec4f v = (vec4f) {0.0f, 1.0f, 0.0f, -1.0f};
                pgl_triangle_clip_plane_intersection(&t, v, &c10, &c20);
                subtriangles[++subtriangle_index] = (triangle_t) {t.c0, c10, c20};
            }
        }

        while (subtriangle_index >= 0) {
            triangle_t* subt = subtriangles + subtriangle_index;
            subtriangle_index--;

            // Screen coordinates
            const vec4f sc0 = mul_mat4f_vec4f(pgl.viewport, normalize_homogeneous(subt->c0));
            const vec4f sc1 = mul_mat4f_vec4f(pgl.viewport, normalize_homogeneous(subt->c1));
            const vec4f sc2 = mul_mat4f_vec4f(pgl.viewport, normalize_homogeneous(subt->c2));

            vec3i v0 = {(int)sc0.x, (int)sc0.y, pgl_depth_value(sc0.z)};
            vec3i v1 = {(int)sc1.x, (int)sc1.y, pgl_depth_value(sc1.z)};
            vec3i v2 = {(int)sc2.x, (int)sc2.y, pgl_depth_value(sc2.z)};
            printf("%d, %d, %d\n", v0.z, v1.z, v2.z);
        
            // Rasterize
            if (mesh->render_type == RENDER_WIRED) {
                pgl_wired_triangle(&v0, &v1, &v2, 0xF100u);
            }
            else if (mesh->render_type == RENDER_FILLED) {
                pgl_filled_triangle(&v0, &v1, &v2, 0xF100u);
            }
        }
    }
}

void pgl_display() {
    lcd_display((uint16_t*)pgl.color_buffer);
}
