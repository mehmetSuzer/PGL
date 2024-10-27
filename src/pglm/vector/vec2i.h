
#ifndef __VEC2I_H__
#define __VEC2I_H__

#include "../core/pglmdef.h"

typedef union {
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
    i32 n[2];
} vec2i;

#define zero_vec2i ((vec2i){0, 0})

inline vec2i neg_vec2i(vec2i v) {
    return (vec2i){
        -v.x,
        -v.y,
    };
}

inline vec2i add_vec2i(vec2i v1, vec2i v2) {
    return (vec2i){
        v1.x + v2.x, 
        v1.y + v2.y, 
    };
}

inline vec2i sub_vec2i(vec2i v1, vec2i v2) {
    return (vec2i){
        v1.x - v2.x, 
        v1.y - v2.y, 
    };
}

inline vec2i mul_vec2i(vec2i v1, vec2i v2) {
    return (vec2i){
        v1.x * v2.x, 
        v1.y * v2.y, 
    };
}

inline vec2i div_vec2i(vec2i v1, vec2i v2) {
    return (vec2i){
        v1.x / v2.x, 
        v1.y / v2.y, 
    };
}

inline vec2i scale_vec2i(vec2i v, i32 scale) {
    return (vec2i){
        v.x * scale, 
        v.y * scale, 
    };
}

inline bool equal_vec2i(vec2i v1, vec2i v2) {
    return v1.x == v2.x && 
           v1.y == v2.y;
}

inline bool not_equal_vec2i(vec2i v1, vec2i v2) {
    return v1.x != v2.x ||
           v1.y != v2.y;
}

#endif // __VEC2I_H__
