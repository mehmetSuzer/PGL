
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
	 0,  1,  2,	// Back
	 0,  2,  3,	// Back
	 4,  5,  6,	// Front
	 4,  6,  7,	// Front
	 8,  9, 10,	// Left
	 8, 10,  4,	// Left
	11, 12, 13,	// Right
	11, 13,  2,	// Right
	10,  5,  4,	// Bottom
	10, 14,  5,	// Bottom
	 1, 15, 11,	// Top
	 1, 11,  2,	// Top
};

// ------------------------------------------------------------------------------------------------------ //

const vertex_t triangle_pyramid_vertices[8] = {
	//                    positions                   /     tex_coords      //
	{{ 0.0f, 					0.0f,  -PGLM_1_SQRT3f}, {0.5f, PGLM_SQRT3_2f}},
	{{ 0.5f, 					0.0f,  PGLM_1_2SQRT3f}, {1.0f,          0.0f}},
	{{-0.5f, 					0.0f,  PGLM_1_2SQRT3f}, {0.0f,          0.0f}},
	{{ 0.0f, PGLM_SQRT2f/PGLM_SQRT3f, 		     0.0f}, {0.5f, PGLM_SQRT3_2f}},
	{{ 0.0f, 					0.0f,  -PGLM_1_SQRT3f}, {1.0f, 			0.0f}},
	{{ 0.0f, 					0.0f,  -PGLM_1_SQRT3f}, {0.0f, 			0.0f}},
	{{ 0.5f, 					0.0f,  PGLM_1_2SQRT3f}, {0.0f,          0.0f}},
	{{-0.5f, 					0.0f,  PGLM_1_2SQRT3f}, {1.0f,          0.0f}},
};

const uint16_t triangle_pyramid_indices[12] = {
	0, 1, 2, // Bottom
	2, 3, 4, // Side 1
	5, 3, 1, // Side 2
	6, 3, 7, // Side 3
};

// ------------------------------------------------------------------------------------------------------ //

const vertex_t square_pyramid_vertices[11] = {
	//     positions     / tex_coords //
	{{-0.6f, 0.0f, -0.6f}, {0.0f, 0.0f}},
	{{ 0.6f, 0.0f, -0.6f}, {1.0f, 0.0f}},
	{{ 0.6f, 0.0f,  0.6f}, {1.0f, 1.0f}},
	{{-0.6f, 0.0f,  0.6f}, {0.0f, 1.0f}},
	{{ 0.0f, 0.8f,  0.0f}, {0.5f, 1.0f}}, // Top
	{{-0.6f, 0.0f,  0.6f}, {1.0f, 0.0f}},
	{{-0.6f, 0.0f,  0.6f}, {0.0f, 0.0f}},
	{{ 0.6f, 0.0f,  0.6f}, {1.0f, 0.0f}},
	{{ 0.6f, 0.0f,  0.6f}, {0.0f, 0.0f}},
	{{ 0.6f, 0.0f, -0.6f}, {0.0f, 0.0f}},
	{{-0.6f, 0.0f, -0.6f}, {1.0f, 0.0f}},
};

