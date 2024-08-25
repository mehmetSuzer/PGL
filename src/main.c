// 0.014990 s

#include <stdio.h>
#include <pico/multicore.h>

#include "light_source.h"
#include "camera.h"
#include "mesh.h"

#define CORE1_RENDER_COMPLETE 1u

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

// Scans column by column
void render(const uint32_t core_number) {
    if (core_number == 0) {
        process_mesh(&cube);
    }
}

void core1_main() {
    render(1);

    while (!multicore_fifo_wready());
    multicore_fifo_push_blocking(CORE1_RENDER_COMPLETE);
}

int main() {
    init_device();
    init_lcd();

    init_camera(
        (vec3f) {0.0f,  0.0f,  0.0f},   // position
        (vec3f) {0.0f,  0.0f, -1.0f},   // forward
        (vec3f) {0.0f,  1.0f,  0.0f},   // up
        0.1f,                           // near
        100.0f,                         // far
        M_PI/4.0f                       // fov
    );

    cube.model = mul_mat4f_mat4f(translate3D((vec3f) {0.0f, 0.0f, -5.0f}), rotate3D_y(M_PI_4));

    while (gpio_get(KEY_A)); // wait until key A is pressed

    const uint32_t start_us =  time_us_32();

    multicore_launch_core1(core1_main);
    render(0);

    while (!multicore_fifo_rvalid());
    if (multicore_fifo_pop_blocking() == CORE1_RENDER_COMPLETE) {
        lcd_display((color_t*)screen);
    }
    
    const uint32_t end_us = time_us_32();
    printf("Render time: %f s\n", (end_us - start_us) / 1E6f);

    return 0;
}
