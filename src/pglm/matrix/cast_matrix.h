
#ifndef __CAST_MATRIX_H__
#define __CAST_MATRIX_H__

#include "../matrix/mat2f.h"
#include "../matrix/mat3f.h"
#include "../matrix/mat4f.h"

inline mat3f cast_mat2f_to_mat3f(mat2f m) {
    return (mat3f){
        m.xx, m.xy, 0.0f,
        m.yx, m.yy, 0.0f,
        0.0f, 0.0f, 1.0f,
    };
}

inline mat2f cast_mat3f_to_mat2f(mat3f m) {
    return (mat2f){
        m.xx, m.xy,
        m.yx, m.yy,
    };
}

inline mat4f cast_mat3f_to_mat4f(mat3f m) {
    return (mat4f){
        m.xx, m.xy, m.xz, 0.0f,
        m.yx, m.yy, m.yz, 0.0f,
        m.zx, m.zy, m.zz, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f,
    };
}

inline mat3f cast_mat4f_to_mat3f(mat4f m) {
    return (mat3f){
        m.xx, m.xy, m.xz,
        m.yx, m.yy, m.yz,
        m.zx, m.zy, m.zz,
    };
}

#endif // __CAST_MATRIX_H__
