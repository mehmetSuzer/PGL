
#ifndef __MAT3F_H__
#define __MAT3F_H__

#include "../vector/vec3f.h"
#include "../matrix/mat2f.h"

// Row-based 3x3 matrix typedef.
// Element layout is as follows:
//  xx, xy, xz
//  yx, yy, yz
//  zx, zy, zz
typedef struct {
    float xx, xy, xz; // row0
    float yx, yy, yz; // row1
    float zx, zy, zz; // row2
} mat3f;

#define zero_mat3f     ((mat3f){0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f})
#define identity_mat3f ((mat3f){1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f})

inline void swap_mat3f(mat3f* m1, mat3f* m2) {
    mat3f temp = *m1;
    *m1 = *m2;
    *m2 = temp;
}

inline mat3f diagonal_mat3f(vec3f v) {
    return (mat3f){
         v.x, 0.0f, 0.0f,
        0.0f,  v.y, 0.0f,
        0.0f, 0.0f,  v.z,
    };
}

inline mat3f neg_mat3f(mat3f m) {
    return (mat3f){
        -m.xx, -m.xy, -m.xz,
        -m.yx, -m.yy, -m.yz,
        -m.zx, -m.zy, -m.zz,
    };
}

inline mat3f mat3f_from_rows(vec3f row0, vec3f row1, vec3f row2) {
    return (mat3f){
        row0.x, row0.y, row0.z,
        row1.x, row1.y, row1.z,
        row2.x, row2.y, row2.z,
    };
}

inline mat3f mat3f_from_cols(vec3f col0, vec3f col1, vec3f col2) {
    return (mat3f){
        col0.x, col1.x, col2.x,
        col0.y, col1.y, col2.y,
        col0.z, col1.z, col2.z,
    };
}

inline mat3f add_mat3f(mat3f m1, mat3f m2) {
    return (mat3f){
        m1.xx + m2.xx, m1.xy + m2.xy, m1.xz + m2.xz,
        m1.yx + m2.yx, m1.yy + m2.yy, m1.yz + m2.yz,
        m1.zx + m2.zx, m1.zy + m2.zy, m1.zz + m2.zz,
    };
}

inline mat3f sub_mat3f(mat3f m1, mat3f m2) {
    return (mat3f){
        m1.xx - m2.xx, m1.xy - m2.xy, m1.xz - m2.xz,
        m1.yx - m2.yx, m1.yy - m2.yy, m1.yz - m2.yz,
        m1.zx - m2.zx, m1.zy - m2.zy, m1.zz - m2.zz,
    };
}

inline mat3f scale_mat3f(mat3f m, float scale) {
    return (mat3f){
        m.xx * scale, m.xy * scale, m.xz * scale,
        m.yx * scale, m.yy * scale, m.yz * scale,
        m.zx * scale, m.zy * scale, m.zz * scale,
    };
}

inline vec3f mul_mat3f_vec3f(mat3f m, vec3f v) {
    return (vec3f){
        m.xx * v.x + m.xy * v.y + m.xz * v.z, // x
        m.yx * v.x + m.yy * v.y + m.yz * v.z, // y
        m.zx * v.x + m.zy * v.y + m.zz * v.z, // z
    };
}

inline mat3f mul_mat3f_mat3f(mat3f m1, mat3f m2) {
    return (mat3f){
        m1.xx * m2.xx + m1.xy * m2.yx + m1.xz * m2.zx,
        m1.xx * m2.xy + m1.xy * m2.yy + m1.xz * m2.zy,
        m1.xx * m2.xz + m1.xy * m2.yz + m1.xz * m2.zz,
        
        m1.yx * m2.xx + m1.yy * m2.yx + m1.yz * m2.zx,
        m1.yx * m2.xy + m1.yy * m2.yy + m1.yz * m2.zy,
        m1.yx * m2.xz + m1.yy * m2.yz + m1.yz * m2.zz,
        
        m1.zx * m2.xx + m1.zy * m2.yx + m1.zz * m2.zx,
        m1.zx * m2.xy + m1.zy * m2.yy + m1.zz * m2.zy,
        m1.zx * m2.xz + m1.zy * m2.yz + m1.zz * m2.zz,
    };
}

