/* Include necessary headers */
#include "threads/thread.h"
#include "threads/interrupt.h"
#include "threads/synch.h"
#include "devices/timer.h"

/* Helper function to compare wake-up ticks for sleeping threads. */
bool thread_compare_ticks(const struct list_elem *a, const struct list_elem *b, void *aux UNUSED) {
    struct thread *t_a = list_entry(a, struct thread, elem);
    struct thread *t_b = list_entry(b, struct thread, elem);
    return t_a->wake_up_tick < t_b->wake_up_tick;
}
