
#ifndef __MESH_H__
#define __MESH_H__

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
    const vec3f* vertices;
    const uint16_t* indices;
    const uint16_t vertex_number;
    const uint16_t index_number;
    const uint16_t mesh_enum;
    union {
        const uint16_t color;       // used for wired rendering 
        const uint16_t tex_index;   // used for filled rendering
    };
} mesh_t;

extern const vec2f tex_coords[5];

extern const vec3f equilateral_triangle_vertices[3];
extern const uint16_t equilateral_triangle_indices[6];

extern const vec3f square_vertices[4];
extern const uint16_t square_indices[12];

extern const vec3f cube_vertices[8];
extern const uint16_t cube_indices[72];

extern const vec3f triangle_pyramid_vertices[4];
extern const uint16_t triangle_pyramid_indices[24];

extern const vec3f square_pyramid_vertices[5];
extern const uint16_t square_pyramid_indices[36];

extern const vec3f dodecagon_pyramid_vertices[13];
extern const uint16_t dodecagon_pyramid_indices[132];

extern const vec3f icositetragon_pyramid_vertices[25];
extern const uint16_t icositetragon_pyramid_indices[276];

extern const vec3f icosahedron_vertices[12];
extern const uint16_t icosahedron_indices[120];

extern const vec3f dodacahedron_vertices[20];
extern const uint16_t dodacahedron_indices[216];

extern const vec3f truncated_icosahedron_vertices[60];
extern const uint16_t truncated_icosahedron_indices[696];

extern const vec3f rhombicubotahedron_vertices[24];
extern const uint16_t rhombicubotahedron_indices[264];

extern const vec3f spaceship_vertices[55];
extern const uint16_t spaceship_indices[636];

void find_mesh_bounding_volume(mesh_t* mesh);

#endif // __MESH_H__
