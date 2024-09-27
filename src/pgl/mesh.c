
#include "mesh.h"

// ------------------------------------------------------------------------------------------------------ //

const vertex_t triangle_vertices[3] = {
    //           positions          /     tex coords      //
	{{-0.5f, -PGLM_1_2SQRT3f,  0.0f}, {0.0f, 		  0.0f}},
	{{ 0.5f, -PGLM_1_2SQRT3f,  0.0f}, {1.0f, 		  0.0f}},
	{{ 0.0f,   PGLM_1_SQRT3f,  0.0f}, {0.5f, PGLM_SQRT3_2f}},
};

const uint16_t triangle_indices[3] = {
    0, 1, 2,
};

// ------------------------------------------------------------------------------------------------------ //

const vertex_t square_vertices[4] = {
    //      positions     / tex coords //
	{{-0.5f, -0.5f,  0.0f}, {0.0f, 0.0f}},
	{{ 0.5f, -0.5f,  0.0f}, {1.0f, 0.0f}},
	{{ 0.5f,  0.5f,  0.0f}, {1.0f, 1.0f}},
	{{-0.5f,  0.5f,  0.0f}, {0.0f, 1.0f}},
};

const uint16_t square_indices[6] = {
    0, 1, 2,
	0, 2, 3,
};

// ------------------------------------------------------------------------------------------------------ //

const vertex_t cube_vertices[16] = {  				 
	//     positions      / tex_coords //
	{{-0.5f, -0.5f, -0.5f}, {0.0f, 0.0f}},
	{{-0.5f,  0.5f, -0.5f}, {0.0f, 1.0f}},
	{{ 0.5f,  0.5f, -0.5f}, {1.0f, 1.0f}},
	{{ 0.5f, -0.5f, -0.5f}, {1.0f, 0.0f}},
	{{-0.5f, -0.5f,  0.5f}, {0.0f, 0.0f}},
	{{ 0.5f, -0.5f,  0.5f}, {1.0f, 0.0f}},
	{{ 0.5f,  0.5f,  0.5f}, {1.0f, 1.0f}},
	{{-0.5f,  0.5f,  0.5f}, {0.0f, 1.0f}},
	{{-0.5f,  0.5f,  0.5f}, {1.0f, 0.0f}},
	{{-0.5f,  0.5f, -0.5f}, {1.0f, 1.0f}},
	{{-0.5f, -0.5f, -0.5f}, {0.0f, 1.0f}},
	{{ 0.5f,  0.5f,  0.5f}, {1.0f, 0.0f}},
	{{ 0.5f, -0.5f,  0.5f}, {0.0f, 0.0f}},
	{{ 0.5f, -0.5f, -0.5f}, {0.0f, 1.0f}},
	{{ 0.5f, -0.5f, -0.5f}, {1.0f, 1.0f}},
	{{-0.5f,  0.5f,  0.5f}, {0.0f, 0.0f}},
};

const uint16_t cube_indices[36] = {
	0, 1, 2,	// Back
	0, 2, 3,	// Back
	4, 5, 6,	// Front
	4, 6, 7,	// Front
	8, 9, 10,	// Left
	8, 10, 4,	// Left
	11, 12, 13,	// Right
	11, 13, 2,	// Right
	10, 14, 5,	// Bottom
	10, 5, 4,	// Bottom
	1, 15, 11,	// Top
	15, 11, 2,	// Top
};

// ------------------------------------------------------------------------------------------------------ //

const vertex_t triangle_pyramid_vertices[8] = {
	//                    positions                   /     tex_coords      //
	{{ 0.0f, 					0.0f,  -PGLM_1_SQRT3f}, {0.5f, PGLM_SQRT3_2f}},
	{{ 0.5f, 					0.0f,  PGLM_1_2SQRT3f}, {0.0f,          0.0f}},
	{{-0.5f, 					0.0f,  PGLM_1_2SQRT3f}, {1.0f,          0.0f}},
	{{ 0.0f, 					0.0f,  -PGLM_1_SQRT3f}, {1.0f, 			0.0f}},
	{{ 0.0f, PGLM_SQRT2f/PGLM_SQRT3f, 		     0.0f}, {0.5f, PGLM_SQRT3_2f}},
	{{-0.5f, 					0.0f,  PGLM_1_2SQRT3f}, {0.0f,          0.0f}},
	{{ 0.5f, 					0.0f,  PGLM_1_2SQRT3f}, {1.0f,          0.0f}},
	{{ 0.0f, PGLM_SQRT2f/PGLM_SQRT3f, 		     0.0f}, {0.0f, 			0.0f}},

};

const uint16_t triangle_pyramid_indices[12] = {
	0, 1, 2, // Bottom
	1, 3, 4, // Side 1
	5, 6, 4, // Side 2
	2, 0, 7, // Side 3
};

// ------------------------------------------------------------------------------------------------------ //

const vertex_t square_pyramid_vertices[7] = {
	//     positions     / tex_coords //
	{{-0.6f, 0.0f,  0.6f}, {0.0f, 0.0f}},
	{{-0.6f, 0.0f, -0.6f}, {0.0f, 1.0f}},
	{{ 0.6f, 0.0f, -0.6f}, {1.0f, 1.0f}},
	{{ 0.6f, 0.0f,  0.6f}, {1.0f, 0.0f}},
	{{-0.6f, 0.0f, -0.6f}, {1.0f, 0.0f}},
	{{ 0.0f, 0.8f,  0.0f}, {0.5f, 1.0f}},
	{{ 0.6f, 0.0f, -0.6f}, {0.0f, 0.0f}},
};

const uint16_t square_pyramid_indices[18] = {
	0, 1, 2, // Bottom
	0, 2, 3, // Bottom
	0, 5, 4, // Left
	4, 5, 6, // Back
	6, 5, 3, // Right
	3, 5, 0, // Front
};

// ------------------------------------------------------------------------------------------------------ //

void find_mesh_bounding_volume(mesh_t* mesh) {
	vec3f center = {0.0f, 0.0f, 0.0f};
	for (uint32_t i = 0; i < mesh->vertex_number; i++) {
		center = add_vec3f(center, mesh->vertices[i].position);
	}
	center = scale_vec3f(center, 1.0f / mesh->vertex_number);

	float radius = 0.0f;
	for (uint32_t i = 0; i < mesh->vertex_number; i++) {
		const float distance = mag_vec3f(sub_vec3f(center, mesh->vertices[i].position));
		if (distance > radius) {
			radius = distance;
		}
	}

	mesh->bounding_volume = (sphere_t){center, radius};
}
