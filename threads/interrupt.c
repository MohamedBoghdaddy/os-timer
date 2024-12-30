#include "interrupt.h"
#include "../lib/list.h"
#include "../lib/debug.h"
#include <stdint.h>
#include <stdio.h>

/* Interrupt stack frame. */
struct intr_frame {
    uint32_t edi;                /* Saved registers. */
    uint32_t esi;
    uint32_t ebp;
    uint32_t esp_dummy;          /* Not used. */
    uint32_t ebx;
    uint32_t edx;
    uint32_t ecx;
    uint32_t eax;
    uint16_t gs, :16;            /* Segment selectors. */
    uint16_t fs, :16;
    uint16_t es, :16;
    uint16_t ds, :16;
    uint32_t vec_no;             /* Interrupt vector number. */
    uint32_t error_code;         /* Error code. */
    uint32_t eip;                /* Saved instruction pointer. */
    uint16_t cs, :16;            /* Code segment. */
    uint32_t eflags;             /* Saved CPU flags. */
    void *esp;                   /* Saved stack pointer. */
    uint16_t ss, :16;            /* Data segment. */
};

void intr_init(void) {
    
    printf("Initializing interrupt subsystem.\n");
}

enum intr_level intr_disable(void) {
    enum intr_level old_level = INTR_OFF;
    __asm volatile("cli" : : : "memory");
    return old_level;
}

enum intr_level intr_enable(void) {
    enum intr_level old_level = INTR_ON;
    __asm volatile("sti" : : : "memory");
    return old_level;
}

enum intr_level intr_set_level(enum intr_level level) {
    if (level == INTR_ON) {
        return intr_enable();
    } else {
        return intr_disable();
    }
}

bool intr_context(void) {
    /* Returns true if the CPU is currently handling an interrupt. */
    return false;  /* Example implementation. Modify as needed. */
}