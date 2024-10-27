
#include "texture.h"

// Choose a power of 2
#define TEXTURE_RESOLUTION  256u

#define TEXTURE_SAMPLING_PERIOD ((1.0f / (float)TEXTURE_SIZE) + 1E-6f)

const texture_t textures[64] = {
    // Red
    {{0xF800, 0xF800, 0xF800, 0xF800, 0xF800, 0xF800, 0xF800, 0xF800},
     {0xF800, 0xF800, 0xF800, 0xF800, 0xF800, 0xF800, 0xF800, 0xF800},
     {0xF800, 0xF800, 0xF800, 0xF800, 0xF800, 0xF800, 0xF800, 0xF800},
     {0xF800, 0xF800, 0xF800, 0xF800, 0xF800, 0xF800, 0xF800, 0xF800},
     {0xF800, 0xF800, 0xF800, 0xF800, 0xF800, 0xF800, 0xF800, 0xF800},
     {0xF800, 0xF800, 0xF800, 0xF800, 0xF800, 0xF800, 0xF800, 0xF800},
     {0xF800, 0xF800, 0xF800, 0xF800, 0xF800, 0xF800, 0xF800, 0xF800},
     {0xF800, 0xF800, 0xF800, 0xF800, 0xF800, 0xF800, 0xF800, 0xF800}},

    // Green
    {{0x07E0, 0x07E0, 0x07E0, 0x07E0, 0x07E0, 0x07E0, 0x07E0, 0x07E0},
     {0x07E0, 0x07E0, 0x07E0, 0x07E0, 0x07E0, 0x07E0, 0x07E0, 0x07E0},
     {0x07E0, 0x07E0, 0x07E0, 0x07E0, 0x07E0, 0x07E0, 0x07E0, 0x07E0},
     {0x07E0, 0x07E0, 0x07E0, 0x07E0, 0x07E0, 0x07E0, 0x07E0, 0x07E0},
     {0x07E0, 0x07E0, 0x07E0, 0x07E0, 0x07E0, 0x07E0, 0x07E0, 0x07E0},
     {0x07E0, 0x07E0, 0x07E0, 0x07E0, 0x07E0, 0x07E0, 0x07E0, 0x07E0},
     {0x07E0, 0x07E0, 0x07E0, 0x07E0, 0x07E0, 0x07E0, 0x07E0, 0x07E0},
     {0x07E0, 0x07E0, 0x07E0, 0x07E0, 0x07E0, 0x07E0, 0x07E0, 0x07E0}},

    // Blue
    {{0x001F, 0x001F, 0x001F, 0x001F, 0x001F, 0x001F, 0x001F, 0x001F},
     {0x001F, 0x001F, 0x001F, 0x001F, 0x001F, 0x001F, 0x001F, 0x001F},
     {0x001F, 0x001F, 0x001F, 0x001F, 0x001F, 0x001F, 0x001F, 0x001F},
     {0x001F, 0x001F, 0x001F, 0x001F, 0x001F, 0x001F, 0x001F, 0x001F},
     {0x001F, 0x001F, 0x001F, 0x001F, 0x001F, 0x001F, 0x001F, 0x001F},
     {0x001F, 0x001F, 0x001F, 0x001F, 0x001F, 0x001F, 0x001F, 0x001F},
     {0x001F, 0x001F, 0x001F, 0x001F, 0x001F, 0x001F, 0x001F, 0x001F},
     {0x001F, 0x001F, 0x001F, 0x001F, 0x001F, 0x001F, 0x001F, 0x001F}},

    // Yellow
    {{0xFFE0, 0xFFE0, 0xFFE0, 0xFFE0, 0xFFE0, 0xFFE0, 0xFFE0, 0xFFE0},
     {0xFFE0, 0xFFE0, 0xFFE0, 0xFFE0, 0xFFE0, 0xFFE0, 0xFFE0, 0xFFE0},
     {0xFFE0, 0xFFE0, 0xFFE0, 0xFFE0, 0xFFE0, 0xFFE0, 0xFFE0, 0xFFE0},
     {0xFFE0, 0xFFE0, 0xFFE0, 0xFFE0, 0xFFE0, 0xFFE0, 0xFFE0, 0xFFE0},
     {0xFFE0, 0xFFE0, 0xFFE0, 0xFFE0, 0xFFE0, 0xFFE0, 0xFFE0, 0xFFE0},
     {0xFFE0, 0xFFE0, 0xFFE0, 0xFFE0, 0xFFE0, 0xFFE0, 0xFFE0, 0xFFE0},
     {0xFFE0, 0xFFE0, 0xFFE0, 0xFFE0, 0xFFE0, 0xFFE0, 0xFFE0, 0xFFE0},
     {0xFFE0, 0xFFE0, 0xFFE0, 0xFFE0, 0xFFE0, 0xFFE0, 0xFFE0, 0xFFE0}},

    // Magenta
    {{0xF11F, 0xF11F, 0xF11F, 0xF11F, 0xF11F, 0xF11F, 0xF11F, 0xF11F},
     {0xF11F, 0xF11F, 0xF11F, 0xF11F, 0xF11F, 0xF11F, 0xF11F, 0xF11F},
     {0xF11F, 0xF11F, 0xF11F, 0xF11F, 0xF11F, 0xF11F, 0xF11F, 0xF11F},
     {0xF11F, 0xF11F, 0xF11F, 0xF11F, 0xF11F, 0xF11F, 0xF11F, 0xF11F},
     {0xF11F, 0xF11F, 0xF11F, 0xF11F, 0xF11F, 0xF11F, 0xF11F, 0xF11F},
     {0xF11F, 0xF11F, 0xF11F, 0xF11F, 0xF11F, 0xF11F, 0xF11F, 0xF11F},
     {0xF11F, 0xF11F, 0xF11F, 0xF11F, 0xF11F, 0xF11F, 0xF11F, 0xF11F},
     {0xF11F, 0xF11F, 0xF11F, 0xF11F, 0xF11F, 0xF11F, 0xF11F, 0xF11F}},

    // Cyan
    {{0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF},
     {0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF},
     {0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF},
     {0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF},
     {0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF},
     {0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF},
     {0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF},
     {0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF}},

    // Black
    {{0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000},
     {0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000},
     {0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000},
     {0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000},
     {0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000},
     {0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000},
     {0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000},
     {0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000}},

    // Gray
    {{0x8410, 0x8410, 0x8410, 0x8410, 0x8410, 0x8410, 0x8410, 0x8410},
     {0x8410, 0x8410, 0x8410, 0x8410, 0x8410, 0x8410, 0x8410, 0x8410},
     {0x8410, 0x8410, 0x8410, 0x8410, 0x8410, 0x8410, 0x8410, 0x8410},
     {0x8410, 0x8410, 0x8410, 0x8410, 0x8410, 0x8410, 0x8410, 0x8410},
     {0x8410, 0x8410, 0x8410, 0x8410, 0x8410, 0x8410, 0x8410, 0x8410},
     {0x8410, 0x8410, 0x8410, 0x8410, 0x8410, 0x8410, 0x8410, 0x8410},
     {0x8410, 0x8410, 0x8410, 0x8410, 0x8410, 0x8410, 0x8410, 0x8410},
     {0x8410, 0x8410, 0x8410, 0x8410, 0x8410, 0x8410, 0x8410, 0x8410}},

    // White
    {{0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF},
     {0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF},
     {0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF},
     {0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF},
     {0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF},
     {0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF},
     {0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF},
     {0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF}},

    // Red background, blue rectangle and diagonals
    {{0x001F, 0x001F, 0x001F, 0x001F, 0x001F, 0x001F, 0x001F, 0x001F},
     {0x001F, 0x001F, 0xF800, 0xF800, 0xF800, 0xF800, 0x001F, 0x001F},
     {0x001F, 0xF800, 0x001F, 0xF800, 0xF800, 0x001F, 0xF800, 0x001F},
     {0x001F, 0xF800, 0xF800, 0x001F, 0x001F, 0xF800, 0xF800, 0x001F},
     {0x001F, 0xF800, 0xF800, 0x001F, 0x001F, 0xF800, 0xF800, 0x001F},
     {0x001F, 0xF800, 0x001F, 0xF800, 0xF800, 0x001F, 0xF800, 0x001F},
     {0x001F, 0x001F, 0xF800, 0xF800, 0xF800, 0xF800, 0x001F, 0x001F},
     {0x001F, 0x001F, 0x001F, 0x001F, 0x001F, 0x001F, 0x001F, 0x001F}},
     
    // Heart
    {{0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF},
     {0xFFFF, 0x0000, 0x0000, 0xFFFF, 0xFFFF, 0x0000, 0x0000, 0xFFFF},
     {0x0000, 0xFFFF, 0xF800, 0x0000, 0x0000, 0xFFFF, 0xF800, 0x0000},
     {0x0000, 0xF800, 0xF800, 0xF800, 0xF800, 0xF800, 0xF800, 0x0000},
     {0x0000, 0xF800, 0xF800, 0x3800, 0xF800, 0x3800, 0xF800, 0x0000},
     {0xFFFF, 0x0000, 0x3800, 0xF800, 0x3800, 0xF800, 0x0000, 0xFFFF},
     {0xFFFF, 0xFFFF, 0x0000, 0x3800, 0x3800, 0x0000, 0xFFFF, 0xFFFF},
     {0xFFFF, 0xFFFF, 0xFFFF, 0x0000, 0x0000, 0xFFFF, 0xFFFF, 0xFFFF}},
};

