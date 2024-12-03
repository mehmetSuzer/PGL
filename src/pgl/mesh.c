
#include "mesh.h"

void mesh_set_bounding_volume(mesh_t* mesh) {
	vec3f center = {0.0f, 0.0f, 0.0f};
	for (u32 i = 0; i < mesh->vertex_number; i++) {
		center = vec3f_add(center, mesh->vertices[i]);
	}
	center = vec3f_scale(center, 1.0f / mesh->vertex_number);

	f32 radius2 = 0.0f;
	for (u32 i = 0; i < mesh->vertex_number; i++) {
		const f32 distance2 = vec3f_mag2(vec3f_sub(center, mesh->vertices[i]));
		if (distance2 > radius2) {
			radius2 = distance2;
		}
	}
	mesh->bounding_volume = (sphere_t){center, sqrtf(radius2)};
}

// ------------------------------------------------------------------------------------------------------ //

const vec2f tex_coords[5] = {
	{0.0f, 			0.0f}, // Left   Bottom
	{1.0f, 			0.0f}, // Right  Bottom
	{1.0f, 			1.0f}, // Right  Top
	{0.0f, 			1.0f}, // Left   Top
	{0.5f, PGLM_SQRT3_2f}, // Middle Top
};

// ------------------------------------------------------------------------------------------------------ //

const vec3f triangle_vertices[3] = {
	{-0.5f, -PGLM_1_2SQRT3f,  0.0f}, // Left
	{ 0.5f, -PGLM_1_2SQRT3f,  0.0f}, // Right
	{ 0.0f,   PGLM_1_SQRT3f,  0.0f}, // Top
};

const u16 triangle_indices_single_color[3] = {
    0,  1,  2,
};

const u16 triangle_indices_colors[4] = {
	0,  1,  2,  PGLM_RGB565_BLUEVIOLET,
};

const u16 triangle_indices_tex_coords[6] = {
    0, 0,  1, 1,  2, 4,
};

// ------------------------------------------------------------------------------------------------------ //

const vec3f square_vertices[4] = {
	{-0.5f, -0.5f,  0.0f}, // Left  Bottom
	{ 0.5f, -0.5f,  0.0f}, // Right Bottom
	{ 0.5f,  0.5f,  0.0f}, // Right Top
	{-0.5f,  0.5f,  0.0f}, // Left  Top
};

const u16 square_indices_single_color[6] = {
    0,  1,  2,
	0,  2,  3,
};

const u16 square_indices_colors[8] = {
    0,  1,  2,  PGLM_RGB565_RED,
	0,  2,  3,  PGLM_RGB565_RED,
};

const u16 square_indices_tex_coords[12] = {
    0, 0,  1, 1,  2, 2,
	0, 0,  2, 2,  3, 3,
};

// ------------------------------------------------------------------------------------------------------ //

const vec3f cube_vertices[8] = {  				 
	{-0.5f, -0.5f,  0.5f}, // Left  Bottom Front
	{ 0.5f, -0.5f,  0.5f}, // Right Bottom Front
	{ 0.5f,  0.5f,  0.5f}, // Right Top    Front
	{-0.5f,  0.5f,  0.5f}, // Left  Top    Front
	{-0.5f, -0.5f, -0.5f}, // Left  Bottom Back
	{ 0.5f, -0.5f, -0.5f}, // Right Bottom Back
	{ 0.5f,  0.5f, -0.5f}, // Right Top    Back
	{-0.5f,  0.5f, -0.5f}, // Left  Top    Back
};

const u16 cube_indices_single_color[36] = {
	0,  1,  2, // Front  Face
	0,  2,  3, // Front  Face
	1,  5,  6, // Right  Face
	1,  6,  2, // Right  Face
	5,  4,  7, // Back   Face
	5,  7,  6, // Back   Face
	4,  0,  3, // Left   Face
	4,  3,  7, // Left   Face
	3,  2,  6, // Top    Face
	3,  6,  7, // Top    Face
	4,  5,  1, // Bottom Face
	4,  1,  0, // Bottom Face
};

const u16 cube_indices_colors[48] = {
	0,  1,  2,  PGLM_RGB565_ALICEBLUE,	// Front  Face
	0,  2,  3,  PGLM_RGB565_ALICEBLUE,  // Front  Face
	1,  5,  6,  PGLM_RGB565_CORAL,      // Right  Face
	1,  6,  2,  PGLM_RGB565_CORAL,    	// Right  Face
	5,  4,  7,  PGLM_RGB565_LIME,       // Back   Face
	5,  7,  6,  PGLM_RGB565_LIME,		// Back   Face
	4,  0,  3,  PGLM_RGB565_GOLDENROD,  // Left   Face
	4,  3,  7,  PGLM_RGB565_GOLDENROD,  // Left   Face
	3,  2,  6,  PGLM_RGB565_IVORY,      // Top    Face
	3,  6,  7,  PGLM_RGB565_IVORY,  	// Top    Face
	4,  5,  1,  PGLM_RGB565_KHAKI,      // Bottom Face
	4,  1,  0,  PGLM_RGB565_KHAKI 		// Bottom Face
};

const u16 cube_indices_tex_coords[72] = {
	0, 0,  1, 1,  2, 2, // Front  Face
	0, 0,  2, 2,  3, 3, // Front  Face
	1, 0,  5, 1,  6, 2, // Right  Face
	1, 0,  6, 2,  2, 3, // Right  Face
	5, 0,  4, 1,  7, 2, // Back   Face
	5, 0,  7, 2,  6, 3, // Back   Face
	4, 0,  0, 1,  3, 2, // Left   Face
	4, 0,  3, 2,  7, 3, // Left   Face
	3, 0,  2, 1,  6, 2, // Top    Face
	3, 0,  6, 2,  7, 3, // Top    Face
	4, 0,  5, 1,  1, 2, // Bottom Face
	4, 0,  1, 2,  0, 3, // Bottom Face
};

// ------------------------------------------------------------------------------------------------------ //

const vec3f triangle_pyramid_vertices[4] = {
	{-0.5f, 					0.0f, PGLM_1_2SQRT3f}, // Left   Bottom Back
	{ 0.5f, 					0.0f, PGLM_1_2SQRT3f}, // Right  Bottom Front
	{ 0.0f, 					0.0f, -PGLM_1_SQRT3f}, // Middle Bottom Back
	{ 0.0f, PGLM_SQRT2f/PGLM_SQRT3f,   		    0.0f}, // Top
};

const u16 triangle_pyramid_indices_single_color[12] = {
	0,  1,  3, // Front  Face
	1,  2,  3, // Right  Face
	2,  0,  3, // Left   Face
	0,  2,  1, // Bottom Face
};

const u16 triangle_pyramid_indices_colors[16] = {
	0,  1,  3,  PGLM_RGB565_AQUA,   // Front  Face
	1,  2,  3,  PGLM_RGB565_YELLOW, // Right  Face
	2,  0,  3,  PGLM_RGB565_AZURE,  // Left   Face
	0,  2,  1,  PGLM_RGB565_OLIVE,  // Bottom Face
};

const u16 triangle_pyramid_indices_tex_coords[24] = {
	0, 0,  1, 1,  3, 4, // Front  Face
	1, 0,  2, 1,  3, 4, // Right  Face
	2, 0,  0, 1,  3, 4, // Left   Face
	0, 0,  2, 1,  1, 4, // Bottom Face
};

// ------------------------------------------------------------------------------------------------------ //

const vec3f square_pyramid_vertices[5] = {
	{-0.5f, 		 0.0f,  0.5f}, // Left  Bottom Front
	{ 0.5f, 		 0.0f,  0.5f}, // Right Bottom Front
	{ 0.5f, 		 0.0f, -0.5f}, // Right Bottom Back
	{-0.5f, 		 0.0f, -0.5f}, // Left  Bottom Back
	{ 0.0f, PGLM_1_SQRT2f,  0.0f}, // Top
};

const u16 square_pyramid_indices_single_color[18] = {
	0,  1,  4, // Front  Face
	1,  2,  4, // Right  Face
	2,  3,  4, // Back   Face
	3,  0,  4, // Left   Face
	3,  2,  1, // Bottom Face 
	3,  1,  0, // Bottom Face
};

const u16 square_pyramid_indices_colors[24] = {
	0,  1,  4,  PGLM_RGB565_CHOCOLATE, // Front  Face
	1,  2,  4,  PGLM_RGB565_HONEYDEW,  // Right  Face
	2,  3,  4,  PGLM_RGB565_DARKKHAKI, // Back   Face
	3,  0,  4,  PGLM_RGB565_IVORY,     // Left   Face
	3,  2,  1,  PGLM_RGB565_PEACHPUFF, // Bottom Face 
	3,  1,  0,  PGLM_RGB565_PEACHPUFF, // Bottom Face
};

const u16 square_pyramid_indices_tex_coords[36] = {
	0, 0,  1, 1,  4, 4, // Front  Face
	1, 0,  2, 1,  4, 4, // Right  Face
	2, 0,  3, 1,  4, 4, // Back   Face
	3, 0,  0, 1,  4, 4, // Left   Face
	3, 0,  2, 1,  1, 2, // Bottom Face 
	3, 0,  1, 2,  0, 3, // Bottom Face
};

// ------------------------------------------------------------------------------------------------------ //

const vec3f dodecagon_pyramid_vertices[13] = {
	{ 1.000000f,  0.000000f,  0.000000f},
	{ 0.866025f,  0.000000f,  0.500000f},
	{ 0.500000f,  0.000000f,  0.866025f},
	{ 0.000000f,  0.000000f,  1.000000f},
	{-0.500000f,  0.000000f,  0.866025f},
	{-0.866025f,  0.000000f,  0.500000f},
	{-1.000000f,  0.000000f,  0.000000f},
	{-0.866025f,  0.000000f, -0.500000f},
	{-0.500000f,  0.000000f, -0.866025f},
	{ 0.000000f,  0.000000f, -1.000000f},
	{ 0.500000f,  0.000000f, -0.866025f},
	{ 0.866025f,  0.000000f, -0.500000f},
	{ 0.000000f,  1.500000f,  0.000000f}, // Top
};

