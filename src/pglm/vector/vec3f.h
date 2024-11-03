
#ifndef __VEC3F_H__
#define __VEC3F_H__

#include "../core/epsilon.h"

typedef union {
    struct {
        f32 x;
        f32 y;
        f32 z;
    };
    struct {
        f32 r;
        f32 g;
        f32 b;
    };
    struct {
        f32 u;
        f32 v;
        f32 w;
    };
    f32 n[3];
} vec3f;

#define vec3f_zero ((vec3f){{0.0f, 0.0f, 0.0f}})

inline vec3f vec3f_negate(vec3f v) {
    return (vec3f){{
        -v.x,
        -v.y,
        -v.z,
    }};
}

inline vec3f vec3f_add(vec3f v1, vec3f v2) {
    return (vec3f){{
        v1.x + v2.x, 
        v1.y + v2.y, 
        v1.z + v2.z,
    }};
}

inline vec3f vec3f_sub(vec3f v1, vec3f v2) {
    return (vec3f){{
        v1.x - v2.x, 
        v1.y - v2.y, 
        v1.z - v2.z,
    }};
}

inline vec3f vec3f_mul(vec3f v1, vec3f v2) {
    return (vec3f){{
        v1.x * v2.x, 
        v1.y * v2.y, 
        v1.z * v2.z,
    }};
}

inline vec3f vec3f_div(vec3f v1, vec3f v2) {
    return (vec3f){{
        v1.x / v2.x, 
        v1.y / v2.y, 
        v1.z / v2.z,
    }};
}

inline vec3f vec3f_scale(vec3f v, f32 scale) {
    return (vec3f){{
        v.x * scale, 
        v.y * scale, 
        v.z * scale,
    }};
}

inline f32 vec3f_dot(vec3f v1, vec3f v2) {
    return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}

inline f32 vec3f_mag2(vec3f v) {
    return vec3f_dot(v, v);
}

inline f32 vec3f_mag(vec3f v) {
    const f32 mag2 = vec3f_mag2(v);
    return sqrtf(mag2);
}

inline vec3f vec3f_normalize(vec3f v) {
    const f32 inverse_mag = 1.0f / vec3f_mag(v);
    return vec3f_scale(v, inverse_mag);
}

inline vec3f vec3f_cross(vec3f v1, vec3f v2) {
    const f32 x = v1.y * v2.z - v1.z * v2.y;
    const f32 y = v1.z * v2.x - v1.x * v2.z;
    const f32 z = v1.x * v2.y - v1.y * v2.x;
    return (vec3f){{x, y, z}};
}

// Reflects unit vector v with respect to unit vector n.
inline vec3f vec3f_reflect(vec3f v, vec3f n) {
    return vec3f_sub(v, vec3f_scale(n, (2.0f * vec3f_dot(n, v))));
}

// Returns the bisector of two unit vectors
inline vec3f vec3f_bisector(vec3f u1, vec3f u2) {
    return vec3f_normalize(vec3f_add(u1, u2));
}

inline vec3f vec3f_interp(vec3f v1, vec3f v2, f32 alpha) {
    return vec3f_add(v2, vec3f_scale(vec3f_sub(v1, v2), alpha));
}

inline bool vec3f_epsilon_equal(vec3f v1, vec3f v2, f32 epsilon) {
    return epsilon_equal(v1.x, v2.x, epsilon) && 
           epsilon_equal(v1.y, v2.y, epsilon) && 
           epsilon_equal(v1.z, v2.z, epsilon);
}

inline bool vec3f_epsilon_not_equal(vec3f v1, vec3f v2, f32 epsilon) {
    return epsilon_not_equal(v1.x, v2.x, epsilon) ||
           epsilon_not_equal(v1.y, v2.y, epsilon) || 
           epsilon_not_equal(v1.z, v2.z, epsilon);
}

#endif // __VEC3F_H__
