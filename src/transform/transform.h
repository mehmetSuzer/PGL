
#ifndef __TRANSFORM_H__
#define __TRANSFORM_H__

#include "pglm.h"

typedef struct {
    vec3f position;
    quat rotation;
    vec3f scalar;
    mat4f model;
} transform_t;

transform_t transform_init(const vec3f position, const quat rotation, const vec3f scalar);

void transform_set_position(transform_t* transform, const vec3f position);
void transform_translate(transform_t* transform, const vec3f translation);

void transform_set_rotation_quat(transform_t* transform, const quat quaternion);
void transform_set_rotation_euler_angles(transform_t* transform, const vec3f angles);
void transform_rotate_quat(transform_t* transform, const quat quaternion);
void transform_rotate_euler_angles(transform_t* transform, const vec3f angles);

void transform_set_scalar_vec3f(transform_t* transform, const vec3f scalar);
void transform_set_scalar(transform_t* transform, float scale);
void transform_scale_vec3f(transform_t* transform, const vec3f scalar);
void transform_scale(transform_t* transform, float scale);

#endif // __TRANSFORM_H__
