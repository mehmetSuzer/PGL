
#include "pglm.h"

const vec2f zero_vec2f     = { 0.0f,  0.0f};
const vec2f one_vec2f      = { 1.0f,  1.0f};
const vec2f right_vec2f    = { 1.0f,  0.0f};
const vec2f left_vec2f     = {-1.0f,  0.0f};
const vec2f up_vec2f       = { 0.0f,  1.0f};
const vec2f down_vec2f     = { 0.0f, -1.0f};

const vec3f zero_vec3f     = { 0.0f,  0.0f,  0.0f};
const vec3f one_vec3f      = { 1.0f,  1.0f,  1.0f};
const vec3f right_vec3f    = { 1.0f,  0.0f,  0.0f};
const vec3f left_vec3f     = {-1.0f,  0.0f,  0.0f};
const vec3f up_vec3f       = { 0.0f,  1.0f,  0.0f};
const vec3f down_vec3f     = { 0.0f, -1.0f,  0.0f};
const vec3f forward_vec3f  = { 0.0f,  0.0f,  1.0f};
const vec3f backward_vec3f = { 0.0f,  0.0f, -1.0f};

const quat quat_identity   = { 0.0f,  0.0f,  0.0f,  1.0f};