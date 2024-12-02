
#ifndef __CAMERA_H__
#define __CAMERA_H__

#include "pgl.h"

typedef enum {
    CHANGE_NEGATIVE,
    CHANGE_NONE,
    CHANGE_POSITIVE,
} change_t;

typedef struct {
    vec3f position;
    vec3f forward;
    vec3f up;
    vec3f right;
    change_t forward_change  : 2;
    change_t right_change    : 2;
    change_t up_change       : 2;
    change_t rotate_y_change : 2;
} camera_t;
extern camera_t camera;

// REQUIREMENT: forward and up must be unit vectors that are perpendicular to each other.
void camera_init(vec3f position, vec3f forward, vec3f up);
void camera_update(f32 dt);

#endif // __CAMERA_H__
