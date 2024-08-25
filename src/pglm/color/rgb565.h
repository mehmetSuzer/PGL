
#ifndef __RGB565_H__
#define __RGB565_H__

#include "../vector/vec3f.h"

#define RGB565_RED_MAX      ((uint8_t)0x1F)
#define RGB565_GREEN_MAX    ((uint8_t)0x3F)
#define RGB565_BLUE_MAX     ((uint8_t)0x1F)

inline uint16_t vec3f_to_rgb565(const vec3f color) {
    const uint8_t red   = (color.x < 1.0f) ? (uint8_t)(color.x * RGB565_RED_MAX)   : RGB565_RED_MAX; 
    const uint8_t green = (color.y < 1.0f) ? (uint8_t)(color.y * RGB565_GREEN_MAX) : RGB565_GREEN_MAX; 
    const uint8_t blue  = (color.z < 1.0f) ? (uint8_t)(color.z * RGB565_BLUE_MAX)  : RGB565_BLUE_MAX; 
    const uint16_t rgb565 = (red << 11) | (green << 5) | blue;
    return rgb565;
}

inline uint16_t average_rgb565(uint16_t c1, uint16_t c2) {
    c1 &= 0xF7DFu;
    c1 >>= 1;
    c2 &= 0xF7DFu;
    c2 >>= 1;

    return c1 + c2;
}

#endif // __RGB565_H__
