#include "devices/timer.h"
#include "../threads/thread.h"
#include "../threads/interrupt.h"
#include "../lib/list.h"
#include "../lib/debug.h"

#define UNUSED __attribute__((unused))

/* Global Variables */
static int64_t ticks; // System tick counter
static struct list sleepingList; // List to store sleeping threads

/* Initialize the timer and the sleeping list */
void timer_init(void) {
    ticks = 0;
    list_init(&sleepingList);
}

/* Return the current number of ticks */
int64_t timer_ticks(void) {
    return ticks;
}

/* Sleep the current thread for a given number of ticks */
void timer_sleep(int64_t ticks_to_sleep) {
    if (ticks_to_sleep <= 0) return;

    int64_t wakeup_tick = timer_ticks() + ticks_to_sleep; // Calculate wakeup tick
    struct thread *current_thread = thread_current();

    ASSERT(current_thread != idle_thread); // Ensure the idle thread does not sleep

    enum intr_level old_level = intr_disable(); // Disable interrupts

    current_thread->wakeup_tick = wakeup_tick; // Set the thread's wakeup tick
    list_insert_ordered(&sleepingList, &current_thread->elem, thread_wakeup_compare, NULL); // Insert into sleeping list

    thread_block(); // Block the thread

    intr_set_level(old_level); // Restore interrupt level
}

/* Timer interrupt handler */
void timer_interrupt(struct intr_frame *args UNUSED) {
    ticks++;
    thread_tick(); // Notify the scheduler of the tick
    thread_wakeup(ticks); // Wake up threads if their wakeup_tick has passed
}

/* Wake up threads whose wakeup_tick has passed */
void thread_wakeup(int64_t current_tick) {
    struct list_elem *e = list_begin(&sleepingList);

    while (e != list_end(&sleepingList)) {
        struct thread *t = list_entry(e, struct thread, elem);

        if (t->wakeup_tick <= current_tick) {
            struct list_elem *next = list_next(e);
            list_remove(e); // Remove the thread from the sleeping list
            thread_unblock(t); // Unblock the thread
            e = next; // Move to the next element
        } else {
            break; // Remaining threads have later wakeup ticks
        }
    }
}

/* Compare threads based on their wakeup_tick */
bool thread_wakeup_compare(const struct list_elem *a, const struct list_elem *b, void *aux UNUSED) {
    const struct thread *thread_a = list_entry(a, struct thread, elem);
    const struct thread *thread_b = list_entry(b, struct thread, elem);
    return thread_a->wakeup_tick < thread_b->wakeup_tick;
}

/* Test function to verify timer_sleep functionality */
void test_timer_sleep(void) {
    printf("Test: Sleeping for 100 ticks...\n");
    timer_sleep(100);
    printf("Test: Woke up after 100 ticks.\n");
}

/* Run alarm clock tests */
void run_alarm_clock_tests(void) {
    timer_init();
    test_timer_sleep();
}
