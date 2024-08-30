
#ifndef __PGL_H__
#define __PGL_H__

#include <stdio.h>
#include "device.h"
#include "mesh.h"

void pgl_view(const vec3f position, const vec3f right, const vec3f up, const vec3f forward);
void pgl_projection(float fov, float near, float far);
void pgl_viewport(int x, int y, uint32_t width, uint32_t height);

void pgl_clear(uint16_t color);
void pgl_draw(const mesh_t* mesh);
void pgl_display();

#endif // __PGL_H__
