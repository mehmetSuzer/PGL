
#ifndef __CAMERA_H__
#define __CAMERA_H__

#include "pgl.h"

typedef enum {
    NEGATIVE = -1,
    NONE = 0,
    POSITIVE = 1,
} move_t;

typedef struct {
    vec3f position;
    vec3f forward;
    vec3f up;
    vec3f right;
    float speed;
    move_t forward_move;
    move_t right_move;
    move_t up_move;
} camera_t;
extern camera_t camera;

// forward and up must be unit perpendicular vectors
void camera_init(const vec3f position, const vec3f forward, const vec3f up);

void camera_update(float dt);

#endif // __CAMERA_H__
