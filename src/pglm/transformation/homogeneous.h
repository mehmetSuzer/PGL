
#ifndef __HOMOGENEOUS_H__
#define __HOMOGENEOUS_H__

#include "../vector/vec3f.h"
#include "../vector/vec4f.h"

inline vec4f homogeneous_point(vec3f p) {
    return (vec4f){{
        p.x, 
        p.y, 
        p.z, 
        1.0f,
    }};
}

inline vec4f homogeneous_vector(vec3f v) {
    return (vec4f){{
        v.x, 
        v.y, 
        v.z, 
        0.0f,
    }}; 
}

// REQUIREMENT: p.w must be non-zero.
inline vec3f homogeneous_point_to_vec3f(vec4f p) {
    return (vec3f){{
        p.x / p.w, 
        p.y / p.w, 
        p.z / p.w,
    }};
}

// If it is certain that v.w = 1.0f, 
// this function can be used to convert a homogeneous point to a vec3f point.
inline vec3f homogeneous_vector_to_vec3f(vec4f v) {
    return (vec3f){{
        v.x,
        v.y,
        v.z,
    }};
}

// REQUIREMENT: p.w must be non-zero.
inline vec4f homogeneous_point_normalize(vec4f p) {
    return (vec4f){{
        p.x / p.w, 
        p.y / p.w, 
        p.z / p.w,
        1.0f,
    }};
}

#endif // __HOMOGENEOUS_H__
