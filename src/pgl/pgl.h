
#ifndef __PGL_H__
#define __PGL_H__

#include <stdio.h>
#include "device.h"
#include "mesh.h"
#include "shapes.h"

typedef enum {
    COLOR_BUFFER_BIT = 1,
    DEPTH_BUFFER_BIT = 2,
} pgl_buffer_bit_t;

void pgl_view(const vec3f position, const vec3f right, const vec3f up, const vec3f forward);
void pgl_projection(float near, float far, float fov);
void pgl_viewport(int x, int y, uint32_t width, uint32_t height);

void pgl_clear_color(uint16_t color);
void pgl_clear(pgl_buffer_bit_t buffer_bits);

void pgl_draw(const mesh_t* mesh);
void pgl_display();

#endif // __PGL_H__
