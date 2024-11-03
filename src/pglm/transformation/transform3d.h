
#ifndef __TRANSFORM3D_H__
#define __TRANSFORM3D_H__

#include "../vector/vec3f.h"
#include "../matrix/mat4f.h"
#include "../cast/cast.h"

inline mat4f translate3D_mat4f(vec3f v) {
    return (mat4f){
        1.0f, 0.0f, 0.0f,  v.x,
        0.0f, 1.0f, 0.0f,  v.y,
        0.0f, 0.0f, 1.0f,  v.z,
        0.0f, 0.0f, 0.0f, 1.0f,
    };
}

inline mat3f scale3D_mat3f(vec3f v) {
    return mat3f_diagonal(v);
}

inline mat4f scale3D_mat4f(vec3f v) {
    return cast_mat3f_to_mat4f(scale3D_mat3f(v));
}

static inline mat3f rotate3D_x_mat3f(f32 radian) {
    const f32 s = sinf(radian);
    const f32 c = cosf(radian);
    return (mat3f){
        1.0f, 0.0f, 0.0f,
        0.0f,    c,   -s,
        0.0f,    s,    c,
    };
}

static inline mat4f rotate3D_x_mat4f(f32 radian) {
    return cast_mat3f_to_mat4f(rotate3D_x_mat3f(radian));
}

static inline mat3f rotate3D_y_mat3f(f32 radian) {
    const f32 s = sinf(radian);
    const f32 c = cosf(radian);
    return (mat3f){
           c, 0.0f,    s,
        0.0f, 1.0f, 0.0f,
          -s, 0.0f,    c,
    };
}

static inline mat4f rotate3D_y_mat4f(f32 radian) {
    return cast_mat3f_to_mat4f(rotate3D_y_mat3f(radian));
}

static inline mat3f rotate3D_z_mat3f(f32 radian) {
    const f32 s = sinf(radian);
    const f32 c = cosf(radian);
    return (mat3f){
           c,   -s, 0.0f,
           s,    c, 0.0f,
        0.0f, 0.0f, 1.0f,
    };
}

static inline mat4f rotate3D_z_mat4f(f32 radian) {
    return cast_mat3f_to_mat4f(rotate3D_z_mat3f(radian));
}

inline mat3f shear3D_mat3f(f32 xy, f32 xz, f32 yx, f32 yz, f32 zx, f32 zy) {
    return (mat3f){
        1.0f,   xy,   xz,
          yx, 1.0f,   yz,
          zx,   zy, 1.0f,
    };
}

static inline mat4f shear3D_mat4f(f32 xy, f32 xz, f32 yx, f32 yz, f32 zx, f32 zy) {
    return cast_mat3f_to_mat4f(shear3D_mat3f(xy, xz, yx, yz, zx, zy));
}

#endif // __TRANSFORM3D_H__
