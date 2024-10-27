
#include "camera.h"

#define CAMERA_MOVE_SPEED       3.0f
#define CAMERA_ROTATION_SPEED   1.0f

camera_t camera = {
    .position = { 0.0f,  0.0f,  0.0f},
    .forward =  { 0.0f,  0.0f, -1.0f},
    .up =       { 0.0f,  1.0f,  0.0f},
    .right =    { 1.0f,  0.0f,  0.0f},
    .forward_change = NONE,
    .right_change = NONE,
    .up_change = NONE,
    .rotate_y_change = NONE,
};

void camera_init(vec3f position, vec3f forward, vec3f up) {
    camera.position = position;
    camera.forward = forward;
    camera.up = up;
    camera.right = cross_vec3f(forward, up);

    camera.forward_change = NONE;
    camera.right_change = NONE;
    camera.up_change = NONE;
    camera.rotate_y_change = NONE;
}

void camera_update(f32 dt) {
    if (camera.forward_change == NONE && camera.right_change == NONE && camera.up_change == NONE && camera.rotate_y_change == NONE) {
        return;
    }

    const f32 dx = CAMERA_MOVE_SPEED * dt;
    vec3f displacement = {0.0f, 0.0f, 0.0f};
    u32 active_axis_number = 0;
    
    if (camera.right_change != NONE) {
        displacement = add_vec3f(displacement, scale_vec3f(camera.right, camera.right_change * dx));
        active_axis_number++;
    }
    if (camera.forward_change != NONE) {
        displacement = add_vec3f(displacement, scale_vec3f(camera.forward, camera.forward_change * dx));
        active_axis_number++;
    }
    if (camera.up_change != NONE) {
        displacement = add_vec3f(displacement, scale_vec3f(camera.up, camera.up_change * dx));
        active_axis_number++;
    }

    if (active_axis_number == 2) {
        displacement = scale_vec3f(displacement, PGLM_1_SQRT2f);
    }
    else if (active_axis_number == 3) {
        displacement = scale_vec3f(displacement, PGLM_1_SQRT3f);
    }
    camera.position = add_vec3f(camera.position, displacement);

    if (camera.rotate_y_change != NONE) {
        const f32 theta = camera.rotate_y_change * CAMERA_ROTATION_SPEED * dt;
        quat rotation = quat_angle_axis(camera.up, theta);
        camera.forward = rotate_quat(rotation, camera.forward);
        camera.right = rotate_quat(rotation, camera.right);
    }

    pgl_view(camera.position, camera.right, camera.up, camera.forward);
}
