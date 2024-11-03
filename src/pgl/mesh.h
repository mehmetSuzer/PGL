
#ifndef __MESH_H__
#define __MESH_H__

#include "transform.h"

typedef enum {
    FILLED_RENDER_NO           = 0, // Do not rasterize the inside of the triangle
    FILLED_RENDER_TEX_COORDS   = 1, // Each vertex of the triangle is coupled with a tex coord in 'indices' list: Index0, Tex0, Index1, Tex1, Index2, Tex2,
    FILLED_RENDER_COLORS       = 2, // Each face of the mesh is painted by the color specified in 'indices' list: Index0, Index1, Index2, Color,
    FILLED_RENDER_SINGLE_COLOR = 3, // The whole mesh is painted by the color specified at 'mesh_t.fill_color': Index0, Index1, Index2,
} filled_render_t;

typedef enum {
    WIRED_RENDER_NO     = 0, // Do not explicitly rasterize the sides of the triangle
    WIRED_RENDER_YES    = 1, // A wireframe rendering is done with the color specified at 'mesh_t.wired_color'
} wired_render_t;

typedef struct {
    transform_t transform;
    sphere_t bounding_volume;
    const vec3f* vertices;
    const u16* indices;
    const u16 vertex_number;
    const u16 index_number;
    
    union {
        const u16 fill_color; // used for painting the mesh when 'mesh_t.filled_render' == FILLED_RENDER_SINGLE_COLOR
        const u16 tex_index;  // used for painting the mesh when 'mesh_t.filled_render' == FILLED_RENDER_TEX_COORDS
    };
    const u16 wired_color;     // used for painting the wireframe structure of the mesh when 'mesh_t.wired_render' == WIRED_RENDER_YES

    const filled_render_t filled_render : 2;
    const wired_render_t wired_render   : 1;
} mesh_t;

void mesh_set_bounding_volume(mesh_t* mesh);

extern const vec2f  tex_coords[5];

extern const vec3f  triangle_vertices[3];
extern const u16    triangle_indices_single_color[3];
extern const u16    triangle_indices_colors[4];
extern const u16    triangle_indices_tex_coords[6];

extern const vec3f  square_vertices[4];
extern const u16    square_indices_single_color[6];
extern const u16    square_indices_colors[8];
extern const u16    square_indices_tex_coords[12];

extern const vec3f  cube_vertices[8];
extern const u16    cube_indices_single_color[36];
extern const u16    cube_indices_colors[48];
extern const u16    cube_indices_tex_coords[72];

extern const vec3f  triangle_pyramid_vertices[4];
extern const u16    triangle_pyramid_indices_single_color[12];
extern const u16    triangle_pyramid_indices_colors[16];
extern const u16    triangle_pyramid_indices_tex_coords[24];

extern const vec3f  square_pyramid_vertices[5];
extern const u16    square_pyramid_indices_single_color[18];
extern const u16    square_pyramid_indices_colors[24];
extern const u16    square_pyramid_indices_tex_coords[36];

extern const vec3f  dodecagon_pyramid_vertices[13];
extern const u16    dodecagon_pyramid_indices_single_color[66];
extern const u16    dodecagon_pyramid_indices_colors[88];
extern const u16    dodecagon_pyramid_indices_tex_coords[132];

extern const vec3f  icositetragon_pyramid_vertices[25];
extern const u16    icositetragon_pyramid_indices_single_color[138];
extern const u16    icositetragon_pyramid_indices_colors[184];
extern const u16    icositetragon_pyramid_indices_tex_coords[276];

extern const vec3f  icosahedron_vertices[12];
extern const u16    icosahedron_indices_single_color[60];
extern const u16    icosahedron_indices_colors[80];
extern const u16    icosahedron_indices_tex_coords[120];

extern const vec3f  dodacahedron_vertices[20];
extern const u16    dodacahedron_indices_single_color[108];
extern const u16    dodacahedron_indices_colors[144];
extern const u16    dodacahedron_indices_tex_coords[216];

extern const vec3f  truncated_icosahedron_vertices[60];
extern const u16    truncated_icosahedron_indices_single_color[348];
extern const u16    truncated_icosahedron_indices_colors[464];
extern const u16    truncated_icosahedron_indices_tex_coords[696];

extern const vec3f  rhombicubotahedron_vertices[24];
extern const u16    rhombicubotahedron_indices_single_color[132];
extern const u16    rhombicubotahedron_indices_colors[176];
extern const u16    rhombicubotahedron_indices_tex_coords[264];

extern const vec3f  spaceship_vertices[55];
extern const u16    spaceship_indices_single_color[318];

#endif // __MESH_H__
