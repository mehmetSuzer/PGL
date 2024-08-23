
#ifndef __VEC2F_H__
#define __VEC2F_H__

#include "core/fast.h"

typedef struct {
    float x;
    float y;
} vec2f;

inline vec2f neg_vec2f(const vec2f v) {
    return (vec2f) {
        -v.x,
        -v.y,
    };
}

inline vec2f add_vec2f(const vec2f v1, const vec2f v2) {
    return (vec2f) {
        v1.x + v2.x, 
        v1.y + v2.y,
    };
}

inline vec2f sub_vec2f(const vec2f v1, const vec2f v2) {
    return (vec2f) {
        v1.x - v2.x, 
        v1.y - v2.y,
    };
}

inline vec2f mul_vec2f(const vec2f v1, const vec2f v2) {
    return (vec2f) {
        v1.x * v2.x, 
        v1.y * v2.y,
    };
}

inline vec2f div_vec2f(const vec2f v1, const vec2f v2) {
    return (vec2f) {
        v1.x / v2.x, 
        v1.y / v2.y,
    };
}

inline vec2f scale_vec2f(const vec2f v, const float scale) {
    return (vec2f) {
        v.x * scale, 
        v.y * scale,
    };
}

inline float dot_vec2f(const vec2f v1, const vec2f v2) {
    return v1.x * v2.x + v1.y * v2.y;
}

static inline float mag2_vec2f(const vec2f v) {
    return dot_vec2f(v, v);
}

static inline float mag_vec2f(const vec2f v) {
    const float mag2 = mag2_vec2f(v);
    return fast_square_root(mag2);
}

static inline vec2f normalize_vec2f(const vec2f v) {
    const float inverse_mag = fast_inverse_square_root(mag2_vec2f(v));
    return scale_vec2f(v, inverse_mag);
}

inline bool epsilon_equal_vec2f(const vec2f v1, const vec2f v2, const float epsilon) {
    return epsilon_equal(v1.x, v2.x, epsilon) && 
           epsilon_equal(v1.y, v2.y, epsilon);
}

inline bool epsilon_not_equal_vec2f(const vec2f v1, const vec2f v2, const float epsilon) {
    return epsilon_not_equal(v1.x, v2.x, epsilon) && 
           epsilon_not_equal(v1.y, v2.y, epsilon);
}

#endif // __VEC2F_H__