const uint16_t square_pyramid_indices[18] = {
	0,  1, 2, // Bottom
	0,  2, 3, // Bottom
	0,  5, 4, // Left
	6,  7, 4, // Front
	8,  1, 4, // Right
	9, 10, 4, // Back
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

const vertex_t spaceship_vertices[55] = {
	// 			positions				 / tex_coords //
	{{ 1.000000f, -1.000000f, -1.000000f}, {0.0f, 0.0f}},
	{{ 1.000000f,  1.000000f, -1.000000f}, {0.0f, 0.0f}},
	{{ 1.000000f, -1.000000f,  1.000000f}, {0.0f, 0.0f}},
	{{ 1.000000f,  1.000000f,  1.000000f}, {0.0f, 0.0f}},
	{{-1.000000f, -1.000000f, -1.000000f}, {0.0f, 0.0f}},
	{{-1.000000f,  1.000000f, -1.000000f}, {0.0f, 0.0f}},
	{{-1.000000f, -1.000000f,  1.000000f}, {0.0f, 0.0f}},
	{{-1.000000f,  1.000000f,  1.000000f}, {0.0f, 0.0f}},
	{{-0.720000f,  0.120000f, -1.400000f}, {0.0f, 0.0f}},
	{{ 0.300000f,  0.000000f,  5.000000f}, {0.0f, 0.0f}},
	{{-0.600000f, -0.600000f, -1.400000f}, {0.0f, 0.0f}},
	{{-0.300000f,  0.000000f,  5.000000f}, {0.0f, 0.0f}},
	{{-1.200000f,  0.200000f,  1.000000f}, {0.0f, 0.0f}},
	{{-0.600000f,  0.600000f, -1.400000f}, {0.0f, 0.0f}},
	{{-1.200000f, -0.200000f, -1.000000f}, {0.0f, 0.0f}},
	{{-1.200000f,  0.200000f, -1.000000f}, {0.0f, 0.0f}},
	{{ 1.200000f, -0.200000f,  1.000000f}, {0.0f, 0.0f}},
	{{ 1.200000f, -0.200000f, -1.000000f}, {0.0f, 0.0f}},
	{{ 1.200000f,  0.200000f, -1.000000f}, {0.0f, 0.0f}},
	{{ 1.200000f,  0.200000f,  1.000000f}, {0.0f, 0.0f}},
	{{-1.200000f, -0.200000f,  1.000000f}, {0.0f, 0.0f}},
	{{ 0.600000f,  0.600000f, -1.400000f}, {0.0f, 0.0f}},
	{{ 0.600000f, -0.600000f, -1.400000f}, {0.0f, 0.0f}},
	{{-4.200000f,  0.060000f,  1.000000f}, {0.0f, 0.0f}},
	{{-4.200000f, -0.060000f,  1.000000f}, {0.0f, 0.0f}},
	{{-4.200000f, -0.060000f, -1.000000f}, {0.0f, 0.0f}},
	{{-4.200000f,  0.060000f, -1.000000f}, {0.0f, 0.0f}},
	{{ 4.200000f, -0.060000f,  1.000000f}, {0.0f, 0.0f}},
	{{ 4.200000f, -0.060000f, -1.000000f}, {0.0f, 0.0f}},
	{{ 4.200000f,  0.060000f, -1.000000f}, {0.0f, 0.0f}},
	{{ 4.200000f,  0.060000f,  1.000000f}, {0.0f, 0.0f}},
	{{ 4.200000f, -0.180000f,  1.000000f}, {0.0f, 0.0f}},
	{{ 4.200000f, -0.180000f, -1.000000f}, {0.0f, 0.0f}},
	{{ 4.200000f,  0.180000f, -1.000000f}, {0.0f, 0.0f}},
	{{ 4.200000f,  0.180000f,  1.000000f}, {0.0f, 0.0f}},
	{{ 4.500000f, -0.180000f,  1.000000f}, {0.0f, 0.0f}},
	{{ 4.500000f, -0.180000f, -1.000000f}, {0.0f, 0.0f}},
	{{ 4.500000f,  0.180000f, -1.000000f}, {0.0f, 0.0f}},
	{{ 4.500000f,  0.180000f,  1.000000f}, {0.0f, 0.0f}},
	{{-4.200000f,  0.180000f,  1.000000f}, {0.0f, 0.0f}},
	{{-4.200000f, -0.180000f,  1.000000f}, {0.0f, 0.0f}},
	{{-4.200000f, -0.180000f, -1.000000f}, {0.0f, 0.0f}},
	{{-4.200000f,  0.180000f, -1.000000f}, {0.0f, 0.0f}},
	{{-4.500000f,  0.180000f,  1.000000f}, {0.0f, 0.0f}},
	{{-4.500000f, -0.180000f,  1.000000f}, {0.0f, 0.0f}},
	{{-4.500000f, -0.180000f, -1.000000f}, {0.0f, 0.0f}},
	{{-4.500000f,  0.180000f, -1.000000f}, {0.0f, 0.0f}},
	{{ 4.350000f, -0.180000f,  3.000000f}, {0.0f, 0.0f}},
	{{ 4.350000f,  0.180000f,  3.000000f}, {0.0f, 0.0f}},
	{{-4.350000f,  0.180000f,  3.000000f}, {0.0f, 0.0f}},
	{{-4.350000f, -0.180000f,  3.000000f}, {0.0f, 0.0f}},
	{{ 0.000000f, -0.700000f,  3.000000f}, {0.0f, 0.0f}},
	{{-0.720000f, -0.120000f, -1.400000f}, {0.0f, 0.0f}},
	{{ 0.720000f, -0.120000f, -1.400000f}, {0.0f, 0.0f}},
	{{ 0.720000f,  0.120000f, -1.400000f}, {0.0f, 0.0f}},
};

const uint16_t spaceship_indices[318] = {
	20, 51, 11,
	 5, 12,  7,
	 4, 22,  0,
	 6,  0,  2,
	 3,  5,  7,
	 3, 11,  9,
	16, 19,  9,
	19,  3,  9,
	16, 51,  2,
	 6,  2, 51,
	15, 13,  8,
	 6, 14,  4,
	19, 29, 18,
	17, 22, 53,
	 3, 18,  1,
	 0, 16,  2,
	12, 24, 20,
	12, 20, 11,
	11, 51,  9,
	 7, 12, 11,
	26, 41, 42,
	14, 26, 15,
	20, 25, 14,
	15, 23, 12,
	30, 33, 29,
	17, 27, 16,
	16, 30, 19,
	18, 28, 17,
	31, 48, 34,
	28, 31, 27,
	30, 31, 34,
	28, 33, 32,
	37, 35, 36,
	33, 36, 32,
	34, 37, 33,
	32, 35, 31,
	42, 43, 39,
	24, 41, 25,
	26, 39, 23,
	24, 39, 40,
	43, 45, 44,
	39, 43, 49,
	41, 46, 42,
	40, 45, 41,
	43, 46, 45,
	31, 35, 47,
	38, 34, 48,
	38, 47, 35,
	44, 50, 49,
	39, 50, 40,
	44, 40, 50,
	44, 49, 43,
	17, 28, 27,
	16, 27, 30,
	 3,  1,  5,
	17, 54, 18,
	14, 10,  4,
	18, 21,  1,
	 1, 13,  5,
	15, 52, 14,
	52,  8, 53,
	18, 29, 28,
	14, 25, 26,
	15, 26, 23,
	12, 23, 24,
	20, 24, 25,
	 6, 20, 14,
	 6,  4,  0,
	20,  6, 51,
	 0, 17, 16,
	16,  9, 51,
	 3, 19, 18,
	19, 30, 29,
	 3,  7, 11,
	42, 46, 43,
	 5, 15, 12,
	39, 49, 50,
	40, 44, 45,
	41, 45, 46,
	 1, 21, 13,
	18, 54, 21,
	17, 53, 54,
	17,  0, 22,
	 4, 10, 22,
	14, 52, 10,
	15,  8, 52,
	15,  5, 13,
	 8, 13, 21,
	21, 54,  8,
	54, 53,  8,
	53, 22, 10,
	10, 52, 53,
	33, 37, 36,
	37, 38, 35,
	38, 48, 47,
	34, 38, 37,
	32, 36, 35,
	24, 40, 41,
	26, 42, 39,
	30, 34, 33,
	28, 32, 31,
	31, 47, 48,
	26, 25, 41,
	30, 27, 31,
	28, 29, 33,
	24, 23, 39,
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
