
#ifndef __VEC4I_H__
#define __VEC4I_H__

typedef union {
    struct {
        int x;
        int y;
        int z;
        int w;
    };

    struct {
        int r;
        int g;
        int b;
        int a;
    };
    int n[4];
} vec4i;

#define zero_vec4i ((vec4i){0, 0, 0, 0})

inline vec4i neg_vec4i(vec4i v) {
    return (vec4i){
        -v.x,
        -v.y,
        -v.z,
        -v.w,
    };
}

inline vec4i add_vec4i(vec4i v1, vec4i v2) {
    return (vec4i){
        v1.x + v2.x, 
        v1.y + v2.y, 
        v1.z + v2.z,
        v1.w + v2.w,
    };
}

inline vec4i sub_vec4i(vec4i v1, vec4i v2) {
    return (vec4i){
        v1.x - v2.x, 
        v1.y - v2.y, 
        v1.z - v2.z,
        v1.w - v2.w,
    };
}

inline vec4i mul_vec4i(vec4i v1, vec4i v2) {
    return (vec4i){
        v1.x * v2.x, 
        v1.y * v2.y, 
        v1.z * v2.z,
        v1.w * v2.w,
    };
}

inline vec4i div_vec4i(vec4i v1, vec4i v2) {
    return (vec4i){
        v1.x / v2.x, 
        v1.y / v2.y, 
        v1.z / v2.z,
        v1.w / v2.w,
    };
}

inline vec4i scale_vec4i(vec4i v, int scale) {
    return (vec4i){
        v.x * scale, 
        v.y * scale, 
        v.z * scale,
        v.w * scale,
    };
}

inline bool equal_vec4i(vec4i v1, vec4i v2) {
    return v1.x == v2.x && 
           v1.y == v2.y && 
           v1.z == v2.z &&
           v1.w == v2.w;
}

inline bool not_equal_vec4i(vec4i v1, vec4i v2) {
    return v1.x != v2.x ||
           v1.y != v2.y || 
           v1.z != v2.z ||
           v1.w != v2.w;
}

#endif // __VEC4I_H__
