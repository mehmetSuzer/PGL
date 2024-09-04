
#ifndef __TRANSFORM3D_H__
#define __TRANSFORM3D_H__

#include "../vector/vec3f.h"
#include "../matrix/mat4f.h"

inline mat4f translate3D_mat4f(const vec3f v) {
    return (mat4f){
        1.0f, 0.0f, 0.0f,  v.x,
        0.0f, 1.0f, 0.0f,  v.y,
        0.0f, 0.0f, 1.0f,  v.z,
        0.0f, 0.0f, 0.0f, 1.0f,
    };
}

inline mat3f scale3D_mat3f(const vec3f v) {
    return diagonal_mat3f(v);
}

inline mat4f scale3D_mat4f(const vec3f v) {
    return cast_mat3f_to_mat4f(scale3D_mat3f(v));
}

static inline mat3f rotate3D_x_mat3f(float radian) {
    const float s = sinf(radian);
    const float c = cosf(radian);
    return (mat3f){
        1.0f, 0.0f, 0.0f,
        0.0f,    c,   -s,
        0.0f,    s,    c,
    };
}

static inline mat4f rotate3D_x_mat4f(float radian) {
    return cast_mat3f_to_mat4f(rotate3D_x_mat3f(radian));
}

static inline mat3f rotate3D_y_mat3f(float radian) {
    const float s = sinf(radian);
    const float c = cosf(radian);
    return (mat3f){
           c, 0.0f,    s,
        0.0f, 1.0f, 0.0f,
          -s, 0.0f,    c,
    };
}

static inline mat4f rotate3D_y_mat4f(float radian) {
    return cast_mat3f_to_mat4f(rotate3D_y_mat3f(radian));
}

static inline mat3f rotate3D_z_mat3f(float radian) {
    const float s = sinf(radian);
    const float c = cosf(radian);
    return (mat3f){
           c,   -s, 0.0f,
           s,    c, 0.0f,
        0.0f, 0.0f, 1.0f,
    };
}

static inline mat4f rotate3D_z_mat4f(float radian) {
    return cast_mat3f_to_mat4f(rotate3D_z_mat3f(radian));
}

inline mat3f shear3D_mat3f(float xy, float xz, float yx, float yz, float zx, float zy) {
    return (mat3f){
        1.0f,   xy,   xz,
          yx, 1.0f,   yz,
          zx,   zy, 1.0f,
    };
}

static inline mat4f shear3D_mat4f(float xy, float xz, float yx, float yz, float zx, float zy) {
    return cast_mat3f_to_mat4f(shear3D_mat3f(xy, xz, yx, yz, zx, zy));
}

#endif // __TRANSFORM3D_H__
