
#ifndef __RGB565_H__
#define __RGB565_H__

#include "../vector/vec3f.h"

                                                   //   rrrr rggg gggb bbbb
#define PGLM_RGB565_RED         ((uint16_t)0xF800) // 0b1111 1000 0000 0000
#define PGLM_RGB565_GREEN       ((uint16_t)0x07E0) // 0b0000 0111 1110 0000
#define PGLM_RGB565_BLUE        ((uint16_t)0x001F) // 0b0000 0000 0001 1111

#define PGLM_RGB565_YELLOW      (PGLM_RGB565_RED   | PGLM_RGB565_GREEN) // 0b1111 1111 1110 0000
#define PGLM_RGB565_MAGENTA     (PGLM_RGB565_RED   | PGLM_RGB565_BLUE)  // 0b1111 1000 0001 1111
#define PGLM_RGB565_CYAN        (PGLM_RGB565_GREEN | PGLM_RGB565_BLUE)  // 0b0000 0111 1111 1111

#define PGLM_RGB565_BLACK       ((uint16_t)0x0000) // 0b0000 0000 0000 0000
#define PGLM_RGB565_WHITE       (PGLM_RGB565_RED | PGLM_RGB565_GREEN | PGLM_RGB565_BLUE) // 0b1111 1111 1111 1111

#define PGLM_RGB565_DRED        ((uint16_t)0x8000) // 0b1000 0000 0000 0000
#define PGLM_RGB565_DGREEN      ((uint16_t)0x0400) // 0b0000 0100 0000 0000
#define PGLM_RGB565_DBLUE       ((uint16_t)0x0010) // 0b0000 0000 0001 0000

#define PGLM_RGB565_DYELLOW     (PGLM_RGB565_DRED   | PGLM_RGB565_DGREEN) // 0b1000 0100 0000 0000
#define PGLM_RGB565_DMAGENTA    (PGLM_RGB565_DRED   | PGLM_RGB565_DBLUE)  // 0b1000 0000 0001 0000
#define PGLM_RGB565_DCYAN       (PGLM_RGB565_DGREEN | PGLM_RGB565_DBLUE)  // 0b0000 0100 0001 0000

#define PGLM_RGB565_GRAY        (PGLM_RGB565_DRED | PGLM_RGB565_DGREEN | PGLM_RGB565_DBLUE) // 0b1000 0100 0001 0000

#define PGLM_RGB565_BROWN       ((uint16_t)0xA145) // 0b1010 0001 0100 0101
#define PGLM_RGB565_ORANGE      ((uint16_t)0xFC07) // 0b1111 1100 0000 0111


#define PGLM_RGB565_RED_MAX     ((uint8_t)0x1F)
#define PGLM_RGB565_GREEN_MAX   ((uint8_t)0x3F)
#define PGLM_RGB565_BLUE_MAX    ((uint8_t)0x1F)

inline uint16_t vec3f_to_rgb565(const vec3f color) {
    const uint8_t red   = (color.x < 1.0f) ? (uint8_t)(color.x * PGLM_RGB565_RED_MAX)   : PGLM_RGB565_RED_MAX; 
    const uint8_t green = (color.y < 1.0f) ? (uint8_t)(color.y * PGLM_RGB565_GREEN_MAX) : PGLM_RGB565_GREEN_MAX; 
    const uint8_t blue  = (color.z < 1.0f) ? (uint8_t)(color.z * PGLM_RGB565_BLUE_MAX)  : PGLM_RGB565_BLUE_MAX; 
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
