
#ifndef __TEXTURE_H__
#define __TEXTURE_H__

#include "pglm.h"

#define TEXTURE_SIZE 8u

typedef uint16_t texture_t[TEXTURE_SIZE][TEXTURE_SIZE];

extern const texture_t textures[64];

// Returns the nearest color at the specified coordinates.
// Bottom left corner of a texture is (0.0f, 0.0f), while the top left corner is (1.0f, 1.0f).
uint16_t sample_texture(texture_t tex, vec2f tex_coords);

#endif // __TEXTURE_H__
