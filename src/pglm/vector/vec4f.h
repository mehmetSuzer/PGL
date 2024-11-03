
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

#define vec4f_zero ((vec4f){{0.0f, 0.0f, 0.0f, 0.0f}})

inline vec4f vec4f_negate(vec4f v) {
    return (vec4f){{
        -v.x,
        -v.y,
        -v.z,
        -v.w,
    }};
}

inline vec4f vec4f_add(vec4f v1, vec4f v2) {
    return (vec4f){{
        v1.x + v2.x, 
        v1.y + v2.y, 
        v1.z + v2.z,
        v1.w + v2.w,
    }};
}

inline vec4f vec4f_sub(vec4f v1, vec4f v2) {
    return (vec4f){{
        v1.x - v2.x, 
        v1.y - v2.y, 
        v1.z - v2.z,
        v1.w - v2.w,
    }};
}

inline vec4f vec4f_mul(vec4f v1, vec4f v2) {
    return (vec4f){{
        v1.x * v2.x, 
        v1.y * v2.y, 
        v1.z * v2.z,
        v1.w * v2.w,
    }};
}

inline vec4f vec4f_div(vec4f v1, vec4f v2) {
    return (vec4f){{
        v1.x / v2.x, 
        v1.y / v2.y, 
        v1.z / v2.z,
        v1.w / v2.w,
    }};
}

inline vec4f vec4f_scale(vec4f v, f32 scale) {
    return (vec4f){{
        v.x * scale, 
        v.y * scale, 
        v.z * scale,
        v.w * scale,
    }};
}

inline f32 vec4f_dot(vec4f v1, vec4f v2) {
    return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z + v1.w * v2.w;
}

inline f32 vec4f_mag2(vec4f v) {
    return vec4f_dot(v, v);
}

inline f32 vec4f_mag(vec4f v) {
    const f32 mag2 = vec4f_mag2(v);
    return sqrtf(mag2);
}

inline vec4f vec4f_normalize(vec4f v) {
    const f32 inverse_mag = 1.0f / vec4f_mag(v);
    return vec4f_scale(v, inverse_mag);
}

inline vec4f vec4f_interp(vec4f v1, vec4f v2, f32 alpha) {
    return vec4f_add(v2, vec4f_scale(vec4f_sub(v1, v2), alpha));
}

inline bool vec4f_epsilon_equal(vec4f v1, vec4f v2, f32 epsilon) {
    return epsilon_equal(v1.x, v2.x, epsilon) && 
           epsilon_equal(v1.y, v2.y, epsilon) && 
           epsilon_equal(v1.z, v2.z, epsilon) &&
           epsilon_equal(v1.w, v2.w, epsilon);
}

inline bool vec4f_epsilon_not_equal(vec4f v1, vec4f v2, f32 epsilon) {
    return epsilon_not_equal(v1.x, v2.x, epsilon) ||
           epsilon_not_equal(v1.y, v2.y, epsilon) || 
           epsilon_not_equal(v1.z, v2.z, epsilon) ||
           epsilon_not_equal(v1.w, v2.w, epsilon);
}

#endif // __VEC4F_H__
