
#include "camera.h"

#define CAMERA_MOVE_SPEED       2.0f
#define CAMERA_ROTATION_SPEED   1.0f

camera_t camera;

void camera_init(const vec3f position, const vec3f forward, const vec3f up) {
    camera.position = position;
    camera.forward = forward;
    camera.up = up;
    camera.right = cross_vec3f(forward, up);

    camera.forward_change = NONE;
    camera.right_change = NONE;
    camera.up_change = NONE;
    camera.rotate_y_change = NONE;
}

void camera_update(float dt) {
    if (camera.forward_change == NONE && camera.right_change == NONE && camera.up_change == NONE && camera.rotate_y_change == NONE) {
        return;
    }

    const float dx = CAMERA_MOVE_SPEED * dt;
    vec3f displacement = {0.0f, 0.0f, 0.0f};
    uint32_t active_axis_number = 0;
    
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
        // TODO: a more efficient way can be implemented such as quaternions
        const float theta = camera.rotate_y_change * CAMERA_ROTATION_SPEED * dt;
        camera.forward = mul_mat3f_vec3f(rotate3D_y_mat3f(theta), camera.forward);
        camera.right = mul_mat3f_vec3f(rotate3D_y_mat3f(theta), camera.right);
    }

    pgl_view(camera.position, camera.right, camera.up, camera.forward);
}
