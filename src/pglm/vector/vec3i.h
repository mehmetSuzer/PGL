
#ifndef __VEC3I_H__
#define __VEC3I_H__

#include "../core/pglmdef.h"

typedef union {
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
    i32 n[3];
} vec3i;

#define zero_vec3i ((vec3i){0, 0, 0})

inline vec3i neg_vec3i(vec3i v) {
    return (vec3i){
        -v.x,
        -v.y,
        -v.z,
    };
}

inline vec3i add_vec3i(vec3i v1, vec3i v2) {
    return (vec3i){
        v1.x + v2.x, 
        v1.y + v2.y, 
        v1.z + v2.z,
    };
}

inline vec3i sub_vec3i(vec3i v1, vec3i v2) {
    return (vec3i){
        v1.x - v2.x, 
        v1.y - v2.y, 
        v1.z - v2.z,
    };
}

inline vec3i mul_vec3i(vec3i v1, vec3i v2) {
    return (vec3i){
        v1.x * v2.x, 
        v1.y * v2.y, 
        v1.z * v2.z,
    };
}

inline vec3i div_vec3i(vec3i v1, vec3i v2) {
    return (vec3i){
        v1.x / v2.x, 
        v1.y / v2.y, 
        v1.z / v2.z,
    };
}

inline vec3i scale_vec3i(vec3i v, i32 scale) {
    return (vec3i){
        v.x * scale, 
        v.y * scale, 
        v.z * scale,
    };
}

inline bool equal_vec3i(vec3i v1, vec3i v2) {
    return v1.x == v2.x && 
           v1.y == v2.y && 
           v1.z == v2.z;
}

inline bool not_equal_vec3i(vec3i v1, vec3i v2) {
    return v1.x != v2.x ||
           v1.y != v2.y || 
           v1.z != v2.z;
}

#endif // __VEC3I_H__
