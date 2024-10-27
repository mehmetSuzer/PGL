
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

#define zero_vec3f ((vec3f){0.0f, 0.0f, 0.0f})

inline vec3f neg_vec3f(vec3f v) {
    return (vec3f){
        -v.x,
        -v.y,
        -v.z,
    };
}

inline vec3f add_vec3f(vec3f v1, vec3f v2) {
    return (vec3f){
        v1.x + v2.x, 
        v1.y + v2.y, 
        v1.z + v2.z,
    };
}

inline vec3f sub_vec3f(vec3f v1, vec3f v2) {
    return (vec3f){
        v1.x - v2.x, 
        v1.y - v2.y, 
        v1.z - v2.z,
    };
}

inline vec3f mul_vec3f(vec3f v1, vec3f v2) {
    return (vec3f){
        v1.x * v2.x, 
        v1.y * v2.y, 
        v1.z * v2.z,
    };
}

inline vec3f div_vec3f(vec3f v1, vec3f v2) {
    return (vec3f){
        v1.x / v2.x, 
        v1.y / v2.y, 
        v1.z / v2.z,
    };
}

inline vec3f scale_vec3f(vec3f v, f32 scale) {
    return (vec3f){
        v.x * scale, 
        v.y * scale, 
        v.z * scale,
    };
}

inline f32 dot_vec3f(vec3f v1, vec3f v2) {
    return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}

inline f32 mag2_vec3f(vec3f v) {
    return dot_vec3f(v, v);
}

inline f32 mag_vec3f(vec3f v) {
    const f32 mag2 = mag2_vec3f(v);
    return sqrtf(mag2);
}

inline vec3f normalize_vec3f(vec3f v) {
    const f32 inverse_mag = 1.0f / mag_vec3f(v);
    return scale_vec3f(v, inverse_mag);
}

inline vec3f cross_vec3f(vec3f v1, vec3f v2) {
    const f32 x = v1.y * v2.z - v1.z * v2.y;
    const f32 y = v1.z * v2.x - v1.x * v2.z;
    const f32 z = v1.x * v2.y - v1.y * v2.x;
    return (vec3f){x, y, z};
}

// Reflects unit vector v with respect to unit vector n.
inline vec3f reflect_vec3f(vec3f v, vec3f n) {
    return sub_vec3f(v, scale_vec3f(n, (2.0f * dot_vec3f(n, v))));
}

// Returns the bisector of two unit vectors
inline vec3f bisector_vec3f(vec3f u1, vec3f u2) {
    return normalize_vec3f(add_vec3f(u1, u2));
}

inline vec3f interp_vec3f(vec3f v1, vec3f v2, f32 alpha) {
    return add_vec3f(v2, scale_vec3f(sub_vec3f(v1, v2), alpha));
}

inline bool epsilon_equal_vec3f(vec3f v1, vec3f v2, f32 epsilon) {
    return epsilon_equal(v1.x, v2.x, epsilon) && 
           epsilon_equal(v1.y, v2.y, epsilon) && 
           epsilon_equal(v1.z, v2.z, epsilon);
}

inline bool epsilon_not_equal_vec3f(vec3f v1, vec3f v2, f32 epsilon) {
    return epsilon_not_equal(v1.x, v2.x, epsilon) ||
           epsilon_not_equal(v1.y, v2.y, epsilon) || 
           epsilon_not_equal(v1.z, v2.z, epsilon);
}

#endif // __VEC3F_H__
