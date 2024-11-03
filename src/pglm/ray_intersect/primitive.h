
#ifndef __PRIMITIVE_H__
#define __PRIMITIVE_H__

#include "../vector/vec3f.h"

typedef struct {
    vec3f center;
    f32   radius;
} sphere_t;

typedef struct {
    vec3f a;
    vec3f b;
    vec3f c;
} triangle_t;

inline vec3f triangle_normal(triangle_t t) {
    const vec3f v_ab = vec3f_sub(t.b, t.a);
    const vec3f v_ac = vec3f_sub(t.c, t.a);
    return vec3f_normalize(vec3f_cross(v_ab, v_ac));
}

typedef struct {
    vec3f normal;
    f32        d;
} plane_t;

inline f32 plane_signed_distance(plane_t p, vec3f v) {
    return vec3f_dot(p.normal, v) + p.d;
} 

#endif // __PRIMITIVE_H__
