
#ifndef __VEC3I_H__
#define __VEC3I_H__

#include "../core/pglmdef.h"

typedef union {
    i32 raw[3];
    struct {
        i32 x;
        i32 y;
        i32 z;
    };
    struct {
        i32 r;
        i32 g;
        i32 b;
    };
    struct {
        i32 u;
        i32 v;
        i32 w;
    };
} vec3i;

#define vec3i_zero      ((vec3i){{ 0,  0,  0}})
#define vec3i_right     ((vec3i){{ 1,  0,  0}})
#define vec3i_up        ((vec3i){{ 0,  1,  0}})
#define vec3i_backward  ((vec3i){{ 0,  0,  1}})
#define vec3i_left      ((vec3i){{-1,  0,  0}})
#define vec3i_down      ((vec3i){{ 0, -1,  0}})
#define vec3i_forward   ((vec3i){{ 0,  0, -1}})

inline vec3i vec3i_negate(vec3i v) {
    return (vec3i){{
        -v.x,
        -v.y,
        -v.z,
    }};
}

inline vec3i vec3i_add(vec3i v1, vec3i v2) {
    return (vec3i){{
        v1.x + v2.x, 
        v1.y + v2.y, 
        v1.z + v2.z,
    }};
}

inline vec3i vec3i_sub(vec3i v1, vec3i v2) {
    return (vec3i){{
        v1.x - v2.x, 
        v1.y - v2.y, 
        v1.z - v2.z,
    }};
}

inline vec3i vec3i_mul(vec3i v1, vec3i v2) {
    return (vec3i){{
        v1.x * v2.x, 
        v1.y * v2.y, 
        v1.z * v2.z,
    }};
}

// REQUIREMENT: v2.x, v2.y, and v2.z must be non-zero.
inline vec3i vec3i_div(vec3i v1, vec3i v2) {
    return (vec3i){{
        v1.x / v2.x, 
        v1.y / v2.y, 
        v1.z / v2.z,
    }};
}

inline vec3i vec3i_scale(vec3i v, i32 scale) {
    return (vec3i){{
        v.x * scale, 
        v.y * scale, 
        v.z * scale,
    }};
}

inline bool vec3i_equal(vec3i v1, vec3i v2) {
    return v1.x == v2.x && 
           v1.y == v2.y && 
           v1.z == v2.z;
}

inline bool vec3i_not_equal(vec3i v1, vec3i v2) {
    return v1.x != v2.x ||
           v1.y != v2.y || 
           v1.z != v2.z;
}

#endif // __VEC3I_H__
