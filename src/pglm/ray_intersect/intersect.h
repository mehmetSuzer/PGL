
#ifndef __INTERSECT_H__
#define __INTERSECT_H__

#include "ray.h"
#include "primitive.h"
#include "../matrix/mat3f.h"

static inline bool intersect_sphere(const ray_t ray, const sphere_t sphere, float near, float far, float* t) {
    const vec3f center_to_source = sub_vec3f(ray.source, sphere.center);
    const float dist2 = mag2_vec3f(center_to_source);
    const float dot = dot_vec3f(center_to_source, ray.dir);
    const float quarter_discriminant = dot * dot - dist2 + sphere.radius * sphere.radius;

    if (quarter_discriminant < 0.0f) {
        return false;
    }

    const float low = -dot - sqrtf(quarter_discriminant); 

    if (near < low && low < far) {
        *t = low;
        return true;
    }
    
    return false;
}

static inline bool intersect_triangle(const ray_t ray, const triangle_t triangle, float near, float far, float* t) {
    const vec3f col0 = sub_vec3f(triangle.a, triangle.b);
    const vec3f col1 = sub_vec3f(triangle.a, triangle.c);
    const vec3f col2 = ray.dir;
    
    const mat3f matrix = mat3f_from_cols(col0, col1, col2);
    const vec3f vector = sub_vec3f(triangle.a, ray.source);
    const vec3f solution = solve_cramers_mat3f(matrix, vector);

    // Alpha > epsilon and Beta > epsilon and Alpha + Beta < 1.0 and near < t < far
    if (solution.x > 0.0f && solution.y > 0.0f && solution.x + solution.y < 1.0f && near < solution.z && solution.z < far) {
        *t = solution.z;
        return true;
    }

    return false;
}

#endif // __INTERSECT_H__
