#ifndef RING_BUFFER_H
#define RING_BUFFER_H

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

// Must be a power of 2 (e.g., 16, 32, 64, 128, 256)
#define RING_BUFFER_SIZE 64 
#define RING_BUFFER_MASK (RING_BUFFER_SIZE - 1) // this is used for wrapping indices 

typedef struct {
    uint8_t buffer[RING_BUFFER_SIZE];
    volatile uint32_t head; // Written by Producer
    volatile uint32_t tail; // Written by Consumer
} ring_buffer_t;

// API Functions
void ring_buffer_init(ring_buffer_t *rb);
bool ring_buffer_push(ring_buffer_t *rb, uint8_t data);
bool ring_buffer_pop(ring_buffer_t *rb, uint8_t *data);
bool ring_buffer_is_full(const ring_buffer_t *rb);
bool ring_buffer_is_empty(const ring_buffer_t *rb);
size_t ring_buffer_num_elements(const ring_buffer_t *rb);

#endif // RING_BUFFER_H