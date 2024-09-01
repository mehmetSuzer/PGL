// 0.014800 s

#include <stdio.h>
#include <pico/multicore.h>

#include "camera.h"
#include "mesh.h"
#include "pgl.h"

const vertex_t cube_vertices[] = {
 	//     positions      /                  normals                     /      colors      //
	{{-0.5f, -0.5f, -0.5f}, {-1.0f/M_SQRT3, -1.0f/M_SQRT3, -1.0f/M_SQRT3}, {1.0f, 0.0f, 0.0f}},
	{{-0.5f,  0.5f, -0.5f}, {-1.0f/M_SQRT3,  1.0f/M_SQRT3, -1.0f/M_SQRT3}, {1.0f, 0.0f, 0.0f}},
	{{ 0.5f,  0.5f, -0.5f}, { 1.0f/M_SQRT3,  1.0f/M_SQRT3, -1.0f/M_SQRT3}, {1.0f, 0.0f, 0.0f}},
	{{ 0.5f, -0.5f, -0.5f}, { 1.0f/M_SQRT3, -1.0f/M_SQRT3, -1.0f/M_SQRT3}, {1.0f, 0.0f, 0.0f}},
	 
	{{-0.5f, -0.5f,  0.5f}, {-1.0f/M_SQRT3, -1.0f/M_SQRT3,  1.0f/M_SQRT3}, {1.0f, 0.0f, 0.0f}},
	{{ 0.5f, -0.5f,  0.5f}, { 1.0f/M_SQRT3, -1.0f/M_SQRT3,  1.0f/M_SQRT3}, {1.0f, 0.0f, 0.0f}},
	{{ 0.5f,  0.5f,  0.5f}, { 1.0f/M_SQRT3,  1.0f/M_SQRT3,  1.0f/M_SQRT3}, {1.0f, 0.0f, 0.0f}},
	{{-0.5f,  0.5f,  0.5f}, {-1.0f/M_SQRT3,  1.0f/M_SQRT3,  1.0f/M_SQRT3}, {1.0f, 0.0f, 0.0f}},
};

const uint32_t cube_indices[] = {
	0, 1, 2,
	0, 2, 3,
	4, 5, 6,
	4, 6, 7,
	0, 3, 6, 
	0, 6, 5,
	1, 4, 7,
	1, 7, 2,
	0, 5, 4,
	0, 4, 1,
	2, 7, 6,
	2, 6, 3,
};

mesh_t cube = {
    .vertices = cube_vertices,
    .indices = cube_indices,
    .vertex_number = sizeof(cube_vertices) / sizeof(vertex_t),
    .index_number = sizeof(cube_indices) / sizeof(uint32_t),
    .mesh_type = MESH_TRIANGLE,
    .render_type = RENDER_WIRED,
};

const vertex_t triangle_vertices[] = {
    //          positions         /             normals         /      colors      //
	{{-0.5f,          0.0f,  0.0f}, {-M_SQRT3/2.0f, -0.5f, 0.0f}, {1.0f, 0.0f, 0.0f}},
	{{ 0.5f,          0.0f,  0.0f}, { M_SQRT3/2.0f,  0.5f, 0.0f}, {1.0f, 0.0f, 0.0f}},
	{{ 0.0f,  M_SQRT3/2.0f,  0.0f}, {         0.0f,  1.0f, 0.0f}, {1.0f, 0.0f, 0.0f}},
};

const uint32_t triangle_indices[] = {
    0, 1, 2
};

mesh_t triangle1 = {
    .vertices = triangle_vertices,
    .indices = triangle_indices,
    .vertex_number = sizeof(triangle_vertices) / sizeof(vertex_t),
    .index_number = sizeof(triangle_indices) / sizeof(uint32_t),
    .mesh_type = MESH_TRIANGLE,
    .render_type = RENDER_WIRED,
};

mesh_t triangle2 = {
    .vertices = triangle_vertices,
    .indices = triangle_indices,
    .vertex_number = sizeof(triangle_vertices) / sizeof(vertex_t),
    .index_number = sizeof(triangle_indices) / sizeof(uint32_t),
    .mesh_type = MESH_TRIANGLE,
    .render_type = RENDER_WIRED,
};

void button_irq_callback(uint gpio, uint32_t event_mask) {
    switch (gpio) {
    case KEY_FORWARD:
        camera.forward_move = (event_mask & GPIO_IRQ_EDGE_FALL) ? POSITIVE : NONE;
        break;
    case KEY_BACKWARD:
        camera.forward_move = (event_mask & GPIO_IRQ_EDGE_FALL) ? NEGATIVE : NONE;
        break;
    case KEY_RIGHT:
        camera.right_move = (event_mask & GPIO_IRQ_EDGE_FALL) ? POSITIVE : NONE;
        break;
    case KEY_LEFT:
        camera.right_move = (event_mask & GPIO_IRQ_EDGE_FALL) ? NEGATIVE : NONE;
        break;
    case KEY_A:
        camera.up_move = (event_mask & GPIO_IRQ_EDGE_FALL) ? POSITIVE : NONE;
        break;
    case KEY_B:
        camera.up_move = (event_mask & GPIO_IRQ_EDGE_FALL) ? NEGATIVE : NONE;
        break;
    case KEY_X:
    case KEY_Y:
    case KEY_CTRL:
    default:
        break;
    }
}

int main() {
    device_init();
    camera_init(
        (vec3f) {0.0f,  0.0f,  0.0f},   // position
        (vec3f) {0.0f,  0.0f, -1.0f},   // forward
        (vec3f) {0.0f,  1.0f,  0.0f}    // up
    );
    
    set_button_irq_callback(button_irq_callback, GPIO_IRQ_EDGE_FALL|GPIO_IRQ_EDGE_RISE, true);

    pgl_viewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
    pgl_projection(0.1f, 30.0f, M_PI_4);
    pgl_view(camera.position, camera.right, camera.up, camera.forward);
    pgl_clear_color(0x0000);

    // cube.model = mul_mat4f_mat4f(translate3D((vec3f) {0.0f, 0.2f, -5.0f}), rotate3D_y(M_PI_4));
    triangle1.model = translate3D((vec3f) {0.0f, -0.5/M_SQRT3, -3.0f});
    triangle2.model = translate3D((vec3f) {0.3f, -0.5/M_SQRT3, -3.5f});

    uint32_t last_time = time_us_32();
    uint32_t current_time;

    while (true) {
        current_time = time_us_32();
        float dt = (current_time - last_time) / 1E6f;
        last_time = current_time;

        camera_update(dt);

        pgl_clear(COLOR_BUFFER_BIT | DEPTH_BUFFER_BIT);

        // pgl_draw(&cube);
        pgl_draw(&triangle1);
        pgl_draw(&triangle2);
        printf("dt: %f\n", dt);
        pgl_display();
    }

    return 0;
}
