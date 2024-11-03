
#ifndef __LIGHT_H__
#define __LIGHT_H__

#include "pglm.h"
#include <assert.h>

typedef struct {
    vec3f color;    // MUST: R,G,B in [0.0f, 1.0f]
    vec3f position;
    f32 quadratic;  // MUST: quadratic >= 0.0f
    f32 linear;     // MUST: linear >= 0.0f
} point_light_t;

point_light_t point_light_init(vec3f color, vec3f position, f32 distance1, f32 attenuation1, f32 distance2, f32 attenuation2);
f32 point_light_attenuation(point_light_t light, vec3f point);

// ------------------------------------------------------------------------------------------------- //

typedef struct {
    vec3f color;        // MUST: R,G,B in [0.0f, 1.0f]
    vec3f direction;    // MUST: magnitude(direction) == 1.0f
    f32 intensity;      // MUST: 0.0f < intensity <= 1.0f
} directional_light_t;

directional_light_t directional_light_init(vec3f color, vec3f direction, f32 intensity);

// ------------------------------------------------------------------------------------------------- //

typedef struct {
    vec3f color;            // MUST: R,G,B in [0.0f, 1.0f]
    vec3f position; 
    vec3f direction;        // MUST: magnitude(direction) == 1.0f
    f32 quadratic;          // MUST: quadratic >= 0.0f
    f32 linear;             // MUST: linear >= 0.0f
    f32 cos_cutoff_angle;   // MUST: 0.0f < cutoff_angle < PI
} spot_light_t;

spot_light_t spot_light_init(vec3f color, vec3f position, vec3f direction, f32 distance1, f32 attenuation1, f32 distance2, f32 attenuation2, f32 cutoff_angle);
f32 spot_light_attenuation(spot_light_t light, vec3f point);

#endif // __LIGHT_H__
