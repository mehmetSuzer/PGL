
#include "mesh.h"

const vec2f tex_coords[5] = {
	{0.0f, 			0.0f}, // Left   Bottom
	{1.0f, 			0.0f}, // Right  Bottom
	{1.0f, 			1.0f}, // Right  Top
	{0.0f, 			1.0f}, // Left   Top
	{0.5f, PGLM_SQRT3_2f}, // Middle Top
};

// ------------------------------------------------------------------------------------------------------ //

const vec3f equilateral_triangle_vertices[3] = {
	{-0.5f, -PGLM_1_2SQRT3f,  0.0f}, // Left
	{ 0.5f, -PGLM_1_2SQRT3f,  0.0f}, // Right
	{ 0.0f,   PGLM_1_SQRT3f,  0.0f}, // Top
};

const uint16_t equilateral_triangle_indices[6] = {
    0, 0,  1, 1,  2, 4,
};

// ------------------------------------------------------------------------------------------------------ //

const vec3f square_vertices[4] = {
	{-0.5f, -0.5f,  0.0f}, // Left  Bottom
	{ 0.5f, -0.5f,  0.0f}, // Right Bottom
	{ 0.5f,  0.5f,  0.0f}, // Right Top
	{-0.5f,  0.5f,  0.0f}, // Left  Top
};

