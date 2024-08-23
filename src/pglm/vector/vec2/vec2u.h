
#ifndef __VEC2U_H__
#define __VEC2U_H__

#include <stdint.h>

// -------------------------------------------------------------------------- //

typedef struct {
    uint8_t x;
    uint8_t y;
} vec2_u8;

inline vec2_u8 add_vec2_u8(const vec2_u8 v1, const vec2_u8 v2) {
    return (vec2_u8) {
        v1.x + v2.x, 
        v1.y + v2.y,
    };
}

inline vec2_u8 sub_vec2_u8(const vec2_u8 v1, const vec2_u8 v2) {
    return (vec2_u8) {
        v1.x - v2.x, 
        v1.y - v2.y,
    };
}

inline vec2_u8 mul_vec2_u8(const vec2_u8 v, const uint8_t scale) {
    return (vec2_u8) {
        v.x * scale, 
        v.y * scale,
    };
}

// -------------------------------------------------------------------------- //

typedef struct {
    uint16_t x;
    uint16_t y;
} vec2_u16;

inline vec2_u16 add_vec2_u16(const vec2_u16 v1, const vec2_u16 v2) {
    return (vec2_u16) {
        v1.x + v2.x, 
        v1.y + v2.y,
    };
}

inline vec2_u16 sub_vec2_u16(const vec2_u16 v1, const vec2_u16 v2) {
    return (vec2_u16) {
        v1.x - v2.x, 
        v1.y - v2.y,
    };
}

inline vec2_u16 mul_vec2_u16(const vec2_u16 v, const uint16_t scale) {
    return (vec2_u16) {
        v.x * scale, 
        v.y * scale,
    };
}

// -------------------------------------------------------------------------- //

typedef struct {
    uint32_t x;
    uint32_t y;
} vec2_u32;

inline vec2_u32 add_vec2_u32(const vec2_u32 v1, const vec2_u32 v2) {
    return (vec2_u32) {
        v1.x + v2.x, 
        v1.y + v2.y,
    };
}

inline vec2_u32 sub_vec2_u32(const vec2_u32 v1, const vec2_u32 v2) {
    return (vec2_u32) {
        v1.x - v2.x, 
        v1.y - v2.y,
    };
}

inline vec2_u32 mul_vec2_u32(const vec2_u32 v, const uint32_t scale) {
    return (vec2_u32) {
        v.x * scale, 
        v.y * scale,
    };
}

// -------------------------------------------------------------------------- //

#endif // __VEC2U_H__
