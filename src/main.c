// wired:  0.021454 s | target = 0.016666 s
// filled: 0.046736 s | target = 0.033333 s

#include <stdio.h>
#include <pico/multicore.h>

#include "pgl.h"
#include "camera.h"

const directional_light_t dl = {
    .direction = {0.6f, -0.8f, 0.0f},
    .intensity = 1.0f,
};

mesh_t meshes[] = {
    {
        .vertices = cube_vertices,
        .indices  = cube_indices,
        .vertex_number = count_of(cube_vertices),
        .index_number  = count_of(cube_indices),
        .mesh_enum = MESH_TRIANGLE | MESH_RENDER_FILLED,
        // .color = PGLM_RGB565_GOLD,
        .tex_index = TEXTURE_HEART,
    },
    {
        .vertices = equilateral_triangle_vertices,
        .indices  = equilateral_triangle_indices,
        .vertex_number = count_of(equilateral_triangle_vertices),
        .index_number  = count_of(equilateral_triangle_indices),
        .mesh_enum = MESH_TRIANGLE | MESH_RENDER_FILLED,
        // .color = PGLM_RGB565_LIMEGREEN,
        .tex_index = TEXTURE_GREEN,
    },
    {
        .vertices = equilateral_triangle_vertices,
        .indices  = equilateral_triangle_indices,
        .vertex_number = count_of(equilateral_triangle_vertices),
        .index_number  = count_of(equilateral_triangle_indices),
        .mesh_enum = MESH_TRIANGLE | MESH_RENDER_FILLED,
        // .color = PGLM_RGB565_CRIMSON,
        .tex_index = TEXTURE_BLUE,
    },
    {
        .vertices = truncated_icosahedron_vertices,
        .indices  = truncated_icosahedron_indices,
        .vertex_number = count_of(truncated_icosahedron_vertices),
        .index_number  = count_of(truncated_icosahedron_indices),
        .mesh_enum = MESH_TRIANGLE | MESH_RENDER_FILLED,
        // .color = PGLM_RGB565_FUCHSIA,
        .tex_index = TEXTURE_RED,
    },
};

void button_irq_callback(uint gpio, uint32_t event_mask) {
    switch (gpio) {
    case KEY_FORWARD:
        camera.forward_change = (event_mask & GPIO_IRQ_EDGE_FALL) ? POSITIVE : NONE;
        break;
    case KEY_BACKWARD:
        camera.forward_change = (event_mask & GPIO_IRQ_EDGE_FALL) ? NEGATIVE : NONE;
        break;
    case KEY_RIGHT:
        camera.right_change = (event_mask & GPIO_IRQ_EDGE_FALL) ? POSITIVE : NONE;
        break;
    case KEY_LEFT:
        camera.right_change = (event_mask & GPIO_IRQ_EDGE_FALL) ? NEGATIVE : NONE;
        break;
    case KEY_A:
        camera.up_change = (event_mask & GPIO_IRQ_EDGE_FALL) ? POSITIVE : NONE;
        break;
    case KEY_B:
        camera.up_change = (event_mask & GPIO_IRQ_EDGE_FALL) ? NEGATIVE : NONE;
        break;
    case KEY_X:
        camera.rotate_y_change = (event_mask & GPIO_IRQ_EDGE_FALL) ? POSITIVE : NONE;
        break;
    case KEY_Y:
        camera.rotate_y_change = (event_mask & GPIO_IRQ_EDGE_FALL) ? NEGATIVE : NONE;
        break;
    case KEY_CTRL:
    default:
        break;
    }
}

int main() {
    device_init();
    camera_init(
        (vec3f){0.0f,  0.0f,  0.0f},   // position
        (vec3f){0.0f,  0.0f, -1.0f},   // forward
        (vec3f){0.0f,  1.0f,  0.0f}    // up
    );
    
    set_button_irq_callback(button_irq_callback, GPIO_IRQ_EDGE_FALL | GPIO_IRQ_EDGE_RISE, true);

    pgl_view(camera.position, camera.right, camera.up, camera.forward);
    pgl_projection(0.1f, 30.0f, PGLM_PI_4f);
    pgl_viewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
    
    pgl_enable(PGL_DEPTH_TEST);
    pgl_clear_color(PGLM_RGB565_BLACK);

    meshes[0].transform = transform_init((vec3f){-3.5f, 0.0f, -4.0f}, quat_angle_axis((vec3f){0.0f, 1.0f, 0.0f}, PGLM_PI_4f), (vec3f){1.0f, 1.0f, 1.0f});
    meshes[1].transform = transform_init((vec3f){2.0f, -PGLM_1_2SQRT3f, -3.0f}, identity_quat, (vec3f){1.0f, 1.0f, 1.0f});
    meshes[2].transform = transform_init((vec3f){2.0f, -PGLM_1_2SQRT3f, -3.0f}, quat_angle_axis((vec3f){0.0f, 1.0f, 0.0f}, PGLM_PI_2f), (vec3f){1.0f, 1.0f, 1.0f});
    meshes[3].transform = transform_init((vec3f){0.0f, -1.0f, -6.0f}, identity_quat, (vec3f){1.0f, 1.0f, 1.0f});

    for (uint32_t i = 0; i < count_of(meshes); i++) {
        find_mesh_bounding_volume(meshes+i);
    }

    uint32_t last_time = time_us_32();
    uint32_t current_time;

    while (true) {
        current_time = time_us_32();
        float dt = (current_time - last_time) / 1E6f;
        last_time = current_time;
        camera_update(dt);
        
        // transform_rotate_quat(&meshes[0].transform, quat_angle_axis((vec3f){0.8f, 0.6f, 0.0f}, dt));

        pgl_clear(PGL_COLOR_BUFFER_BIT | PGL_DEPTH_BUFFER_BIT);
        for (uint32_t i = 0; i < count_of(meshes); i++) {
            pgl_draw(meshes+i, &dl);
        }
        printf("dt: %f\n", dt);
        pgl_display();
    }

    return 0;
}
