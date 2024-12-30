#include "synch.h"
#include "thread.h"
#include "interrupt.h" // Include this for enum intr_level
#include "../lib/list.h"
#include "../lib/debug.h"

/* Initializes semaphore SEMA to VALUE. */
void sema_init(struct semaphore *sema, unsigned value) {
    ASSERT(sema != NULL);
    sema->value = value;
    list_init(&sema->waiters); // Initialize the waiters list
}

/* Down or "P" operation on a semaphore.
   Waits for SEMA's value to become positive and then decrements it. */
void sema_down(struct semaphore *sema) {
    ASSERT(sema != NULL);
    enum intr_level old_level = intr_disable(); // Disable interrupts

    while (sema->value == 0) { // If semaphore is 0, block the thread
        struct thread *cur = thread_current();
        ASSERT(cur != NULL); // Ensure the current thread is valid
        list_push_back(&sema->waiters, &cur->elem); // Add thread to waiters list
        thread_block(); // Block the current thread
    }
    sema->value--; // Decrement semaphore value
    intr_set_level(old_level); // Restore interrupt level
}

/* Up or "V" operation on a semaphore.
   Increments SEMA's value and wakes up one waiting thread, if any. */
void sema_up(struct semaphore *sema) {
    ASSERT(sema != NULL);
    enum intr_level old_level = intr_disable(); // Disable interrupts

    if (!list_empty(&sema->waiters)) { // If there are waiting threads
        struct list_elem *e = list_pop_front(&sema->waiters); // Get the first waiting thread
        ASSERT(e != NULL); // Sanity check
        struct thread *t = list_entry(e, struct thread, elem); // Convert list element to thread
        thread_unblock(t); // Unblock the thread
    }
    sema->value++; // Increment semaphore value
    intr_set_level(old_level); // Restore interrupt level
}
