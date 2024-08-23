
#ifndef __TRANSFORM2D_H__
#define __TRANSFORM2D_H__

#include "../vector/vec2f.h"
#include "../matrix/mat3f.h"

inline mat3f translate2D(const vec2f v) {
    return (mat3f) {
        1.0f, 0.0f,  v.x,
        0.0f, 1.0f,  v.y,
        0.0f, 0.0f, 1.0f,
    };
}

inline mat3f scale2D(const vec2f v) {
    return (mat3f) {
         v.x, 0.0f, 0.0f,
        0.0f,  v.y, 0.0f,
        0.0f, 0.0f, 1.0f,
    };
}

inline mat3f rotate2D(const float radian) {
    const float s = fast_sin_extended(radian);
    const float c = fast_cos_extended(radian);
    return (mat3f) {
           c,   -s, 0.0f,
           s,    c, 0.0f,
        0.0f, 0.0f, 1.0f,
    };
}

inline mat3f shear2D(const float xy, const float yx) {
    return (mat3f) {
        1.0f,   xy, 0.0f,
          yx, 1.0f, 0.0f,
        0.0f, 0.0f, 1.0f,
    };
}

#endif // __TRANSFORM2D_H__
