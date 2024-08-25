
#ifndef __VIEWPORT_H__
#define __VIEWPORT_H__

#include "../matrix/mat4f.h"

// Origin is the top left corner of the window
inline mat4f viewport(int x, int y, uint32_t width, uint32_t height, float near, float far) {
    const float hwidth  =  width * 0.5f;
    const float hheight = height * 0.5f;

    return (mat4f) {
        hwidth,     0.0f,            0.0f,      x + hwidth,
          0.0f, -hheight,            0.0f,     y + hheight,
          0.0f,     0.0f, (far-near)*0.5f, (far+near)*0.5f,
          0.0f,     0.0f,            0.0f,            1.0f,
    };
}

#endif // __VIEWPORT_H__
