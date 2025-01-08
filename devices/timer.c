// timer.c
#include "devices/timer.h"
#include "threads/thread.h"
#include "threads/interrupt.h"
#include "../lib/list.h"
#include "../lib/debug.h"
#include <stdio.h>

#define UNUSED __attribute__((unused))

/* Global Variables */
static int64_t ticks;                /* Number of timer ticks since OS boot */
static struct list sleepingList;     /* List of sleeping threads */

/* Initialize the timer */
void timer_init(void) {
    ticks = 0;
    list_init(&sleepingList);
}

/* Get the current tick count */
int64_t timer_ticks(void) {
    return ticks;
}

/* Compare threads by wakeup_tick for sleeping list */
bool thread_wakeup_compare(const struct list_elem *a, const struct list_elem *b, void *aux UNUSED) {
    const struct thread *t_a = list_entry(a, struct thread, elem);
    const struct thread *t_b = list_entry(b, struct thread, elem);
    return t_a->wakeup_tick < t_b->wakeup_tick;
}

/* Put the current thread to sleep for the given number of ticks */
void timer_sleep(int64_t ticks_to_sleep) {
    if (ticks_to_sleep <= 0) return;

    struct thread *current_thread = thread_current();
    current_thread->wakeup_tick = timer_ticks() + ticks_to_sleep;

    enum intr_level old_level = intr_disable();
    list_insert_ordered(&sleepingList, &current_thread->elem, thread_wakeup_compare, NULL);
    thread_block();
    intr_set_level(old_level);
}

/* Wake up threads whose wakeup_tick has passed */
void thread_wakeup(int64_t current_tick) {
    struct list_elem *e = list_begin(&sleepingList);

    while (e != list_end(&sleepingList)) {
        struct thread *t = list_entry(e, struct thread, elem);

        if (t->wakeup_tick <= current_tick) {
            struct list_elem *next = list_next(e);
            list_remove(e);
            thread_unblock(t);
            e = next;
        } else {
            break; // Remaining threads have later wakeup ticks
        }
    }
}

/* Timer interrupt handler */
void timer_interrupt(struct intr_frame *args UNUSED) {
    ticks++;
    thread_tick();        // Notify the scheduler of the tick
    thread_wakeup(ticks); // Wake up threads if their wakeup_tick has passed
}