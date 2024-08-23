
#include "camera.h"

// forward and up must be unit perpendicular vectors
void init_camera(Camera* camera, const vec3f position, const vec3f forward, const vec3f up, const float near, const float far, const float fov) {
    const vec3f right = cross_vec3f(forward, up);
    camera->position = position;
    camera->forward = forward;
    camera->up = up;
    camera->right = right;
    camera->near = near;
    camera->far = far;
    camera->fov = fov;
    camera->projection = perspective(fov, ASPECT_RATIO, near, far);
    camera->view = view(position, right, up, forward);
}
