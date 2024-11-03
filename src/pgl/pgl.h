
#ifndef __PGL_H__
#define __PGL_H__

#include "device.h"
#include "light.h"
#include "texture.h"
#include "mesh.h"

#ifndef DEVICE_SCREEN_WIDTH
#error "DEVICE_SCREEN_WIDTH is not defined."
#endif

#ifndef DEVICE_SCREEN_HEIGHT
#error "DEVICE_SCREEN_HEIGHT is not defined."
#endif

#define DEVICE_SCREEN_ASPECT_RATIO ((float)DEVICE_SCREEN_HEIGHT / (float)DEVICE_SCREEN_WIDTH)

#define PGL_FALSE   false
#define PGL_TRUE    true

typedef enum {
    PGL_DEPTH_TEST   = 0b01,
    PGL_STENCIL_TEST = 0b10,
} pgl_test_t;

typedef enum {
    PGL_COLOR_BUFFER_BIT   = 0b001, // Color   Buffer Clear Bit
    PGL_DEPTH_BUFFER_BIT   = 0b010, // Depth   Buffer Clear Bit
    PGL_STENCIL_BUFFER_BIT = 0b100, // Stencil Buffer Clear Bit
} pgl_buffer_bit_t;

typedef enum {
    PGL_NEVER    = 0b000, // Test Function ><=: 000
    PGL_EQUAL    = 0b001, // Test Function ><=: 001
    PGL_LESS     = 0b010, // Test Function ><=: 010
    PGL_LEQUAL   = 0b011, // Test Function ><=: 011
    PGL_GREATER  = 0b100, // Test Function ><=: 100
    PGL_GEQUAL   = 0b101, // Test Function ><=: 101
    PGL_NOTEQUAL = 0b110, // Test Function ><=: 110
    PGL_ALWAYS   = 0b111, // Test Function ><=: 111
} pgl_test_func_t;

typedef enum {
    PGL_CULL_NO             = 0b00, // Cull no face
    PGL_CULL_FRONT          = 0b01, // Cull only the front face
    PGL_CULL_BACK           = 0b10, // Cull only the back face
    PGL_CULL_FRONT_AND_BACK = 0b11, // Cull both front and back faces
} pgl_cull_face_t;

typedef enum {
    PGL_CCW = 0b0, // Faces having a counter-clockwise winding order are front faces
    PGL_CW  = 0b1, // Faces having a clockwise winding order are front faces
} pgl_cull_winding_order_t;

typedef enum {
    PGL_KEEP    = 0b0, // The currently stored stencil value is kept
    PGL_REPLACE = 0b1, // The currently stored stencil value is replaced with the one that is compared 
} pgl_stencil_op_t;

typedef enum {
    PGL_TEST_DISABLED = 0b0,
    PGL_TEST_ENABLED  = 0b1,
} pgl_enable_t;

typedef enum {
    PGL_UNMASKED = 0b0,
    PGL_MASKED   = 0b1,
} pgl_mask_t;

void pgl_view(vec3f position, vec3f right, vec3f up, vec3f forward);
void pgl_projection(f32 near, f32 far, f32 fovw);
void pgl_viewport(i32 x, i32 y, u32 width, u32 height);

void pgl_clear_color(u16 color);
void pgl_clear(pgl_buffer_bit_t buffer_bits);

void pgl_set_tests(pgl_test_t tests, pgl_enable_t enabled);

void pgl_depth_mask(pgl_mask_t mask);
void pgl_depth_func(pgl_test_func_t test_func);

void pgl_stencil_mask(pgl_mask_t mask);
void pgl_stencil_func(pgl_test_func_t test_func);
void pgl_stencil_op(pgl_stencil_op_t sfail, pgl_stencil_op_t dpfail, pgl_stencil_op_t dppass);

void pgl_cull_face(pgl_cull_face_t face);
void pgl_cull_winding_order(pgl_cull_winding_order_t cull_winding_order);

void pgl_draw(const mesh_t* mesh, const directional_light_t* dl);
void pgl_display();

#endif // __PGL_H__
