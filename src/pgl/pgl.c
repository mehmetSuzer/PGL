
#include "pgl.h"
#include "triangle_queue.h"

typedef struct {
    uint16_t screen[SCREEN_HEIGHT][SCREEN_HEIGHT];
    TriangleQueue queue;
    mat4f view;
    mat4f projection;
    mat4f viewport;
} PGL;

PGL pgl = {
    .screen = {{0x0000}},
};

// ------------------------------------------------ RASTERIZATION -------------------------------------------------------------- // 

static void pgl_horizontal_line(int x0, int x1, int y, uint16_t color) {
    for (int x = x0; x <= x1; x++) {
        pgl.screen[y][x] = color;
    }
}

static void pgl_vertical_line(int y0, int y1, int x, uint16_t color) {
    for (int y = y0; y <= y1; y++) {
        pgl.screen[y][x] = color;
    }
}

// Draws a line where x0 < x1 and the slope is in [-1, 1]
static void pgl_line_low(int x0, int y0, int x1, int y1, uint16_t color) {    
    int dx = x1 - x0; // 120
    int dy = y1 - y0; // 0
    int yi = 1;

    if (dy < 0) {
        yi = -1;
        dy = -dy;
    }

    int d = 2*dy - dx; // -120
    int y = y0; // 180

    for (int x = x0; x <= x1; x++) {
        pgl.screen[y][x] = color;
        if (d > 0) {
            y += yi;
            d += 2 * (dy - dx);
        }
        else {
            d += 2*dy;
        }
    }
}

// Draws a line where x0 < x1 and the slope is in [-inf, -1] U [1, inf]
static void pgl_line_high(int x0, int y0, int x1, int y1, uint16_t color) {
    int dx = x1 - x0;
    int dy = y1 - y0;
    int xi = 1;

    if (dx < 0) {
        xi = -1;
        dx = -dx;
    }

    int d = 2*dx - dy;
    int x = x0;

    for (int y = y0; y <= y1; y++) {
        pgl.screen[y][x] = color;
        if (d > 0) {
            x += xi;
            d += 2 * (dx - dy);
        }
        else {
            d += 2*dx;
        }
    }
}

// Draws a line and handles all cases
static void pgl_line(int x0, int y0, int x1, int y1, uint16_t color) {
    if (abs(y1 - y0) < abs(x1 - x0)) {
        if (x0 > x1) {
            pgl_line_low(x1, y1, x0, y0, color);
        }
        else {
            pgl_line_low(x0, y0, x1, y1, color);
        }
    }
    else {
        if (y0 > y1) {
            pgl_line_high(x1, y1, x0, y0, color);
        }
        else {
            pgl_line_high(x0, y0, x1, y1, color);
        }
    }
}

static void clamp_coordinates(int* x0, int* y0, int* x1, int* y1, int* x2, int* y2) {
    *x0 = clamp(*x0, 0, SCREEN_WIDTH-1);
    *x1 = clamp(*x1, 0, SCREEN_WIDTH-1);
    *x2 = clamp(*x2, 0, SCREEN_WIDTH-1);

    *y0 = clamp(*y0, 0, SCREEN_HEIGHT-1);
    *y1 = clamp(*y1, 0, SCREEN_HEIGHT-1);
    *y2 = clamp(*y2, 0, SCREEN_HEIGHT-1);
}

static void pgl_wired_triangle(int x0, int y0, int x1, int y1, int x2, int y2, uint16_t color) {
    clamp_coordinates(&x0, &y0, &x1, &y1, &x2, &y2);

    pgl_line(x0, y0, x1, y1, color);
    pgl_line(x0, y0, x2, y2, color);
    pgl_line(x1, y1, x2, y2, color);
}

