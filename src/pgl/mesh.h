
#ifndef __MESH_H__
#define __MESH_H__

#include "vertex.h"

typedef enum {
    TRIANGLE,
    TRIANGLE_FAN,
    TRIANGLE_STRIP,
} mesh_type_t;

typedef enum {
    WIRED,
    FILLED,
} render_type_t;

typedef struct {
    mat4f model;
    const vertex_t* vertices;
    const uint32_t* indices;
    uint32_t vertex_number;
    uint32_t index_number;
    mesh_type_t mesh_type;
    render_type_t render_type;
} mesh_t;

#endif // __MESH_H__
