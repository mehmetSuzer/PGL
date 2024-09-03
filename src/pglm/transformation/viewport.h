
#ifndef __VIEWPORT_H__
#define __VIEWPORT_H__

#include "../matrix/mat4f.h"

// Origin is the top left corner of the window.
// near plane is mapped to 0.0f, while the far plane is mapped to 1.0f.
inline mat4f viewport(int x, int y, uint32_t width, uint32_t height) {
    const float hw =  width * 0.5f;
    const float hh = height * 0.5f;

    return (mat4f){
          hw,  0.0f,   0.0f,  x + hw,
        0.0f,   -hh,   0.0f,  y + hh,
        0.0f,  0.0f,   0.5f,    0.5f,
        0.0f,  0.0f,   0.0f,    1.0f,
    };
}

#endif // __VIEWPORT_H__
