
#ifndef __PGL_QUEUE_H__
#define __PGL_QUEUE_H__

#include "pglm.h"

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
} pgl_queue_t;

inline void triangle_queue_init(pgl_queue_t* queue) {
    queue->front = 0;
    queue->back = 0;
}

inline bool triangle_queue_is_empty(const pgl_queue_t* queue) {
    return queue->front == queue->back;
}

inline bool triangle_queue_is_full(const pgl_queue_t* queue) {
    return (queue->back + 1) % PGL_QUEUE_CAPACITY == queue->front;
}

inline u32 triangle_queue_length(const pgl_queue_t* queue) {
    return (queue->front <= queue->back) ? queue->back - queue->front : 
        (queue->back + PGL_QUEUE_CAPACITY) - queue->front;
}

// WARNING: Does nothing if the queue is full.
inline void triangle_queue_push(pgl_queue_t* queue, const pgl_queue_triangle_t* triangle) {
    if (triangle_queue_is_full(queue)) { return; }
    queue->triangles[queue->back] = *triangle;
    queue->back = (queue->back + 1) % PGL_QUEUE_CAPACITY;
}

// WARNING: Returns NULL if the queue is empty. 
inline pgl_queue_triangle_t* triangle_queue_pop(pgl_queue_t* queue) {
    if (triangle_queue_is_empty(queue)) { return NULL; }
    pgl_queue_triangle_t* foremost_triangle = queue->triangles + queue->front;
    queue->front = (queue->front + 1) % PGL_QUEUE_CAPACITY;
    return foremost_triangle;
}

#endif // __PGL_QUEUE_H__
