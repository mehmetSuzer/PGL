
#ifndef __TRIANGLE_QUEUE_H__
#define __TRIANGLE_QUEUE_H__

#include "pglm.h"

typedef struct {
    vec4f c0;
    vec4f c1;
    vec4f c2;
} Triangle;

typedef struct {
    Triangle triangles[16];
    uint32_t front; // next slot for pop
    uint32_t back;  // next slot for push
    bool empty;
} TriangleQueue;

inline void triangle_queue_init(TriangleQueue* queue) {
    queue->front = 0;
    queue->back = 0;
    queue->empty = true;
}

inline Triangle* triangle_queue_front(TriangleQueue* queue) {
    return (queue->empty) ? NULL : queue->triangles + queue->front;
}

inline Triangle* triangle_queue_back(TriangleQueue* queue) {
    return (queue->empty) ? NULL : queue->triangles + queue->back;
}

inline bool triangle_queue_full(TriangleQueue* queue) {
    return queue->front == queue->back && !queue->empty;
}

inline uint32_t triangle_queue_length(TriangleQueue* queue) {
    if (queue->empty) {
        return 0;
    }
    else if (queue->front < queue->back) {
        return queue->back - queue->front;
    }
    else {
        return (queue->back + 16) - queue->front;
    }
}

inline void triangle_queue_push(TriangleQueue* queue, Triangle* triangle) {
    if (triangle_queue_full(queue)) {
        return;
    }
    
    queue->triangles[queue->back] = *triangle;
    queue->back = (queue->back + 1) & 0xF; // rolls back to 0 when it reaches 16
    queue->empty = false;
}

inline Triangle* triangle_queue_pop(TriangleQueue* queue) {
    if (queue->empty) {
        return NULL;
    }

    Triangle* triangle = queue->triangles + queue->front;
    queue->front = (queue->front + 1) & 0xF; // rolls back to 0 when it reaches 16
    queue->empty = queue->front == queue->back;
    return triangle;
}

#endif // __TRIANGLE_QUEUE_H__
