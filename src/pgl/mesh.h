
#ifndef __MESH_H__
#define __MESH_H__

#include "vertex.h"

typedef struct {
    mat4f model;
    Vertex* vertices;
    uint32_t* indices;
    uint32_t vertex_number;
    uint32_t index_number;
} Mesh;

#endif // __MESH_H__
