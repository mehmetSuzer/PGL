
#include "camera.h"

Camera camera;

void init_camera(const vec3f position, const vec3f forward, const vec3f up, float near, float far, float fov) {
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
    camera.forward_move = NONE;
    camera.right_move = NONE;
}

void update_camera(float dt) {
    if (camera.forward_move == NONE && camera.right_move == NONE) {
        return;
    }

    vec3f dx = {0.0f, 0.0f, 0.0f};
    dx = add_vec3f(dx, scale_vec3f(camera.forward, camera.forward_move * dt));
    dx = add_vec3f(dx, scale_vec3f(camera.right,     camera.right_move * dt));
    
    if (camera.forward_move != NONE && camera.right_move != NONE) {
        dx = scale_vec3f(dx, M_SQRT1_2);
    }

    camera.position = add_vec3f(camera.position, dx);
    camera.view = view(camera.position, camera.right, camera.up, camera.forward);
}
