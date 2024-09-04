
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

#endif // __PRIMITIVE_H__
