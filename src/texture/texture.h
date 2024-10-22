
#ifndef __TEXTURE_H__
#define __TEXTURE_H__

#include "pglm.h"

#define TEXTURE_SIZE        8u

#define TEXTURE_RED         0u
#define TEXTURE_GREEN       1u
#define TEXTURE_BLUE        2u
#define TEXTURE_YELLOW      3u
#define TEXTURE_MAGENTA     4u
#define TEXTURE_CYAN        5u
#define TEXTURE_BLACK       6u
#define TEXTURE_GRAY        7u
#define TEXTURE_WHITE       8u
#define TEXTURE_DIAGONAL    9u
#define TEXTURE_HEART       10u

typedef uint16_t texture_t[TEXTURE_SIZE][TEXTURE_SIZE];

vec2i tex_coord_vec2f_to_vec2i(vec2f tex_coord);

// Returns the nearest color at the specified coordinates.
uint16_t texture_sample_vec2i(vec2i tex_coord, uint16_t tex_index);

// Returns the nearest color at the specified coordinates.
// Bottom left corner of a texture is (0.0f, 0.0f), while the top left corner is (1.0f, 1.0f).
uint16_t texture_sample_vec2f(vec2f tex_coord, uint16_t tex_index);

#endif // __TEXTURE_H__
