
#ifndef __VIEWPORT_H__
#define __VIEWPORT_H__

#include "../matrix/mat4f.h"

// Origin is the top left corner of the window.
// near plane is mapped to 0.0f, while the far plane is mapped to 1.0f.
inline mat4f viewport(i32 x, i32 y, u32 width, u32 height) {
    const i32 hw = (width  - 1) / 2; // -1 is needed to ensure that x of a screen space coordinate is less than x + width.
    const i32 hh = (height - 1) / 2; // -1 is needed to ensure that y of a screen space coordinate is less than y + height.

    return (mat4f){
          hw,  0.0f,   0.0f,  x + hw,
        0.0f,   -hh,   0.0f,  y + hh,
        0.0f,  0.0f,   0.5f,    0.5f,
        0.0f,  0.0f,   0.0f,    1.0f,
    };
}

#endif // __VIEWPORT_H__
