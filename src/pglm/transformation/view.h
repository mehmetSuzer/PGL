
#ifndef __VIEW_H__
#define __VIEW_H__

#include "../vector/vec3f.h"
#include "../matrix/mat4f.h"

inline mat4f view(const vec3f position, const vec3f right, const vec3f up, const vec3f forward) {
    const float dot0 = -dot_vec3f(right, position);
    const float dot1 = -dot_vec3f(up, position);
    const float dot2 = -dot_vec3f(forward, position);
    
    return (mat4f) {
        right.x,    up.x, forward.x, 0.0f,
        right.y,    up.y, forward.y, 0.0f,
        right.z,    up.z, forward.z, 0.0f,
           dot0,    dot1,      dot2, 1.0f,
    };
}

#endif // __VIEW_H__
