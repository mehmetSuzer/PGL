
#ifndef __MAT2F_H__
#define __MAT2F_H__

#include "../vector/vec2f.h"

// Row-based 2x2 matrix typedef.
// Element layout is as follows:
//  xx, xy
//  yx, yy
typedef struct {
    float xx, xy; // row0
    float yx, yy; // row1
} mat2f;

#define zero_mat2f     ((mat2f){0.0f, 0.0f, 0.0f, 0.0f})
#define identity_mat2f ((mat2f){1.0f, 0.0f, 0.0f, 1.0f})

inline void swap_mat2f(mat2f* m1, mat2f* m2) {
    mat2f temp = *m1;
    *m1 = *m2;
    *m2 = temp;
}

inline mat2f diagonal_mat2f(vec2f v) {
    return (mat2f){
         v.x, 0.0f,
        0.0f,  v.y,
    };
}

inline mat2f neg_mat2f(mat2f m) {
    return (mat2f){
        -m.xx, -m.xy,
        -m.yx, -m.yy,
    };
}

inline mat2f mat2f_from_rows(vec2f row0, vec2f row1) {
    return (mat2f){
        row0.x, row0.y,
        row1.x, row1.y,
    };
}

inline mat2f mat2f_from_cols(vec2f col0, vec2f col1) {
    return (mat2f){
        col0.x, col1.x,
        col0.y, col1.y,
    };
}

inline mat2f add_mat2f(mat2f m1, mat2f m2) {
    return (mat2f){
        m1.xx + m2.xx, m1.xy + m2.xy, 
        m1.yx + m2.yx, m1.yy + m2.yy,
    };
}

inline mat2f sub_mat2f(mat2f m1, mat2f m2) {
    return (mat2f){
        m1.xx - m2.xx, m1.xy - m2.xy, 
        m1.yx - m2.yx, m1.yy - m2.yy,
    };
}

inline mat2f scale_mat2f(mat2f m, float scale) {
    return (mat2f){
        m.xx * scale, m.xy * scale, 
        m.yx * scale, m.yy * scale,
    };
}

inline vec2f mul_mat2f_vec2f(mat2f m, vec2f v) {
    return (vec2f){
        m.xx * v.x + m.xy * v.y, // x
        m.yx * v.x + m.yy * v.y, // y
    };
}

inline mat2f mul_mat2f_mat2f(mat2f m1, mat2f m2) {
    return (mat2f){
        m1.xx * m2.xx + m1.xy * m2.yx, m1.xx * m2.xy + m1.xy * m2.yy,
        m1.yx * m2.xx + m1.yy * m2.yx, m1.yx * m2.xy + m1.yy * m2.yy,
    };
}

inline mat2f tr_mat2f(mat2f m) {
    return (mat2f){
        m.xx, m.yx, 
        m.xy, m.yy,
    };
}

inline float det_mat2f(mat2f m) {
    return m.xx * m.yy - m.xy * m.yx;
}

inline mat2f inv_mat2f(mat2f m) {
    const float inverse_determinant = 1.0f / det_mat2f(m);

    return (mat2f){
         m.yy * inverse_determinant, -m.xy * inverse_determinant, 
        -m.yx * inverse_determinant,  m.xx * inverse_determinant,
    };
}

inline vec2f solve_cramers_mat2f(mat2f m, vec2f v) {
    const float inverse_determinant = 1.0f / det_mat2f(m);
    const float detx = m.yy * v.x - m.xy * v.y;
    const float dety = m.xx * v.y - m.yx * v.x;

    return (vec2f){
        detx * inverse_determinant, // x
        dety * inverse_determinant, // y
    };
}

inline bool epsilon_equal_mat2f(mat2f m1, mat2f m2, float epsilon) {
    return epsilon_equal(m1.xx, m2.xx, epsilon) && 
           epsilon_equal(m1.xy, m2.xy, epsilon) && 

           epsilon_equal(m1.yx, m2.yx, epsilon) &&
           epsilon_equal(m1.yy, m2.yy, epsilon);
}

inline bool epsilon_not_equal_mat2f(mat2f m1, mat2f m2, float epsilon) {
    return epsilon_not_equal(m1.xx, m2.xx, epsilon) ||
           epsilon_not_equal(m1.xy, m2.xy, epsilon) || 
           
           epsilon_not_equal(m1.yx, m2.yx, epsilon) ||
           epsilon_not_equal(m1.yy, m2.yy, epsilon);
}

#endif // __MAT2F_H__
