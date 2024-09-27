
#ifndef __CAMERA_H__
#define __CAMERA_H__

#include "pgl.h"

typedef enum {
    NEGATIVE = -1,
    NONE = 0,
    POSITIVE = 1,
} camera_change_t;

typedef struct {
    vec3f position;
    vec3f forward;
    vec3f up;
    vec3f right;
    camera_change_t forward_change;
    camera_change_t right_change;
    camera_change_t up_change;
    camera_change_t rotate_y_change;
} camera_t;
extern camera_t camera;

// forward and up must be unit perpendicular vectors
void camera_init(vec3f position, vec3f forward, vec3f up);

void camera_update(float dt);

#endif // __CAMERA_H__
