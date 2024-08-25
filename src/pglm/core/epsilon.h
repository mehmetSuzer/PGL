
#ifndef __EPSILON_H__
#define __EPSILON_H__

#include "core.h"

inline bool epsilon_equal(float x, float y, float epsilon) {
    return abs(x-y) < epsilon;
}

inline bool epsilon_not_equal(float x, float y, float epsilon) {
    return abs(x-y) >= epsilon;
}

#endif // __EPSILON_H__