const u16 dodecagon_pyramid_indices_single_color[66] = {
	 1,   0,  12, // Side Face
	 2,   1,  12, // Side Face
	 3,   2,  12, // Side Face
	 4,   3,  12, // Side Face
	 5,   4,  12, // Side Face
	 6,   5,  12, // Side Face
	 7,   6,  12, // Side Face
	 8,   7,  12, // Side Face
	 9,   8,  12, // Side Face
	10,   9,  12, // Side Face
	11,  10,  12, // Side Face
	 0,  11,  12, // Side Face
	 0,   1,   2, // Bottom Face
	 0,   2,   3, // Bottom Face
	 0,   3,   4, // Bottom Face
	 0,   4,   5, // Bottom Face
	 0,   5,   6, // Bottom Face
	 0,   6,   7, // Bottom Face
	 0,   7,   8, // Bottom Face
	 0,   8,   9, // Bottom Face
	 0,   9,  10, // Bottom Face
	 0,  10,  11, // Bottom Face
};

const u16 dodecagon_pyramid_indices_colors[88] = {
	 1,   0,  12,  PGLM_RGB565_NAVAJOWHITE,   // Side Face
	 2,   1,  12,  PGLM_RGB565_FUCHSIA,       // Side Face
	 3,   2,  12,  PGLM_RGB565_DARKORCHID,    // Side Face
	 4,   3,  12,  PGLM_RGB565_MEDIUMORCHID,  // Side Face
	 5,   4,  12,  PGLM_RGB565_AQUAMARINE,    // Side Face
	 6,   5,  12,  PGLM_RGB565_ALICEBLUE,     // Side Face
	 7,   6,  12,  PGLM_RGB565_BROWN,         // Side Face
	 8,   7,  12,  PGLM_RGB565_CORAL,         // Side Face
	 9,   8,  12,  PGLM_RGB565_GHOSTWHITE,    // Side Face
	10,   9,  12,  PGLM_RGB565_SEAGREEN,      // Side Face
	11,  10,  12,  PGLM_RGB565_DARKBLUE,      // Side Face
	 0,  11,  12,  PGLM_RGB565_YELLOWGREEN,   // Side Face
	 0,   1,   2,  PGLM_RGB565_LAVENDERBLUSH, // Bottom Face
	 0,   2,   3,  PGLM_RGB565_LAVENDERBLUSH, // Bottom Face
	 0,   3,   4,  PGLM_RGB565_LAVENDERBLUSH, // Bottom Face
	 0,   4,   5,  PGLM_RGB565_LAVENDERBLUSH, // Bottom Face
	 0,   5,   6,  PGLM_RGB565_LAVENDERBLUSH, // Bottom Face
	 0,   6,   7,  PGLM_RGB565_LAVENDERBLUSH, // Bottom Face
	 0,   7,   8,  PGLM_RGB565_LAVENDERBLUSH, // Bottom Face
	 0,   8,   9,  PGLM_RGB565_LAVENDERBLUSH, // Bottom Face
	 0,   9,  10,  PGLM_RGB565_LAVENDERBLUSH, // Bottom Face
	 0,  10,  11,  PGLM_RGB565_LAVENDERBLUSH, // Bottom Face
};

const u16 dodecagon_pyramid_indices_tex_coords[132] = {
	 1, 0,   0, 0,  12, 0, // Side Face
	 2, 0,   1, 0,  12, 0, // Side Face
	 3, 0,   2, 0,  12, 0, // Side Face
	 4, 0,   3, 0,  12, 0, // Side Face
	 5, 0,   4, 0,  12, 0, // Side Face
	 6, 0,   5, 0,  12, 0, // Side Face
	 7, 0,   6, 0,  12, 0, // Side Face
	 8, 0,   7, 0,  12, 0, // Side Face
	 9, 0,   8, 0,  12, 0, // Side Face
	10, 0,   9, 0,  12, 0, // Side Face
	11, 0,  10, 0,  12, 0, // Side Face
	 0, 0,  11, 0,  12, 0, // Side Face
	 0, 0,   1, 0,   2, 0, // Bottom Face
	 0, 0,   2, 0,   3, 0, // Bottom Face
	 0, 0,   3, 0,   4, 0, // Bottom Face
	 0, 0,   4, 0,   5, 0, // Bottom Face
	 0, 0,   5, 0,   6, 0, // Bottom Face
	 0, 0,   6, 0,   7, 0, // Bottom Face
	 0, 0,   7, 0,   8, 0, // Bottom Face
	 0, 0,   8, 0,   9, 0, // Bottom Face
	 0, 0,   9, 0,  10, 0, // Bottom Face
	 0, 0,  10, 0,  11, 0, // Bottom Face
};

// ------------------------------------------------------------------------------------------------------ //

const vec3f icositetragon_pyramid_vertices[25] = {
	{ 1.000000f, 0.000000f,  0.000000f},
	{ 0.965926f, 0.000000f,  0.258819f},
	{ 0.866025f, 0.000000f,  0.500000f},
	{ 0.707107f, 0.000000f,  0.707107f},
	{ 0.500000f, 0.000000f,  0.866025f},
	{ 0.258819f, 0.000000f,  0.965926f},
	{ 0.000000f, 0.000000f,  1.000000f},
	{-0.258819f, 0.000000f,  0.965926f},
	{-0.500000f, 0.000000f,  0.866025f},
	{-0.707107f, 0.000000f,  0.707107f},
	{-0.866025f, 0.000000f,  0.500000f},
	{-0.965926f, 0.000000f,  0.258819f},
	{-1.000000f, 0.000000f,  0.000000f},
	{-0.965926f, 0.000000f, -0.258819f},
	{-0.866025f, 0.000000f, -0.500000f},
	{-0.707107f, 0.000000f, -0.707107f},
	{-0.500000f, 0.000000f, -0.866025f},
	{-0.258819f, 0.000000f, -0.965926f},
	{ 0.000000f, 0.000000f, -1.000000f},
	{ 0.258819f, 0.000000f, -0.965926f},
	{ 0.500000f, 0.000000f, -0.866025f},
	{ 0.707107f, 0.000000f, -0.707107f},
	{ 0.866025f, 0.000000f, -0.500000f},
	{ 0.965926f, 0.000000f, -0.258819f},
	{ 0.000000f, 1.500000f,  0.000000f}, // Top
};

const u16 icositetragon_pyramid_indices_single_color[138] = {
	 1,   0,  24, // Side Face 
	 2,   1,  24, // Side Face
	 3,   2,  24, // Side Face
	 4,   3,  24, // Side Face
	 5,   4,  24, // Side Face
	 6,   5,  24, // Side Face
	 7,   6,  24, // Side Face
	 8,   7,  24, // Side Face
	 9,   8,  24, // Side Face
	10,   9,  24, // Side Face
	11,  10,  24, // Side Face
	12,  11,  24, // Side Face
	13,  12,  24, // Side Face
	14,  13,  24, // Side Face
	15,  14,  24, // Side Face
	16,  15,  24, // Side Face
	17,  16,  24, // Side Face
	18,  17,  24, // Side Face
	19,  18,  24, // Side Face
	20,  19,  24, // Side Face
	21,  20,  24, // Side Face
	22,  21,  24, // Side Face
	23,  22,  24, // Side Face
	 0,  23,  24, // Side Face
	 0,   1,   2, // Bottom Face
	 0,   2,   3, // Bottom Face
	 0,   3,   4, // Bottom Face
	 0,   4,   5, // Bottom Face
	 0,   5,   6, // Bottom Face
	 0,   6,   7, // Bottom Face
	 0,   7,   8, // Bottom Face
	 0,   8,   9, // Bottom Face
	 0,   9,  10, // Bottom Face
	 0,  10,  11, // Bottom Face
	 0,  11,  12, // Bottom Face
	 0,  12,  13, // Bottom Face
	 0,  13,  14, // Bottom Face
	 0,  14,  15, // Bottom Face
	 0,  15,  16, // Bottom Face
	 0,  16,  17, // Bottom Face
	 0,  17,  18, // Bottom Face
	 0,  18,  19, // Bottom Face
	 0,  19,  20, // Bottom Face
	 0,  20,  21, // Bottom Face
	 0,  21,  22, // Bottom Face
	 0,  22,  23, // Bottom Face
};

