
#ifndef __CAMERA_H__
#define __CAMERA_H__

#include "pgl.h"

typedef enum {
    NEGATIVE = -1,
    NONE = 0,
    POSITIVE = 1,
} Move;

typedef struct {
    vec3f position;
    vec3f forward;
    vec3f up;
    vec3f right;
    float fov;
    float near;
    float far;
    float speed;
    Move forward_move;
    Move right_move;
} Camera;
extern Camera camera;

// forward and up must be unit perpendicular vectors
void camera_init(const vec3f position, const vec3f forward, const vec3f up, float near, float far, float fov);

void camera_update(float dt);

#endif // __CAMERA_H__
