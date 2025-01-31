
#ifndef __MAT4F_H__
#define __MAT4F_H__

#include "../vector/vec4f.h"

// Row-based 4x4 matrix typedef.
// Element layout is as follows:
//  xx, xy, xz, xw
//  yx, yy, yz, yw
//  zx, zy, zz, zw
//  wx, wy, wz, ww
typedef union {
    struct {
        f32 xx, xy, xz, xw; // row0
        f32 yx, yy, yz, yw; // row1
        f32 zx, zy, zz, zw; // row2
        f32 wx, wy, wz, ww; // row3
    };
    f32 raw[4][4];
    vec4f rows[4];
} mat4f;

#define mat4f_zero ((mat4f){{       \
    0.0f, 0.0f, 0.0f, 0.0f,         \
    0.0f, 0.0f, 0.0f, 0.0f,         \
    0.0f, 0.0f, 0.0f, 0.0f,         \
    0.0f, 0.0f, 0.0f, 0.0f          \
}})

#define mat4f_identity ((mat4f){{   \
    1.0f, 0.0f, 0.0f, 0.0f,         \
    0.0f, 1.0f, 0.0f, 0.0f,         \
    0.0f, 0.0f, 1.0f, 0.0f,         \
    0.0f, 0.0f, 0.0f, 1.0f          \
}})

inline mat4f mat4f_diagonal(vec4f v) {
    return (mat4f){{
         v.x, 0.0f, 0.0f, 0.0f,
        0.0f,  v.y, 0.0f, 0.0f,
        0.0f, 0.0f,  v.z, 0.0f,
        0.0f, 0.0f, 0.0f,  v.w,
    }};
}

inline mat4f mat4f_negate(mat4f m) {
    return (mat4f){{
        -m.xx, -m.xy, -m.xz, -m.xw,
        -m.yx, -m.yy, -m.yz, -m.yw,
        -m.zx, -m.zy, -m.zz, -m.zw,
        -m.wx, -m.wy, -m.wz, -m.ww,
    }};
}

inline mat4f mat4f_from_rows(vec4f row0, vec4f row1, vec4f row2, vec4f row3) {
    return (mat4f){{
        row0.x, row0.y, row0.z, row0.w,
        row1.x, row1.y, row1.z, row1.w,
        row2.x, row2.y, row2.z, row2.w,
        row3.x, row3.y, row3.z, row3.w,
    }};
}

inline mat4f mat4f_from_cols(vec4f col0, vec4f col1, vec4f col2, vec4f col3) {
    return (mat4f){{
        col0.x, col1.x, col2.x, col3.x,
        col0.y, col1.y, col2.y, col3.y,
        col0.z, col1.z, col2.z, col3.z,
        col0.w, col1.w, col2.w, col3.w,
    }};
}

inline mat4f mat4f_add(mat4f m1, mat4f m2) {
    return (mat4f){{
        m1.xx + m2.xx, m1.xy + m2.xy, m1.xz + m2.xz, m1.xw + m2.xw,
        m1.yx + m2.yx, m1.yy + m2.yy, m1.yz + m2.yz, m1.yw + m2.yw,
        m1.zx + m2.zx, m1.zy + m2.zy, m1.zz + m2.zz, m1.zw + m2.zw,
        m1.wx + m2.wx, m1.wy + m2.wy, m1.wz + m2.wz, m1.ww + m2.ww,
    }};
}

inline mat4f mat4f_sub(mat4f m1, mat4f m2) {
    return (mat4f){{
        m1.xx - m2.xx, m1.xy - m2.xy, m1.xz - m2.xz, m1.xw - m2.xw,
        m1.yx - m2.yx, m1.yy - m2.yy, m1.yz - m2.yz, m1.yw - m2.yw,
        m1.zx - m2.zx, m1.zy - m2.zy, m1.zz - m2.zz, m1.zw - m2.zw,
        m1.wx - m2.wx, m1.wy - m2.wy, m1.wz - m2.wz, m1.ww - m2.ww,
    }};
}

inline mat4f mat4f_scale(mat4f m, f32 scale) {
    return (mat4f){{
        m.xx * scale, m.xy * scale, m.xz * scale, m.xw * scale,
        m.yx * scale, m.yy * scale, m.yz * scale, m.yw * scale,
        m.zx * scale, m.zy * scale, m.zz * scale, m.zw * scale,
        m.wx * scale, m.wy * scale, m.wz * scale, m.ww * scale,
    }};
}

inline vec4f mat4f_mul_vec4f(mat4f m, vec4f v) {
    return (vec4f){{
        m.xx * v.x + m.xy * v.y + m.xz * v.z + m.xw * v.w, // x
        m.yx * v.x + m.yy * v.y + m.yz * v.z + m.yw * v.w, // y
        m.zx * v.x + m.zy * v.y + m.zz * v.z + m.zw * v.w, // z
        m.wx * v.x + m.wy * v.y + m.wz * v.z + m.ww * v.w, // w
    }};
}

