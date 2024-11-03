
#ifndef __VEC2F_H__
#define __VEC2F_H__

#include "../core/epsilon.h"

typedef union {
    struct {
        f32 x;
        f32 y;
    };
    struct {
        f32 r;
        f32 i;
    };
    struct {
        f32 u;
        f32 v;
    };
    struct {
        f32 s;
        f32 t;
    };
    f32 n[2];
} vec2f;

#define vec2f_zero ((vec2f){0.0f, 0.0f})

inline vec2f vec2f_negate(vec2f v) {
    return (vec2f){
        -v.x,
        -v.y,
    };
}

inline vec2f vec2f_add(vec2f v1, vec2f v2) {
    return (vec2f){
        v1.x + v2.x, 
        v1.y + v2.y,
    };
}

inline vec2f vec2f_sub(vec2f v1, vec2f v2) {
    return (vec2f){
        v1.x - v2.x, 
        v1.y - v2.y,
    };
}

inline vec2f vec2f_mul(vec2f v1, vec2f v2) {
    return (vec2f){
        v1.x * v2.x, 
        v1.y * v2.y,
    };
}

inline vec2f vec2f_div(vec2f v1, vec2f v2) {
    return (vec2f){
        v1.x / v2.x, 
        v1.y / v2.y,
    };
}

inline vec2f vec2f_scale(vec2f v, f32 scale) {
    return (vec2f){
        v.x * scale, 
        v.y * scale,
    };
}

inline f32 vec2f_dot(vec2f v1, vec2f v2) {
    return v1.x * v2.x + v1.y * v2.y;
}

inline f32 vec2f_mag2(vec2f v) {
    return vec2f_dot(v, v);
}

inline f32 vec2f_mag(vec2f v) {
    const f32 mag2 = vec2f_mag2(v);
    return sqrtf(mag2);
}

inline vec2f vec2f_normalize(vec2f v) {
    const f32 inverse_mag = 1.0f / vec2f_mag(v);
    return vec2f_scale(v, inverse_mag);
}

inline vec2f vec2f_interp(vec2f v1, vec2f v2, f32 alpha) {
    return vec2f_add(v2, vec2f_scale(vec2f_sub(v1, v2), alpha));
}

inline bool vec2f_epsilon_equal(vec2f v1, vec2f v2, f32 epsilon) {
    return epsilon_equal(v1.x, v2.x, epsilon) && 
           epsilon_equal(v1.y, v2.y, epsilon);
}

inline bool vec2f_epsilon_not_equal(vec2f v1, vec2f v2, f32 epsilon) {
    return epsilon_not_equal(v1.x, v2.x, epsilon) ||
           epsilon_not_equal(v1.y, v2.y, epsilon);
}

#endif // __VEC2F_H__
