
#ifndef __QUAT_H__
#define __QUAT_H__

#include "../matrix/mat3f.h"
#include "../matrix/mat4f.h"

typedef struct {
    vec3f v;
    float w;
} quat;

#define identity_quat ((quat){(vec3f){0.0f, 0.0f, 0.0f}, 1.0f})

inline void swap_quat(quat* q1, quat* q2) {
    quat temp = *q1;
    *q1 = *q2;
    *q2 = temp;
}

inline quat quat_from_euler_angles(const vec3f angles) {
    const vec3f half_angles = scale_vec3f(angles, 0.5f);
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

    const float w = c.x * c.y * c.z + s.x * s.y * s.z;
	const float x = s.x * c.y * c.z - c.x * s.y * s.z;
	const float y = c.x * s.y * c.z + s.x * c.y * s.z;
	const float z = c.x * c.y * s.z - s.x * s.y * c.z;
    const vec3f v = {x, y, z};

    return (quat){v, w};
}

inline quat quat_from_vec3f_angle(const vec3f v, const float radian) {
    const float half_radian = radian * 0.5f;
    const float c = cosf(half_radian);
    const float s = sinf(half_radian);

    return (quat){scale_vec3f(v, s), c};
}

inline quat neg_quat(const quat q) {
    return (quat){neg_vec3f(q.v), -q.w};
}

inline quat add_quat(const quat q1, const quat q2) {
    const float w = q1.w + q2.w;
    const vec3f v = {
        q1.v.x + q2.v.x, 
        q1.v.y + q2.v.y, 
        q1.v.z + q2.v.z,
    };

    return (quat){v, w};
}

inline quat sub_quat(const quat q1, const quat q2) {
    const float w = q1.w - q2.w;
    const vec3f v = {
        q1.v.x - q2.v.x, 
        q1.v.y - q2.v.y, 
        q1.v.z - q2.v.z,
    };

    return (quat){v, w};
}

inline quat scale_quat(const quat q1, const float scale) {
    const float w = q1.w * scale;
    const vec3f v = {
        q1.v.x * scale, 
        q1.v.y * scale, 
        q1.v.z * scale,
    };

    return (quat){v, w};
}

inline quat conjugate_quat(const quat q) {
    return (quat){neg_vec3f(q.v), q.w};
}

inline float norm2_quat(const quat q) {
    return q.v.x * q.v.x +
           q.v.y * q.v.y +
           q.v.z * q.v.z +
           q.w   * q.w;
}

inline float norm_quat(const quat q) {
    return sqrtf(norm2_quat(q));
}

inline quat normalize_quat(const quat q) {
    return scale_quat(q, norm_quat(q));
}

inline quat inv_quat(const quat q) {
    const float inverse_norm2 = 1.0f / norm2_quat(q);
    return scale_quat(conjugate_quat(q), inverse_norm2);
}

inline quat mul_quat_quat(const quat q1, const quat q2) {
    const float w = q1.w * q2.w - dot_vec3f(q1.v, q2.v);
    const vec3f w1v2 = scale_vec3f(q2.v, q1.w);
    const vec3f w2v1 = scale_vec3f(q1.v, q2.w);
    const vec3f v1v2 = cross_vec3f(q1.v, q2.v);
    const vec3f v = add_vec3f(w1v2, add_vec3f(w2v1, v1v2));

    return (quat){v, w};
}

inline quat mul_quat_vec3f(const quat q, const vec3f v) {
    const float w = -dot_vec3f(q.v, v);
    const vec3f xyz1 = scale_vec3f(v, q.w);
    const vec3f xyz2 = cross_vec3f(q.v, v);
    const vec3f xyz  = add_vec3f(xyz1, xyz2);

    return (quat){xyz, w};
}

inline quat mul_vec3f_quat(const vec3f v, const quat q) {
    const float w = -dot_vec3f(v, q.v);
    const vec3f xyz1 = scale_vec3f(v, q.w);
    const vec3f xyz2 = cross_vec3f(v, q.v);
    const vec3f xyz  = add_vec3f(xyz1, xyz2);

    return (quat){xyz, w};
}

inline vec3f rotate_vec3f(const quat q, const vec3f v) {
    const vec3f uv  = cross_vec3f(q.v, v);
    const vec3f uuv = cross_vec3f(q.v, uv);
    const vec3f wuv = scale_vec3f(uv, q.w);

    return add_vec3f(v, scale_vec3f(add_vec3f(wuv, uuv), 2.0f));
}

inline mat3f cast_quat_to_mat3f(const quat q) {
    const float xx = q.v.x * q.v.x;
    const float yy = q.v.y * q.v.y;
    const float zz = q.v.z * q.v.z;
    const float xy = q.v.x * q.v.y;
    const float xz = q.v.x * q.v.z;
    const float yz = q.v.y * q.v.z;
    const float wx = q.w   * q.v.x;
    const float wy = q.w   * q.v.y;
    const float wz = q.w   * q.v.z;

    return (mat3f){
        1.0f - 2.0f * (yy + zz),        2.0f * (xy - wz),        2.0f * (xz + wy),
               2.0f * (xy + wz), 1.0f - 2.0f * (xx + zz),        2.0f * (yz - wx),
               2.0f * (xz - wy),        2.0f * (yz + wx), 1.0f - 2.0f * (xx + yy),
    };
}

inline mat4f cast_quat_to_mat4f(const quat q) {
    return cast_mat3f_to_mat4f(cast_quat_to_mat3f(q));
}

inline bool epsilon_equal_quat(const quat q1, const quat q2, float epsilon) {
    return epsilon_equal(q1.w,   q2.w,   epsilon) && 
           epsilon_equal(q1.v.x, q2.v.x, epsilon) && 
           epsilon_equal(q1.v.y, q2.v.y, epsilon) && 
           epsilon_equal(q1.v.z, q2.v.z, epsilon);
}

inline bool epsilon_not_equal_quat(const quat q1, const quat q2, float epsilon) {
    return epsilon_not_equal(q1.w,   q2.w,   epsilon) ||
           epsilon_not_equal(q1.v.x, q2.v.x, epsilon) || 
           epsilon_not_equal(q1.v.y, q2.v.y, epsilon) || 
           epsilon_not_equal(q1.v.z, q2.v.z, epsilon);
}

#endif // __QUAT_H__