const u16 icositetragon_pyramid_indices_colors[184] = {
	 1,   0,  24,  PGLM_RGB565_ALICEBLUE,		// Side Face 
	 2,   1,  24,  PGLM_RGB565_AQUA, 			// Side Face
	 3,   2,  24,  PGLM_RGB565_BEIGE, 			// Side Face
	 4,   3,  24,  PGLM_RGB565_DARKCYAN, 		// Side Face
	 5,   4,  24,  PGLM_RGB565_YELLOW, 			// Side Face
	 6,   5,  24,  PGLM_RGB565_KHAKI, 			// Side Face
	 7,   6,  24,  PGLM_RGB565_BLANCHEDALMOND,	// Side Face
	 8,   7,  24,  PGLM_RGB565_PALEGREEN, 		// Side Face
	 9,   8,  24,  PGLM_RGB565_TURQUOISE, 		// Side Face
	10,   9,  24,  PGLM_RGB565_WHITE, 			// Side Face
	11,  10,  24,  PGLM_RGB565_ROSYBROWN, 		// Side Face
	12,  11,  24,  PGLM_RGB565_PERU, 			// Side Face
	13,  12,  24,  PGLM_RGB565_FORESTGREEN, 	// Side Face
	14,  13,  24,  PGLM_RGB565_OLDLACE, 		// Side Face
	15,  14,  24,  PGLM_RGB565_LAVENDER, 		// Side Face
	16,  15,  24,  PGLM_RGB565_MAGENTA, 		// Side Face
	17,  16,  24,  PGLM_RGB565_BISQUE, 			// Side Face
	18,  17,  24,  PGLM_RGB565_HONEYDEW, 		// Side Face
	19,  18,  24,  PGLM_RGB565_BLUE, 			// Side Face
	20,  19,  24,  PGLM_RGB565_VIOLET, 			// Side Face
	21,  20,  24,  PGLM_RGB565_INDIGO, 			// Side Face
	22,  21,  24,  PGLM_RGB565_THISTLE, 		// Side Face
	23,  22,  24,  PGLM_RGB565_FIREBRICK, 		// Side Face
	 0,  23,  24,  PGLM_RGB565_LIME, 			// Side Face
	 0,   1,   2,  PGLM_RGB565_GOLD, 			// Bottom Face
	 0,   2,   3,  PGLM_RGB565_GOLD, 			// Bottom Face
	 0,   3,   4,  PGLM_RGB565_GOLD, 			// Bottom Face
	 0,   4,   5,  PGLM_RGB565_GOLD, 			// Bottom Face
	 0,   5,   6,  PGLM_RGB565_GOLD, 			// Bottom Face
	 0,   6,   7,  PGLM_RGB565_GOLD, 			// Bottom Face
	 0,   7,   8,  PGLM_RGB565_GOLD, 			// Bottom Face
	 0,   8,   9,  PGLM_RGB565_GOLD, 			// Bottom Face
	 0,   9,  10,  PGLM_RGB565_GOLD, 			// Bottom Face
	 0,  10,  11,  PGLM_RGB565_GOLD, 			// Bottom Face
	 0,  11,  12,  PGLM_RGB565_GOLD, 			// Bottom Face
	 0,  12,  13,  PGLM_RGB565_GOLD, 			// Bottom Face
	 0,  13,  14,  PGLM_RGB565_GOLD, 			// Bottom Face
	 0,  14,  15,  PGLM_RGB565_GOLD, 			// Bottom Face
	 0,  15,  16,  PGLM_RGB565_GOLD, 			// Bottom Face
	 0,  16,  17,  PGLM_RGB565_GOLD, 			// Bottom Face
	 0,  17,  18,  PGLM_RGB565_GOLD, 			// Bottom Face
	 0,  18,  19,  PGLM_RGB565_GOLD, 			// Bottom Face
	 0,  19,  20,  PGLM_RGB565_GOLD, 			// Bottom Face
	 0,  20,  21,  PGLM_RGB565_GOLD, 			// Bottom Face
	 0,  21,  22,  PGLM_RGB565_GOLD, 			// Bottom Face
	 0,  22,  23,  PGLM_RGB565_GOLD, 			// Bottom Face
};

const u16 icositetragon_pyramid_indices_tex_coords[276] = {
	 1, 0,   0, 0,  24, 0, // Side Face 
	 2, 0,   1, 0,  24, 0, // Side Face
	 3, 0,   2, 0,  24, 0, // Side Face
	 4, 0,   3, 0,  24, 0, // Side Face
	 5, 0,   4, 0,  24, 0, // Side Face
	 6, 0,   5, 0,  24, 0, // Side Face
	 7, 0,   6, 0,  24, 0, // Side Face
	 8, 0,   7, 0,  24, 0, // Side Face
	 9, 0,   8, 0,  24, 0, // Side Face
	10, 0,   9, 0,  24, 0, // Side Face
	11, 0,  10, 0,  24, 0, // Side Face
	12, 0,  11, 0,  24, 0, // Side Face
	13, 0,  12, 0,  24, 0, // Side Face
	14, 0,  13, 0,  24, 0, // Side Face
	15, 0,  14, 0,  24, 0, // Side Face
	16, 0,  15, 0,  24, 0, // Side Face
	17, 0,  16, 0,  24, 0, // Side Face
	18, 0,  17, 0,  24, 0, // Side Face
	19, 0,  18, 0,  24, 0, // Side Face
	20, 0,  19, 0,  24, 0, // Side Face
	21, 0,  20, 0,  24, 0, // Side Face
	22, 0,  21, 0,  24, 0, // Side Face
	23, 0,  22, 0,  24, 0, // Side Face
	 0, 0,  23, 0,  24, 0, // Side Face
	 0, 0,   1, 0,   2, 0, // Bottom Face
	 0, 0,   2, 0,   3, 0, // Bottom Face
	 0, 0,   3, 0,   4, 0, // Bottom Face
	 0, 0,   4, 0,   5, 0, // Bottom Face
	 0, 0,   5, 0,   6, 0, // Bottom Face
	 0, 0,   6, 0,   7, 0, // Bottom Face
	 0, 0,   7, 0,   8, 0, // Bottom Face
	 0, 0,   8, 0,   9, 0, // Bottom Face
	 0, 0,   9, 0,  10, 0, // Bottom Face
	 0, 0,  10, 0,  11, 0, // Bottom Face
	 0, 0,  11, 0,  12, 0, // Bottom Face
	 0, 0,  12, 0,  13, 0, // Bottom Face
	 0, 0,  13, 0,  14, 0, // Bottom Face
	 0, 0,  14, 0,  15, 0, // Bottom Face
	 0, 0,  15, 0,  16, 0, // Bottom Face
	 0, 0,  16, 0,  17, 0, // Bottom Face
	 0, 0,  17, 0,  18, 0, // Bottom Face
	 0, 0,  18, 0,  19, 0, // Bottom Face
	 0, 0,  19, 0,  20, 0, // Bottom Face
	 0, 0,  20, 0,  21, 0, // Bottom Face
	 0, 0,  21, 0,  22, 0, // Bottom Face
	 0, 0,  22, 0,  23, 0, // Bottom Face
};

// ------------------------------------------------------------------------------------------------------ //

// "Truncated Icosahedron" (https://skfb.ly/LOzY) 
// by Vladimir E. is licensed under 
// Creative Commons Attribution (http://creativecommons.org/licenses/by/4.0/).

const vec3f icosahedron_vertices[12] = {
	{ 0.579431f,  -0.294227f,  1.511052f},
	{ 1.040907f,   0.378462f,  0.934872f},
	{ 0.272957f,   0.656274f,  1.511125f},
	{ 0.839055f,  -0.513793f,  0.562169f},
	{ 0.699876f,   0.269474f,  0.000000f},
	{ 0.348726f,   1.006035f,  0.577492f},
	{-0.399287f,  -0.084705f,  1.511052f},
	{-0.045488f,  -0.819658f,  0.934872f},
	{ 0.029431f,  -0.471639f,  0.000000f},
	{-0.276418f,   0.479434f,  0.000000f},
	{-0.541473f,   0.720055f,  0.934872f},
	{-0.738577f,  -0.193068f,  0.577213f},
};

const u16 icosahedron_indices_single_color[60] = {
	 0,   1,   2,
	 3,   1,   0,
	 4,   1,   3,
	 4,   5,   1,
	 5,   2,   1,
	 0,   2,   6,
	 7,   0,   6,
	 0,   7,   3,
	 3,   7,   8,
	 3,   8,   4,
	 4,   8,   9,
	 5,   4,   9,
	10,   5,   9,
	10,   2,   5,
	10,   6,   2,
	10,  11,   6,
	11,   7,   6,
	 7,  11,   8,
	 9,   8,  11,
	 9,  11,  10,
};

const u16 icosahedron_indices_colors[80] = {
	 0,   1,   2,  PGLM_RGB565_CRIMSON,
	 3,   1,   0,  PGLM_RGB565_ANTIQUEWHITE,
	 4,   1,   3,  PGLM_RGB565_BISQUE,
	 4,   5,   1,  PGLM_RGB565_FUCHSIA,
	 5,   2,   1,  PGLM_RGB565_PALEVIOLETRED,
	 0,   2,   6,  PGLM_RGB565_OLDLACE,
	 7,   0,   6,  PGLM_RGB565_FORESTGREEN,
	 0,   7,   3,  PGLM_RGB565_ROYALBLUE,
	 3,   7,   8,  PGLM_RGB565_LEMONCHIFFON,
	 3,   8,   4,  PGLM_RGB565_YELLOW,
	 4,   8,   9,  PGLM_RGB565_SADDLEBROWN,
	 5,   4,   9,  PGLM_RGB565_GOLD,
	10,   5,   9,  PGLM_RGB565_ORANGE,
	10,   2,   5,  PGLM_RGB565_DARKGREEN,
	10,   6,   2,  PGLM_RGB565_HONEYDEW,
	10,  11,   6,  PGLM_RGB565_HOTPINK,
	11,   7,   6,  PGLM_RGB565_ORCHID,
	 7,  11,   8,  PGLM_RGB565_LEMONCHIFFON,
	 9,   8,  11,  PGLM_RGB565_NAVAJOWHITE,
	 9,  11,  10,  PGLM_RGB565_LINEN,
};

const u16 icosahedron_indices_tex_coords[120] = {
	 0, 0,   1, 1,   2, 4,
	 3, 0,   1, 1,   0, 4,
	 4, 0,   1, 1,   3, 4,
	 4, 0,   5, 1,   1, 4,
	 5, 0,   2, 1,   1, 4,
	 0, 0,   2, 1,   6, 4,
	 7, 0,   0, 1,   6, 4,
	 0, 0,   7, 1,   3, 4,
	 3, 0,   7, 1,   8, 4,
	 3, 0,   8, 1,   4, 4,
	 4, 0,   8, 1,   9, 4,
	 5, 0,   4, 1,   9, 4,
	10, 0,   5, 1,   9, 4,
	10, 0,   2, 1,   5, 4,
	10, 0,   6, 1,   2, 4,
	10, 0,  11, 1,   6, 4,
	11, 0,   7, 1,   6, 4,
	 7, 0,  11, 1,   8, 4,
	 9, 0,   8, 1,  11, 4,
	 9, 0,  11, 1,  10, 4,
};

