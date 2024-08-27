
#include "pgl.h"

PGL pgl = {
    .screen = {{0x0000}},
    .mesh_type = PGL_TRIANGLE,
    .render_type = PGL_WIRED,
};

// Delete when no longer needed
static void printf_mat4f(const mat4f m) {
    printf("%f, %f, %f, %f\n%f, %f, %f, %f\n%f, %f, %f, %f\n%f, %f, %f, %f\n\n", 
        m.xx, m.xy, m.xz, m.xw, 
        m.yx, m.yy, m.yz, m.yw,
        m.zx, m.zy, m.zz, m.zw,
        m.wx, m.wy, m.wz, m.ww);
}

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

static void pgl_wired_triangle(int x0, int y0, int x1, int y1, int x2, int y2, uint16_t color) {
    pgl_line(x0, y0, x1, y1, color);
    pgl_line(x0, y0, x2, y2, color);
    pgl_line(x1, y1, x2, y2, color);
}

static void pgl_filled_triangle(int x0, int y0, int x1, int y1, int x2, int y2, uint16_t color) {
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

// ------------------------------------------------------------------------------------------------------------------- // 

void pgl_view(const vec3f position, const vec3f right, const vec3f up, const vec3f forward) {
    pgl.view = view(position, right, up, forward);
}

void pgl_projection(float fov, float near, float far) {
    pgl.projection = perspective(fov, ASPECT_RATIO, near, far);
}

void pgl_viewport(int x, int y, uint32_t width, uint32_t height) {
    pgl.viewport = viewport(0, 0, width, height, 0.0f, 1.0f);
}

void pgl_clear(uint16_t color) {
    uint16_t* color_ptr = (uint16_t*)pgl.screen;
    for (uint32_t i = 0; i < LCD_HEIGHT*LCD_WIDTH; i++) {
        color_ptr[i] = color;
    }
}

void pgl_draw(const Mesh* mesh) {
    const mat4f projection_view_model = mul_mat4f_mat4f(pgl.projection, mul_mat4f_mat4f(pgl.view, mesh->model));

    // Broad Phase Clipping

    for (uint32_t i = 0; i < mesh->index_number; i += 3) {
        const Vertex* v0 = &mesh->vertices[mesh->indices[i]];
        const Vertex* v1 = &mesh->vertices[mesh->indices[i+1]];
        const Vertex* v2 = &mesh->vertices[mesh->indices[i+2]];
    
        // Local space coordinates
        const vec4f p0 = {v0->position.x, v0->position.y, v0->position.z, 1.0f};
        const vec4f p1 = {v1->position.x, v1->position.y, v1->position.z, 1.0f};
        const vec4f p2 = {v2->position.x, v2->position.y, v2->position.z, 1.0f};

        // Normalized clip space coordinates
        vec4f c0 = normalize_homogeneous(mul_mat4f_vec4f(projection_view_model, p0));
        vec4f c1 = normalize_homogeneous(mul_mat4f_vec4f(projection_view_model, p1));
        vec4f c2 = normalize_homogeneous(mul_mat4f_vec4f(projection_view_model, p2));
        vec4f c3;

        // Check whether the points are in the view frustum
        const int in0 = abs(c0.x) < 1.0f && abs(c0.y) < 1.0f && abs(c0.z) < 1.0f;
        const int in1 = abs(c1.x) < 1.0f && abs(c1.y) < 1.0f && abs(c1.z) < 1.0f;
        const int in2 = abs(c2.x) < 1.0f && abs(c2.y) < 1.0f && abs(c2.z) < 1.0f;
        const int in_number = in0 + in1 + in2;

        // All vertices are out of the view frustum; discard the triangle
        if (in_number == 0) {
            continue;
        }
        // One of the vertices is in the view frustum
        else if (in_number == 1) {
            // Ensure c0 is the one that is in the view frustum
            if      (in1) { swap_vec4f(&c0, &c1); }
            else if (in2) { swap_vec4f(&c0, &c2); }

            printf("In1: %d | (%f, %f, %f, %f), (%f, %f, %f, %f), (%f, %f, %f, %f), (%f, %f, %f, %f)\n", in_number,
            c0.x, c0.y, c0.z, c0.w,
            c1.x, c1.y, c1.z, c1.w,
            c2.x, c2.y, c2.z, c2.w,
            c3.x, c3.y, c3.z, c3.w);

            float alpha;
            float beta;
            if (c1.x >= 1.0f) {
                alpha = (c0.x - 0.999f) / (c0.x - c1.x);
                beta  = (c0.x - 0.999f) / (c0.x - c2.x);
            }
            else if (c1.x <= -1.0f) {
                alpha = (c0.x + 0.999f) / (c0.x - c1.x);
                beta  = (c0.x + 0.999f) / (c0.x - c2.x);
            }
            else if (c1.y >= 1.0f) {
                alpha = (c0.y - 0.999f) / (c0.y - c1.y);
                beta  = (c0.y - 0.999f) / (c0.y - c2.y);
            }
            else if (c1.y <= -1.0f) {
                alpha = (c0.y + 0.999f) / (c0.y - c1.y);
                beta  = (c0.y + 0.999f) / (c0.y - c2.y);
            }
            else if (c1.z >= 1.0f) {
                alpha = (c0.z - 0.999f) / (c0.z - c1.z);
                beta  = (c0.z - 0.999f) / (c0.z - c2.z);
            }
            else if (c1.z <= -1.0f) {
                alpha = (c0.z + 0.999f) / (c0.z - c1.z);
                beta  = (c0.z + 0.999f) / (c0.z - c2.z);
            }
            
            printf("Alpha: %f, Beta: %f\n", alpha, beta);
            c1 = add_vec4f(c0, scale_vec4f(sub_vec4f(c1, c0), alpha));
            c2 = add_vec4f(c0, scale_vec4f(sub_vec4f(c2, c0), beta));
        }
        // Two of the vertices are in the view frustum
        else if (in_number == 2) {
            // Ensure c0 is the one that is out the view frustum
            if      (!in1) { swap_vec4f(&c0, &c1); }
            else if (!in2) { swap_vec4f(&c0, &c2); }

            float alpha;
            float beta;
            if (c0.x >= 1.0f) {
                alpha = (c0.x - 0.999f) / (c0.x - c1.x);
                beta  = (c0.x - 0.999f) / (c0.x - c2.x);
            }
            else if (c0.x <= -1.0f) {
                alpha = (c0.x + 0.999f) / (c0.x - c1.x);
                beta  = (c0.x + 0.999f) / (c0.x - c2.x);
            }
            else if (c0.y >= 1.0f) {
                alpha = (c0.y - 0.999f) / (c0.y - c1.y);
                beta  = (c0.y - 0.999f) / (c0.y - c2.y);
            }
            else if (c0.y <= -1.0f) {
                alpha = (c0.y + 0.999f) / (c0.y - c1.y);
                beta  = (c0.y + 0.999f) / (c0.y - c2.y);
            }
            else if (c0.z >= 1.0f) {
                alpha = (c0.z - 0.999f) / (c0.z - c1.z);
                beta  = (c0.z - 0.999f) / (c0.z - c2.z);
            }
            else if (c0.z <= -1.0f) {
                alpha = (c0.z + 0.999f) / (c0.z - c1.z);
                beta  = (c0.z + 0.999f) / (c0.z - c2.z);
            }
            
            c0 = add_vec4f(c0, scale_vec4f(sub_vec4f(c1, c0), alpha));
            c3 = add_vec4f(c0, scale_vec4f(sub_vec4f(c2, c0), beta));
        }
            
        printf("In2: %d | (%f, %f, %f, %f), (%f, %f, %f, %f), (%f, %f, %f, %f), (%f, %f, %f, %f)\n", in_number,
            c0.x, c0.y, c0.z, c0.w,
            c1.x, c1.y, c1.z, c1.w,
            c2.x, c2.y, c2.z, c2.w,
            c3.x, c3.y, c3.z, c3.w);

        // Screen coordinates
        const vec4f sc0 = mul_mat4f_vec4f(pgl.viewport, c0);
        const vec4f sc1 = mul_mat4f_vec4f(pgl.viewport, c1);
        const vec4f sc2 = mul_mat4f_vec4f(pgl.viewport, c2);
        
        // Rasterize
        const uint16_t color = vec3f_to_rgb565(v0->color);
        pgl_wired_triangle((int)sc0.x, (int)sc0.y, (int)sc1.x, (int)sc1.y, (int)sc2.x, (int)sc2.y, color);

        if (in_number == 2) {
            const vec4f sc3 = mul_mat4f_vec4f(pgl.viewport, c3);
            const uint16_t color = vec3f_to_rgb565(v0->color);
            pgl_wired_triangle((int)sc0.x, (int)sc0.y, (int)sc2.x, (int)sc2.y, (int)sc3.x, (int)sc3.y, color);
        }
    }
}

void pgl_display() {
    lcd_display((uint16_t*)pgl.screen);
}
