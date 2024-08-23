
#ifndef __EPSILON_H__
#define __EPSILON_H__

#include <stdbool.h>
#include "fast.h"

inline bool epsilon_equal(const float x, const float y, const float epsilon) {
    return fast_absolute(x-y) < epsilon;
}

inline bool epsilon_not_equal(const float x, const float y, const float epsilon) {
    return fast_absolute(x-y) >= epsilon;
}

#endif // __EPSILON_H__
