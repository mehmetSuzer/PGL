
#ifndef __VEC4F_H__
#define __VEC4F_H__

#include "../core/epsilon.h"

typedef union {
    struct {
        f32 x;
        f32 y;
        f32 z;
        f32 w;
    };
    struct {
        f32 r;
        f32 g;
        f32 b;
        f32 a;
    };
    f32 n[4];
} vec4f;

#define zero_vec4f ((vec4f){0.0f, 0.0f, 0.0f, 0.0f})

inline vec4f neg_vec4f(vec4f v) {
    return (vec4f){
        -v.x,
        -v.y,
        -v.z,
        -v.w,
    };
}

inline vec4f add_vec4f(vec4f v1, vec4f v2) {
    return (vec4f){
        v1.x + v2.x, 
        v1.y + v2.y, 
        v1.z + v2.z,
        v1.w + v2.w,
    };
}

inline vec4f sub_vec4f(vec4f v1, vec4f v2) {
    return (vec4f){
        v1.x - v2.x, 
        v1.y - v2.y, 
        v1.z - v2.z,
        v1.w - v2.w,
    };
}

inline vec4f mul_vec4f(vec4f v1, vec4f v2) {
    return (vec4f){
        v1.x * v2.x, 
        v1.y * v2.y, 
        v1.z * v2.z,
        v1.w * v2.w,
    };
}

inline vec4f div_vec4f(vec4f v1, vec4f v2) {
    return (vec4f){
        v1.x / v2.x, 
        v1.y / v2.y, 
        v1.z / v2.z,
        v1.w / v2.w,
    };
}

inline vec4f scale_vec4f(vec4f v, f32 scale) {
    return (vec4f){
        v.x * scale, 
        v.y * scale, 
        v.z * scale,
        v.w * scale,
    };
}

inline f32 dot_vec4f(vec4f v1, vec4f v2) {
    return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z + v1.w * v2.w;
}

inline f32 mag2_vec4f(vec4f v) {
    return dot_vec4f(v, v);
}

inline f32 mag_vec4f(vec4f v) {
    const f32 mag2 = mag2_vec4f(v);
    return sqrtf(mag2);
}

inline vec4f normalize_vec4f(vec4f v) {
    const f32 inverse_mag = 1.0f / mag_vec4f(v);
    return scale_vec4f(v, inverse_mag);
}

inline vec4f interp_vec4f(vec4f v1, vec4f v2, f32 alpha) {
    return add_vec4f(v2, scale_vec4f(sub_vec4f(v1, v2), alpha));
}

inline bool epsilon_equal_vec4f(vec4f v1, vec4f v2, f32 epsilon) {
    return epsilon_equal(v1.x, v2.x, epsilon) && 
           epsilon_equal(v1.y, v2.y, epsilon) && 
           epsilon_equal(v1.z, v2.z, epsilon) &&
           epsilon_equal(v1.w, v2.w, epsilon);
}

inline bool epsilon_not_equal_vec4f(vec4f v1, vec4f v2, f32 epsilon) {
    return epsilon_not_equal(v1.x, v2.x, epsilon) ||
           epsilon_not_equal(v1.y, v2.y, epsilon) || 
           epsilon_not_equal(v1.z, v2.z, epsilon) ||
           epsilon_not_equal(v1.w, v2.w, epsilon);
}

#endif // __VEC4F_H__
