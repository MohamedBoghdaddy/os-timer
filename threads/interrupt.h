#ifndef THREADS_INTERRUPT_H
#define THREADS_INTERRUPT_H
#define UNUSED __attribute__((unused))

#include <stdbool.h>
#include <stdint.h>

/* Interrupt levels. */
enum intr_level {
    INTR_OFF, /* Interrupts disabled. */
    INTR_ON   /* Interrupts enabled. */
};

/* Interrupt frame structure. */
struct intr_frame {
    uint32_t edi;
    uint32_t esi;
    uint32_t ebp;
    uint32_t esp_dummy; /* Unused */
    uint32_t ebx;
    uint32_t edx;
    uint32_t ecx;
    uint32_t eax;
    uint16_t gs, padding1;
    uint16_t fs, padding2;
    uint16_t es, padding3;
    uint16_t ds, padding4;
    uint32_t vec_no;
    uint32_t error_code;
    void (*eip)(void);
    uint32_t cs;
    uint32_t eflags;
    void *esp;
    uint32_t ss;
};

/* Function declarations for managing interrupt levels. */
enum intr_level intr_disable(void);                /* Disable interrupts and return previous level. */
enum intr_level intr_enable(void);                 /* Enable interrupts and return previous level. */
enum intr_level intr_set_level(enum intr_level);   /* Set interrupt level and return previous level. */
enum intr_level intr_get_level(void);              /* Get current interrupt level. */
bool intr_context(void);                           /* Check if running in interrupt context. */

#endif /* THREADS_INTERRUPT_H */
