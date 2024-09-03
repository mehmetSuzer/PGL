
#include "camera.h"

camera_t camera;

void camera_init(const vec3f position, const vec3f forward, const vec3f up) {
    camera.position = position;
    camera.forward = forward;
    camera.up = up;
    camera.right = cross_vec3f(forward, up);
    camera.speed = 2.0f;
    camera.forward_move = NONE;
    camera.right_move = NONE;
    camera.up_move = NONE;
}

void camera_update(float dt) {
    if (camera.forward_move == NONE && camera.right_move == NONE && camera.up_move == NONE) {
        return;
    }

    const float dx = camera.speed * dt;
    vec3f displacement = {0.0f, 0.0f, 0.0f};
    uint32_t active_axis_number = 0;
    
    if (camera.right_move != NONE) {
        displacement = add_vec3f(displacement, scale_vec3f(camera.right, camera.right_move * dx));
        active_axis_number++;
    }
    if (camera.forward_move != NONE) {
        displacement = add_vec3f(displacement, scale_vec3f(camera.forward, camera.forward_move * dx));
        active_axis_number++;
    }
    if (camera.up_move != NONE) {
        displacement = add_vec3f(displacement, scale_vec3f(camera.up, camera.up_move * dx));
        active_axis_number++;
    }

    if (active_axis_number == 2) {
        displacement = scale_vec3f(displacement, PGLM_1_SQRT2f);
    }
    else if (active_axis_number == 3) {
        displacement = scale_vec3f(displacement, PGLM_1_SQRT3f);
    }

    camera.position = add_vec3f(camera.position, displacement);
    pgl_view(camera.position, camera.right, camera.up, camera.forward);
}
