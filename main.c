#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include "ring_buffer.h"

ring_buffer_t rx_buffer;

// =======================================================
// SIMULATED UART ISR (Triggers instantly on key press)
// =======================================================
void* simulated_uart_isr(void* arg) {
    int ch;

    // Continuously monitor keyboard inputs as "incoming UART bytes"
    while ((ch = getchar()) != EOF) {
        uint8_t byte_received = (uint8_t)ch;

        // Skip newline character if desired, or push everything
        if (byte_received == '\r') continue;

        // Push byte into ring buffer (as if hardware ISR was triggered)
        if (ring_buffer_push(&rx_buffer, byte_received)) {
            printf("\n  [ISR FIRED] RX Byte: '%c' (0x%02X) -> Pushed to Buffer (Head: %u)\n", 
                   byte_received == '\n' ? ' ' : byte_received, 
                   byte_received, 
                   rx_buffer.head);
        } else {
            printf("\n  [ISR OVERFLOW] Buffer full! Byte '0x%02X' dropped.\n", byte_received);
        }
    }
    return NULL;
}

// =======================================================
// MAIN APPLICATION LOOP (Processes bytes from buffer)
// =======================================================
int main(void) {
    pthread_t isr_thread;
    ring_buffer_init(&rx_buffer);

    printf("====================================================\n");
    printf("     INTERACTIVE UART RING BUFFER SIMULATOR         \n");
    printf("====================================================\n");
    printf("Type characters and press ENTER to send them over 'UART'.\n");
    printf("The main processing loop will wake up every 2 seconds\n");
    printf("and process whatever has accumulated in the buffer.\n");
    printf("----------------------------------------------------\n\n");

    // Start background thread acting as UART Hardware/Keyboard listener
    if (pthread_create(&isr_thread, NULL, simulated_uart_isr, NULL) != 0) {
        printf("Failed to spawn ISR thread!\n");
        return 1;
    }

    uint8_t rx_data = 0;

    // Main processing loop
    while (1) {
        // Sleep 2 seconds to simulate a busy main thread
        sleep(2);

        // Process queued data in the ring buffer
        if (!ring_buffer_is_empty(&rx_buffer)) {
            printf("\n[MAIN LOOP WAKEUP] Processing buffer queue...\n");
            
            while (ring_buffer_pop(&rx_buffer, &rx_data)) {
                if (rx_data == '\n') {
                    printf("  [MAIN LOOP] Read: '\\n' (Line End Detected! Tail: %u)\n", rx_buffer.tail);
                } else {
                    printf("  [MAIN LOOP] Read: '%c' (0x%02X) | Tail: %u | Remaining: %zu\n", 
                           rx_data, rx_data, rx_buffer.tail, ring_buffer_num_elements(&rx_buffer));
                }
            }
            printf("[MAIN LOOP SLEEPING...]\n\n");
        }
    }

    return 0;
}