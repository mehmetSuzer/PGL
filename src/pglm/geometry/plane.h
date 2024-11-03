
#ifndef __PLANE_H__
#define __PLANE_H__

#include "../geometry/ray.h"

typedef struct {
    vec3f normal;
    f32 d;
} plane_t;

inline f32 plane_signed_distance(plane_t p, vec3f v) {
    return vec3f_dot(p.normal, v) + p.d;
}

static inline bool plane_intersects_ray(ray_t ray, plane_t plane, f32 near, f32 far, f32* t) {
    if (vec3f_dot(ray.dir, plane.normal) < 1E-6f) {
        return false;
    }

    const f32 dot_sn = vec3f_dot(ray.source, plane.normal);
    const f32 dot_dn = vec3f_dot(ray.dir, plane.normal);
    *t = -(plane.d + dot_sn) / dot_dn;
    return true;
}

#endif // __PLANE_H__
