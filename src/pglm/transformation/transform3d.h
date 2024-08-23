
#ifndef __TRANSFORM3D_H__
#define __TRANSFORM3D_H__

#include "../vector/vec3f.h"
#include "../matrix/mat4f.h"

inline mat4f translate3D(const vec3f v) {
    return (mat4f) {
        1.0f, 0.0f, 0.0f,  v.x,
        0.0f, 1.0f, 0.0f,  v.y,
        0.0f, 0.0f, 1.0f,  v.z,
        0.0f, 0.0f, 0.0f, 1.0f,
    };
}

inline mat4f scale3D(const vec3f v) {
    return (mat4f) {
         v.x, 0.0f, 0.0f, 0.0f,
        0.0f,  v.y, 0.0f, 0.0f,
        0.0f, 0.0f,  v.z, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f,
    };
}

inline mat4f rotate3D_x(const float radian) {
    const float s = fast_sin_extended(radian);
    const float c = fast_cos_extended(radian);
    return (mat4f) {
        1.0f, 0.0f, 0.0f, 0.0f,
        0.0f,    c,   -s, 0.0f,
        0.0f,    s,    c, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f,
    };
}

inline mat4f rotate3D_y(const float radian) {
    const float s = fast_sin_extended(radian);
    const float c = fast_cos_extended(radian);
    return (mat4f) {
           c, 0.0f,    s, 0.0f,
        0.0f, 1.0f, 0.0f, 0.0f,
          -s, 0.0f,    c, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f,
    };
}

inline mat4f rotate3D_z(const float radian) {
    const float s = fast_sin_extended(radian);
    const float c = fast_cos_extended(radian);
    return (mat4f) {
           c,   -s, 0.0f, 0.0f,
           s,    c, 0.0f, 0.0f,
        0.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f,
    };
}

inline mat4f shear3D(const float xy, const float xz, const float yx, const float yz, const float zx, const float zy) {
    return (mat4f) {
        1.0f,   xy,   xz, 0.0f,
          yx, 1.0f,   yz, 0.0f,
          zx,   zy, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f,
    };
}

#endif // __TRANSFORM3D_H__
