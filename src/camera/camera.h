
#ifndef __CAMERA_H__
#define __CAMERA_H__

#include "pglm.h"

typedef struct {
    vec3f position;
    vec3f low_left;
    vec3f right_per_x;
    vec3f up_per_y;
    float near;
    float far;
} Camera;

Camera init_camera(vec3f position, vec3f forward, vec3f up, float near, float far, float fov_radian, uint32_t width, uint32_t height);

#endif // __CAMERA_H__
