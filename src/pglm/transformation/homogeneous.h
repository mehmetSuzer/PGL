
#ifndef __HOMOGENEOUS_H__
#define __HOMOGENEOUS_H__

#include "../vector/vec3f.h"
#include "../vector/vec4f.h"

inline vec4f to_homogeneous(const vec3f v) {
    return (vec4f){
        v.x, 
        v.y, 
        v.z, 
        1.0f,
    };
}

inline vec3f from_homogeneous(const vec4f v) {
    return (vec3f){
        v.x / v.w, 
        v.y / v.w, 
        v.z / v.w,
    };
}

inline vec4f normalize_homogeneous(const vec4f v) {
    return (vec4f){
        v.x / v.w, 
        v.y / v.w, 
        v.z / v.w,
        1.0f,
    };
}

#endif // __HOMOGENEOUS_H__
