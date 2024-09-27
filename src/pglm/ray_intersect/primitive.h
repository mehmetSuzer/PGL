
#ifndef __PRIMITIVE_H__
#define __PRIMITIVE_H__

#include "../vector/vec3f.h"

typedef struct {
    vec3f center;
    float radius;
} sphere_t;

typedef struct {
    vec3f a;
    vec3f b;
    vec3f c;
} triangle_t;

inline vec3f triangle_normal(triangle_t t) {
    const vec3f vab = sub_vec3f(t.b, t.a);
    const vec3f vac = sub_vec3f(t.c, t.a);
    return normalize_vec3f(cross_vec3f(vab, vac));
}

typedef struct {
    vec3f normal;
    float d;
} plane_t;

inline float plane_signed_distance(plane_t p, vec3f v) {
    return dot_vec3f(p.normal, v) + p.d;
} 

#endif // __PRIMITIVE_H__
