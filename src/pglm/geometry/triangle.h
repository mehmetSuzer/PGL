
#ifndef __TRIANGLE_H__
#define __TRIANGLE_H__

#include "../geometry/ray.h"
#include "../matrix/mat3f.h"

typedef struct {
    vec3f v0;
    vec3f v1;
    vec3f v2;
} triangle_t;

inline vec3f triangle_normal(triangle_t t) {
    const vec3f edge1 = vec3f_sub(t.v1, t.v0);
    const vec3f edge2 = vec3f_sub(t.v2, t.v0);
    const vec3f normal = vec3f_normalize(vec3f_cross(edge1, edge2));
    return normal;
}

static inline bool triangle_intersects_ray(ray_t ray, triangle_t triangle, f32 near, f32 far, f32* t) {
    const vec3f col0 = vec3f_sub(triangle.v0, triangle.v1);
    const vec3f col1 = vec3f_sub(triangle.v0, triangle.v2);
    const vec3f col2 = ray.dir;
    
    const mat3f matrix = mat3f_from_cols(col0, col1, col2);
    const vec3f vector = vec3f_sub(triangle.v0, ray.source);
    const vec3f solution = mat3f_solve_cramers(matrix, vector);

    // Alpha > epsilon and Beta > epsilon and Alpha + Beta < 1.0 and near < t < far
    if (solution.x > PGLM_EPSILON && solution.y > PGLM_EPSILON && solution.x + solution.y < 1.0f && near < solution.z && solution.z < far) {
        *t = solution.z;
        return true;
    }

    return false;
}

#endif // __TRIANGLE_H__
