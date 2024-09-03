
#ifndef __RAY_H__
#define __RAY_H__

#include "../vector/vec3f.h"

typedef struct {
    vec3f source;
    vec3f dir; // must be a unit vector
} ray_t;

#endif // __RAY_H__
