
#ifndef __EPSILON_H__
#define __EPSILON_H__

#include "core.h"

#define PGLM_EPSILON 1E-6f

inline bool epsilon_equal(f32 x, f32 y, f32 epsilon) {
    return abs(x-y) < epsilon;
}

inline bool epsilon_not_equal(f32 x, f32 y, f32 epsilon) {
    return abs(x-y) >= epsilon;
}

#endif // __EPSILON_H__
