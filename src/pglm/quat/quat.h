
#ifndef __QUAT_H__
#define __QUAT_H__

#include "../matrix/cast_matrix.h"

typedef struct {
    vec3f v;
    f32   w;
} quat;

#define identity_quat ((quat){(vec3f){0.0f, 0.0f, 0.0f}, 1.0f})

// Returns a unit quaternion equivalent to the rotation corresponding to the (x,y,z) euler angles
inline quat quat_euler_angles(vec3f angles) {
    const vec3f half_angles = vec3f_scale(angles, 0.5f);
    const vec3f c = {
        cosf(half_angles.x),
        cosf(half_angles.y),
        cosf(half_angles.z),
    };
    const vec3f s = {
        sinf(half_angles.x),
        sinf(half_angles.y),
        sinf(half_angles.z),
    };

    const f32 w = c.x * c.y * c.z + s.x * s.y * s.z;
	const f32 x = s.x * c.y * c.z - c.x * s.y * s.z;
	const f32 y = c.x * s.y * c.z + s.x * c.y * s.z;
	const f32 z = c.x * c.y * s.z - s.x * s.y * c.z;
    const vec3f v = {x, y, z};

    return (quat){v, w};
}

// v must be a unit vector
// Returns a unit quaternion
inline quat quat_angle_axis(vec3f v, f32 radian) {
    const f32 half_radian = radian * 0.5f;
    const f32 c = cosf(half_radian);
    const f32 s = sinf(half_radian);

    return (quat){vec3f_scale(v, s), c};
}

inline quat neg_quat(quat q) {
    return (quat){vec3f_negate(q.v), -q.w};
}

inline quat add_quat(quat q1, quat q2) {
    const f32 w = q1.w + q2.w;
    const vec3f v = {
        q1.v.x + q2.v.x, 
        q1.v.y + q2.v.y, 
        q1.v.z + q2.v.z,
    };

    return (quat){v, w};
}

inline quat sub_quat(quat q1, quat q2) {
    const f32 w = q1.w - q2.w;
    const vec3f v = {
        q1.v.x - q2.v.x, 
        q1.v.y - q2.v.y, 
        q1.v.z - q2.v.z,
    };

    return (quat){v, w};
}

inline quat scale_quat(quat q1, f32 scale) {
    const f32 w = q1.w * scale;
    const vec3f v = {
        q1.v.x * scale, 
        q1.v.y * scale, 
        q1.v.z * scale,
    };

    return (quat){v, w};
}

inline quat conjugate_quat(quat q) {
    return (quat){vec3f_negate(q.v), q.w};
}

inline f32 norm2_quat(quat q) {
    return q.v.x * q.v.x +
           q.v.y * q.v.y +
           q.v.z * q.v.z +
           q.w   * q.w;
}

inline f32 norm_quat(quat q) {
    return sqrtf(norm2_quat(q));
}

inline quat normalize_quat(quat q) {
    const f32 inverse_norm = 1.0f / norm_quat(q);
    return scale_quat(q, inverse_norm);
}

inline quat inv_quat(quat q) {
    const f32 inverse_norm2 = 1.0f / norm2_quat(q);
    return scale_quat(conjugate_quat(q), inverse_norm2);
}

inline quat mul_quat_quat(quat q1, quat q2) {
    const f32 w = q1.w * q2.w - vec3f_dot(q1.v, q2.v);
    const vec3f w1v2 = vec3f_scale(q2.v, q1.w);
    const vec3f w2v1 = vec3f_scale(q1.v, q2.w);
    const vec3f v1v2 = vec3f_cross(q1.v, q2.v);
    const vec3f v = vec3f_add(vec3f_add(w1v2, w2v1), v1v2);

    return (quat){v, w};
}

inline quat mul_quat_vec3f(quat q, vec3f v) {
    const f32 w = -vec3f_dot(q.v, v);
    const vec3f xyz1 = vec3f_scale(v, q.w);
    const vec3f xyz2 = vec3f_cross(q.v, v);
    const vec3f xyz  = vec3f_add(xyz1, xyz2);

    return (quat){xyz, w};
}

inline quat mul_vec3f_quat(vec3f v, quat q) {
    const f32 w = -vec3f_dot(v, q.v);
    const vec3f xyz1 = vec3f_scale(v, q.w);
    const vec3f xyz2 = vec3f_cross(v, q.v);
    const vec3f xyz  = vec3f_add(xyz1, xyz2);

    return (quat){xyz, w};
}

// q must be unit quaternion
inline vec3f rotate_quat(quat q, vec3f v) {
    const vec3f uv  = vec3f_cross(q.v, v);
    const vec3f uuv = vec3f_cross(q.v, uv);
    const vec3f wuv = vec3f_scale(uv, q.w);

    return vec3f_add(v, vec3f_scale(vec3f_add(wuv, uuv), 2.0f));
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

    return (mat3f){
        1.0f - 2.0f * (yy + zz),        2.0f * (xy - wz),        2.0f * (xz + wy),
               2.0f * (xy + wz), 1.0f - 2.0f * (xx + zz),        2.0f * (yz - wx),
               2.0f * (xz - wy),        2.0f * (yz + wx), 1.0f - 2.0f * (xx + yy),
    };
}

inline mat4f cast_quat_to_mat4f(quat q) {
    return cast_mat3f_to_mat4f(cast_quat_to_mat3f(q));
}

inline bool epsilon_equal_quat(quat q1, quat q2, f32 epsilon) {
    return epsilon_equal(q1.w,   q2.w,   epsilon) && 
           epsilon_equal(q1.v.x, q2.v.x, epsilon) && 
           epsilon_equal(q1.v.y, q2.v.y, epsilon) && 
           epsilon_equal(q1.v.z, q2.v.z, epsilon);
}

inline bool epsilon_not_equal_quat(quat q1, quat q2, f32 epsilon) {
    return epsilon_not_equal(q1.w,   q2.w,   epsilon) ||
           epsilon_not_equal(q1.v.x, q2.v.x, epsilon) || 
           epsilon_not_equal(q1.v.y, q2.v.y, epsilon) || 
           epsilon_not_equal(q1.v.z, q2.v.z, epsilon);
}

#endif // __QUAT_H__
