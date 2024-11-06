
// wired  target = 0.016666 s
// filled target = 0.033333 s

#include <stdio.h>
#include <pico/multicore.h>
#include "pgl.h"
#include "camera.h"

const directional_light_t dl = {
    .direction = {{0.6f, -0.8f, 0.0f}},
    .intensity = 1.0f,
};

mesh_t meshes[] = {
    {
        .vertices = triangle_vertices,
        .indices  = triangle_indices_single_color,
        .vertex_number = count_of(triangle_vertices),
        .index_number  = count_of(triangle_indices_single_color),
        .filled_render = FILLED_RENDER_SINGLE_COLOR,
        .fill_color = PGLM_RGB565_GREEN,
        .wired_render  = WIRED_RENDER_NO,
    },
    {
        .vertices = triangle_vertices,
        .indices  = triangle_indices_single_color,
        .vertex_number = count_of(triangle_vertices),
        .index_number  = count_of(triangle_indices_single_color),
        .filled_render = FILLED_RENDER_SINGLE_COLOR,
        .fill_color = PGLM_RGB565_BLUE,
        .wired_render  = WIRED_RENDER_NO,
    },
    {
        .vertices = cube_vertices,
        .indices  = cube_indices_tex_coords,
        .vertex_number = count_of(cube_vertices),
        .index_number  = count_of(cube_indices_tex_coords),
        .filled_render = FILLED_RENDER_TEX_COORDS,
        .tex_index = TEXTURE_HEART,
        .wired_render  = WIRED_RENDER_NO,
    },
    {
        .vertices = icositetragon_pyramid_vertices,
        .indices  = icositetragon_pyramid_indices_single_color,
        .vertex_number = count_of(icositetragon_pyramid_vertices),
        .index_number  = count_of(icositetragon_pyramid_indices_single_color),
        .filled_render = FILLED_RENDER_NO,
        .wired_render  = WIRED_RENDER_YES,
        .wired_color = PGLM_RGB565_FUCHSIA,
    },
    {
        .vertices = spaceship_vertices,
        .indices  = spaceship_indices_single_color,
        .vertex_number = count_of(spaceship_vertices),
        .index_number  = count_of(spaceship_indices_single_color),
        .filled_render = FILLED_RENDER_SINGLE_COLOR,
        .fill_color = PGLM_RGB565_FUCHSIA,
        .wired_render  = WIRED_RENDER_YES,
        .wired_color = PGLM_RGB565_BLACK,
    },
};

void button_irq_callback(uint gpio, uint32_t event_mask) {
    switch (gpio) {
    case DEVICE_KEY_FORWARD:
        camera.forward_change = (event_mask & GPIO_IRQ_EDGE_FALL) ? CHANGE_POSITIVE : CHANGE_NONE;
        break;
    case DEVICE_KEY_BACKWARD:
        camera.forward_change = (event_mask & GPIO_IRQ_EDGE_FALL) ? CHANGE_NEGATIVE : CHANGE_NONE;
        break;
    case DEVICE_KEY_RIGHT:
        camera.right_change = (event_mask & GPIO_IRQ_EDGE_FALL) ? CHANGE_POSITIVE : CHANGE_NONE;
        break;
    case DEVICE_KEY_LEFT:
        camera.right_change = (event_mask & GPIO_IRQ_EDGE_FALL) ? CHANGE_NEGATIVE : CHANGE_NONE;
        break;
    case DEVICE_KEY_A:
        camera.up_change = (event_mask & GPIO_IRQ_EDGE_FALL) ? CHANGE_POSITIVE : CHANGE_NONE;
        break;
    case DEVICE_KEY_B:
        camera.up_change = (event_mask & GPIO_IRQ_EDGE_FALL) ? CHANGE_NEGATIVE : CHANGE_NONE;
        break;
    case DEVICE_KEY_X:
        camera.rotate_y_change = (event_mask & GPIO_IRQ_EDGE_FALL) ? CHANGE_POSITIVE : CHANGE_NONE;
        break;
    case DEVICE_KEY_Y:
        camera.rotate_y_change = (event_mask & GPIO_IRQ_EDGE_FALL) ? CHANGE_NEGATIVE : CHANGE_NONE;
        break;
    case DEVICE_KEY_CTRL:
    default:
        break;
    }
}

int main() {
    device_init();
    device_set_button_irq_callback(button_irq_callback, GPIO_IRQ_EDGE_FALL | GPIO_IRQ_EDGE_RISE, true);

    pgl_view(camera.position, camera.right, camera.up, camera.forward);
    pgl_projection(0.1f, 30.0f, PGLM_PI_4f);
    pgl_viewport(0, 0, DEVICE_SCREEN_WIDTH, DEVICE_SCREEN_HEIGHT);
    
    pgl_set_tests(PGL_DEPTH_TEST, PGL_TEST_ENABLED);
    pgl_depth_func(PGL_LESS);
    pgl_depth_mask(PGL_MASKED);

    pgl_cull_face(PGL_CULL_BACK);
    pgl_cull_winding_order(PGL_CCW);

    pgl_clear_color(PGLM_RGB565_BLACK);

    meshes[0].transform = transform_init((vec3f){{2.0f, -PGLM_1_2SQRT3f, -3.0f}}, quat_identity, (vec3f){{1.0f, 1.0f, 1.0f}});
    meshes[1].transform = transform_init((vec3f){{2.0f, -PGLM_1_2SQRT3f, -3.0f}}, quat_angle_axis((vec3f){{0.0f, 1.0f, 0.0f}}, PGLM_PI_2f), (vec3f){{1.0f, 1.0f, 1.0f}});
    meshes[2].transform = transform_init((vec3f){{-3.5f, 0.0f, -4.0f}}, quat_angle_axis((vec3f){{0.0f, 1.0f, 0.0f}}, PGLM_PI_4f), (vec3f){{1.0f, 1.0f, 1.0f}});
    meshes[3].transform = transform_init((vec3f){{0.0f, -0.4f, -3.0f}}, quat_identity, (vec3f){{1.0f, 1.0f, 1.0f}});
    meshes[4].transform = transform_init((vec3f){{0.0f, -1.0f,  6.0f}}, quat_identity, (vec3f){{0.6f, 0.6f, 0.6f}});

    for (u32 i = 0; i < count_of(meshes); i++) {
        mesh_set_bounding_volume(meshes+i);
    }

    u32 last_time = time_us_32();

    while (true) {
        u32 current_time = time_us_32();
        f32 dt = (current_time - last_time) / 1E6f;
        last_time = current_time;
        camera_update(dt);
        
        const vec3f cube_rotation_axis = (vec3f){{0.8f, 0.6f, 0.0f}};
        transform_rotate_quat(&meshes[2].transform, quat_angle_axis(cube_rotation_axis, dt));
        const vec3f icositetragon_rotation_axis = vec3f_j;
        transform_rotate_quat(&meshes[3].transform, quat_angle_axis(icositetragon_rotation_axis, dt));

        pgl_clear(PGL_COLOR_BUFFER_BIT | PGL_DEPTH_BUFFER_BIT);
        for (u32 i = 0; i < count_of(meshes); i++) {
            pgl_draw(meshes+i, &dl);
        }
        printf("dt: %f\n", dt);
        pgl_display();
    }

    return 0;
}
