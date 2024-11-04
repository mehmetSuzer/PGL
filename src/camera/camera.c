
#include "camera.h"

#define CAMERA_MOVE_SPEED       3.0f
#define CAMERA_ROTATION_SPEED   1.0f

camera_t camera = {
    .position        = { 0.0f,  0.0f,  0.0f},
    .forward         = { 0.0f,  0.0f, -1.0f},
    .up              = { 0.0f,  1.0f,  0.0f},
    .right           = { 1.0f,  0.0f,  0.0f},
    .forward_change  = CHANGE_NONE,
    .right_change    = CHANGE_NONE,
    .up_change       = CHANGE_NONE,
    .rotate_y_change = CHANGE_NONE,
};

void camera_init(vec3f position, vec3f forward, vec3f up) {
    camera.position = position;
    camera.forward = forward;
    camera.up = up;
    camera.right = vec3f_cross(forward, up);

    camera.forward_change  = CHANGE_NONE;
    camera.right_change    = CHANGE_NONE;
    camera.up_change       = CHANGE_NONE;
    camera.rotate_y_change = CHANGE_NONE;
}

void camera_update(f32 dt) {
    if (camera.forward_change  == CHANGE_NONE && 
        camera.right_change    == CHANGE_NONE && 
        camera.up_change       == CHANGE_NONE && 
        camera.rotate_y_change == CHANGE_NONE) {
        return;
    }

    vec3f direction = {0.0f, 0.0f, 0.0f};
    u32 active_axis_number = 0;
    
    if (camera.right_change != CHANGE_NONE) {
        direction = vec3f_add(direction, (camera.right_change == CHANGE_POSITIVE) ? camera.right : vec3f_negate(camera.right));
        active_axis_number++;
    }
    if (camera.forward_change != CHANGE_NONE) {
        direction = vec3f_add(direction, (camera.forward_change == CHANGE_POSITIVE) ? camera.forward : vec3f_negate(camera.forward));
        active_axis_number++;
    }
    if (camera.up_change != CHANGE_NONE) {
        direction = vec3f_add(direction, (camera.up_change == CHANGE_POSITIVE) ? camera.up : vec3f_negate(camera.up));
        active_axis_number++;
    }

    if (active_axis_number == 2) {
        direction = vec3f_scale(direction, PGLM_1_SQRT2f);
    }
    else if (active_axis_number == 3) {
        direction = vec3f_scale(direction, PGLM_1_SQRT3f);
    }

    const vec3f displacement = vec3f_scale(direction, CAMERA_MOVE_SPEED * dt);
    camera.position = vec3f_add(camera.position, displacement);

    if (camera.rotate_y_change != CHANGE_NONE) {
        const f32 angular_velocity = (camera.rotate_y_change == CHANGE_POSITIVE) ? CAMERA_ROTATION_SPEED : -CAMERA_ROTATION_SPEED;
        const f32 angle = angular_velocity * dt;
        const quat rotation = quat_angle_axis(camera.up, angle);
        camera.forward = quat_rotate_vec3f(rotation, camera.forward);
        camera.right = quat_rotate_vec3f(rotation, camera.right);
    }

    pgl_view(camera.position, camera.right, camera.up, camera.forward);
}
