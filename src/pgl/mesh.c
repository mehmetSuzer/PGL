
#include "mesh.h"

// ------------------------------------------------------------------------------------------------------ //

const vertex_t triangle_vertices[3] = {
    //           positions          /      colors      //
	{{-0.5f, -PGLM_1_2SQRT3f,  0.0f}, {0.0f, 1.0f, 0.0f}},
	{{ 0.5f, -PGLM_1_2SQRT3f,  0.0f}, {0.0f, 1.0f, 0.0f}},
	{{ 0.0f,   PGLM_1_SQRT3f,  0.0f}, {0.0f, 1.0f, 0.0f}},
};

const uint32_t triangle_indices[3] = {
    0, 1, 2,
};

// ------------------------------------------------------------------------------------------------------ //

const vertex_t square_vertices[4] = {
    //      positions     /      colors      //
	{{-0.5f, -0.5f,  0.0f}, {0.0f, 0.0f, 1.0f}},
	{{ 0.5f, -0.5f,  0.0f}, {0.0f, 0.0f, 1.0f}},
	{{ 0.5f,  0.5f,  0.0f}, {0.0f, 0.0f, 1.0f}},
	{{-0.5f,  0.5f,  0.0f}, {0.0f, 0.0f, 1.0f}},
};

const uint32_t square_indices[6] = {
    0, 1, 2,
	0, 2, 3,
};

// ------------------------------------------------------------------------------------------------------ //

const vertex_t cube_vertices[8] = {
 	//     positions      /      colors      //
	{{-0.5f, -0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}},
	{{-0.5f,  0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}},
	{{ 0.5f,  0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}},
	{{ 0.5f, -0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}},
	{{-0.5f, -0.5f,  0.5f}, {1.0f, 0.0f, 0.0f}},
	{{ 0.5f, -0.5f,  0.5f}, {1.0f, 0.0f, 0.0f}},
	{{ 0.5f,  0.5f,  0.5f}, {1.0f, 0.0f, 0.0f}},
	{{-0.5f,  0.5f,  0.5f}, {1.0f, 0.0f, 0.0f}},
};

const uint32_t cube_indices[36] = {
	0, 1, 2,
	0, 2, 3,
	4, 5, 6,
	4, 6, 7,
	7, 1, 0,
	7, 0, 4,
	6, 5, 3,
	6, 3, 2,
	0, 3, 5,
	0, 5, 4,
	1, 7, 6,
	1, 6, 2,
};

// ------------------------------------------------------------------------------------------------------ //

const vertex_t triangle_pyramid_vertices[4] = {
	//                    positions                   /      colors      //
	{{-0.5f, 					0.0f,  PGLM_1_2SQRT3f}, {0.0f, 1.0f, 0.0f}},
	{{ 0.5f, 					0.0f,  PGLM_1_2SQRT3f}, {0.0f, 1.0f, 0.0f}},
	{{ 0.0f, 					0.0f,  -PGLM_1_SQRT3f}, {0.0f, 1.0f, 0.0f}},
	{{ 0.0f, PGLM_SQRT2f/PGLM_SQRT3f, 		     0.0f}, {0.0f, 1.0f, 0.0f}},
};

const uint32_t triangle_pyramid_indices[12] = {
	0, 2, 1,
	0, 1, 3,
	1, 2, 3,
	2, 0, 3, 
};

// ------------------------------------------------------------------------------------------------------ //

const vertex_t square_pyramid_vertices[5] = {
	//     positions     /     colors       //
	{{-0.6f, 0.0f, -0.6f}, {0.0f, 1.0f, 1.0f}}, 
	{{ 0.6f, 0.0f, -0.6f}, {0.0f, 1.0f, 1.0f}}, 
	{{ 0.6f, 0.0f,  0.6f}, {0.0f, 1.0f, 1.0f}}, 	
	{{-0.6f, 0.0f,  0.6f}, {0.0f, 1.0f, 1.0f}}, 
	{{ 0.0f, 0.8f,  0.0f}, {0.0f, 1.0f, 1.0f}}, 
};

const uint32_t square_pyramid_indices[18] = {
	0, 1, 2,
	0, 2, 3,
	4, 0, 1,
	4, 2, 1,
	4, 3, 2,
	4, 3, 0,
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
