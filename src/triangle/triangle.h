
#ifndef __TRIANGLE_H__
#define __TRIANGLE_H__

#include "pglm.h"

// a, b, and c should be in counter-clockwise winding order
typedef struct {
    vec3f color;  // xyz -> rgb must be in [0.0f, 1.0f]
    vec3f a;
    vec3f b;
    vec3f c;
    vec3f normal; // init_triangle calculates the unit normal vector
} Triangle;

// xyz -> rgb color values must be between [0.0f, and 1.0f]
static inline Triangle init_triangle(const vec3f color, const vec3f a, const vec3f b, const vec3f c) {
    vec3f v1 = sub_vec3f(b, a);
    vec3f v2 = sub_vec3f(c, a); 
    vec3f normal = normalize_vec3f(cross_vec3f(v1, v2));
    return (Triangle) {color, a, b, c, normal};
}

#endif // __TRIANGLE_H__
