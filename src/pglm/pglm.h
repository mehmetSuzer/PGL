
#ifndef __MATHS_H__
#define __MATHS_H__

#include "core/defines.h"
#include "core/fast.h"
#include "core/epsilon.h"

#include "vector/vec2f.h"
#include "vector/vec3f.h"
#include "vector/vec4f.h"

#include "matrix/mat2f.h"
#include "matrix/mat3f.h"
#include "matrix/mat4f.h"

#include "quat/quat.h"

extern const vec2f zero_vec2f;
extern const vec2f one_vec2f;
extern const vec2f right_vec2f;
extern const vec2f left_vec2f;
extern const vec2f up_vec2f;
extern const vec2f down_vec2f;

extern const vec3f zero_vec3f;
extern const vec3f one_vec3f;
extern const vec3f right_vec3f;
extern const vec3f left_vec3f;
extern const vec3f up_vec3f;
extern const vec3f down_vec3f;
extern const vec3f forward_vec3f;
extern const vec3f backward_vec3f;

extern const quat quat_identity;

#endif // __MATHS_H__