// ------------------------------------------------------------------------------------------------------ //

// "Truncated Icosahedron" (https://skfb.ly/LOzY) 
// by Vladimir E. is licensed under 
// Creative Commons Attribution (http://creativecommons.org/licenses/by/4.0/).

const vec3f dodacahedron_vertices[20] = {
	{ 0.000000f,  0.499996f, -0.654510f},
	{ 0.000000f,  0.809017f, -0.154510f},
	{ 0.475531f,  0.654507f,  0.154507f},
	{ 0.769421f,  0.250001f, -0.154510f},
	{ 0.475531f,  0.154509f, -0.654510f},
	{-0.475530f,  0.154509f, -0.654510f},
	{-0.769420f,  0.250001f, -0.154510f},
	{-0.475530f,  0.654507f,  0.154507f},
	{-0.293890f, -0.404508f, -0.654510f},
	{-0.475530f, -0.654508f, -0.154510f},
	{-0.769420f, -0.249999f,  0.154507f},
	{ 0.293890f, -0.404508f, -0.654510f},
	{ 0.475531f, -0.654508f, -0.154510f},
	{ 0.000000f, -0.809016f,  0.154507f},
	{ 0.769421f, -0.249999f,  0.154507f},
	{ 0.293890f,  0.404507f,  0.654510f},
	{ 0.475531f, -0.154508f,  0.654510f},
	{-0.293890f,  0.404507f,  0.654510f},
	{-0.475530f, -0.154508f,  0.654510f},
	{ 0.000000f, -0.499999f,  0.654510f},
};

const u16 dodacahedron_indices_single_color[108] = {
	 0,   1,   2, // 0, 1, 2, 3, 4,
	 0,   2,   3, 
	 0,   3,   4, 
	 5,   6,   7, // 5, 6, 7, 1, 0,
	 5,   7,   1, 
	 5,   1,   0, 
	 8,   9,  10, // 8, 9, 10, 6, 5,
	 8,  10,   6, 
	 8,   6,   5, 
	11,  12,  13, // 11, 12, 13, 9, 8,
	11,  13,   9, 
	11,   9,   8, 
	 4,   3,  14, // 4, 3, 14, 12, 11,
	 4,  14,  12, 
	 4,  12,  11, 
	 2,  15,  16, // 2, 15, 16, 14, 3,
	 2,  16,  14, 
	 2,  14,   3, 
	 7,  17,  15, // 7, 17, 15, 2, 1,
	 7,  15,   2, 
	 7,   2,   1, 
	10,  18,  17, // 10, 18, 17, 7, 6,
	10,  17,   7, 
	10,   7,   6, 
	13,  19,  18, // 13, 19, 18, 10, 9,
	13,  18,  10, 
	13,  10,   9, 
	14,  16,  19, // 14, 16, 19, 13, 12,
	14,  19,  13, 
	14,  13,  12, 
	17,  18,  19, // 17, 18, 19, 16, 15,
	17,  19,  16, 
	17,  16,  15, 
	 8,   5,   0, // 8, 5, 0, 4, 11,
	 8,   0,   4, 
	 8,   4,  11, 
};

const u16 dodacahedron_indices_colors[144] = {
	 0,   1,   2,  PGLM_RGB565_FUCHSIA, // 0, 1, 2, 3, 4,
	 0,   2,   3,  PGLM_RGB565_FUCHSIA, 
	 0,   3,   4,  PGLM_RGB565_FUCHSIA, 
	 5,   6,   7,  PGLM_RGB565_FUCHSIA, // 5, 6, 7, 1, 0,
	 5,   7,   1,  PGLM_RGB565_FUCHSIA, 
	 5,   1,   0,  PGLM_RGB565_FUCHSIA, 
	 8,   9,  10,  PGLM_RGB565_FUCHSIA, // 8, 9, 10, 6, 5,
	 8,  10,   6,  PGLM_RGB565_FUCHSIA, 
	 8,   6,   5,  PGLM_RGB565_FUCHSIA, 
	11,  12,  13,  PGLM_RGB565_FUCHSIA, // 11, 12, 13, 9, 8,
	11,  13,   9,  PGLM_RGB565_FUCHSIA, 
	11,   9,   8,  PGLM_RGB565_FUCHSIA, 
	 4,   3,  14,  PGLM_RGB565_FUCHSIA, // 4, 3, 14, 12, 11,
	 4,  14,  12,  PGLM_RGB565_FUCHSIA, 
	 4,  12,  11,  PGLM_RGB565_FUCHSIA, 
	 2,  15,  16,  PGLM_RGB565_FUCHSIA, // 2, 15, 16, 14, 3,
	 2,  16,  14,  PGLM_RGB565_FUCHSIA, 
	 2,  14,   3,  PGLM_RGB565_FUCHSIA, 
	 7,  17,  15,  PGLM_RGB565_FUCHSIA, // 7, 17, 15, 2, 1,
	 7,  15,   2,  PGLM_RGB565_FUCHSIA, 
	 7,   2,   1,  PGLM_RGB565_FUCHSIA, 
	10,  18,  17,  PGLM_RGB565_FUCHSIA, // 10, 18, 17, 7, 6,
	10,  17,   7,  PGLM_RGB565_FUCHSIA, 
	10,   7,   6,  PGLM_RGB565_FUCHSIA, 
	13,  19,  18,  PGLM_RGB565_FUCHSIA, // 13, 19, 18, 10, 9,
	13,  18,  10,  PGLM_RGB565_FUCHSIA, 
	13,  10,   9,  PGLM_RGB565_FUCHSIA, 
	14,  16,  19,  PGLM_RGB565_FUCHSIA, // 14, 16, 19, 13, 12,
	14,  19,  13,  PGLM_RGB565_FUCHSIA, 
	14,  13,  12,  PGLM_RGB565_FUCHSIA, 
	17,  18,  19,  PGLM_RGB565_FUCHSIA, // 17, 18, 19, 16, 15,
	17,  19,  16,  PGLM_RGB565_FUCHSIA, 
	17,  16,  15,  PGLM_RGB565_FUCHSIA, 
	 8,   5,   0,  PGLM_RGB565_FUCHSIA, // 8, 5, 0, 4, 11,
	 8,   0,   4,  PGLM_RGB565_FUCHSIA, 
	 8,   4,  11,  PGLM_RGB565_FUCHSIA, 
};

const u16 dodacahedron_indices_tex_coords[216] = {
	 0, 0,   1, 0,   2, 0, // 0, 1, 2, 3, 4,
	 0, 0,   2, 0,   3, 0, 
	 0, 0,   3, 0,   4, 0, 
	 5, 0,   6, 0,   7, 0, // 5, 6, 7, 1, 0,
	 5, 0,   7, 0,   1, 0, 
	 5, 0,   1, 0,   0, 0, 
	 8, 0,   9, 0,  10, 0, // 8, 9, 10, 6, 5,
	 8, 0,  10, 0,   6, 0, 
	 8, 0,   6, 0,   5, 0, 
	11, 0,  12, 0,  13, 0, // 11, 12, 13, 9, 8,
	11, 0,  13, 0,   9, 0, 
	11, 0,   9, 0,   8, 0, 
	 4, 0,   3, 0,  14, 0, // 4, 3, 14, 12, 11,
	 4, 0,  14, 0,  12, 0, 
	 4, 0,  12, 0,  11, 0, 
	 2, 0,  15, 0,  16, 0, // 2, 15, 16, 14, 3,
	 2, 0,  16, 0,  14, 0, 
	 2, 0,  14, 0,   3, 0, 
	 7, 0,  17, 0,  15, 0, // 7, 17, 15, 2, 1,
	 7, 0,  15, 0,   2, 0, 
	 7, 0,   2, 0,   1, 0, 
	10, 0,  18, 0,  17, 0, // 10, 18, 17, 7, 6,
	10, 0,  17, 0,   7, 0, 
	10, 0,   7, 0,   6, 0, 
	13, 0,  19, 0,  18, 0, // 13, 19, 18, 10, 9,
	13, 0,  18, 0,  10, 0, 
	13, 0,  10, 0,   9, 0, 
	14, 0,  16, 0,  19, 0, // 14, 16, 19, 13, 12,
	14, 0,  19, 0,  13, 0, 
	14, 0,  13, 0,  12, 0, 
	17, 0,  18, 0,  19, 0, // 17, 18, 19, 16, 15,
	17, 0,  19, 0,  16, 0, 
	17, 0,  16, 0,  15, 0, 
	 8, 0,   5, 0,   0, 0, // 8, 5, 0, 4, 11,
	 8, 0,   0, 0,   4, 0, 
	 8, 0,   4, 0,  11, 0, 
};

// ------------------------------------------------------------------------------------------------------ //

// "Truncated Icosahedron" (https://skfb.ly/LOzY) 
// by Vladimir E. is licensed under 
// Creative Commons Attribution (http://creativecommons.org/licenses/by/4.0/).

