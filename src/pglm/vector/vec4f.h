
#ifndef __VEC4F_H__
#define __VEC4F_H__

#include "core/epsilon.h"

typedef union {
    struct {
        float x;
        float y;
        float z;
        float w;
    };

    struct {
        float r;
        float g;
        float b;
        float a;
    };
} vec4f;

#define zero_vec4f ((vec4f){0.0f, 0.0f, 0.0f, 0.0f})

inline void swap_vec4f(vec4f* v1, vec4f* v2) {
    vec4f temp = *v1;
    *v1 = *v2;
    *v2 = temp;
}

inline vec4f neg_vec4f(const vec4f v) {
    return (vec4f){
        -v.x,
        -v.y,
        -v.z,
        -v.w,
    };
}

inline vec4f add_vec4f(const vec4f v1, const vec4f v2) {
    return (vec4f){
        v1.x + v2.x, 
        v1.y + v2.y, 
        v1.z + v2.z,
        v1.w + v2.w,
    };
}

inline vec4f sub_vec4f(const vec4f v1, const vec4f v2) {
    return (vec4f){
        v1.x - v2.x, 
        v1.y - v2.y, 
        v1.z - v2.z,
        v1.w - v2.w,
    };
}

inline vec4f mul_vec4f(const vec4f v1, const vec4f v2) {
    return (vec4f){
        v1.x * v2.x, 
        v1.y * v2.y, 
        v1.z * v2.z,
        v1.w * v2.w,
    };
}

inline vec4f div_vec4f(const vec4f v1, const vec4f v2) {
    return (vec4f){
        v1.x / v2.x, 
        v1.y / v2.y, 
        v1.z / v2.z,
        v1.w / v2.w,
    };
}

inline vec4f scale_vec4f(const vec4f v, float scale) {
    return (vec4f){
        v.x * scale, 
        v.y * scale, 
        v.z * scale,
        v.w * scale,
    };
}

inline float dot_vec4f(const vec4f v1, const vec4f v2) {
    return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z + v1.w * v2.w;
}

inline float mag2_vec4f(const vec4f v) {
    return dot_vec4f(v, v);
}

inline float mag_vec4f(const vec4f v) {
    const float mag2 = mag2_vec4f(v);
    return sqrtf(mag2);
}

inline vec4f normalize_vec4f(const vec4f v) {
    const float inverse_mag = 1.0f / mag_vec4f(v);
    return scale_vec4f(v, inverse_mag);
}

inline bool epsilon_equal_vec4f(const vec4f v1, const vec4f v2, float epsilon) {
    return epsilon_equal(v1.x, v2.x, epsilon) && 
           epsilon_equal(v1.y, v2.y, epsilon) && 
           epsilon_equal(v1.z, v2.z, epsilon) &&
           epsilon_equal(v1.w, v2.w, epsilon);
}

inline bool epsilon_not_equal_vec4f(const vec4f v1, const vec4f v2, float epsilon) {
    return epsilon_not_equal(v1.x, v2.x, epsilon) ||
           epsilon_not_equal(v1.y, v2.y, epsilon) || 
           epsilon_not_equal(v1.z, v2.z, epsilon) ||
           epsilon_not_equal(v1.w, v2.w, epsilon);
}

#endif // __VEC4F_H__
