// 0.014800 s

#include <stdio.h>
#include <pico/multicore.h>

#include "camera.h"
#include "mesh.h"

Vertex cube_vertices[] = {
 	//     positions      /                  normals                     /      colors      //
	{{-0.5f, -0.5f, -0.5f}, {-1.0f/M_SQRT3, -1.0f/M_SQRT3, -1.0f/M_SQRT3}, {1.0f, 0.0f, 0.0f}},
	{{-0.5f,  0.5f, -0.5f}, {-1.0f/M_SQRT3,  1.0f/M_SQRT3, -1.0f/M_SQRT3}, {1.0f, 0.0f, 0.0f}},
	{{ 0.5f,  0.5f, -0.5f}, { 1.0f/M_SQRT3,  1.0f/M_SQRT3, -1.0f/M_SQRT3}, {1.0f, 0.0f, 0.0f}},
	{{ 0.5f, -0.5f, -0.5f}, { 1.0f/M_SQRT3, -1.0f/M_SQRT3, -1.0f/M_SQRT3}, {1.0f, 0.0f, 0.0f}},
	 
	{{-0.5f, -0.5f,  0.5f}, {-1.0f/M_SQRT3, -1.0f/M_SQRT3,  1.0f/M_SQRT3}, {1.0f, 0.0f, 1.0f}},
	{{ 0.5f, -0.5f,  0.5f}, { 1.0f/M_SQRT3, -1.0f/M_SQRT3,  1.0f/M_SQRT3}, {1.0f, 0.0f, 1.0f}},
	{{ 0.5f,  0.5f,  0.5f}, { 1.0f/M_SQRT3,  1.0f/M_SQRT3,  1.0f/M_SQRT3}, {1.0f, 0.0f, 1.0f}},
	{{-0.5f,  0.5f,  0.5f}, {-1.0f/M_SQRT3,  1.0f/M_SQRT3,  1.0f/M_SQRT3}, {1.0f, 0.0f, 1.0f}},
};

uint32_t cube_indices[] = {
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

Mesh cube = {
    .vertices = cube_vertices,
    .indices = cube_indices,
    .vertex_number = sizeof(cube_vertices) / sizeof(Vertex),
    .index_number = sizeof(cube_indices) / sizeof(uint32_t),
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
    case KEY_CTRL:
    case KEY_A:
    case KEY_B:
    case KEY_X:
    case KEY_Y:
    default:
        break;
    }
}

void render(uint32_t core_number) {
    if (core_number == 0) {
        process_mesh(&cube);
    }
}

int main() {
    device_init();
    set_button_irq_callback(button_irq_callback, GPIO_IRQ_EDGE_FALL|GPIO_IRQ_EDGE_RISE, true);

    init_camera(
        (vec3f) {0.0f,  0.0f,  0.0f},   // position
        (vec3f) {0.0f,  0.0f, -1.0f},   // forward
        (vec3f) {0.0f,  1.0f,  0.0f},   // up
        0.1f,                           // near
        100.0f,                         // far
        M_PI/4.0f                       // fov
    );

    cube.model = mul_mat4f_mat4f(translate3D((vec3f) {0.0f, 0.0f, -5.0f}), rotate3D_y(M_PI_4));

    uint32_t last_time = time_us_32();
    uint32_t current_time;

    while (true) {
        current_time = time_us_32();
        float dt = (current_time - last_time) / 1E6f;
        last_time = current_time;

        update_camera(dt);

        clear_screen(0x0000);
        render(0);
        lcd_display((uint16_t*)screen);
    }

    return 0;
}