const vec3f truncated_icosahedron_vertices[60] = {
	{-0.999999f,  2.265064f,  0.000000f},
	{-0.499999f,  2.265064f,  0.866025f},
	{ 0.499999f,  2.265064f,  0.866025f},
	{ 1.000000f,  2.265064f,  0.000000f},
	{ 0.499999f,  2.265064f, -0.866025f},
	{-0.499999f,  2.265064f, -0.866025f},
	{-1.804193f,  1.688757f, -0.110026f},
	{-2.115514f,  1.113953f,  0.645170f},
	{-1.616233f,  1.113953f,  1.511169f},
	{-0.807090f,  1.688757f,  1.619712f},
	{-1.000000f,  1.688757f, -1.507400f},
	{-1.804193f,  1.333907f, -1.043400f},
	{-0.499999f,  1.113954f, -2.150076f},
	{ 1.000000f,  1.688757f, -1.507400f},
	{ 0.499999f,  1.113954f, -2.150076f},
	{-0.807090f,  0.178885f, -2.326523f},
	{-1.616233f, -0.178885f, -1.868255f},
	{-2.115514f,  0.398028f, -1.220962f},
	{-0.000000f, -0.398023f, -2.442351f},
	{-0.000000f, -1.333907f, -2.088525f},
	{-0.807090f, -1.688744f, -1.620493f},
	{-1.616233f, -1.113953f, -1.507400f},
	{ 0.807090f,  0.178885f, -2.326523f},
	{ 1.616233f, -0.178885f, -1.868255f},
	{ 1.616233f, -1.113953f, -1.507400f},
	{ 0.807090f, -1.688744f, -1.620493f},
	{ 1.804193f,  1.333907f, -1.043400f},
	{ 2.115514f,  0.398028f, -1.220962f},
	{ 1.804193f,  1.688757f, -0.110026f},
	{ 2.115514f,  1.113953f,  0.645170f},
	{ 2.425423f,  0.178885f,  0.466387f},
	{ 2.425423f, -0.178885f, -0.466387f},
	{ 0.807090f,  1.688757f,  1.620493f},
	{ 1.616233f,  1.113953f,  1.511169f},
	{-0.000000f,  1.327845f,  2.089476f},
	{-1.616233f,  0.178885f,  1.867856f},
	{-0.807090f, -0.178885f,  2.324709f},
	{-0.000000f,  0.398029f,  2.442960f},
	{-2.425423f,  0.178885f,  0.466387f},
	{-2.115514f, -0.398022f,  1.222988f},
	{-2.425423f, -0.178885f, -0.466387f},
	{-2.115514f, -1.113953f, -0.645166f},
	{-1.804193f, -1.688744f,  0.110038f},
	{-1.804193f, -1.333907f,  1.043400f},
	{-0.499999f, -2.261019f, -0.866025f},
	{-1.000000f, -2.261019f,  0.000000f},
	{ 0.499999f, -2.261019f, -0.866025f},
	{ 2.115514f, -1.113953f, -0.645166f},
	{ 1.804193f, -1.688744f,  0.110038f},
	{ 1.000000f, -2.261019f,  0.000000f},
	{ 2.115514f, -0.398022f,  1.222988f},
	{ 1.804193f, -1.333907f,  1.043400f},
	{ 1.616233f,  0.178885f,  1.867856f},
	{ 0.807090f, -0.178885f,  2.324709f},
	{ 0.499999f, -1.113951f,  2.150076f},
	{ 1.000000f, -1.688744f,  1.511169f},
	{-0.499999f, -1.113951f,  2.150076f},
	{-1.000000f, -1.688744f,  1.511169f},
	{-0.499999f, -2.261019f,  0.866025f},
	{ 0.499999f, -2.261019f,  0.866025f},
};

const u16 truncated_icosahedron_indices_single_color[348] = {
	 0,   1,   2, // 0, 1, 2, 3, 4, 5,
	 0,   2,   3,
	 0,   3,   4,
	 0,   4,   5,
	 1,   0,   6, // 1, 0, 6, 7, 8, 9,
	 1,   6,   7,
	 1,   7,   8,
	 1,   8,   9,
	 5,  10,  11, // 5, 10, 11, 6, 0,
	 5,  11,   6,
	 5,   6,   0,
	12,  10,   5, // 12, 10, 5, 4, 13, 14,
	12,   5,   4,
	12,   4,  13,
	12,  13,  14,
	15,  16,  17, // 15, 16, 17, 11, 10, 12,
	15,  17,  11,
	15,  11,  10,
	15,  10,  12,
	18,  19,  20, // 18, 19, 20, 21, 16, 15,
	18,  20,  21,
	18,  21,  16,
	18,  16,  15,
	19,  18,  22, // 19, 18, 22, 23, 24, 25,
	19,  22,  23,
	19,  23,  24,
	19,  24,  25,
	12,  14,  22, // 12, 14, 22, 18, 15,
	12,  22,  18,
	12,  18,  15,
	26,  27,  23, // 26,  27,  23,  22,  14,  13,
	26,  23,  22,
	26,  22,  14,
	26,  14,  13,
	27,  26,  28, // 27,  26,  28,  29,  30,  31,
	27,  28,  29,
	27,  29,  30,
	27,  30,  31,
	13,   4,   3, // 13,   4,   3,  28,  26, 
	13,   3,  28,
	13,  28,  26,
	29,  28,   3, // 29,  28,   3,   2,  32,  33,
	29,   3,   2,
	29,   2,  32,
	29,  32,  33,
	32,   2,   1, // 32,   2,   1,   9,  34, 
	32,   1,   9,
	32,   9,  34,
	 9,   8,  35, //  9,   8,  35,  36,  37,  34,
	 9,  35,  36,
	 9,  36,  37,
	 9,  37,  34,
	 7,  38,  39, //  7,  38,  39,  35,   8, 
	 7,  39,  35,
	 7,  35,   8,
	40,  38,   7, // 40,  38,   7,   6,  11,  17,
	40,   7,   6,
	40,   6,  11,
	40,  11,  17,
	41,  42,  43, // 41,  42,  43,  39,  38,  40,
	41,  43,  39,
	41,  39,  38,
	41,  38,  40,
	21,  20,  44, // 21,  20,  44,  45,  42,  41,
	21,  44,  45,
	21,  45,  42,
	21,  42,  41,
	20,  19,  25, // 20,  19,  25,  46,  44, 
	20,  25,  46,
	20,  46,  44,
	47,  48,  49, // 47,  48,  49,  46,  25,  24,
	47,  49,  46,
	47,  46,  25,
	47,  25,  24,
	50,  51,  48, // 50,  51,  48,  47,  31,  30,
	50,  48,  47,
	50,  47,  31,
	50,  31,  30,
	52,  53,  54, // 52,  53,  54,  55,  51,  50,
	52,  54,  55,
	52,  55,  51,
	52,  51,  50,
	53,  52,  33, // 53,  52,  33,  32,  34,  37,
	53,  33,  32,
	53,  32,  34,
	53,  34,  37,
	30,  29,  33, // 30,  29,  33,  52,  50, 
	30,  33,  52,
	30,  52,  50,
	54,  53,  37, // 54,  53,  37,  36,  56, 
	54,  37,  36,
	54,  36,  56,
	57,  56,  36, // 57,  56,  36,  35,  39,  43,
	57,  36,  35,
	57,  35,  39,
	57,  39,  43,
	58,  59,  55, // 58,  59,  55,  54,  56,  57,
	58,  55,  54,
	58,  54,  56,
	58,  56,  57,
	46,  49,  59, // 46,  49,  59,  58,  45,  44,
	46,  59,  58,
	46,  58,  45,
	46,  45,  44,
	51,  55,  59, // 51,  55,  59,  49,  48, 
	51,  59,  49,
	51,  49,  48,
	58,  57,  43, // 58,  57,  43,  42,  45, 
	58,  43,  42,
	58,  42,  45,
	24,  23,  27, // 24,  23,  27,  31,  47, 
	24,  27,  31,
	24,  31,  47,
	17,  16,  21, // 17,  16,  21,  41,  40, 
	17,  21,  41,
	17,  41,  40,
};

