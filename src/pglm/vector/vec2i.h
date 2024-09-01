
#ifndef __VEC2I_H__
#define __VEC2I_H__

typedef struct {
    int x;
    int y;
} vec2i;

inline void swap_vec2i(vec2i* v1, vec2i* v2) {
    vec2i temp = *v1;
    *v1 = *v2;
    *v2 = temp;
}

inline vec2i neg_vec2i(const vec2i v) {
    return (vec2i) {
        -v.x,
        -v.y,
    };
}

inline vec2i add_vec2i(const vec2i v1, const vec2i v2) {
    return (vec2i) {
        v1.x + v2.x, 
        v1.y + v2.y, 
    };
}

inline vec2i sub_vec2i(const vec2i v1, const vec2i v2) {
    return (vec2i) {
        v1.x - v2.x, 
        v1.y - v2.y, 
    };
}

inline vec2i mul_vec2i(const vec2i v1, const vec2i v2) {
    return (vec2i) {
        v1.x * v2.x, 
        v1.y * v2.y, 
    };
}

inline vec2i div_vec2i(const vec2i v1, const vec2i v2) {
    return (vec2i) {
        v1.x / v2.x, 
        v1.y / v2.y, 
    };
}

inline vec2i scale_vec2i(const vec2i v, const float scale) {
    return (vec2i) {
        (int)(v.x * scale), 
        (int)(v.y * scale), 
    };
}

inline bool equal_vec2i(const vec2i v1, const vec2i v2) {
    return v1.x == v2.x && 
           v1.y == v2.y;
}

inline bool not_equal_vec2i(const vec2i v1, const vec2i v2) {
    return v1.x != v2.x ||
           v1.y != v2.y;
}

#endif // __VEC2I_H__
