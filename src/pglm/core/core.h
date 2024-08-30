
#ifndef __DEFINES_H__
#define __DEFINES_H__

#include <math.h>
#include <stdint.h>
#include <stdbool.h>

#ifndef NULL
#define NULL ((void *)0)
#endif 

#ifndef M_SQRT1_3
#define M_SQRT1_3 (1.0f/M_SQRT3)
#endif

#define smaller(x, y)       ((x < y)   ? (x)   :  (y))
#define greater(x, y)       ((x > y)   ? (x)   :  (y))
#define abs(x)              ((x >= 0)  ? (x)   : -(x))
#define clamp(x, min, max)  ((x < min) ? (min) : (x > max) ? (max) : (x))

#endif // __DEFINES_H__
