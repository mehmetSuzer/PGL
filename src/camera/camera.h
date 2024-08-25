
#ifndef __CAMERA_H__
#define __CAMERA_H__

#include "pglm.h"
#include "lcd.h"

typedef struct {
    mat4f projection;
    mat4f view;
    vec3f position;
    vec3f forward;
    vec3f up;
    vec3f right;
    float fov;
    float near;
    float far;
} Camera;
extern Camera camera;

// forward and up must be unit perpendicular vectors
void init_camera(const vec3f position, const vec3f forward, const vec3f up, float near, float far, float fov);

#endif // __CAMERA_H__
