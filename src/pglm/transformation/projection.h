
#ifndef __PROJECTION_H__
#define __PROJECTION_H__

#include "../matrix/mat4f.h"

inline mat4f orthographic(f32 width, f32 height, f32 near, f32 far) {
    const f32 inverse_far_minus_near = 1.0f / (far - near);
    mat4f result = mat4f_zero;
	
    result.xx =  2.0f / width;
	result.yy =  2.0f / height;
	result.zz = -2.0f * inverse_far_minus_near;
    result.zw = -(far + near) * inverse_far_minus_near;
    result.ww = 1.0f;
    
    return result;
}

inline mat4f perspective(f32 fovw, f32 aspect, f32 near, f32 far) {
    const f32 inverse_tan_half_fov = 1.0f / tanf(fovw * 0.5f);
    const f32 inverse_far_minus_near = 1.0f / (far - near);
	mat4f result = mat4f_zero;

	result.xx =  inverse_tan_half_fov;
	result.yy =  inverse_tan_half_fov * aspect;
	result.zz = -(far + near) * inverse_far_minus_near;
	result.zw = -2.0f * far * near * inverse_far_minus_near;
    result.wz = -1.0f;
	
    return result;    
}

#endif // __PROJECTION_H__