const uint16_t square_indices[12] = {
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

const uint16_t cube_indices[72] = {
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

const uint16_t triangle_pyramid_indices[24] = {
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

const uint16_t square_pyramid_indices[36] = {
	0, 0,  1, 1,  4, 4, // Front  Face
	1, 0,  2, 1,  4, 4, // Right  Face
	2, 0,  3, 1,  4, 4, // Back   Face
	3, 0,  0, 1,  4, 4, // Left   Face
	3, 0,  2, 1,  1, 2, // Bottom Face 
	3, 0,  1, 2,  0, 3, // Bottom Face
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

const uint16_t spaceship_indices[636] = {
	20, 0,  51, 0,  11, 0,
	 5, 0,  12, 0,   7, 0,
	 4, 0,  22, 0,   0, 0,
	 6, 0,   0, 0,   2, 0,
	 3, 0,   5, 0,   7, 0,
	 3, 0,  11, 0,   9, 0,
	16, 0,  19, 0,   9, 0,
	19, 0,   3, 0,   9, 0,
	16, 0,  51, 0,   2, 0,
	 6, 0,   2, 0,  51, 0,
	15, 0,  13, 0,   8, 0,
	 6, 0,  14, 0,   4, 0,
	19, 0,  29, 0,  18, 0,
	17, 0,  22, 0,  53, 0,
	 3, 0,  18, 0,   1, 0,
	 0, 0,  16, 0,   2, 0,
	12, 0,  24, 0,  20, 0,
	12, 0,  20, 0,  11, 0,
	11, 0,  51, 0,   9, 0,
	 7, 0,  12, 0,  11, 0,
	26, 0,  41, 0,  42, 0,
	14, 0,  26, 0,  15, 0,
	20, 0,  25, 0,  14, 0,
	15, 0,  23, 0,  12, 0,
	30, 0,  33, 0,  29, 0,
	17, 0,  27, 0,  16, 0,
	16, 0,  30, 0,  19, 0,
	18, 0,  28, 0,  17, 0,
	31, 0,  48, 0,  34, 0,
	28, 0,  31, 0,  27, 0,
	30, 0,  31, 0,  34, 0,
	28, 0,  33, 0,  32, 0,
	37, 0,  35, 0,  36, 0,
	33, 0,  36, 0,  32, 0,
	34, 0,  37, 0,  33, 0,
	32, 0,  35, 0,  31, 0,
	42, 0,  43, 0,  39, 0,
	24, 0,  41, 0,  25, 0,
	26, 0,  39, 0,  23, 0,
	24, 0,  39, 0,  40, 0,
	43, 0,  45, 0,  44, 0,
	39, 0,  43, 0,  49, 0,
	41, 0,  46, 0,  42, 0,
	40, 0,  45, 0,  41, 0,
	43, 0,  46, 0,  45, 0,
	31, 0,  35, 0,  47, 0,
	38, 0,  34, 0,  48, 0,
	38, 0,  47, 0,  35, 0,
	44, 0,  50, 0,  49, 0,
	39, 0,  50, 0,  40, 0,
	44, 0,  40, 0,  50, 0,
	44, 0,  49, 0,  43, 0,
	17, 0,  28, 0,  27, 0,
	16, 0,  27, 0,  30, 0,
	 3, 0,   1, 0,   5, 0,
	17, 0,  54, 0,  18, 0,
	14, 0,  10, 0,   4, 0,
	18, 0,  21, 0,   1, 0,
	 1, 0,  13, 0,   5, 0,
	15, 0,  52, 0,  14, 0,
	52, 0,   8, 0,  53, 0,
	18, 0,  29, 0,  28, 0,
	14, 0,  25, 0,  26, 0,
	15, 0,  26, 0,  23, 0,
	12, 0,  23, 0,  24, 0,
	20, 0,  24, 0,  25, 0,
	 6, 0,  20, 0,  14, 0,
	 6, 0,   4, 0,   0, 0,
	20, 0,   6, 0,  51, 0,
	 0, 0,  17, 0,  16, 0,
	16, 0,   9, 0,  51, 0,
	 3, 0,  19, 0,  18, 0,
	19, 0,  30, 0,  29, 0,
	 3, 0,   7, 0,  11, 0,
	42, 0,  46, 0,  43, 0,
	 5, 0,  15, 0,  12, 0,
	39, 0,  49, 0,  50, 0,
	40, 0,  44, 0,  45, 0,
	41, 0,  45, 0,  46, 0,
	 1, 0,  21, 0,  13, 0,
	18, 0,  54, 0,  21, 0,
	17, 0,  53, 0,  54, 0,
	17, 0,   0, 0,  22, 0,
	 4, 0,  10, 0,  22, 0,
	14, 0,  52, 0,  10, 0,
	15, 0,   8, 0,  52, 0,
	15, 0,   5, 0,  13, 0,
	 8, 0,  13, 0,  21, 0,
	21, 0,  54, 0,   8, 0,
	54, 0,  53, 0,   8, 0,
	53, 0,  22, 0,  10, 0,
	10, 0,  52, 0,  53, 0,
	33, 0,  37, 0,  36, 0,
	37, 0,  38, 0,  35, 0,
	38, 0,  48, 0,  47, 0,
	34, 0,  38, 0,  37, 0,
	32, 0,  36, 0,  35, 0,
	24, 0,  40, 0,  41, 0,
	26, 0,  42, 0,  39, 0,
	30, 0,  34, 0,  33, 0,
	28, 0,  32, 0,  31, 0,
	31, 0,  47, 0,  48, 0,
	26, 0,  25, 0,  41, 0,
	30, 0,  27, 0,  31, 0,
	28, 0,  29, 0,  33, 0,
	24, 0,  23, 0,  39, 0,
};

// ------------------------------------------------------------------------------------------------------ //

void find_mesh_bounding_volume(mesh_t* mesh) {
	vec3f center = {0.0f, 0.0f, 0.0f};
	for (uint32_t i = 0; i < mesh->vertex_number; i++) {
		center = add_vec3f(center, mesh->vertices[i]);
	}
	center = scale_vec3f(center, 1.0f / mesh->vertex_number);

	float radius = 0.0f;
	for (uint32_t i = 0; i < mesh->vertex_number; i++) {
		const float distance = mag_vec3f(sub_vec3f(center, mesh->vertices[i]));
		if (distance > radius) {
			radius = distance;
		}
	}
	mesh->bounding_volume = (sphere_t){center, radius};
}
