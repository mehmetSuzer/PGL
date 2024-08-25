
#ifndef __FAST_H__
#define __FAST_H__

#include <math.h>

// Set the sign bit to 1. 
inline float fast_absolute(const float number) {
    int i = *(int*)&number;
    int val = i & 0x7FFFFFFFu;
    float y = *(float*)&val;

    return y;
}

// The famous fast inverse square root algorithm in Quake III.
inline float fast_inverse_square_root(const float number) {
    int i;
    float x2, y;

    x2 = number * 0.5f;
    y  = number;
    i  = *(int*)&y;                       // evil floating point bit level hacking
    i  = 0x5f3759df - (i >> 1);           // what the fuck?
    y  = *(float*)&i;
    y  = y * (1.5f - (x2 * y * y));       // 1st iteration
    // y  = y * (1.5f - (x2 * y * y));    // 2nd iteration, this can be removed

    return y;
}

// Calculates the square root of a number by using the famous fast inverse square root algorithm in Quake III.
inline float fast_square_root(const float number) {
    return 1.0f / fast_inverse_square_root(number);
}

// Reduces angle to [-pi, +pi].
inline float reduce_angle(const float radian) {
    float reduced = fmodf(radian, M_TWOPI);
    if (reduced > M_PI) {
        reduced -= M_TWOPI;
    }
    else if (reduced < -M_PI) {
        reduced += M_TWOPI;
    }
    return reduced;
}

// Accurate in [-pi, +pi].
inline float fast_sin(const float radian) {
    const float radian2 = radian * radian;
    return radian * (1.0f - radian2 * (0.16666667f - radian2 * (0.008333334f - radian2 * 1.984127E-4f)));
}

// Accurate in [-inf, +inf].
// If you are sure that radian is in [-pi, +pi], use fast_sin, which is faster.
static inline float fast_sin_extended(const float radian) {
    const float reduced_radian = reduce_angle(radian);
    return fast_sin(reduced_radian);
}

// Accurate in [-pi, +pi].
inline float fast_cos(const float radian) {
    const float radian2 = radian * radian;
    return 1.0f - radian2 * (0.5f - radian2 * (0.041666668f - radian2 * (0.0013888889f - radian2 * 2.4801588E-5f)));
}

// Accurate in [-inf, +inf].
// If you are sure that radian is in [-pi, +pi], use fast_cos, which is faster.
static inline float fast_cos_extended(const float radian) {
    const float reduced_radian = reduce_angle(radian);
    return fast_cos(reduced_radian);
}

// Accurate in [-pi/4, +pi/4].
inline float fast_tan(const float radian) {
    const float radian2 = radian * radian;
    return radian * (1.0f + radian2 * (0.33333333f + radian2 * 0.13333333f));
}

// Accurate in [-pi/2, +pi/2].
// If you are sure that radian is in [-pi/4, +pi/4], use fast_tan, which is faster.
inline float fast_tan_extended(const float radian) {
    if (radian > M_PI_4) {
        return 1.0f / fast_tan(M_PI_2 - radian);
    }
    else if (radian < -M_PI_4) {
        return -1.0f / fast_tan(M_PI_2 + radian);
    }
    else {
        return fast_tan(radian);
    }
}

// Accurate in [-pi/4, +pi/4].
inline float fast_cot(const float radian) {
    return 1.0f / fast_tan(radian);
}

// Accurate in [-pi/2, +pi/2].
// If you are sure that radian is in [-pi/2, +pi/2], use fast_cot, which is faster.
inline float fast_cot_extended(const float radian) {
    return 1.0f / fast_tan_extended(radian);
}

// Accurate in [-1, +1].
inline float fast_arctan(const float number) {
    const float number2 = number * number;
    return number * (0.9951816951228549f + number2 * (0.0776509570923569f * number2 - 0.287434475393028f));
}

// Accurate in [-inf, +inf].
// If you are sure that number is in [-1, +1], use fast_arctan, which is faster.
inline float fast_arctan_extended(const float number) {
    if (number > 1.0f) {
        return M_PI_2 - fast_arctan(1.0f/number);
    }
    else if (number < -1.0f) {
        return fast_arctan(-1.0f/number) - M_PI_2;
    }
    else {
        return fast_arctan(number);
    }
}

#endif // __FAST_H__
