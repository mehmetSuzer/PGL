
#ifndef __MESH_H__
#define __MESH_H__

#include <stdio.h>
#include "camera.h"
#include "vertex.h"
#include "rasterizer.h"

typedef struct {
    mat4f model;
    Vertex* vertices;
    uint32_t* indices;
    uint32_t vertex_number;
    uint32_t index_number;
} Mesh;

void process_mesh(const Mesh* mesh);

#endif // __MESH_H__
