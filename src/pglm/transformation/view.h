
#ifndef __VIEW_H__
#define __VIEW_H__

#include "../vector/vec3f.h"
#include "../matrix/mat4f.h"

inline mat4f view(vec3f position, vec3f right, vec3f up, vec3f forward) {
    const f32 dotr = vec3f_dot(right, position);
    const f32 dotu = vec3f_dot(up, position);
    const f32 dotf = vec3f_dot(forward, position);
    
    return (mat4f){
           right.x,    right.y,    right.z, -dotr,
              up.x,       up.y,       up.z, -dotu,
        -forward.x, -forward.y, -forward.z,  dotf,
              0.0f,       0.0f,       0.0f,  1.0f,
    };
}

#endif // __VIEW_H__
