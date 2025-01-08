// interrupt.c
#include "interrupt.h"
#include "../lib/list.h"
#include "../lib/debug.h"
#include <stdint.h>
#include <stdio.h>

void intr_init(void) {
    printf("Initializing interrupt subsystem.\n");
}

enum intr_level intr_disable(void) {
    enum intr_level old_level = INTR_OFF;
    __asm__ volatile("cli" : : : "memory");
    return old_level;
}

enum intr_level intr_enable(void) {
    enum intr_level old_level = INTR_ON;
    __asm__ volatile("sti" : : : "memory");
    return old_level;
}

enum intr_level intr_set_level(enum intr_level level) {
    if (level == INTR_ON) {
        return intr_enable();
    } else {
        return intr_disable();
    }
}

enum intr_level intr_get_level(void) {
    uint32_t eflags;
    __asm__ volatile("pushfl; popl %0" : "=r"(eflags));
    return (eflags & 0x200) ? INTR_ON : INTR_OFF;
}

bool intr_context(void) {
    /* Returns true if the CPU is currently handling an interrupt. */
    return false;  /* Example implementation. Modify as needed. */
}