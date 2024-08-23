
#include "camera.h"

// forward and up must be unit perpendicular vectors
Camera init_camera(const vec3f position, const vec3f forward, const vec3f up, 
    const float near, const float far, const float fov_radian, const uint32_t width, const uint32_t height) {
    
    const float screen_half_width = near * fast_tan(fov_radian / 2.0f); // if you want to use fov_radian greater than pi/2, use fast_tan_extended
    const float screen_half_height = (screen_half_width / height) * width;

    const vec3f right = cross_vec3f(forward, up);
    const vec3f low_left = sub_vec3f(scale_vec3f(forward, near), add_vec3f(scale_vec3f(right, screen_half_width), scale_vec3f(up, screen_half_height)));

    const vec3f right_per_x = scale_vec3f(right, 2.0f * screen_half_width);
    const vec3f up_per_y = scale_vec3f(up, 2.0f * screen_half_height);

    return (Camera) {
        position,
        low_left,
        right_per_x,
        up_per_y,
        near,
        far,
    };
}

