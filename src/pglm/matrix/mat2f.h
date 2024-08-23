
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

inline mat2f neg_mat2f(const mat2f m) {
    return (mat2f) {
        -m.xx, -m.xy,
        -m.yx, -m.yy,
    };
}

inline mat2f mat2f_from_rows(const vec2f row0, const vec2f row1) {
    return (mat2f) {
        row0.x, row0.y,
        row1.x, row1.y,
    };
}

inline mat2f mat2f_from_cols(const vec2f col0, const vec2f col1) {
    return (mat2f) {
        col0.x, col1.x,
        col0.y, col1.y,
    };
}

inline mat2f add_mat2f(const mat2f m1, const mat2f m2) {
    return (mat2f) {
        m1.xx + m2.xx, m1.xy + m2.xy, 
        m1.yx + m2.yx, m1.yy + m2.yy,
    };
}

inline mat2f sub_mat2f(const mat2f m1, const mat2f m2) {
    return (mat2f) {
        m1.xx - m2.xx, m1.xy - m2.xy, 
        m1.yx - m2.yx, m1.yy - m2.yy,
    };
}

inline mat2f scale_mat2f(const mat2f m, const float scale) {
    return (mat2f) {
        m.xx * scale, m.xy * scale, 
        m.yx * scale, m.yy * scale,
    };
}

inline vec2f mul_mat2f_vec2f(const mat2f m, const vec2f v) {
    return (vec2f) {
        m.xx * v.x + m.xy * v.y, // x
        m.yx * v.x + m.yy * v.y, // y
    };
}

inline mat2f mul_mat2f_mat2f(const mat2f m1, const mat2f m2) {
    return (mat2f) {
        m1.xx * m2.xx + m1.xy * m2.yx, m1.xx * m2.xy + m1.xy * m2.yy,
        m1.yx * m2.xx + m1.yy * m2.yx, m1.yx * m2.xy + m1.yy * m2.yy,
    };
}

inline mat2f tr_mat2f(const mat2f m) {
    return (mat2f) {
        m.xx, m.yx, 
        m.xy, m.yy,
    };
}

inline float det_mat2f(const mat2f m) {
    return m.xx * m.yy - m.xy * m.yx;
}

static inline mat2f inv_mat2f(const mat2f m) {
    const float one_over_determinant = 1.0f / det_mat2f(m);

    return (mat2f) {
         m.yy * one_over_determinant, -m.xy * one_over_determinant, 
        -m.yx * one_over_determinant,  m.xx * one_over_determinant,
    };
}

static inline vec2f solve_cramers_mat2f(const mat2f m, const vec2f v) {
    const float one_over_determinant = 1.0f / det_mat2f(m);
    const float detx = m.yy * v.x - m.xy * v.y;
    const float dety = m.xx * v.y - m.yx * v.x;

    return (vec2f) {
        detx * one_over_determinant, // x
        dety * one_over_determinant, // y
    };
}

inline bool epsilon_equal_mat2f(const mat2f m1, const mat2f m2, const float epsilon) {
    return epsilon_equal(m1.xx, m2.xx, epsilon) && 
           epsilon_equal(m1.xy, m2.xy, epsilon) && 

           epsilon_equal(m1.yx, m2.yx, epsilon) &&
           epsilon_equal(m1.yy, m2.yy, epsilon);
}

inline bool epsilon_not_equal_mat2f(const mat2f m1, const mat2f m2, const float epsilon) {
    return epsilon_not_equal(m1.xx, m2.xx, epsilon) ||
           epsilon_not_equal(m1.xy, m2.xy, epsilon) || 
           
           epsilon_not_equal(m1.yx, m2.yx, epsilon) ||
           epsilon_not_equal(m1.yy, m2.yy, epsilon);
}

#endif // __MAT2F_H__
