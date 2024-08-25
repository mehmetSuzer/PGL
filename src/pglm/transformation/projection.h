
#ifndef __PROJECTION_H__
#define __PROJECTION_H__

#include "../matrix/mat4f.h"

inline mat4f orthographic(const float width ,const float height, const float near, const float far) {
    const float inverse_far_minus_near   = 1.0f / (far - near);
    mat4f result = diagonal_mat4f(1.0f);
	
    result.xx =  2.0f / width;
	result.yy =  2.0f / height;
	result.zz = -2.0f * inverse_far_minus_near;
    result.zw = -(far + near) * inverse_far_minus_near;

    return result;
}

// FOV must be maximum pi/2
inline mat4f perspective(const float fov, const float aspect, const float near, const float far) {
    const float inverse_tan_half_fov = 1.0f / fast_tan(fov * 0.5f);
    const float inverse_far_minus_near = 1.0f / (far - near);
	mat4f result = diagonal_mat4f(0.0f);

	result.xx =  inverse_tan_half_fov;
	result.yy =  inverse_tan_half_fov * aspect;
	result.zz = -(far + near) * inverse_far_minus_near;
	result.zw = -2.0f * far * near * inverse_far_minus_near;
    result.wz = -1.0f;
	
    return result;    
}

#endif // __PROJECTION_H__
