
#ifndef __MESH_H__
#define __MESH_H__

#include "vertex.h"

// TODO: these enums can be collected under one uint32_t
typedef enum {
    MESH_TRIANGLE,
    MESH_TRIANGLE_FAN,
    MESH_TRIANGLE_STRIP,
} mesh_type_t;

typedef enum {
    RENDER_WIRED,
    RENDER_FILLED,
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

extern const vertex_t triangle_vertices[3];
extern const uint32_t triangle_indices[3];

extern const vertex_t square_vertices[4];
extern const uint32_t square_indices[6];

extern const vertex_t cube_vertices[8];
extern const uint32_t cube_indices[36];

extern const vertex_t triangle_pyramid_vertices[4];
extern const uint32_t triangle_pyramid_indices[12];

extern const vertex_t square_pyramid_vertices[5];
extern const uint32_t square_pyramid_indices[18];

#endif // __MESH_H__
