
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

#define zero_vec2f ((vec2f){0.0f, 0.0f})

inline vec2f neg_vec2f(vec2f v) {
    return (vec2f){
        -v.x,
        -v.y,
    };
}

inline vec2f add_vec2f(vec2f v1, vec2f v2) {
    return (vec2f){
        v1.x + v2.x, 
        v1.y + v2.y,
    };
}

inline vec2f sub_vec2f(vec2f v1, vec2f v2) {
    return (vec2f){
        v1.x - v2.x, 
        v1.y - v2.y,
    };
}

inline vec2f mul_vec2f(vec2f v1, vec2f v2) {
    return (vec2f){
        v1.x * v2.x, 
        v1.y * v2.y,
    };
}

inline vec2f div_vec2f(vec2f v1, vec2f v2) {
    return (vec2f){
        v1.x / v2.x, 
        v1.y / v2.y,
    };
}

inline vec2f scale_vec2f(vec2f v, f32 scale) {
    return (vec2f){
        v.x * scale, 
        v.y * scale,
    };
}

inline f32 dot_vec2f(vec2f v1, vec2f v2) {
    return v1.x * v2.x + v1.y * v2.y;
}

inline f32 mag2_vec2f(vec2f v) {
    return dot_vec2f(v, v);
}

inline f32 mag_vec2f(vec2f v) {
    const f32 mag2 = mag2_vec2f(v);
    return sqrtf(mag2);
}

inline vec2f normalize_vec2f(vec2f v) {
    const f32 inverse_mag = 1.0f / mag_vec2f(v);
    return scale_vec2f(v, inverse_mag);
}

inline vec2f interp_vec2f(vec2f v1, vec2f v2, f32 alpha) {
    return add_vec2f(v2, scale_vec2f(sub_vec2f(v1, v2), alpha));
}

inline bool epsilon_equal_vec2f(vec2f v1, vec2f v2, f32 epsilon) {
    return epsilon_equal(v1.x, v2.x, epsilon) && 
           epsilon_equal(v1.y, v2.y, epsilon);
}

inline bool epsilon_not_equal_vec2f(vec2f v1, vec2f v2, f32 epsilon) {
    return epsilon_not_equal(v1.x, v2.x, epsilon) ||
           epsilon_not_equal(v1.y, v2.y, epsilon);
}

#endif // __VEC2F_H__
