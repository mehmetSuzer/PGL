
#include "mesh.h"

static void printf_mat4f(const mat4f m) {
    printf("%f, %f, %f, %f\n%f, %f, %f, %f\n%f, %f, %f, %f\n%f, %f, %f, %f\n\n", 
        m.xx, m.xy, m.xz, m.xw, 
        m.yx, m.yy, m.yz, m.yw,
        m.zx, m.zy, m.zz, m.zw,
        m.wx, m.wy, m.wz, m.ww);
}

void process_mesh(const Mesh* mesh) {
    const mat4f viewp = viewport(0, 0, LCD_WIDTH, LCD_HEIGHT, 0.0f, 1.0f);
    const mat4f projection_view_model = mul_mat4f_mat4f(camera.projection, mul_mat4f_mat4f(camera.view, mesh->model));

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
        const vec4f sc0 = mul_mat4f_vec4f(viewp, c0);
        const vec4f sc1 = mul_mat4f_vec4f(viewp, c1);
        const vec4f sc2 = mul_mat4f_vec4f(viewp, c2);
        
        // Rasterize
        const uint16_t color = vec3f_to_rgb565(v0->color);
        plot_wired_triangle((int)sc0.x, (int)sc0.y, (int)sc1.x, (int)sc1.y, (int)sc2.x, (int)sc2.y, color);

        if (in_number == 2) {
            const vec4f sc3 = mul_mat4f_vec4f(viewp, c3);
            const uint16_t color = vec3f_to_rgb565(v0->color);
            plot_wired_triangle((int)sc0.x, (int)sc0.y, (int)sc2.x, (int)sc2.y, (int)sc3.x, (int)sc3.y, color);
        }
    }
}