const u16 truncated_icosahedron_indices_colors[464] = {
	 0,   1,   2,  PGLM_RGB565_RED, // 0, 1, 2, 3, 4, 5,
	 0,   2,   3,  PGLM_RGB565_RED,
	 0,   3,   4,  PGLM_RGB565_RED,
	 0,   4,   5,  PGLM_RGB565_RED,
	 1,   0,   6,  PGLM_RGB565_RED, // 1, 0, 6, 7, 8, 9,
	 1,   6,   7,  PGLM_RGB565_RED,
	 1,   7,   8,  PGLM_RGB565_RED,
	 1,   8,   9,  PGLM_RGB565_RED,
	 5,  10,  11,  PGLM_RGB565_RED, // 5, 10, 11, 6, 0,
	 5,  11,   6,  PGLM_RGB565_RED,
	 5,   6,   0,  PGLM_RGB565_RED,
	12,  10,   5,  PGLM_RGB565_RED, // 12, 10, 5, 4, 13, 14,
	12,   5,   4,  PGLM_RGB565_RED,
	12,   4,  13,  PGLM_RGB565_RED,
	12,  13,  14,  PGLM_RGB565_RED,
	15,  16,  17,  PGLM_RGB565_RED, // 15, 16, 17, 11, 10, 12,
	15,  17,  11,  PGLM_RGB565_RED,
	15,  11,  10,  PGLM_RGB565_RED,
	15,  10,  12,  PGLM_RGB565_RED,
	18,  19,  20,  PGLM_RGB565_RED, // 18, 19, 20, 21, 16, 15,
	18,  20,  21,  PGLM_RGB565_RED,
	18,  21,  16,  PGLM_RGB565_RED,
	18,  16,  15,  PGLM_RGB565_RED,
	19,  18,  22,  PGLM_RGB565_RED, // 19, 18, 22, 23, 24, 25,
	19,  22,  23,  PGLM_RGB565_RED,
	19,  23,  24,  PGLM_RGB565_RED,
	19,  24,  25,  PGLM_RGB565_RED,
	12,  14,  22,  PGLM_RGB565_RED, // 12, 14, 22, 18, 15,
	12,  22,  18,  PGLM_RGB565_RED,
	12,  18,  15,  PGLM_RGB565_RED,
	26,  27,  23,  PGLM_RGB565_RED, // 26,  27,  23,  22,  14,  13,
	26,  23,  22,  PGLM_RGB565_RED,
	26,  22,  14,  PGLM_RGB565_RED,
	26,  14,  13,  PGLM_RGB565_RED,
	27,  26,  28,  PGLM_RGB565_RED, // 27,  26,  28,  29,  30,  31,
	27,  28,  29,  PGLM_RGB565_RED,
	27,  29,  30,  PGLM_RGB565_RED,
	27,  30,  31,  PGLM_RGB565_RED,
	13,   4,   3,  PGLM_RGB565_RED, // 13,   4,   3,  28,  26, 
	13,   3,  28,  PGLM_RGB565_RED,
	13,  28,  26,  PGLM_RGB565_RED,
	29,  28,   3,  PGLM_RGB565_RED, // 29,  28,   3,   2,  32,  33,
	29,   3,   2,  PGLM_RGB565_RED,
	29,   2,  32,  PGLM_RGB565_RED,
	29,  32,  33,  PGLM_RGB565_RED,
	32,   2,   1,  PGLM_RGB565_RED, // 32,   2,   1,   9,  34, 
	32,   1,   9,  PGLM_RGB565_RED,
	32,   9,  34,  PGLM_RGB565_RED,
	 9,   8,  35,  PGLM_RGB565_RED, //  9,   8,  35,  36,  37,  34,
	 9,  35,  36,  PGLM_RGB565_RED,
	 9,  36,  37,  PGLM_RGB565_RED,
	 9,  37,  34,  PGLM_RGB565_RED,
	 7,  38,  39,  PGLM_RGB565_RED, //  7,  38,  39,  35,   8, 
	 7,  39,  35,  PGLM_RGB565_RED,
	 7,  35,   8,  PGLM_RGB565_RED,
	40,  38,   7,  PGLM_RGB565_RED, // 40,  38,   7,   6,  11,  17,
	40,   7,   6,  PGLM_RGB565_RED,
	40,   6,  11,  PGLM_RGB565_RED,
	40,  11,  17,  PGLM_RGB565_RED,
	41,  42,  43,  PGLM_RGB565_RED, // 41,  42,  43,  39,  38,  40,
	41,  43,  39,  PGLM_RGB565_RED,
	41,  39,  38,  PGLM_RGB565_RED,
	41,  38,  40,  PGLM_RGB565_RED,
	21,  20,  44,  PGLM_RGB565_RED, // 21,  20,  44,  45,  42,  41,
	21,  44,  45,  PGLM_RGB565_RED,
	21,  45,  42,  PGLM_RGB565_RED,
	21,  42,  41,  PGLM_RGB565_RED,
	20,  19,  25,  PGLM_RGB565_RED, // 20,  19,  25,  46,  44, 
	20,  25,  46,  PGLM_RGB565_RED,
	20,  46,  44,  PGLM_RGB565_RED,
	47,  48,  49,  PGLM_RGB565_RED, // 47,  48,  49,  46,  25,  24,
	47,  49,  46,  PGLM_RGB565_RED,
	47,  46,  25,  PGLM_RGB565_RED,
	47,  25,  24,  PGLM_RGB565_RED,
	50,  51,  48,  PGLM_RGB565_RED, // 50,  51,  48,  47,  31,  30,
	50,  48,  47,  PGLM_RGB565_RED,
	50,  47,  31,  PGLM_RGB565_RED,
	50,  31,  30,  PGLM_RGB565_RED,
	52,  53,  54,  PGLM_RGB565_RED, // 52,  53,  54,  55,  51,  50,
	52,  54,  55,  PGLM_RGB565_RED,
	52,  55,  51,  PGLM_RGB565_RED,
	52,  51,  50,  PGLM_RGB565_RED,
	53,  52,  33,  PGLM_RGB565_RED, // 53,  52,  33,  32,  34,  37,
	53,  33,  32,  PGLM_RGB565_RED,
	53,  32,  34,  PGLM_RGB565_RED,
	53,  34,  37,  PGLM_RGB565_RED,
	30,  29,  33,  PGLM_RGB565_RED, // 30,  29,  33,  52,  50, 
	30,  33,  52,  PGLM_RGB565_RED,
	30,  52,  50,  PGLM_RGB565_RED,
	54,  53,  37,  PGLM_RGB565_RED, // 54,  53,  37,  36,  56, 
	54,  37,  36,  PGLM_RGB565_RED,
	54,  36,  56,  PGLM_RGB565_RED,
	57,  56,  36,  PGLM_RGB565_RED, // 57,  56,  36,  35,  39,  43,
	57,  36,  35,  PGLM_RGB565_RED,
	57,  35,  39,  PGLM_RGB565_RED,
	57,  39,  43,  PGLM_RGB565_RED,
	58,  59,  55,  PGLM_RGB565_RED, // 58,  59,  55,  54,  56,  57,
	58,  55,  54,  PGLM_RGB565_RED,
	58,  54,  56,  PGLM_RGB565_RED,
	58,  56,  57,  PGLM_RGB565_RED,
	46,  49,  59,  PGLM_RGB565_RED, // 46,  49,  59,  58,  45,  44,
	46,  59,  58,  PGLM_RGB565_RED,
	46,  58,  45,  PGLM_RGB565_RED,
	46,  45,  44,  PGLM_RGB565_RED,
	51,  55,  59,  PGLM_RGB565_RED, // 51,  55,  59,  49,  48, 
	51,  59,  49,  PGLM_RGB565_RED,
	51,  49,  48,  PGLM_RGB565_RED,
	58,  57,  43,  PGLM_RGB565_RED, // 58,  57,  43,  42,  45, 
	58,  43,  42,  PGLM_RGB565_RED,
	58,  42,  45,  PGLM_RGB565_RED,
	24,  23,  27,  PGLM_RGB565_RED, // 24,  23,  27,  31,  47, 
	24,  27,  31,  PGLM_RGB565_RED,
	24,  31,  47,  PGLM_RGB565_RED,
	17,  16,  21,  PGLM_RGB565_RED, // 17,  16,  21,  41,  40, 
	17,  21,  41,  PGLM_RGB565_RED,
	17,  41,  40,  PGLM_RGB565_RED,
};

