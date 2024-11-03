
#ifndef __PGL_QUEUE_H__
#define __PGL_QUEUE_H__

#include "pglm.h"

// Must be a power of 2
#define PGL_QUEUE_CAPACITY 16u

typedef struct {
    vec4f position;
    vec2f tex_coord;
} pgl_vertex_t;

typedef struct {
    pgl_vertex_t v0;
    pgl_vertex_t v1;
    pgl_vertex_t v2;
} pgl_queue_triangle_t;

typedef struct {
    pgl_queue_triangle_t triangles[PGL_QUEUE_CAPACITY];
    u8 front; // next slot for pop
    u8 back;  // next slot for push
    bool is_empty;
} pgl_queue_t;

inline void triangle_queue_init(pgl_queue_t* queue) {
    queue->front = 0;
    queue->back = 0;
    queue->is_empty = true;
}

inline pgl_queue_triangle_t* triangle_queue_front(pgl_queue_t* queue) {
    return (queue->is_empty) ? NULL : queue->triangles + queue->front;
}

inline pgl_queue_triangle_t* triangle_queue_back(pgl_queue_t* queue) {
    return (queue->is_empty) ? NULL : queue->triangles + queue->back;
}

inline bool triangle_queue_is_full(pgl_queue_t* queue) {
    return queue->front == queue->back && !queue->is_empty;
}

inline u32 triangle_queue_length(pgl_queue_t* queue) {
    if (queue->is_empty) {
        return 0;
    }
    else if (queue->front < queue->back) {
        return queue->back - queue->front;
    }
    else {
        return (queue->back + PGL_QUEUE_CAPACITY) - queue->front;
    }
}

inline void triangle_queue_push(pgl_queue_t* queue, pgl_queue_triangle_t* triangle) {
    if (triangle_queue_is_full(queue)) {
        return;
    }
    
    queue->triangles[queue->back] = *triangle;
    queue->back = (queue->back + 1) & (PGL_QUEUE_CAPACITY - 1); // rolls back to 0 when it reaches to PGL_QUEUE_CAPACITY
    queue->is_empty = false;
}

inline pgl_queue_triangle_t* triangle_queue_pop(pgl_queue_t* queue) {
    if (queue->is_empty) {
        return NULL;
    }

    pgl_queue_triangle_t* triangle = queue->triangles + queue->front;
    queue->front = (queue->front + 1) & (PGL_QUEUE_CAPACITY - 1); // rolls back to 0 when it reaches to PGL_QUEUE_CAPACITY
    queue->is_empty = queue->front == queue->back;
    return triangle;
}

#endif // __PGL_QUEUE_H__
