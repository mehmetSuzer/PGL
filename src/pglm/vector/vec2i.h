
#ifndef __VEC2I_H__
#define __VEC2I_H__

#include "../core/pglmdef.h"

typedef union {
    i32 raw[2];
    struct {
        i32 x;
        i32 y;
    };
    struct {
        i32 r;
        i32 i;
    };
    struct {
        i32 u;
        i32 v;
    };
    struct {
        i32 s;
        i32 t;
    };
} vec2i;

#define vec2i_zero ((vec2i){{0, 0}})

inline vec2i vec2i_negate(vec2i v) {
    return (vec2i){{
        -v.x,
        -v.y,
    }};
}

inline vec2i vec2i_add(vec2i v1, vec2i v2) {
    return (vec2i){{
        v1.x + v2.x, 
        v1.y + v2.y, 
    }};
}

inline vec2i vec2i_sub(vec2i v1, vec2i v2) {
    return (vec2i){{
        v1.x - v2.x, 
        v1.y - v2.y, 
    }};
}

inline vec2i vec2i_mul(vec2i v1, vec2i v2) {
    return (vec2i){{
        v1.x * v2.x, 
        v1.y * v2.y, 
    }};
}

// REQUIREMENT: v2.x and v2.y must be non-zero.
inline vec2i vec2i_div(vec2i v1, vec2i v2) {
    return (vec2i){{
        v1.x / v2.x, 
        v1.y / v2.y, 
    }};
}

inline vec2i vec2i_scale(vec2i v, i32 scale) {
    return (vec2i){{
        v.x * scale, 
        v.y * scale, 
    }};
}

inline bool vec2i_equal(vec2i v1, vec2i v2) {
    return v1.x == v2.x && 
           v1.y == v2.y;
}

inline bool vec2i_not_equal(vec2i v1, vec2i v2) {
    return v1.x != v2.x ||
           v1.y != v2.y;
}

#endif // __VEC2I_H__
