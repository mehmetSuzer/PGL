
#include "camera.h"

Camera camera;

// forward and up must be unit perpendicular vectors
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
}