static void pgl_filled_triangle(int x0, int y0, int x1, int y1, int x2, int y2, uint16_t color) {
    clamp_coordinates(&x0, &y0, &x1, &y1, &x2, &y2);

    if (y1 < y0) {
        const int tempx = x1;
        const int tempy = y1;
        x1 = x0;
        y1 = y0;
        x0 = tempx;
        y0 = tempy;
    }
    if (y2 < y1) {
        const int tempx = x2;
        const int tempy = y2;
        x2 = x1;
        y2 = y1;
        x1 = tempx;
        y1 = tempy;
    }
    if (y1 < y0) {
        const int tempx = x1;
        const int tempy = y1;
        x1 = x0;
        y1 = y0;
        x0 = tempx;
        y0 = tempy;
    }

    // Check whether the neighbor triangles have gap between them. If there is a gap, uncomment.
    // pgl_wire_triangle(x0, y0, x1, y1, x2, y2, color);

    for (int y = y0; y <= y2; y++) {
        int x_left;
        int x_right;

        if (y >= y1) {
            const int dxl = (y2 != y1) ? (y - y1) * (x2 - x1) / (y2 - y1) : 0;
            const int dxr = (y2 != y0) ? (y - y0) * (x2 - x0) / (y2 - y0) : 0;
            x_left  = x1 + dxl;
            x_right = x0 + dxr;
        } 
        else {
            const int dxl = (y1 != y0) ? (y - y0) * (x1 - x0) / (y1 - y0) : 0;
            const int dxr = (y2 != y0) ? (y - y0) * (x2 - x0) / (y2 - y0) : 0;
            x_left  = x0 + dxl;
            x_right = x0 + dxr;
        }

        if (x_left > x_right) {
            const int temp = x_left;
            x_left = x_right;
            x_right = temp;
        }

        for (int x = x_left; x <= x_right; x++) {
            pgl.screen[y][x] = color;
        }
    }
}

// -------------------------------------------- TRANSFORMATION MATRICES --------------------------------------------------------- // 

void pgl_view(const vec3f position, const vec3f right, const vec3f up, const vec3f forward) {
    pgl.view = view(position, right, up, forward);
}

void pgl_projection(float fov, float near, float far) {
    pgl.projection = perspective(fov, ASPECT_RATIO, near, far);
}

void pgl_viewport(int x, int y, uint32_t width, uint32_t height) {
    pgl.viewport = viewport(x, y, width, height, 0.0f, 1.0f);
}

void pgl_clear(uint16_t color) {
    uint16_t* color_ptr = (uint16_t*)pgl.screen;
    for (uint32_t i = 0; i < SCREEN_HEIGHT*SCREEN_WIDTH; i++) {
        color_ptr[i] = color;
    }
}

// ----------------------------------------------- DRAW ------------------------------------------------------------ // 

void triangle_clip_plane_intersection(Triangle* t, const vec4f clip_plane_vector, vec4f* c10, vec4f* c20) {
    const float d0 = dot_vec4f(t->c0, clip_plane_vector); // TODO: clip plane vector can be sent with a pointer
    const float d1 = dot_vec4f(t->c1, clip_plane_vector);
    const float d2 = dot_vec4f(t->c2, clip_plane_vector);

    const float a = d0 / (d0 - d1);
    const float b = d0 / (d0 - d2);

    *c10 = add_vec4f(scale_vec4f(t->c0, 1.0f - a), scale_vec4f(t->c1, a));
    *c20 = add_vec4f(scale_vec4f(t->c0, 1.0f - b), scale_vec4f(t->c2, b));
}

