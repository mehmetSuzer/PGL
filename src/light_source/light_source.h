
#ifndef __LIGHT_SOURCE_H__
#define __LIGHT_SOURCE_H__

#include "pglm.h"

typedef struct {
    vec3f color; // xyz -> rgb must be between [0.0f, 1.0f]
    vec3f position;
    float quadratic;
    float linear;
} point_light_t;

inline float point_light_attenuation(point_light_t light, vec3f point) {
    const float distance = mag_vec3f(sub_vec3f(light.position, point));
    const float attenuation = 1.0f / ((light.quadratic * distance + light.linear) * distance + 1.0f);
    return attenuation;
}

// ------------------------------------------------------------------------------------------------- //

typedef struct {
    vec3f color; // xyz -> rgb must be between [0.0f, 1.0f]
    vec3f direction; // must be a unit vector
    float intensity;
} directional_light_t;

#endif // __LIGHT_SOURCE_H__
