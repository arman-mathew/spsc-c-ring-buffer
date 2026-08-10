# High-Performance Lock-Free SPSC Ring Buffer in C

A lightweight, deterministic, lock-free Single-Producer Single-Consumer (SPSC) ring buffer implemented in standard C (C99). Optimized for high-speed embedded systems, real-time interrupt service routines (ISRs), and UART/DMA data queueing.

---

## Key Features

* **$O(1)$ Single-Cycle Execution:** Replaces heavy modulo (`%`) division with bitwise masking (`& RING_BUFFER_MASK`) for fast index wrapping.
* **Lock-Free SPSC Architecture:** Safe concurrent access between a single writer (Producer/ISR) and a single reader (Consumer/Main Loop) without mutexes or critical sections.
* **Zero Dynamic Allocation:** Static memory footprint with zero risk of heap fragmentation or `malloc()` failure.
* **Overflow Protection:** Built-in safeguards against buffer overrun and data corruption.

---

## Memory & Performance Design

| Parameter | Specification |
| :--- | :--- |
| **Capacity** | $64\text{ Bytes}$ (Power-of-2 required for bitwise wrapping) |
| **Index Wrapping** | `(index & 63)` (1 CPU Clock Cycle) |
| **Thread Safety** | Atomic index updates with `volatile` qualifiers |
| **Allocation** | Stack / Static Global Memory |

---

## Project Structure

```text
├── ring_buffer.h     # Interface, buffer struct, and bitwise macro definitions
├── ring_buffer.c     # Core ring buffer push/pop implementation
├── main.c            # Multi-threaded UART ISR simulation & interactive test runner
└── README.md         # Documentation