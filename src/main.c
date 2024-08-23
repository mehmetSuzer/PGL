
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
        plot_wire_triangle(60, 180, 180, 180, 120, 76, 0xFFFF);
        plot_filled_triangle(60, 180, 180, 180, 120, 76, 0xF100);
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

    camera = init_camera(
        (vec3f) {0.0f,  0.0f,  0.0f},
        (vec3f) {0.0f,  0.0f, -1.0f},
        (vec3f) {0.0f,  1.0f,  0.0f},
        1.0f,
        INFINITY,
        M_PI / 3.0f,
        LCD_WIDTH,
        LCD_HEIGHT
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
