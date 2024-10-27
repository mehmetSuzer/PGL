
#ifndef __TRANSFORM2D_H__
#define __TRANSFORM2D_H__

#include "../vector/vec2f.h"
#include "../matrix/mat3f.h"

inline mat3f translate2D_mat3f(vec2f v) {
    return (mat3f){
        1.0f, 0.0f,  v.x,
        0.0f, 1.0f,  v.y,
        0.0f, 0.0f, 1.0f,
    };
}

inline mat2f scale2D_mat2f(vec2f v) {
    return diagonal_mat2f(v);
}

inline mat3f scale2D_mat3f(vec2f v) {
    return cast_mat2f_to_mat3f(scale2D_mat2f(v));
}

inline mat2f rotate2D_mat2f(f32 radian) {
    const f32 s = sinf(radian);
    const f32 c = cosf(radian);
    return (mat2f){
           c,   -s,
           s,    c,
    };
}

inline mat3f rotate2D_mat3f(f32 radian) {
    return cast_mat2f_to_mat3f(rotate2D_mat2f(radian));
}

inline mat2f shear2D_mat2f(f32 xy, f32 yx) {
    return (mat2f){
        1.0f,   xy,
          yx, 1.0f,
    };
}

inline mat3f shear2D_mat3f(f32 xy, f32 yx) {
    return cast_mat2f_to_mat3f(shear2D_mat2f(xy, yx));
}

#endif // __TRANSFORM2D_H__
