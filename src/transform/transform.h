
#ifndef __TRANSFORM_H__
#define __TRANSFORM_H__

#include "pglm.h"

typedef struct {
    vec3f position; // internal field, not recommended to use directly
    quat  rotation; // internal field, not recommended to use directly
    vec3f scalar;   // internal field, not recommended to use directly
    mat4f model;    // the model matrix of Translation * Rotation * Scalar
} transform_t;

transform_t transform_init(vec3f position, quat rotation, vec3f scalar);

void transform_set_position(transform_t* transform, vec3f position);
void transform_translate(transform_t* transform, vec3f translation);

void transform_set_rotation_quat(transform_t* transform, quat quaternion);
void transform_set_rotation_euler_angles(transform_t* transform, vec3f angles);
void transform_rotate_quat(transform_t* transform, quat quaternion);
void transform_rotate_euler_angles(transform_t* transform, vec3f angles);

void transform_set_scalar_vec3f(transform_t* transform, vec3f scalar);
void transform_set_scalar(transform_t* transform, f32 scale);
void transform_scale_vec3f(transform_t* transform, vec3f scalar);
void transform_scale(transform_t* transform, f32 scale);

#endif // __TRANSFORM_H__
