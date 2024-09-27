
#ifndef __VEC3I_H__
#define __VEC3I_H__

typedef union {
    struct {
        int x;
        int y;
        int z;
    };

    struct {
        int r;
        int g;
        int b;
    };
} vec3i;

#define zero_vec3i ((vec3i){0, 0, 0})

inline vec3i neg_vec3i(vec3i v) {
    return (vec3i){
        -v.x,
        -v.y,
        -v.z,
    };
}

inline vec3i add_vec3i(vec3i v1, vec3i v2) {
    return (vec3i){
        v1.x + v2.x, 
        v1.y + v2.y, 
        v1.z + v2.z,
    };
}

inline vec3i sub_vec3i(vec3i v1, vec3i v2) {
    return (vec3i){
        v1.x - v2.x, 
        v1.y - v2.y, 
        v1.z - v2.z,
    };
}

inline vec3i mul_vec3i(vec3i v1, vec3i v2) {
    return (vec3i){
        v1.x * v2.x, 
        v1.y * v2.y, 
        v1.z * v2.z,
    };
}

inline vec3i div_vec3i(vec3i v1, vec3i v2) {
    return (vec3i){
        v1.x / v2.x, 
        v1.y / v2.y, 
        v1.z / v2.z,
    };
}

inline vec3i scale_vec3i(vec3i v, int scale) {
    return (vec3i){
        v.x * scale, 
        v.y * scale, 
        v.z * scale,
    };
}

inline bool equal_vec3i(vec3i v1, vec3i v2) {
    return v1.x == v2.x && 
           v1.y == v2.y && 
           v1.z == v2.z;
}

inline bool not_equal_vec3i(vec3i v1, vec3i v2) {
    return v1.x != v2.x ||
           v1.y != v2.y || 
           v1.z != v2.z;
}

#endif // __VEC3I_H__