static inline mat4f mat4f_mul_mat4f(mat4f m1, mat4f m2) {
    return (mat4f){{
        m1.xx * m2.xx + m1.xy * m2.yx + m1.xz * m2.zx + m1.xw * m2.wx,
        m1.xx * m2.xy + m1.xy * m2.yy + m1.xz * m2.zy + m1.xw * m2.wy,
        m1.xx * m2.xz + m1.xy * m2.yz + m1.xz * m2.zz + m1.xw * m2.wz,
        m1.xx * m2.xw + m1.xy * m2.yw + m1.xz * m2.zw + m1.xw * m2.ww,
        
        m1.yx * m2.xx + m1.yy * m2.yx + m1.yz * m2.zx + m1.yw * m2.wx,
        m1.yx * m2.xy + m1.yy * m2.yy + m1.yz * m2.zy + m1.yw * m2.wy,
        m1.yx * m2.xz + m1.yy * m2.yz + m1.yz * m2.zz + m1.yw * m2.wz,
        m1.yx * m2.xw + m1.yy * m2.yw + m1.yz * m2.zw + m1.yw * m2.ww,
        
        m1.zx * m2.xx + m1.zy * m2.yx + m1.zz * m2.zx + m1.zw * m2.wx,
        m1.zx * m2.xy + m1.zy * m2.yy + m1.zz * m2.zy + m1.zw * m2.wy,
        m1.zx * m2.xz + m1.zy * m2.yz + m1.zz * m2.zz + m1.zw * m2.wz,
        m1.zx * m2.xw + m1.zy * m2.yw + m1.zz * m2.zw + m1.zw * m2.ww,
        
        m1.wx * m2.xx + m1.wy * m2.yx + m1.wz * m2.zx + m1.ww * m2.wx,
        m1.wx * m2.xy + m1.wy * m2.yy + m1.wz * m2.zy + m1.ww * m2.wy,
        m1.wx * m2.xz + m1.wy * m2.yz + m1.wz * m2.zz + m1.ww * m2.wz,
        m1.wx * m2.xw + m1.wy * m2.yw + m1.wz * m2.zw + m1.ww * m2.ww,
    }};
}

inline mat4f mat4f_tr(mat4f m) {
    return (mat4f){{
        m.xx, m.yx, m.zx, m.wx,
        m.xy, m.yy, m.zy, m.wy,
        m.xz, m.yz, m.zz, m.wz,
        m.xw, m.yw, m.zw, m.ww,
    }};
}

static inline f32 mat4f_det(mat4f m) {
    const f32 factor0 = m.zz * m.ww - m.wz * m.zw;
	const f32 factor1 = m.zy * m.ww - m.wy * m.zw;
	const f32 factor2 = m.zy * m.wz - m.wy * m.zz;
	const f32 factor3 = m.zx * m.ww - m.wx * m.zw;
	const f32 factor4 = m.zx * m.wz - m.wx * m.zz;
	const f32 factor5 = m.zx * m.wy - m.wx * m.zy;

	const f32 coef0 = m.yy * factor0 - m.yz * factor1 + m.yw * factor2;
	const f32 coef1 = m.yx * factor0 - m.yz * factor3 + m.yw * factor4;
	const f32 coef2 = m.yx * factor1 - m.yy * factor3 + m.yw * factor5;
	const f32 coef3 = m.yx * factor2 - m.yy * factor4 + m.yz * factor5;

    return + m.xx * coef0 - m.xy * coef1
	       + m.xz * coef2 - m.xw * coef3;
}

// REQUIREMENT: m must be invertible. det(m) != 0.0f.
static inline mat4f mat4f_inv(mat4f m) {
    const f32 factor0 = m.zz * m.ww - m.wz * m.zw;
    const f32 factor1 = m.zy * m.ww - m.wy * m.zw;
    const f32 factor2 = m.zy * m.wz - m.wy * m.zz;
    const f32 factor3 = m.zx * m.ww - m.wx * m.zw;
    const f32 factor4 = m.zx * m.wz - m.wx * m.zz;
    const f32 factor5 = m.zx * m.wy - m.wx * m.zy;

    const f32 coef0 = m.yy * factor0 - m.yz * factor1 + m.yw * factor2;
    const f32 coef1 = m.yx * factor0 - m.yz * factor3 + m.yw * factor4;
    const f32 coef2 = m.yx * factor1 - m.yy * factor3 + m.yw * factor5;
    const f32 coef3 = m.yx * factor2 - m.yy * factor4 + m.yz * factor5;

    const mat4f adjugate = {{
        coef0, 
        -coef1, 
        coef2, 
        -coef3,
        
        -(m.xz * factor0 - m.xy * factor1 + m.xw * factor2),
        (m.xx * factor0 - m.xz * factor3 + m.xw * factor4),
        -(m.xx * factor1 - m.xy * factor3 + m.xw * factor5),
        (m.xx * factor2 - m.xy * factor4 + m.xz * factor5),
        
        (m.yx * factor1 - m.yy * factor2 + m.yz * factor5),
        -(m.yx * factor0 - m.yy * factor3 + m.yz * factor4),
        (m.yx * factor1 - m.yz * factor2 + m.yy * factor5),
        -(m.yx * factor2 - m.yy * factor4 + m.yz * factor5),
        
        -(m.yz * factor1 - m.yy * factor2 + m.yz * factor5),
        (m.xx * factor0 - m.xy * factor3 + m.xz * factor4),
        -(m.xx * factor1 - m.xy * factor2 + m.xz * factor5),
        (m.xx * factor2 - m.xy * factor4 + m.xz * factor5)
    }};

    const f32 inverse_determinant = 1.0f / mat4f_det(m);
    return mat4f_scale(adjugate, inverse_determinant);
}