void pgl_draw(const Mesh* mesh) {
    const mat4f projection_view_model = mul_mat4f_mat4f(pgl.projection, mul_mat4f_mat4f(pgl.view, mesh->model));

    // TODO: Broad Phase Clipping

    Triangle subtriangles[16];
    int subtriangle_index;
    vec4f c10;
    vec4f c20;

    for (uint32_t i = 0; i < mesh->index_number; i += 3) {
        const Vertex* v0 = &mesh->vertices[mesh->indices[i]];
        const Vertex* v1 = &mesh->vertices[mesh->indices[i+1]];
        const Vertex* v2 = &mesh->vertices[mesh->indices[i+2]];
    
        // Homogeneous local space coordinates
        const vec4f p0 = to_homogeneous(v0->position);
        const vec4f p1 = to_homogeneous(v1->position);
        const vec4f p2 = to_homogeneous(v2->position);

        // Clip space coordinates
        Triangle triangle = {
            .c0 = mul_mat4f_vec4f(projection_view_model, p0),
            .c1 = mul_mat4f_vec4f(projection_view_model, p1),
            .c2 = mul_mat4f_vec4f(projection_view_model, p2),
        };

        triangle_queue_init(&pgl.queue);
        triangle_queue_push(&pgl.queue, &triangle);
        subtriangle_index = -1;

        // Near Clip: Z + W > 0
        while (!pgl.queue.empty) {
            Triangle t = *triangle_queue_pop(&pgl.queue);
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
                triangle_clip_plane_intersection(&t, v, &c10, &c20);
                subtriangles[++subtriangle_index] = (Triangle) {c10, t.c1, t.c2};
                subtriangles[++subtriangle_index] = (Triangle) {c10,  c20, t.c2};
            }
            else if (in_number == 1) {
                // Ensure that c0 is the vertex that is in
                if      (in1) { swap_vec4f(&t.c0, &t.c1); }
                else if (in2) { swap_vec4f(&t.c0, &t.c2); }

                const vec4f v = (vec4f) {0.0f, 0.0f, 1.0f, 1.0f};
                triangle_clip_plane_intersection(&t, v, &c10, &c20);
                subtriangles[++subtriangle_index] = (Triangle) {t.c0, c10, c20};
            }
        }

        while (subtriangle_index >= 0) {
            triangle_queue_push(&pgl.queue, subtriangles + subtriangle_index);
            subtriangle_index--;
        }

        // Far Clip: Z - W < 0
        while (!pgl.queue.empty) {
            Triangle t = *triangle_queue_pop(&pgl.queue);
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
                triangle_clip_plane_intersection(&t, v, &c10, &c20);
                subtriangles[++subtriangle_index] = (Triangle) {c10, t.c1, t.c2};
                subtriangles[++subtriangle_index] = (Triangle) {c10,  c20, t.c2};
            }
            else if (in_number == 1) {
                // Ensure that c0 is the vertex that is in
                if      (in1) { swap_vec4f(&t.c0, &t.c1); }
                else if (in2) { swap_vec4f(&t.c0, &t.c2); }

                const vec4f v = (vec4f) {0.0f, 0.0f, 1.0f, -1.0f};
                triangle_clip_plane_intersection(&t, v, &c10, &c20);
                subtriangles[++subtriangle_index] = (Triangle) {t.c0, c10, c20};
            }
        }

        while (subtriangle_index >= 0) {
            triangle_queue_push(&pgl.queue, subtriangles + subtriangle_index);
            subtriangle_index--;
        }

        // Left Clip: X + W > 0
        while (!pgl.queue.empty) {
            Triangle t = *triangle_queue_pop(&pgl.queue);
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
                triangle_clip_plane_intersection(&t, v, &c10, &c20);
                subtriangles[++subtriangle_index] = (Triangle) {c10, t.c1, t.c2};
                subtriangles[++subtriangle_index] = (Triangle) {c10,  c20, t.c2};
            }
            else if (in_number == 1) {
                // Ensure that c0 is the vertex that is in
                if      (in1) { swap_vec4f(&t.c0, &t.c1); }
                else if (in2) { swap_vec4f(&t.c0, &t.c2); }

                const vec4f v = (vec4f) {1.0f, 0.0f, 0.0f, 1.0f};
                triangle_clip_plane_intersection(&t, v, &c10, &c20);
                subtriangles[++subtriangle_index] = (Triangle) {t.c0, c10, c20};
            }
        }

        while (subtriangle_index >= 0) {
            triangle_queue_push(&pgl.queue, subtriangles + subtriangle_index);
            subtriangle_index--;
        }

        // Right Clip: X - W < 0
        while (!pgl.queue.empty) {
            Triangle t = *triangle_queue_pop(&pgl.queue);
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
                triangle_clip_plane_intersection(&t, v, &c10, &c20);
                subtriangles[++subtriangle_index] = (Triangle) {c10, t.c1, t.c2};
                subtriangles[++subtriangle_index] = (Triangle) {c10,  c20, t.c2};
            }
            else if (in_number == 1) {
                // Ensure that c0 is the vertex that is in
                if      (in1) { swap_vec4f(&t.c0, &t.c1); }
                else if (in2) { swap_vec4f(&t.c0, &t.c2); }

                const vec4f v = (vec4f) {1.0f, 0.0f, 0.0f, -1.0f};
                triangle_clip_plane_intersection(&t, v, &c10, &c20);
                subtriangles[++subtriangle_index] = (Triangle) {t.c0, c10, c20};
            }
        }

        while (subtriangle_index >= 0) {
            triangle_queue_push(&pgl.queue, subtriangles + subtriangle_index);
            subtriangle_index--;
        }

        // Bottom Clip: Y + W > 0
        while (!pgl.queue.empty) {
            Triangle t = *triangle_queue_pop(&pgl.queue);
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
                triangle_clip_plane_intersection(&t, v, &c10, &c20);
                subtriangles[++subtriangle_index] = (Triangle) {c10, t.c1, t.c2};
                subtriangles[++subtriangle_index] = (Triangle) {c10,  c20, t.c2};
            }
            else if (in_number == 1) {
                // Ensure that c0 is the vertex that is in
                if      (in1) { swap_vec4f(&t.c0, &t.c1); }
                else if (in2) { swap_vec4f(&t.c0, &t.c2); }

                const vec4f v = (vec4f) {0.0f, 1.0f, 0.0f, 1.0f};
                triangle_clip_plane_intersection(&t, v, &c10, &c20);
                subtriangles[++subtriangle_index] = (Triangle) {t.c0, c10, c20};
            }
        }

        while (subtriangle_index >= 0) {
            triangle_queue_push(&pgl.queue, subtriangles + subtriangle_index);
            subtriangle_index--;
        }

        // Top Clip: Y - W < 0
        while (!pgl.queue.empty) {
            Triangle t = *triangle_queue_pop(&pgl.queue);
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
                triangle_clip_plane_intersection(&t, v, &c10, &c20);
                subtriangles[++subtriangle_index] = (Triangle) {c10, t.c1, t.c2};
                subtriangles[++subtriangle_index] = (Triangle) {c10,  c20, t.c2};
            }
            else if (in_number == 1) {
                // Ensure that c0 is the vertex that is in
                if      (in1) { swap_vec4f(&t.c0, &t.c1); }
                else if (in2) { swap_vec4f(&t.c0, &t.c2); }

                const vec4f v = (vec4f) {0.0f, 1.0f, 0.0f, -1.0f};
                triangle_clip_plane_intersection(&t, v, &c10, &c20);
                subtriangles[++subtriangle_index] = (Triangle) {t.c0, c10, c20};
            }
        }

        while (subtriangle_index >= 0) {
            Triangle* subt = subtriangles + subtriangle_index;
            subtriangle_index--;

            // Screen coordinates
            const vec4f sc0 = mul_mat4f_vec4f(pgl.viewport, normalize_homogeneous(subt->c0));
            const vec4f sc1 = mul_mat4f_vec4f(pgl.viewport, normalize_homogeneous(subt->c1));
            const vec4f sc2 = mul_mat4f_vec4f(pgl.viewport, normalize_homogeneous(subt->c2));
        
            // Rasterize
            if (mesh->render_type == WIRED) {
                pgl_wired_triangle((int)sc0.x, (int)sc0.y, (int)sc1.x, (int)sc1.y, (int)sc2.x, (int)sc2.y, 0xF100u);
            }
            else if (mesh->render_type == FILLED) {
                pgl_filled_triangle((int)sc0.x, (int)sc0.y, (int)sc1.x, (int)sc1.y, (int)sc2.x, (int)sc2.y, 0xF100u);
            }
        }
    }
}

void pgl_display() {
    lcd_display((uint16_t*)pgl.screen);
}
