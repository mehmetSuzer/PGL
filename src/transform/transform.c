
#include "transform.h"

static void transform_update_model(transform_t* transform) {
    const mat4f t = translate3D_mat4f(transform->position);
    const mat4f r = cast_quat_to_mat4f(transform->rotation);
    const mat4f s = scale3D_mat4f(transform->scalar);
    transform->model = mul_mat4f_mat4f(t, mul_mat4f_mat4f(r, s));
}

transform_t transform_init(vec3f position, quat rotation, vec3f scalar) {
    transform_t transform = {position, rotation, scalar};
    transform_update_model(&transform);
    return transform;
}

void transform_set_position(transform_t* transform, vec3f position) {
    transform->position = position;
    transform_update_model(transform);
}

void transform_translate(transform_t* transform, vec3f translation) {
    transform->position = add_vec3f(transform->position, translation);
    transform_update_model(transform);
}

void transform_set_rotation_quat(transform_t* transform, quat quaternion) {
    transform->rotation = quaternion;
    transform_update_model(transform);
}

void transform_set_rotation_euler_angles(transform_t* transform, vec3f angles) {
    transform->rotation = quat_euler_angles(angles);
    transform_update_model(transform);
}

void transform_rotate_quat(transform_t* transform, quat quaternion) {
    transform->rotation = mul_quat_quat(quaternion, transform->rotation);
    transform_update_model(transform);
}

void transform_rotate_euler_angles(transform_t* transform, vec3f angles) {
    transform->rotation = mul_quat_quat(quat_euler_angles(angles), transform->rotation);
    transform_update_model(transform);
}

void transform_set_scalar_vec3f(transform_t* transform, vec3f scalar) {
    transform->scalar = scalar;
    transform_update_model(transform);
}

void transform_set_scalar(transform_t* transform, float scale) {
    transform->scalar = (vec3f){scale, scale, scale};
    transform_update_model(transform);
}

void transform_scale_vec3f(transform_t* transform, vec3f scalar) {
    transform->scalar = mul_vec3f(transform->scalar, scalar);
    transform_update_model(transform);
}

void transform_scale(transform_t* transform, float scale) {
    transform->scalar = scale_vec3f(transform->scalar, scale);
    transform_update_model(transform);
}
