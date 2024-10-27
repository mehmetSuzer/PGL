
#ifndef __MESH_H__
#define __MESH_H__

#include "transform.h"

typedef enum {
    FILLED_RENDER_NO           = 0, // Do not rasterize insides of triangles
    FILLED_RENDER_TEX_COORDS   = 1, // Each triangle vertex is coupled with a tex coord in 'indices' list: Index0, Tex0, Index1, Tex1, Index2, Tex2,
    FILLED_RENDER_COLORS       = 2, // Each face of a mesh is painted by a single color specified in 'indices' list: Index0, Index1, Index2, Color,
    FILLED_RENDER_SINGLE_COLOR = 3, // Whole mesh is painted by a single color specified at 'mesh_t.fill_color': Index0, Index1, Index2,
} filled_render_t;

typedef enum {
    WIRED_RENDER_NO     = 0, // Do not explicitly rasterize sides of triangles
    WIRED_RENDER_YES    = 1, // A wireframe rendering is done with the color specified at 'mesh_t.wired_color'
} wired_render_t;

typedef struct {
    transform_t transform;          // Indicates the scale, the rotation, and the translation of the mesh
    sphere_t bounding_volume;       // The spherical bounding volume containing all vertices and used in broad phase clipping
    const vec3f* vertices;
    const uint16_t* indices;
    const uint16_t vertex_number;
    const uint16_t index_number;
    
    union {
        const uint16_t fill_color; // used to paint the mesh when 'mesh_t.filled_render' == FILLED_RENDER_SINGLE_COLOR
        const uint16_t tex_index;  // used to paint the mesh when 'mesh_t.filled_render' == FILLED_RENDER_TEX_COORDS
    };
    const uint16_t wired_color;     // used to paint the wireframe structure of the mesh when 'mesh_t.wired_render' == WIRED_RENDER_YES

    const filled_render_t filled_render : 2;
    const wired_render_t wired_render   : 1;
} mesh_t;

void mesh_find_bounding_volume(mesh_t* mesh);

extern const vec2f    tex_coords[5];

extern const vec3f    triangle_vertices[3];
extern const uint16_t triangle_indices_single_color[3];
extern const uint16_t triangle_indices_colors[4];
extern const uint16_t triangle_indices_tex_coords[6];

extern const vec3f    square_vertices[4];
extern const uint16_t square_indices_single_color[6];
extern const uint16_t square_indices_colors[8];
extern const uint16_t square_indices_tex_coords[12];

extern const vec3f    cube_vertices[8];
extern const uint16_t cube_indices_single_color[36];
extern const uint16_t cube_indices_colors[48];
extern const uint16_t cube_indices_tex_coords[72];

extern const vec3f    triangle_pyramid_vertices[4];
extern const uint16_t triangle_pyramid_indices_single_color[12];
extern const uint16_t triangle_pyramid_indices_colors[16];
extern const uint16_t triangle_pyramid_indices_tex_coords[24];

extern const vec3f    square_pyramid_vertices[5];
extern const uint16_t square_pyramid_indices_single_color[18];
extern const uint16_t square_pyramid_indices_colors[24];
extern const uint16_t square_pyramid_indices_tex_coords[36];

extern const vec3f    dodecagon_pyramid_vertices[13];
extern const uint16_t dodecagon_pyramid_indices_single_color[66];
extern const uint16_t dodecagon_pyramid_indices_colors[88];
extern const uint16_t dodecagon_pyramid_indices_tex_coords[132];

extern const vec3f    icositetragon_pyramid_vertices[25];
extern const uint16_t icositetragon_pyramid_indices_single_color[138];
extern const uint16_t icositetragon_pyramid_indices_colors[184];
extern const uint16_t icositetragon_pyramid_indices_tex_coords[276];

extern const vec3f    icosahedron_vertices[12];
extern const uint16_t icosahedron_indices_single_color[60];
extern const uint16_t icosahedron_indices_colors[80];
extern const uint16_t icosahedron_indices_tex_coords[120];

extern const vec3f    dodacahedron_vertices[20];
extern const uint16_t dodacahedron_indices_single_color[108];
extern const uint16_t dodacahedron_indices_colors[144];
extern const uint16_t dodacahedron_indices_tex_coords[216];

extern const vec3f    truncated_icosahedron_vertices[60];
extern const uint16_t truncated_icosahedron_indices_single_color[348];
extern const uint16_t truncated_icosahedron_indices_colors[464];
extern const uint16_t truncated_icosahedron_indices_tex_coords[696];

extern const vec3f    rhombicubotahedron_vertices[24];
extern const uint16_t rhombicubotahedron_indices_single_color[132];
extern const uint16_t rhombicubotahedron_indices_colors[176];
extern const uint16_t rhombicubotahedron_indices_tex_coords[264];

extern const vec3f    spaceship_vertices[55];
extern const uint16_t spaceship_indices_single_color[318];

#endif // __MESH_H__
