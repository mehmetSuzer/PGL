
#ifndef __PROJECTION_H__
#define __PROJECTION_H__

#include "../matrix/mat4f.h"

inline mat4f orthographic(f32 width, f32 height, f32 near, f32 far) {
    const f32 inverse_far_minus_near = 1.0f / (far - near);
    
    const f32 xx =  2.0f / width;
	const f32 yy =  2.0f / height;
	const f32 zz = -2.0f * inverse_far_minus_near;
    const f32 zw = -(far + near) * inverse_far_minus_near;
    const f32 ww = 1.0f;
    
    return (mat4f){{
          xx, 0.0f, 0.0f, 0.0f,
        0.0f,   yy, 0.0f, 0.0f,
        0.0f, 0.0f,   zz,   zw,
        0.0f, 0.0f, 0.0f,   ww,
    }};
}

inline mat4f perspective(f32 fovw, f32 aspect, f32 near, f32 far) {
    const f32 inverse_tan_half_fovw = 1.0f / tanf(fovw * 0.5f);
    const f32 inverse_far_minus_near = 1.0f / (far - near);

	const f32 xx =  inverse_tan_half_fovw;
	const f32 yy =  inverse_tan_half_fovw * aspect;
	const f32 zz = -(far + near) * inverse_far_minus_near;
	const f32 zw = -2.0f * far * near * inverse_far_minus_near;
    const f32 wz = -1.0f;
	
    return (mat4f){{
          xx, 0.0f, 0.0f, 0.0f,
        0.0f,   yy, 0.0f, 0.0f,
        0.0f, 0.0f,   zz,   zw,
        0.0f, 0.0f,   wz, 0.0f,
    }};
}

#endif // __PROJECTION_H__
