
#ifndef __CAST_H__
#define __CAST_H__

#include "../matrix/mat2f.h"
#include "../matrix/mat3f.h"
#include "../matrix/mat4f.h"
#include "../quat/quat.h"

inline mat3f cast_mat2f_to_mat3f(mat2f m) {
    return (mat3f){{
        m.xx, m.xy, 0.0f,
        m.yx, m.yy, 0.0f,
        0.0f, 0.0f, 1.0f,
    }};
}

inline mat2f cast_mat3f_to_mat2f(mat3f m) {
    return (mat2f){{
        m.xx, m.xy,
        m.yx, m.yy,
    }};
}

inline mat4f cast_mat3f_to_mat4f(mat3f m) {
    return (mat4f){{
        m.xx, m.xy, m.xz, 0.0f,
        m.yx, m.yy, m.yz, 0.0f,
        m.zx, m.zy, m.zz, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f,
    }};
}

inline mat3f cast_mat4f_to_mat3f(mat4f m) {
    return (mat3f){{
        m.xx, m.xy, m.xz,
        m.yx, m.yy, m.yz,
        m.zx, m.zy, m.zz,
    }};
}

inline mat3f cast_quat_to_mat3f(quat q) {
    const f32 xx = q.v.x * q.v.x;
    const f32 yy = q.v.y * q.v.y;
    const f32 zz = q.v.z * q.v.z;
    const f32 xy = q.v.x * q.v.y;
    const f32 xz = q.v.x * q.v.z;
    const f32 yz = q.v.y * q.v.z;
    const f32 wx = q.w   * q.v.x;
    const f32 wy = q.w   * q.v.y;
    const f32 wz = q.w   * q.v.z;

    return (mat3f){{
        1.0f - 2.0f * (yy + zz),        2.0f * (xy - wz),        2.0f * (xz + wy),
               2.0f * (xy + wz), 1.0f - 2.0f * (xx + zz),        2.0f * (yz - wx),
               2.0f * (xz - wy),        2.0f * (yz + wx), 1.0f - 2.0f * (xx + yy),
    }};
}

inline mat4f cast_quat_to_mat4f(quat q) {
    return cast_mat3f_to_mat4f(cast_quat_to_mat3f(q));
}

#endif // __CAST_H__