vec2i tex_coord_vec2f_to_vec2i(vec2f tex_coord) {
    return (vec2i) {
        TEXTURE_RESOLUTION * tex_coord.u,
        TEXTURE_RESOLUTION * tex_coord.v,
    };
}

u16 texture_sample_vec2i(vec2i tex_coord, u16 tex_index) {
    if (tex_coord.u < 0 || tex_coord.u > TEXTURE_RESOLUTION) {
        tex_coord.u -= (tex_coord.u & ~(TEXTURE_RESOLUTION-1));
    }
    if (tex_coord.v < 0 || tex_coord.v > TEXTURE_RESOLUTION) {
        tex_coord.v -= (tex_coord.v & ~(TEXTURE_RESOLUTION-1));
    }

    const i32 u = tex_coord.u / (TEXTURE_RESOLUTION / TEXTURE_SIZE + 1);
    const i32 v = tex_coord.v / (TEXTURE_RESOLUTION / TEXTURE_SIZE + 1);

    return textures[tex_index][7-v][u];
}

u16 texture_sample_vec2f(vec2f tex_coord, u16 tex_index) {
    if (tex_coord.u < 0.0f || tex_coord.u > 1.0f) {
        tex_coord.u -= floorf(tex_coord.u);
    }
    if (tex_coord.v < 0.0f || tex_coord.v > 1.0f) {
        tex_coord.v -= floorf(tex_coord.v);
    }

    const i32 u = (i32)(tex_coord.u / TEXTURE_SAMPLING_PERIOD);
    const i32 v = (i32)(tex_coord.v / TEXTURE_SAMPLING_PERIOD);

    return textures[tex_index][7-v][u];
}
