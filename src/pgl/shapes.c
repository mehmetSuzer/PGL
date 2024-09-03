
#include "shapes.h"

// ------------------------------------------------------------------------------------------------------ //

const vertex_t triangle_vertices[3] = {
    //          positions          /             normals          /      colors      //
	{{-0.5f,           0.0f,  0.0f}, {-PGLM_SQRT3_2f, -0.5f, 0.0f}, {0.0f, 1.0f, 0.0f}},
	{{ 0.5f,           0.0f,  0.0f}, { PGLM_SQRT3_2f,  0.5f, 0.0f}, {0.0f, 1.0f, 0.0f}},
	{{ 0.0f,  PGLM_SQRT3_2f,  0.0f}, {          0.0f,  1.0f, 0.0f}, {0.0f, 1.0f, 0.0f}},
};

const uint32_t triangle_indices[3] = {
    0, 1, 2,
};

// ------------------------------------------------------------------------------------------------------ //

const vertex_t square_vertices[4] = {
    //          positions          /             normals          /      colors      //
	{{-0.5f, -0.5f,  0.0f}, {-PGLM_1_SQRT2f, -PGLM_1_SQRT2f, 0.0f}, {0.0f, 0.0f, 1.0f}},
	{{ 0.5f, -0.5f,  0.0f}, { PGLM_1_SQRT2f, -PGLM_1_SQRT2f, 0.0f}, {0.0f, 0.0f, 1.0f}},
	{{ 0.5f,  0.5f,  0.0f}, { PGLM_1_SQRT2f,  PGLM_1_SQRT2f, 0.0f}, {0.0f, 0.0f, 1.0f}},
	{{-0.5f,  0.5f,  0.0f}, {-PGLM_1_SQRT2f,  PGLM_1_SQRT2f, 0.0f}, {0.0f, 0.0f, 1.0f}},
};

const uint32_t square_indices[6] = {
    0, 1, 2,
	0, 2, 3,
};

// ------------------------------------------------------------------------------------------------------ //

const vertex_t cube_vertices[8] = {
 	//     positions      /                    normals                      /      colors      //
	{{-0.5f, -0.5f, -0.5f}, {-PGLM_1_SQRT3f, -PGLM_1_SQRT3f, -PGLM_1_SQRT3f}, {1.0f, 0.0f, 0.0f}},
	{{-0.5f,  0.5f, -0.5f}, {-PGLM_1_SQRT3f,  PGLM_1_SQRT3f, -PGLM_1_SQRT3f}, {1.0f, 0.0f, 0.0f}},
	{{ 0.5f,  0.5f, -0.5f}, { PGLM_1_SQRT3f,  PGLM_1_SQRT3f, -PGLM_1_SQRT3f}, {1.0f, 0.0f, 0.0f}},
	{{ 0.5f, -0.5f, -0.5f}, { PGLM_1_SQRT3f, -PGLM_1_SQRT3f, -PGLM_1_SQRT3f}, {1.0f, 0.0f, 0.0f}},
	 
	{{-0.5f, -0.5f,  0.5f}, {-PGLM_1_SQRT3f, -PGLM_1_SQRT3f,  PGLM_1_SQRT3f}, {1.0f, 0.0f, 0.0f}},
	{{ 0.5f, -0.5f,  0.5f}, { PGLM_1_SQRT3f, -PGLM_1_SQRT3f,  PGLM_1_SQRT3f}, {1.0f, 0.0f, 0.0f}},
	{{ 0.5f,  0.5f,  0.5f}, { PGLM_1_SQRT3f,  PGLM_1_SQRT3f,  PGLM_1_SQRT3f}, {1.0f, 0.0f, 0.0f}},
	{{-0.5f,  0.5f,  0.5f}, {-PGLM_1_SQRT3f,  PGLM_1_SQRT3f,  PGLM_1_SQRT3f}, {1.0f, 0.0f, 0.0f}},
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

// TODO: triangle pyramid

// ------------------------------------------------------------------------------------------------------ //

// TODO: calculate normal vectors when necessary
const vertex_t square_pyramid_vertices[5] = {
	//         positions /       normals        /     colors       //
	{{-0.6f, 0.0f, -0.6f}, { 0.0f, -1.0f,  0.0f}, {0.0f, 1.0f, 1.0f}}, 
	{{ 0.6f, 0.0f, -0.6f}, { 0.0f, -1.0f,  0.0f}, {0.0f, 1.0f, 1.0f}}, 
	{{ 0.6f, 0.0f,  0.6f}, { 0.0f, -1.0f,  0.0f}, {0.0f, 1.0f, 1.0f}}, 	
	{{-0.6f, 0.0f,  0.6f}, { 0.0f, -1.0f,  0.0f}, {0.0f, 1.0f, 1.0f}}, 
	{{ 0.0f, 0.8f,  0.0f}, { 0.0f,  1.0f,  0.0f}, {0.0f, 1.0f, 1.0f}}, 
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
