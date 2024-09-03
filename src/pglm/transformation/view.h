
#ifndef __VIEW_H__
#define __VIEW_H__

#include "../vector/vec3f.h"
#include "../matrix/mat4f.h"

inline mat4f view(const vec3f position, const vec3f right, const vec3f up, const vec3f forward) {
    const float dotr = dot_vec3f(right, position);
    const float dotu = dot_vec3f(up, position);
    const float dotf = dot_vec3f(forward, position);
    
    return (mat4f){
           right.x,    right.y,    right.z, -dotr,
              up.x,       up.y,       up.z, -dotu,
        -forward.x, -forward.y, -forward.z,  dotf,
              0.0f,       0.0f,       0.0f,  1.0f,
    };
}

#endif // __VIEW_H__
