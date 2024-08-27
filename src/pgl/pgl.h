
#ifndef __PGL_H__
#define __PGL_H__

#include <stdio.h>
#include "device.h"
#include "mesh.h"

typedef enum {
    PGL_TRIANGLE,
    PGL_TRIANGLE_FAN,
    PGL_TRIANGLE_STRIP,
} MeshType;

typedef enum {
    PGL_WIRED,
    PGL_FILLED,
} RenderType;

typedef struct {
    uint16_t screen[LCD_HEIGHT][LCD_HEIGHT];
    mat4f view;
    mat4f projection;
    mat4f viewport;
    MeshType mesh_type;
    RenderType render_type;
} PGL;

void pgl_view(const vec3f position, const vec3f right, const vec3f up, const vec3f forward);
void pgl_projection(float fov, float near, float far);
void pgl_viewport(int x, int y, uint32_t width, uint32_t height);

void pgl_clear(uint16_t color);
void pgl_draw(const Mesh* mesh);
void pgl_display();

#endif // __PGL_H__
