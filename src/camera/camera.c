
#include "camera.h"

Camera camera;

void camera_init(const vec3f position, const vec3f forward, const vec3f up, float near, float far, float fov) {
    const vec3f right = cross_vec3f(forward, up);
    camera.projection = perspective(fov, ASPECT_RATIO, near, far);
    camera.view = view(position, right, up, forward);
    camera.position = position;
    camera.forward = forward;
    camera.up = up;
    camera.right = right;
    camera.fov = fov;
    camera.near = near;
    camera.far = far;
    camera.speed = 2.0f;
    camera.forward_move = NONE;
    camera.right_move = NONE;
}

void camera_update(float dt) {
    if (camera.forward_move == NONE && camera.right_move == NONE) {
        return;
    }

    const float dx = camera.speed * dt;
    vec3f displacement = {0.0f, 0.0f, 0.0f};
    displacement = add_vec3f(displacement, scale_vec3f(camera.forward, camera.forward_move * dx));
    displacement = add_vec3f(displacement, scale_vec3f(camera.right,     camera.right_move * dx));
    
    if (camera.forward_move != NONE && camera.right_move != NONE) {
        displacement = scale_vec3f(displacement, M_SQRT1_2);
    }

    camera.position = add_vec3f(camera.position, displacement);
    camera.view = view(camera.position, camera.right, camera.up, camera.forward);
}
