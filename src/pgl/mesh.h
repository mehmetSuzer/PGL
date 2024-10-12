
#ifndef __MESH_H__
#define __MESH_H__

#include "vertex.h"
#include "transform.h"

typedef enum {
    MESH_TRIANGLE = 0,
    MESH_TRIANGLE_FAN = 1,
    MESH_TRIANGLE_STRIP = 2,

    MESH_RENDER_WIRED = 0,
    MESH_RENDER_FILLED = 4,
} mesh_enum_t;

typedef struct {
    transform_t transform;
    sphere_t bounding_volume;
    const vertex_t* vertices;
    const uint16_t* indices;
    const uint16_t vertex_number;
    const uint16_t index_number;
    const uint16_t tex_index;
    const uint16_t mesh_enum;
} mesh_t;

extern const vertex_t triangle_vertices[3];
extern const uint16_t triangle_indices[3];

extern const vertex_t square_vertices[4];
extern const uint16_t square_indices[6];

extern const vertex_t cube_vertices[16];
extern const uint16_t cube_indices[36];

extern const vertex_t triangle_pyramid_vertices[8];
extern const uint16_t triangle_pyramid_indices[12];

extern const vertex_t square_pyramid_vertices[11];
extern const uint16_t square_pyramid_indices[18];

extern const vertex_t spaceship_vertices[55];
extern const uint16_t spaceship_indices[318];

void find_mesh_bounding_volume(mesh_t* mesh);

#endif // __MESH_H__
