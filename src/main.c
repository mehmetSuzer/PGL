
#include <stdio.h>
#include <pico/multicore.h>

#include "light_source.h"
#include "camera.h"
#include "triangle.h"
#include "rasterizer.h"

#define CORE1_RENDER_COMPLETE 1u

Camera camera;

// ------------------------------------------------------------------------------------------------- //

const PointLight point_lights[] = {
    {
        .color = {1.0f, 1.0f, 1.0f},
        .position = {80.0f, 120.0f, -130.0f},
        .quadratic = 0.00006f,
        .linear = 0.000006f,
    },
    {
        .color = {1.0f, 1.0f, 1.0f},
        .position = {-80.0f, 120.0f, -130.0f},
        .quadratic = 0.00006f,
        .linear = 0.000006f,
    },
};
const uint point_light_number = 0; // sizeof(point_lights) / sizeof(PointLight);

// ------------------------------------------------------------------------------------------------- //

const DirectionalLight directional_lights[] = {
    {
        .color = {1.0f, 1.0f, 1.0f},
        .direction = {-0.6f, -0.8f, 0.0f},
        .intensity = 0.7f
    },
    {
        .color = {1.0f, 1.0f, 1.0f},
        .direction = {0.6f, -0.8f, 0.0f},
        .intensity = 0.7f
    }
};
const uint directional_light_number = sizeof(directional_lights) / sizeof(DirectionalLight);

// ------------------------------------------------------------------------------------------------- //

// Scans column by column
void render(const uint core_number) {
    if (core_number == 0) {
        int x0 = 60;
        int y0 = 60;
        int x1 = 180;
        int y1 = 60;
        int x2 = 10;
        int y2 = 150;
        plot_wire_triangle(x0, y0, x1, y1, x2, y2, 0xFFFF);
        plot_filled_triangle(x0, y0, x1, y1, x2, y2, 0xF100);
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
        &camera,
        (vec3f) {0.0f,  0.0f,  0.0f},   // position
        (vec3f) {0.0f,  0.0f, -1.0f},   // forward
        (vec3f) {0.0f,  1.0f,  0.0f},   // up
        0.1f,                           // near
        100.0f,                         // far
        M_PI/4.0f                       // fov
    );

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