inline mat3f tr_mat3f(mat3f m) {
    return (mat3f){
        m.xx, m.yx, m.zx,
        m.xy, m.yy, m.zy,
        m.xz, m.yz, m.zz,
    };
}

inline float det_mat3f(mat3f m) {
    return + m.xx * (m.yy * m.zz - m.yz * m.zy)
           - m.xy * (m.yx * m.zz - m.yz * m.zx)
           + m.xz * (m.yx * m.zy - m.yy * m.zx);
}

inline mat3f inv_mat3f(mat3f m) {
    const float inverse_determinant = 1.0f / det_mat3f(m);

    return (mat3f){
        (m.yy * m.zz - m.yz * m.zy) * inverse_determinant,
        (m.xz * m.zy - m.xy * m.zz) * inverse_determinant,
        (m.xy * m.yz - m.xz * m.yy) * inverse_determinant,
        
        (m.yz * m.zx - m.yx * m.zz) * inverse_determinant,
        (m.xx * m.zz - m.xz * m.zx) * inverse_determinant,
        (m.xz * m.yx - m.xx * m.yz) * inverse_determinant,
        
        (m.yx * m.zy - m.yy * m.zx) * inverse_determinant,
        (m.xy * m.zx - m.xx * m.zy) * inverse_determinant,
        (m.xx * m.yy - m.xy * m.yx) * inverse_determinant,
    };
}

static inline vec3f solve_cramers_mat3f(mat3f m, vec3f v) {
    const float factor0 = m.yy * m.zz - m.yz * m.zy;
    const float factor1 = m.yx * m.zz - m.yz * m.zx;
    const float factor2 = m.yx * m.zy - m.yy * m.zx;
    const float factor3 = m.zz * v.y  - m.yz * v.z;
    const float factor4 = m.yx * v.z  - m.zx * v.y;
    const float factor5 = m.zy * v.y  - m.yy * v.z;

    const float determinant = m.xx * factor0 - m.xy * factor1 + m.xz * factor2;
    const float inverse_determinant = 1.0f / determinant;

    const float detx = v.x  * factor0 - m.xy * factor3 + m.xz * factor5;
    const float dety = m.xx * factor3 - v.x  * factor1 + m.xz * factor4;
    const float detz = v.x  * factor2 - m.xx * factor5 - m.xy * factor4;

    return (vec3f){
        detx * inverse_determinant, // x
        dety * inverse_determinant, // y
        detz * inverse_determinant, // z
    };
}

inline mat3f cast_mat2f_to_mat3f(mat2f m) {
    return (mat3f){
        m.xx, m.xy, 0.0f,
        m.yx, m.yy, 0.0f,
        0.0f, 0.0f, 1.0f,
    };
}

inline bool epsilon_equal_mat3f(mat3f m1, mat3f m2, float epsilon) {
    return epsilon_equal(m1.xx, m2.xx, epsilon) && 
           epsilon_equal(m1.xy, m2.xy, epsilon) && 
           epsilon_equal(m1.xz, m2.xz, epsilon) && 

           epsilon_equal(m1.yx, m2.yx, epsilon) &&
           epsilon_equal(m1.yy, m2.yy, epsilon) &&
           epsilon_equal(m1.yz, m2.yz, epsilon) && 

           epsilon_equal(m1.zx, m2.zx, epsilon) &&
           epsilon_equal(m1.zy, m2.zy, epsilon) &&
           epsilon_equal(m1.zz, m2.zz, epsilon);
}

inline bool epsilon_not_equal_mat3f(mat3f m1, mat3f m2, float epsilon) {
    return epsilon_not_equal(m1.xx, m2.xx, epsilon) || 
           epsilon_not_equal(m1.xy, m2.xy, epsilon) || 
           epsilon_not_equal(m1.xz, m2.xz, epsilon) || 

           epsilon_not_equal(m1.yx, m2.yx, epsilon) ||
           epsilon_not_equal(m1.yy, m2.yy, epsilon) ||
           epsilon_not_equal(m1.yz, m2.yz, epsilon) || 

           epsilon_not_equal(m1.zx, m2.zx, epsilon) ||
           epsilon_not_equal(m1.zy, m2.zy, epsilon) ||
           epsilon_not_equal(m1.zz, m2.zz, epsilon);
}

#endif // __MAT3F_H__
