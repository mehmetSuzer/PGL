
#ifndef __COLOR_H__
#define __COLOR_H__

#include "pglm.h"

#define RGB565_RED_MAX      ((uint8_t)0x1F)
#define RGB565_GREEN_MAX    ((uint8_t)0x3F)
#define RGB565_BLUE_MAX     ((uint8_t)0x1F)

typedef uint16_t color_t; // bits: r4_r3_r2_r1 | r0_y5_y4_y3 | y2_y1_y0_b4 | b3_b2_b1_b0

static inline color_t vec3f_to_rgb565(const vec3f color) {
    const uint8_t red   = (color.x < 1.0f) ? (uint8_t)(color.x * RGB565_RED_MAX)   : RGB565_RED_MAX; 
    const uint8_t green = (color.y < 1.0f) ? (uint8_t)(color.y * RGB565_GREEN_MAX) : RGB565_GREEN_MAX; 
    const uint8_t blue  = (color.z < 1.0f) ? (uint8_t)(color.z * RGB565_BLUE_MAX)  : RGB565_BLUE_MAX; 
    const color_t rgb565 = (red << 11) | (green << 5) | blue;
    return rgb565;
}

inline color_t average_rgb565(color_t c1, color_t c2) {
    c1 &= 0xF7DFu;
    c1 >>= 1;
    c2 &= 0xF7DFu;
    c2 >>= 1;

    return c1 + c2;
}

#endif // __COLOR_H__
