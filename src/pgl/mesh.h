
#ifndef __MESH_H__
#define __MESH_H__

#include "vertex.h"

typedef enum {
    TRIANGLE,
    TRIANGLE_FAN,
    TRIANGLE_STRIP,
} MeshType;

typedef enum {
    WIRED,
    FILLED,
} RenderType;

typedef struct {
    mat4f model;
    const Vertex* vertices;
    const uint32_t* indices;
    uint32_t vertex_number;
    uint32_t index_number;
    MeshType mesh_type;
    RenderType render_type;
} Mesh;

#endif // __MESH_H__
