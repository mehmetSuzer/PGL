
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
    const mat4f combined = mul_mat4f_mat4f(camera.projection, mul_mat4f_mat4f(camera.view, mesh->model));

    // printf_mat4f(mesh->model);
    // printf_mat4f(camera.view);
    // printf_mat4f(camera.projection);
    // printf_mat4f(viewp);
    // printf_mat4f(combined);

    for (uint32_t i = 0; i < mesh->index_number; i += 3) {
        const Vertex* v0 = &mesh->vertices[mesh->indices[i]];
        const Vertex* v1 = &mesh->vertices[mesh->indices[i+1]];
        const Vertex* v2 = &mesh->vertices[mesh->indices[i+2]];
    
        const vec4f p0 = {v0->position.x, v0->position.y, v0->position.z, 1.0f};
        const vec4f p1 = {v1->position.x, v1->position.y, v1->position.z, 1.0f};
        const vec4f p2 = {v2->position.x, v2->position.y, v2->position.z, 1.0f};

        const vec4f ndc0 = mul_mat4f_vec4f(combined, p0);
        const vec4f ndc1 = mul_mat4f_vec4f(combined, p1);
        const vec4f ndc2 = mul_mat4f_vec4f(combined, p2);

        // clipping

        const vec4f sc0 = mul_mat4f_vec4f(viewp, ndc0);
        const vec4f sc1 = mul_mat4f_vec4f(viewp, ndc1);
        const vec4f sc2 = mul_mat4f_vec4f(viewp, ndc2);

        // printf("%u, %u, %u\n", i, i+1, i+2);
        // printf("SC0 = (%f, %f, %f)\n", sc0.x / sc0.w, sc0.y / sc0.w, sc0.z / sc0.w);
        // printf("SC1 = (%f, %f, %f)\n", sc1.x / sc1.w, sc1.y / sc1.w, sc1.z / sc1.w);
        // printf("SC2 = (%f, %f, %f)\n", sc2.x / sc2.w, sc2.y / sc2.w, sc2.z / sc2.w);
        
        const color_t color = vec3f_to_rgb565(v0->color);
        plot_wired_triangle((int)(sc0.x/sc0.w), (int)(sc0.y/sc0.w), (int)(sc1.x/sc1.w), (int)(sc1.y/sc1.w), (int)(sc2.x/sc2.w), (int)(sc2.y/sc2.w), color);
    }
}
