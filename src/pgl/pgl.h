
#ifndef __PGL_H__
#define __PGL_H__

#include <stdio.h>
#include "device.h"
#include "texture.h"
#include "mesh.h"

#define PGL_FALSE   false
#define PGL_TRUE    true

// ------------ PGL STATE BITS --------------

// stencil enable:     0b 0000 0000 0000 0001
// stencil write mask: 0b 0000 0000 0000 0010
// stencil function:   0b 0000 0000 0001 1100
// stencil op sfail:   0b 0000 0000 0010 0000 
// stencil op dpfail:  0b 0000 0000 0100 0000
// stencil op dppass:  0b 0000 0000 1000 0000

// depth enable:       0b 0000 0001 0000 0000
// depth write mask:   0b 0000 0010 0000 0000
// depth function:     0b 0001 1100 0000 0000

// cull face:          0b 0110 0000 0000 0000
// cull winding order: 0b 1000 0000 0000 0000

typedef enum {
    PGL_COLOR_BUFFER_BIT    = 0b0001, // Color Buffer Clear Bit
    PGL_DEPTH_BUFFER_BIT    = 0b0010, // Depth Buffer Clear Bit
    PGL_STENCIL_BUFFER_BIT  = 0b0100, // Stencil Buffer Clear Bit
    
    PGL_DEPTH_TEST          = (0b1 << 8), // Depth Test Enable/Disable Bit
    PGL_STENCIL_TEST        = (0b1 << 0), // Stencil Test Enable/Disable Bit

    PGL_CULL_NO             = (0b00 << 13), // Cull no face
    PGL_CULL_FRONT          = (0b01 << 13), // Cull only the front face
    PGL_CULL_BACK           = (0b10 << 13), // Cull only the back face
    PGL_CULL_FRONT_AND_BACK = (0b11 << 13), // Cull both front and back faces

    PGL_CCW                 = (0b0 << 15), // Faces having a counter-clockwise winding order are front faces
    PGL_CW                  = (0b1 << 15), // FAces having a clockwise winding order are front faces

    PGL_NEVER               = 0b000, // Test Function ><=: 000
    PGL_EQUAL               = 0b001, // Test Function ><=: 001
    PGL_LESS                = 0b010, // Test Function ><=: 010
    PGL_LEQUAL              = 0b011, // Test Function ><=: 011
    PGL_GREATER             = 0b100, // Test Function ><=: 100
    PGL_GEQUAL              = 0b101, // Test Function ><=: 101
    PGL_NOTEQUAL            = 0b110, // Test Function ><=: 110
    PGL_ALWAYS              = 0b111, // Test Function ><=: 111

    PGL_KEEP                = 0b0, // The currently stored stencil value is kept
    PGL_REPLACE             = 0b1, // The currently stored stencil value is replaced with the one that is compared 
} pgl_enum_t;

void pgl_view(const vec3f position, const vec3f right, const vec3f up, const vec3f forward);
void pgl_projection(float near, float far, float fovw);
void pgl_viewport(int x, int y, uint32_t width, uint32_t height);

void pgl_enable(pgl_enum_t test);
void pgl_disable(pgl_enum_t test);

void pgl_clear_color(uint16_t color);
void pgl_clear(pgl_enum_t buffer_bits);

void pgl_depth_mask(bool enable_write);
void pgl_depth_func(pgl_enum_t test_func);

void pgl_stencil_mask(bool enable_write);
void pgl_stencil_func(pgl_enum_t test_func);
void pgl_stencil_op(pgl_enum_t sfail, pgl_enum_t dpfail, pgl_enum_t dppass);

void pgl_cull_face(pgl_enum_t face);
void pgl_front_face(pgl_enum_t winding_order);

void pgl_draw(const mesh_t* mesh);
void pgl_display();

#endif // __PGL_H__