const u16 truncated_icosahedron_indices_tex_coords[696] = {
	 0, 0,   1, 0,   2, 0, // 0, 1, 2, 3, 4, 5,
	 0, 0,   2, 0,   3, 0,
	 0, 0,   3, 0,   4, 0,
	 0, 0,   4, 0,   5, 0,
	 1, 0,   0, 0,   6, 0, // 1, 0, 6, 7, 8, 9,
	 1, 0,   6, 0,   7, 0,
	 1, 0,   7, 0,   8, 0,
	 1, 0,   8, 0,   9, 0,
	 5, 0,  10, 0,  11, 0, // 5, 10, 11, 6, 0,
	 5, 0,  11, 0,   6, 0,
	 5, 0,   6, 0,   0, 0,
	12, 0,  10, 0,   5, 0, // 12, 10, 5, 4, 13, 14,
	12, 0,   5, 0,   4, 0,
	12, 0,   4, 0,  13, 0,
	12, 0,  13, 0,  14, 0,
	15, 0,  16, 0,  17, 0, // 15, 16, 17, 11, 10, 12,
	15, 0,  17, 0,  11, 0,
	15, 0,  11, 0,  10, 0,
	15, 0,  10, 0,  12, 0,
	18, 0,  19, 0,  20, 0, // 18, 19, 20, 21, 16, 15,
	18, 0,  20, 0,  21, 0,
	18, 0,  21, 0,  16, 0,
	18, 0,  16, 0,  15, 0,
	19, 0,  18, 0,  22, 0, // 19, 18, 22, 23, 24, 25,
	19, 0,  22, 0,  23, 0,
	19, 0,  23, 0,  24, 0,
	19, 0,  24, 0,  25, 0,
	12, 0,  14, 0,  22, 0, // 12, 14, 22, 18, 15,
	12, 0,  22, 0,  18, 0,
	12, 0,  18, 0,  15, 0,
	26, 0,  27, 0,  23, 0, // 26,  27,  23,  22,  14,  13,
	26, 0,  23, 0,  22, 0,
	26, 0,  22, 0,  14, 0,
	26, 0,  14, 0,  13, 0,
	27, 0,  26, 0,  28, 0, // 27,  26,  28,  29,  30,  31,
	27, 0,  28, 0,  29, 0,
	27, 0,  29, 0,  30, 0,
	27, 0,  30, 0,  31, 0,
	13, 0,   4, 0,   3, 0, // 13,   4,   3,  28,  26, 
	13, 0,   3, 0,  28, 0,
	13, 0,  28, 0,  26, 0,
	29, 0,  28, 0,   3, 0, // 29,  28,   3,   2,  32,  33,
	29, 0,   3, 0,   2, 0,
	29, 0,   2, 0,  32, 0,
	29, 0,  32, 0,  33, 0,
	32, 0,   2, 0,   1, 0, // 32,   2,   1,   9,  34, 
	32, 0,   1, 0,   9, 0,
	32, 0,   9, 0,  34, 0,
	 9, 0,   8, 0,  35, 0, //  9,   8,  35,  36,  37,  34,
	 9, 0,  35, 0,  36, 0,
	 9, 0,  36, 0,  37, 0,
	 9, 0,  37, 0,  34, 0,
	 7, 0,  38, 0,  39, 0, //  7,  38,  39,  35,   8, 
	 7, 0,  39, 0,  35, 0,
	 7, 0,  35, 0,   8, 0,
	40, 0,  38, 0,   7, 0, // 40,  38,   7,   6,  11,  17,
	40, 0,   7, 0,   6, 0,
	40, 0,   6, 0,  11, 0,
	40, 0,  11, 0,  17, 0,
	41, 0,  42, 0,  43, 0, // 41,  42,  43,  39,  38,  40,
	41, 0,  43, 0,  39, 0,
	41, 0,  39, 0,  38, 0,
	41, 0,  38, 0,  40, 0,
	21, 0,  20, 0,  44, 0, // 21,  20,  44,  45,  42,  41,
	21, 0,  44, 0,  45, 0,
	21, 0,  45, 0,  42, 0,
	21, 0,  42, 0,  41, 0,
	20, 0,  19, 0,  25, 0, // 20,  19,  25,  46,  44, 
	20, 0,  25, 0,  46, 0,
	20, 0,  46, 0,  44, 0,
	47, 0,  48, 0,  49, 0, // 47,  48,  49,  46,  25,  24,
	47, 0,  49, 0,  46, 0,
	47, 0,  46, 0,  25, 0,
	47, 0,  25, 0,  24, 0,
	50, 0,  51, 0,  48, 0, // 50,  51,  48,  47,  31,  30,
	50, 0,  48, 0,  47, 0,
	50, 0,  47, 0,  31, 0,
	50, 0,  31, 0,  30, 0,
	52, 0,  53, 0,  54, 0, // 52,  53,  54,  55,  51,  50,
	52, 0,  54, 0,  55, 0,
	52, 0,  55, 0,  51, 0,
	52, 0,  51, 0,  50, 0,
	53, 0,  52, 0,  33, 0, // 53,  52,  33,  32,  34,  37,
	53, 0,  33, 0,  32, 0,
	53, 0,  32, 0,  34, 0,
	53, 0,  34, 0,  37, 0,
	30, 0,  29, 0,  33, 0, // 30,  29,  33,  52,  50, 
	30, 0,  33, 0,  52, 0,
	30, 0,  52, 0,  50, 0,
	54, 0,  53, 0,  37, 0, // 54,  53,  37,  36,  56, 
	54, 0,  37, 0,  36, 0,
	54, 0,  36, 0,  56, 0,
	57, 0,  56, 0,  36, 0, // 57,  56,  36,  35,  39,  43,
	57, 0,  36, 0,  35, 0,
	57, 0,  35, 0,  39, 0,
	57, 0,  39, 0,  43, 0,
	58, 0,  59, 0,  55, 0, // 58,  59,  55,  54,  56,  57,
	58, 0,  55, 0,  54, 0,
	58, 0,  54, 0,  56, 0,
	58, 0,  56, 0,  57, 0,
	46, 0,  49, 0,  59, 0, // 46,  49,  59,  58,  45,  44,
	46, 0,  59, 0,  58, 0,
	46, 0,  58, 0,  45, 0,
	46, 0,  45, 0,  44, 0,
	51, 0,  55, 0,  59, 0, // 51,  55,  59,  49,  48, 
	51, 0,  59, 0,  49, 0,
	51, 0,  49, 0,  48, 0,
	58, 0,  57, 0,  43, 0, // 58,  57,  43,  42,  45, 
	58, 0,  43, 0,  42, 0,
	58, 0,  42, 0,  45, 0,
	24, 0,  23, 0,  27, 0, // 24,  23,  27,  31,  47, 
	24, 0,  27, 0,  31, 0,
	24, 0,  31, 0,  47, 0,
	17, 0,  16, 0,  21, 0, // 17,  16,  21,  41,  40, 
	17, 0,  21, 0,  41, 0,
	17, 0,  41, 0,  40, 0,
};

// ------------------------------------------------------------------------------------------------------ //

// "Truncated Icosahedron" (https://skfb.ly/LOzY) 
// by Vladimir E. is licensed under 
// Creative Commons Attribution (http://creativecommons.org/licenses/by/4.0/).

const vec3f rhombicubotahedron_vertices[24] = {
	{-0.000000f,  0.431469f, -1.000000f},
	{-0.707107f,  0.431469f, -0.707107f},
	{-0.500000f,  0.972665f, -0.207107f},
	{ 0.207107f,  0.972665f, -0.500000f},
	{ 0.000000f, -0.333898f, -1.000000f},
	{-0.707107f, -0.333898f, -0.707107f},
	{ 0.707107f,  0.431469f, -0.707107f},
	{ 0.707107f, -0.333898f, -0.707107f},
	{ 0.500000f,  0.972665f,  0.207107f},
	{ 1.000000f,  0.431469f,  0.000000f},
	{-0.207107f,  0.972665f,  0.500000f},
	{-1.000000f,  0.431469f,  0.000000f},
	{-0.707107f,  0.431469f,  0.707107f},
	{-0.000000f,  0.431469f,  1.000000f},
	{-0.707107f, -0.333898f,  0.707107f},
	{ 0.000000f, -0.333898f,  1.000000f},
	{-1.000000f, -0.333898f, -0.000000f},
	{-0.500000f, -0.875094f, -0.207107f},
	{ 0.207107f, -0.875094f, -0.500000f},
	{ 0.500000f, -0.875094f,  0.207107f},
	{ 1.000000f, -0.333898f, -0.000000f},
	{-0.207107f, -0.875094f,  0.500000f},
	{ 0.707107f, -0.333898f,  0.707107f},
	{ 0.707107f,  0.431469f,  0.707107f},
};

const u16 rhombicubotahedron_indices_single_color[132] = {
	 0,   1,   2,
	 0,   2,   3,
	 1,   0,   4,
	 1,   4,   5,
	 6,   7,   4,
	 6,   4,   0,
	 6,   0,   3,
	 3,   8,   9,
	 3,   9,   6,
	 8,   3,   2,
	 8,   2,  10, 
	12,  10,   2,
	12,   2,  11,
	10,  12,  13,
	12,  14,  15,
	12,  15,  13,
	14,  12,  11,
	14,  11,  16,
	11,   1,   5,
	11,   5,  16,
	 1,  11,   2,
	17,  16,   5,
	 4,  18,  17,
	 4,  17,   5,
	 4,   7,  18,
	19,  18,   7,
	19,   7,  20,
	18,  19,  21,
	18,  21,  17,
	21,  19,  22,
	21,  22,  15,
	22,  19,  20,
	23,  22,  20,
	23,  20,   9,
	23,  13,  15,
	23,  15,  22,
	23,   8,  10,
	23,  10,  13,
	 8,  23,   9,
	 9,  20,   7,
	 9,   7,   6,
	21,  15,  14,
	21,  14,  16,
	21,  16,  17,
};

const u16 rhombicubotahedron_indices_colors[176] = {
	 0,   1,   2,  PGLM_RGB565_BLUEVIOLET,
	 0,   2,   3,  PGLM_RGB565_BLUEVIOLET,
	 1,   0,   4,  PGLM_RGB565_BLUEVIOLET,
	 1,   4,   5,  PGLM_RGB565_BLUEVIOLET,
	 6,   7,   4,  PGLM_RGB565_BLUEVIOLET,
	 6,   4,   0,  PGLM_RGB565_BLUEVIOLET,
	 6,   0,   3,  PGLM_RGB565_BLUEVIOLET,
	 3,   8,   9,  PGLM_RGB565_BLUEVIOLET,
	 3,   9,   6,  PGLM_RGB565_BLUEVIOLET,
	 8,   3,   2,  PGLM_RGB565_BLUEVIOLET,
	 8,   2,  10,  PGLM_RGB565_BLUEVIOLET, 
	12,  10,   2,  PGLM_RGB565_BLUEVIOLET,
	12,   2,  11,  PGLM_RGB565_BLUEVIOLET,
	10,  12,  13,  PGLM_RGB565_BLUEVIOLET,
	12,  14,  15,  PGLM_RGB565_BLUEVIOLET,
	12,  15,  13,  PGLM_RGB565_BLUEVIOLET,
	14,  12,  11,  PGLM_RGB565_BLUEVIOLET,
	14,  11,  16,  PGLM_RGB565_BLUEVIOLET,
	11,   1,   5,  PGLM_RGB565_BLUEVIOLET,
	11,   5,  16,  PGLM_RGB565_BLUEVIOLET,
	 1,  11,   2,  PGLM_RGB565_BLUEVIOLET,
	17,  16,   5,  PGLM_RGB565_BLUEVIOLET,
	 4,  18,  17,  PGLM_RGB565_BLUEVIOLET,
	 4,  17,   5,  PGLM_RGB565_BLUEVIOLET,
	 4,   7,  18,  PGLM_RGB565_BLUEVIOLET,
	19,  18,   7,  PGLM_RGB565_BLUEVIOLET,
	19,   7,  20,  PGLM_RGB565_BLUEVIOLET,
	18,  19,  21,  PGLM_RGB565_BLUEVIOLET,
	18,  21,  17,  PGLM_RGB565_BLUEVIOLET,
	21,  19,  22,  PGLM_RGB565_BLUEVIOLET,
	21,  22,  15,  PGLM_RGB565_BLUEVIOLET,
	22,  19,  20,  PGLM_RGB565_BLUEVIOLET,
	23,  22,  20,  PGLM_RGB565_BLUEVIOLET,
	23,  20,   9,  PGLM_RGB565_BLUEVIOLET,
	23,  13,  15,  PGLM_RGB565_BLUEVIOLET,
	23,  15,  22,  PGLM_RGB565_BLUEVIOLET,
	23,   8,  10,  PGLM_RGB565_BLUEVIOLET,
	23,  10,  13,  PGLM_RGB565_BLUEVIOLET,
	 8,  23,   9,  PGLM_RGB565_BLUEVIOLET,
	 9,  20,   7,  PGLM_RGB565_BLUEVIOLET,
	 9,   7,   6,  PGLM_RGB565_BLUEVIOLET,
	21,  15,  14,  PGLM_RGB565_BLUEVIOLET,
	21,  14,  16,  PGLM_RGB565_BLUEVIOLET,
	21,  16,  17,  PGLM_RGB565_BLUEVIOLET,
};

