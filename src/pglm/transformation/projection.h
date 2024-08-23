
#ifndef __PROJECTION_H__
#define __PROJECTION_H__

#include "../matrix/mat4f.h"

inline mat4f orthographic(const float left, const float right, const float bottom, const float top, const float near, const float far) {
    const float one_over_right_minus_left = 1.0f / (right - left);
    const float one_over_top_minus_bottom = 1.0f / (top - bottom);
    const float one_over_far_minus_near   = 1.0f / (far - near);
    mat4f result = fix_value_mat4f(1.0f);
	
    result.xx =  2.0f * one_over_right_minus_left;
	result.yy =  2.0f * one_over_top_minus_bottom;
	result.zz = -2.0f * one_over_far_minus_near;
	result.xw = -(right + left) * one_over_right_minus_left;
	result.yw = -(top + bottom) * one_over_top_minus_bottom;
    result.zw = -(far + near)   * one_over_far_minus_near;

    return result;
}

// FOV must be maximum pi/2
inline mat4f perspective(const float fovy, const float aspect, const float near, const float far) {
    const float one_over_tan_half_fovy = 1.0f / fast_tan(fovy * 0.5f);
    const float one_over_far_minus_near = 1.0f / (far - near);
	mat4f result = fix_value_mat4f(0.0f);

	result.xx =  one_over_tan_half_fovy / aspect;
	result.yy =  one_over_tan_half_fovy;
	result.zz = -(far + near) * one_over_far_minus_near;
	result.wz = -1.0f;
	result.zw = -2.0f * far * near * one_over_far_minus_near;
	
    return result;    
}

#endif // __PROJECTION_H__