// REQUIREMENT: m must be invertible. det(m) != 0.0f.
static inline vec4f mat4f_solve_cramers(mat4f m, vec4f v) {
    const vec4f col0 = (vec4f){{m.xx, m.yx, m.zx, m.wx}};
    const vec4f col1 = (vec4f){{m.xy, m.yy, m.zy, m.wy}};
    const vec4f col2 = (vec4f){{m.xz, m.yz, m.zz, m.wz}};
    const vec4f col3 = (vec4f){{m.xw, m.yw, m.zw, m.ww}};

    const mat4f m0 = mat4f_from_cols(   v, col1, col2, col3);
    const mat4f m1 = mat4f_from_cols(col0,    v, col2, col3);
    const mat4f m2 = mat4f_from_cols(col0, col1,    v, col3);
    const mat4f m3 = mat4f_from_cols(col0, col1, col2,    v);

    const f32 inverse_determinant = 1.0f / mat4f_det(m);
    const f32 det0 = mat4f_det(m0);
    const f32 det1 = mat4f_det(m1);
    const f32 det2 = mat4f_det(m2);
    const f32 det3 = mat4f_det(m3);

    const f32 x = det0 * inverse_determinant;
    const f32 y = det1 * inverse_determinant;
    const f32 z = det2 * inverse_determinant;
    const f32 w = det3 * inverse_determinant;

    return (vec4f){{x, y, z, w}};
}

inline bool mat4f_epsilon_equal(mat4f m1, mat4f m2, f32 epsilon) {
    return epsilon_equal(m1.xx, m2.xx, epsilon) && 
           epsilon_equal(m1.xy, m2.xy, epsilon) && 
           epsilon_equal(m1.xz, m2.xz, epsilon) && 
           epsilon_equal(m1.xw, m2.xw, epsilon) && 

           epsilon_equal(m1.yx, m2.yx, epsilon) &&
           epsilon_equal(m1.yy, m2.yy, epsilon) &&
           epsilon_equal(m1.yz, m2.yz, epsilon) && 
           epsilon_equal(m1.yw, m2.yw, epsilon) && 

           epsilon_equal(m1.zx, m2.zx, epsilon) &&
           epsilon_equal(m1.zy, m2.zy, epsilon) &&
           epsilon_equal(m1.zz, m2.zz, epsilon) &&
           epsilon_equal(m1.zw, m2.zw, epsilon) && 

           epsilon_equal(m1.wx, m2.wx, epsilon) &&
           epsilon_equal(m1.wy, m2.wy, epsilon) &&
           epsilon_equal(m1.wz, m2.wz, epsilon) &&
           epsilon_equal(m1.ww, m2.ww, epsilon);
}

inline bool mat4f_epsilon_not_equal(mat4f m1, mat4f m2, f32 epsilon) {
    return epsilon_not_equal(m1.xx, m2.xx, epsilon) ||
           epsilon_not_equal(m1.xy, m2.xy, epsilon) ||
           epsilon_not_equal(m1.xz, m2.xz, epsilon) ||
           epsilon_not_equal(m1.xw, m2.xw, epsilon) ||

           epsilon_not_equal(m1.yx, m2.yx, epsilon) ||
           epsilon_not_equal(m1.yy, m2.yy, epsilon) ||
           epsilon_not_equal(m1.yz, m2.yz, epsilon) || 
           epsilon_not_equal(m1.yw, m2.yw, epsilon) || 
    
           epsilon_not_equal(m1.zx, m2.zx, epsilon) ||
           epsilon_not_equal(m1.zy, m2.zy, epsilon) ||
           epsilon_not_equal(m1.zz, m2.zz, epsilon) ||
           epsilon_not_equal(m1.zw, m2.zw, epsilon) || 
         
           epsilon_not_equal(m1.wx, m2.wx, epsilon) ||
           epsilon_not_equal(m1.wy, m2.wy, epsilon) ||
           epsilon_not_equal(m1.wz, m2.wz, epsilon) ||
           epsilon_not_equal(m1.ww, m2.ww, epsilon);
}

#endif // __MAT4F_H__
