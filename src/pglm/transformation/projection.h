
#ifndef __PROJECTION_H__
#define __PROJECTION_H__

#include "../matrix/mat4f.h"

inline mat4f orthographic(float width, float height, float near, float far) {
    const float inverse_far_minus_near = 1.0f / (far - near);
    mat4f result = identity_mat4f;
	
    result.xx =  2.0f / width;
	result.yy =  2.0f / height;
	result.zz = -2.0f * inverse_far_minus_near;
    result.zw = -(far + near) * inverse_far_minus_near;

    return result;
}

// FOV must be maximum pi/2
inline mat4f perspective(float fov, float aspect, float near, float far) {
    const float inverse_tan_half_fov = 1.0f / tanf(fov * 0.5f);
    const float inverse_far_minus_near = 1.0f / (far - near);
	mat4f result = zero_mat4f;

	result.xx =  inverse_tan_half_fov;
	result.yy =  inverse_tan_half_fov * aspect;
	result.zz = -(far + near) * inverse_far_minus_near;
	result.zw = -2.0f * far * near * inverse_far_minus_near;
    result.wz = -1.0f;
	
    return result;    
}

#endif // __PROJECTION_H__
