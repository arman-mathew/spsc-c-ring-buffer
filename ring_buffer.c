#include "ring_buffer.h"

void ring_buffer_init(ring_buffer_t *rb) {
    if (rb == NULL) return;
    rb->head = 0;
    rb->tail = 0;
}

bool ring_buffer_is_full(const ring_buffer_t *rb) {
    // Full when head is 1 step behind tail (wrapping using mask)
    return (((rb->head + 1) & RING_BUFFER_MASK) == (rb->tail & RING_BUFFER_MASK));
}

bool ring_buffer_is_empty(const ring_buffer_t *rb) {
    return ((rb->head & RING_BUFFER_MASK) == (rb->tail & RING_BUFFER_MASK));
}

bool ring_buffer_push(ring_buffer_t *rb, uint8_t data) {
    if (ring_buffer_is_full(rb)) {
        return false; // Buffer overflow protection
    }

    // Write data at head index
    rb->buffer[rb->head & RING_BUFFER_MASK] = data;

    // Increment head atomically relative to consumer
    rb->head++;
    return true;
}

bool ring_buffer_pop(ring_buffer_t *rb, uint8_t *data) {
    if (ring_buffer_is_empty(rb)) {
        return false; // Underflow protection
    }

    // Read data at tail index
    *data = rb->buffer[rb->tail & RING_BUFFER_MASK];

    // Increment tail atomically relative to producer
    rb->tail++;
    return true;
}

size_t ring_buffer_num_elements(const ring_buffer_t *rb) {
    return (rb->head - rb->tail);
}