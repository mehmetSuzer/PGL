
#ifndef __CAMERA_H__
#define __CAMERA_H__

#include "pglm.h"
#include "lcd.h"

typedef struct {
    vec3f position;
    vec3f forward;
    vec3f up;
    vec3f right;
    mat4f projection;
    mat4f view;
    float fov;
    float near;
    float far;
} Camera;

void init_camera(Camera* camera, const vec3f position, const vec3f forward, const vec3f up, const float near, const float far, const float fov);

#endif // __CAMERA_H__
