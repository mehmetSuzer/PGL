
#ifndef __VEC2F_H__
#define __VEC2F_H__

#include "core/epsilon.h"

typedef union {
    struct {
        float x;
        float y;
    };

    struct {
        float r;
        float i;
    };

    struct {
        float u;
        float v;
    };

    struct {
        float s;
        float t;
    };
} vec2f;

#define zero_vec2f ((vec2f){0.0f, 0.0f})

inline void swap_vec2f(vec2f* v1, vec2f* v2) {
    vec2f temp = *v1;
    *v1 = *v2;
    *v2 = temp;
}

inline vec2f neg_vec2f(const vec2f v) {
    return (vec2f){
        -v.x,
        -v.y,
    };
}

inline vec2f add_vec2f(const vec2f v1, const vec2f v2) {
    return (vec2f){
        v1.x + v2.x, 
        v1.y + v2.y,
    };
}

inline vec2f sub_vec2f(const vec2f v1, const vec2f v2) {
    return (vec2f){
        v1.x - v2.x, 
        v1.y - v2.y,
    };
}

inline vec2f mul_vec2f(const vec2f v1, const vec2f v2) {
    return (vec2f){
        v1.x * v2.x, 
        v1.y * v2.y,
    };
}

inline vec2f div_vec2f(const vec2f v1, const vec2f v2) {
    return (vec2f){
        v1.x / v2.x, 
        v1.y / v2.y,
    };
}

inline vec2f scale_vec2f(const vec2f v, float scale) {
    return (vec2f){
        v.x * scale, 
        v.y * scale,
    };
}

inline float dot_vec2f(const vec2f v1, const vec2f v2) {
    return v1.x * v2.x + v1.y * v2.y;
}

inline float mag2_vec2f(const vec2f v) {
    return dot_vec2f(v, v);
}

inline float mag_vec2f(const vec2f v) {
    const float mag2 = mag2_vec2f(v);
    return sqrtf(mag2);
}

inline vec2f normalize_vec2f(const vec2f v) {
    const float inverse_mag = 1.0f / mag_vec2f(v);
    return scale_vec2f(v, inverse_mag);
}

inline bool epsilon_equal_vec2f(const vec2f v1, const vec2f v2, float epsilon) {
    return epsilon_equal(v1.x, v2.x, epsilon) && 
           epsilon_equal(v1.y, v2.y, epsilon);
}

inline bool epsilon_not_equal_vec2f(const vec2f v1, const vec2f v2, float epsilon) {
    return epsilon_not_equal(v1.x, v2.x, epsilon) ||
           epsilon_not_equal(v1.y, v2.y, epsilon);
}

#endif // __VEC2F_H__
