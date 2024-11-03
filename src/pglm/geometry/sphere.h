
#ifndef __SPHERE_H__
#define __SPHERE_H__

#include "../geometry/ray.h"

typedef struct {
    vec3f center;
    f32 radius;
} sphere_t;

inline vec3f sphere_normal_at(sphere_t sphere, vec3f point_on_sphere) {
    return vec3f_normalize(vec3f_sub(point_on_sphere, sphere.center));
}

static inline bool sphere_intersects_ray(ray_t ray, sphere_t sphere, f32 near, f32 far, f32* t) {
    const vec3f center_to_source = vec3f_sub(ray.source, sphere.center);
    const f32 dist2 = vec3f_mag2(center_to_source);
    const f32 dot = vec3f_dot(center_to_source, ray.dir);
    const f32 quarter_discriminant = dot * dot - dist2 + sphere.radius * sphere.radius;

    if (quarter_discriminant < 1E-6f) {
        return false;
    }

    const f32 low = -dot - sqrtf(quarter_discriminant); 
    if (near < low && low < far) {
        *t = low;
        return true;
    }
    
    return false;
}

#endif // __SPHERE_H__
