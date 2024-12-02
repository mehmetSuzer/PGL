
#ifndef __RAY_H__
#define __RAY_H__

#include "../vector/vec3f.h"

typedef struct {
    vec3f source;
    vec3f direction; // REQUIREMENT: direction must be a unit vector
} ray_t;

inline vec3f ray_position(ray_t ray, f32 distance) {
    return vec3f_add(ray.source, vec3f_scale(ray.direction, distance));
}

#endif // __RAY_H__
