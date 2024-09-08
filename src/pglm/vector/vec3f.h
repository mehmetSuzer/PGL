
#ifndef __VEC3F_H__
#define __VEC3F_H__

#include "core/epsilon.h"

typedef struct {
    float x;
    float y;
    float z;
} vec3f;

#define zero_vec3f ((vec3f){0.0f, 0.0f, 0.0f})

inline void swap_vec3f(vec3f* v1, vec3f* v2) {
    vec3f temp = *v1;
    *v1 = *v2;
    *v2 = temp;
}

inline vec3f neg_vec3f(const vec3f v) {
    return (vec3f){
        -v.x,
        -v.y,
        -v.z,
    };
}

inline vec3f add_vec3f(const vec3f v1, const vec3f v2) {
    return (vec3f){
        v1.x + v2.x, 
        v1.y + v2.y, 
        v1.z + v2.z,
    };
}

inline vec3f sub_vec3f(const vec3f v1, const vec3f v2) {
    return (vec3f){
        v1.x - v2.x, 
        v1.y - v2.y, 
        v1.z - v2.z,
    };
}

inline vec3f mul_vec3f(const vec3f v1, const vec3f v2) {
    return (vec3f){
        v1.x * v2.x, 
        v1.y * v2.y, 
        v1.z * v2.z,
    };
}

inline vec3f div_vec3f(const vec3f v1, const vec3f v2) {
    return (vec3f){
        v1.x / v2.x, 
        v1.y / v2.y, 
        v1.z / v2.z,
    };
}

inline vec3f scale_vec3f(const vec3f v, float scale) {
    return (vec3f){
        v.x * scale, 
        v.y * scale, 
        v.z * scale,
    };
}

inline float dot_vec3f(const vec3f v1, const vec3f v2) {
    return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}

inline float mag2_vec3f(const vec3f v) {
    return dot_vec3f(v, v);
}

inline float mag_vec3f(const vec3f v) {
    const float mag2 = mag2_vec3f(v);
    return sqrtf(mag2);
}

inline vec3f normalize_vec3f(const vec3f v) {
    const float inverse_mag = 1.0f / mag_vec3f(v);
    return scale_vec3f(v, inverse_mag);
}

inline vec3f cross_vec3f(const vec3f v1, const vec3f v2) {
    const float x = v1.y * v2.z - v1.z * v2.y;
    const float y = v1.z * v2.x - v1.x * v2.z;
    const float z = v1.x * v2.y - v1.y * v2.x;
    return (vec3f){x, y, z};
}

// Reflects unit vector v with respect to unit vector n.
inline vec3f reflect_vec3f(const vec3f v, const vec3f n) {
    return sub_vec3f(v, scale_vec3f(n, (2.0f * dot_vec3f(n, v))));
}

// Returns the bisector of two unit vectors
inline vec3f bisector_vec3f(const vec3f u1, const vec3f u2) {
    return normalize_vec3f(add_vec3f(u1, u2));
}

inline bool epsilon_equal_vec3f(const vec3f v1, const vec3f v2, float epsilon) {
    return epsilon_equal(v1.x, v2.x, epsilon) && 
           epsilon_equal(v1.y, v2.y, epsilon) && 
           epsilon_equal(v1.z, v2.z, epsilon);
}

inline bool epsilon_not_equal_vec3f(const vec3f v1, const vec3f v2, float epsilon) {
    return epsilon_not_equal(v1.x, v2.x, epsilon) ||
           epsilon_not_equal(v1.y, v2.y, epsilon) || 
           epsilon_not_equal(v1.z, v2.z, epsilon);
}

#endif // __VEC3F_H__
