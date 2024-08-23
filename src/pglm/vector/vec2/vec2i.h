
#ifndef __VEC2I_H__
#define __VEC2I_H__

#include <stdint.h>

// -------------------------------------------------------------------------- //

typedef struct  {
    int8_t x;
    int8_t y;
} vec2_i8;

inline vec2_i8 add_vec2_i8(const vec2_i8 v1, const vec2_i8 v2) {
    return (vec2_i8) {
        v1.x + v2.x, 
        v1.y + v2.y,
    };
}

inline vec2_i8 sub_vec2_i8(const vec2_i8 v1, const vec2_i8 v2) {
    return (vec2_i8) {
        v1.x - v2.x, 
        v1.y - v2.y,
    };
}

inline vec2_i8 mul_vec2_i8(const vec2_i8 v, const int8_t scale) {
    return (vec2_i8) {
        v.x * scale, 
        v.y * scale,
    };
}

// -------------------------------------------------------------------------- //

typedef struct {
    int16_t x;
    int16_t y;
} vec2_i16;

inline vec2_i16 add_vec2_i16(const vec2_i16 v1, const vec2_i16 v2) {
    return (vec2_i16) {
        v1.x + v2.x, 
        v1.y + v2.y,
    };
}

inline vec2_i16 sub_vec2_i16(const vec2_i16 v1, const vec2_i16 v2) {
    return (vec2_i16) {
        v1.x - v2.x, 
        v1.y - v2.y,
    };
}

inline vec2_i16 mul_vec2_i16(const vec2_i16 v, const int16_t scale) {
    return (vec2_i16) {
        v.x * scale, 
        v.y * scale,
    };
}

// -------------------------------------------------------------------------- //

typedef struct {
    int32_t x;
    int32_t y;
} vec2_i32;

inline vec2_i32 add_vec2_i32(const vec2_i32 v1, const vec2_i32 v2) {
    return (vec2_i32) {
        v1.x + v2.x, 
        v1.y + v2.y,
    };
}

inline vec2_i32 sub_vec2_i32(const vec2_i32 v1, const vec2_i32 v2) {
    return (vec2_i32) {
        v1.x - v2.x, 
        v1.y - v2.y,
    };
}

inline vec2_i32 mul_vec2_i32(const vec2_i16 v, const int32_t scale) {
    return (vec2_i32) {
        v.x * scale, 
        v.y * scale,
    };
}

// -------------------------------------------------------------------------- //

#endif // __VEC2I_H__