const u16 rhombicubotahedron_indices_tex_coords[264] = {
	 0, 0,   1, 0,   2, 0,
	 0, 0,   2, 0,   3, 0,
	 1, 0,   0, 0,   4, 0,
	 1, 0,   4, 0,   5, 0,
	 6, 0,   7, 0,   4, 0,
	 6, 0,   4, 0,   0, 0,
	 6, 0,   0, 0,   3, 0,
	 3, 0,   8, 0,   9, 0,
	 3, 0,   9, 0,   6, 0,
	 8, 0,   3, 0,   2, 0,
	 8, 0,   2, 0,  10, 0, 
	12, 0,  10, 0,   2, 0,
	12, 0,   2, 0,  11, 0,
	10, 0,  12, 0,  13, 0,
	12, 0,  14, 0,  15, 0,
	12, 0,  15, 0,  13, 0,
	14, 0,  12, 0,  11, 0,
	14, 0,  11, 0,  16, 0,
	11, 0,   1, 0,   5, 0,
	11, 0,   5, 0,  16, 0,
	 1, 0,  11, 0,   2, 0,
	17, 0,  16, 0,   5, 0,
	 4, 0,  18, 0,  17, 0,
	 4, 0,  17, 0,   5, 0,
	 4, 0,   7, 0,  18, 0,
	19, 0,  18, 0,   7, 0,
	19, 0,   7, 0,  20, 0,
	18, 0,  19, 0,  21, 0,
	18, 0,  21, 0,  17, 0,
	21, 0,  19, 0,  22, 0,
	21, 0,  22, 0,  15, 0,
	22, 0,  19, 0,  20, 0,
	23, 0,  22, 0,  20, 0,
	23, 0,  20, 0,   9, 0,
	23, 0,  13, 0,  15, 0,
	23, 0,  15, 0,  22, 0,
	23, 0,   8, 0,  10, 0,
	23, 0,  10, 0,  13, 0,
	 8, 0,  23, 0,   9, 0,
	 9, 0,  20, 0,   7, 0,
	 9, 0,   7, 0,   6, 0,
	21, 0,  15, 0,  14, 0,
	21, 0,  14, 0,  16, 0,
	21, 0,  16, 0,  17, 0,
};

// ------------------------------------------------------------------------------------------------------ //

// Copyright 2018-2022 OneLoneCoder.com
// 
// Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:
// 
// 1. Redistributions or derivations of source code must retain the above copyright notice, 
// this list of conditions and the following disclaimer.
// 2. Redistributions or derivative works in binary form must reproduce the above copyright notice. 
// This list of conditions and the following disclaimer must be reproduced in the documentation and/or other materials provided with the distribution.
// 3. Neither the name of the copyright holder nor the names of its contributors may be used to 
// endorse or promote products derived from this software without specific prior written permission.
// 
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND 
// ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, 
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. 
// IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, 
// INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, 
// PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) 
// HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, 
// OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, 
// EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
// 
// The Spaceship model was taken from the following file and its format is changed:
// https://github.com/OneLoneCoder/Javidx9/blob/master/ConsoleGameEngine/BiggerProjects/Engine3D/VideoShip.obj
//

const vec3f spaceship_vertices[55] = {
	{ 1.000000f, -1.000000f, -1.000000f},
	{ 1.000000f,  1.000000f, -1.000000f},
	{ 1.000000f, -1.000000f,  1.000000f},
	{ 1.000000f,  1.000000f,  1.000000f},
	{-1.000000f, -1.000000f, -1.000000f},
	{-1.000000f,  1.000000f, -1.000000f},
	{-1.000000f, -1.000000f,  1.000000f},
	{-1.000000f,  1.000000f,  1.000000f},
	{-0.720000f,  0.120000f, -1.400000f},
	{ 0.300000f,  0.000000f,  5.000000f},
	{-0.600000f, -0.600000f, -1.400000f},
	{-0.300000f,  0.000000f,  5.000000f},
	{-1.200000f,  0.200000f,  1.000000f},
	{-0.600000f,  0.600000f, -1.400000f},
	{-1.200000f, -0.200000f, -1.000000f},
	{-1.200000f,  0.200000f, -1.000000f},
	{ 1.200000f, -0.200000f,  1.000000f},
	{ 1.200000f, -0.200000f, -1.000000f},
	{ 1.200000f,  0.200000f, -1.000000f},
	{ 1.200000f,  0.200000f,  1.000000f},
	{-1.200000f, -0.200000f,  1.000000f},
	{ 0.600000f,  0.600000f, -1.400000f},
	{ 0.600000f, -0.600000f, -1.400000f},
	{-4.200000f,  0.060000f,  1.000000f},
	{-4.200000f, -0.060000f,  1.000000f},
	{-4.200000f, -0.060000f, -1.000000f},
	{-4.200000f,  0.060000f, -1.000000f},
	{ 4.200000f, -0.060000f,  1.000000f},
	{ 4.200000f, -0.060000f, -1.000000f},
	{ 4.200000f,  0.060000f, -1.000000f},
	{ 4.200000f,  0.060000f,  1.000000f},
	{ 4.200000f, -0.180000f,  1.000000f},
	{ 4.200000f, -0.180000f, -1.000000f},
	{ 4.200000f,  0.180000f, -1.000000f},
	{ 4.200000f,  0.180000f,  1.000000f},
	{ 4.500000f, -0.180000f,  1.000000f},
	{ 4.500000f, -0.180000f, -1.000000f},
	{ 4.500000f,  0.180000f, -1.000000f},
	{ 4.500000f,  0.180000f,  1.000000f},
	{-4.200000f,  0.180000f,  1.000000f},
	{-4.200000f, -0.180000f,  1.000000f},
	{-4.200000f, -0.180000f, -1.000000f},
	{-4.200000f,  0.180000f, -1.000000f},
	{-4.500000f,  0.180000f,  1.000000f},
	{-4.500000f, -0.180000f,  1.000000f},
	{-4.500000f, -0.180000f, -1.000000f},
	{-4.500000f,  0.180000f, -1.000000f},
	{ 4.350000f, -0.180000f,  3.000000f},
	{ 4.350000f,  0.180000f,  3.000000f},
	{-4.350000f,  0.180000f,  3.000000f},
	{-4.350000f, -0.180000f,  3.000000f},
	{ 0.000000f, -0.700000f,  3.000000f},
	{-0.720000f, -0.120000f, -1.400000f},
	{ 0.720000f, -0.120000f, -1.400000f},
	{ 0.720000f,  0.120000f, -1.400000f},
};

const u16 spaceship_indices_single_color[318] = {
	20,  51,  11,
	 5,  12,   7,
	 4,  22,   0,
	 6,   0,   2,
	 3,   5,   7,
	 3,  11,   9,
	16,  19,   9,
	19,   3,   9,
	16,  51,   2,
	 6,   2,  51,
	15,  13,   8,
	 6,  14,   4,
	19,  29,  18,
	17,  22,  53,
	 3,  18,   1,
	 0,  16,   2,
	12,  24,  20,
	12,  20,  11,
	11,  51,   9,
	 7,  12,  11,
	26,  41,  42,
	14,  26,  15,
	20,  25,  14,
	15,  23,  12,
	30,  33,  29,
	17,  27,  16,
	16,  30,  19,
	18,  28,  17,
	31,  48,  34,
	28,  31,  27,
	30,  31,  34,
	28,  33,  32,
	37,  35,  36,
	33,  36,  32,
	34,  37,  33,
	32,  35,  31,
	42,  43,  39,
	24,  41,  25,
	26,  39,  23,
	24,  39,  40,
	43,  45,  44,
	39,  43,  49,
	41,  46,  42,
	40,  45,  41,
	43,  46,  45,
	31,  35,  47,
	38,  34,  48,
	38,  47,  35,
	44,  50,  49,
	39,  50,  40,
	44,  40,  50,
	44,  49,  43,
	17,  28,  27,
	16,  27,  30,
	 3,   1,   5,
	17,  54,  18,
	14,  10,   4,
	18,  21,   1,
	 1,  13,   5,
	15,  52,  14,
	52,   8,  53,
	18,  29,  28,
	14,  25,  26,
	15,  26,  23,
	12,  23,  24,
	20,  24,  25,
	 6,  20,  14,
	 6,   4,   0,
	20,   6,  51,
	 0,  17,  16,
	16,   9,  51,
	 3,  19,  18,
	19,  30,  29,
	 3,   7,  11,
	42,  46,  43,
	 5,  15,  12,
	39,  49,  50,
	40,  44,  45,
	41,  45,  46,
	 1,  21,  13,
	18,  54,  21,
	17,  53,  54,
	17,   0,  22,
	 4,  10,  22,
	14,  52,  10,
	15,   8,  52,
	15,   5,  13,
	 8,  13,  21,
	21,  54,   8,
	54,  53,   8,
	53,  22,  10,
	10,  52,  53,
	33,  37,  36,
	37,  38,  35,
	38,  48,  47,
	34,  38,  37,
	32,  36,  35,
	24,  40,  41,
	26,  42,  39,
	30,  34,  33,
	28,  32,  31,
	31,  47,  48,
	26,  25,  41,
	30,  27,  31,
	28,  29,  33,
	24,  23,  39,
};

// ------------------------------------------------------------------------------------------------------ //
