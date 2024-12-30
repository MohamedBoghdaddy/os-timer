#include "threads/thread.h"
#include "threads/interrupt.h"
#include "threads/synch.h"
#include "../lib/list.h"
#include <stdint.h>
#include <stddef.h>
#include <stdio.h>

/* Idle thread placeholder */
static struct thread *idle_thread;

/* List of sleeping threads */
static struct list sleepingList;

/* Initialize the thread system */
void thread_init(void) {
    list_init(&sleepingList);
}

/* Idle thread function */
static void idle(void *aux UNUSED) {
    while (true) {
        thread_block();
    }
}

/* Start the thread system and create the idle thread */
void thread_start(void) {
    idle_thread = thread_create("idle", PRI_MIN, idle, NULL);
    ASSERT(idle_thread != NULL);
}

/* Thread tick function to manage sleeping threads */
void thread_tick(void) {
    int64_t current_tick = timer_ticks();
    thread_wakeup(current_tick);
}

/* Put the current thread to sleep until `wakeup_tick` */
void thread_sleep(int64_t wakeup_tick) {
    struct thread *cur = thread_current();
    enum intr_level old_level;

    ASSERT(cur != idle_thread);

    old_level = intr_disable();
    cur->wakeup_tick = wakeup_tick;
    list_push_back(&sleepingList, &cur->elem);
    thread_block();
    intr_set_level(old_level);
}

/* Wake up threads in the sleeping list if their time has come */
void thread_wakeup(int64_t current_tick) {
    struct list_elem *e = list_begin(&sleepingList);

    while (e != list_end(&sleepingList)) {
        struct thread *t = list_entry(e, struct thread, elem);

        if (t->wakeup_tick <= current_tick) {
            e = list_remove(e);
            thread_unblock(t);
        } else {
            e = list_next(e);
        }
    }
}

/* Function to create a new thread */
struct thread *thread_create(const char *name, int priority, thread_func *function, void *aux) {
    struct thread *new_thread = allocate_thread();  // Function to allocate memory for a new thread
    ASSERT(new_thread != NULL);

    strlcpy(new_thread->name, name, sizeof new_thread->name);
    new_thread->priority = priority;
    thread_unblock(new_thread);

    if (function != NULL) {
        function(aux);
    }

    return new_thread;
}

/* Allocate memory for a new thread */
static struct thread *allocate_thread(void) {
    struct thread *t = malloc(sizeof *t);
    ASSERT(t != NULL);
    return t;
}

/* Blocking the current thread */
void thread_block(void) {
    ASSERT(!intr_context());
    intr_disable();
    thread_current()->status = THREAD_BLOCKED;
    schedule();
}

/* Unblocking a thread */
void thread_unblock(struct thread *t) {
    ASSERT(t->status == THREAD_BLOCKED);
    t->status = THREAD_READY;
    list_push_back(&ready_list, &t->elem);  // Assuming `ready_list` exists
}

/* Get the current thread */
struct thread *thread_current(void) {
    struct thread *t = running_thread();  // Assumes a function to get the running thread
    ASSERT(is_thread(t));
    return t;
}

/* Run the scheduler */
static void schedule(void) {
    // Placeholder for scheduling logic
}

/* Timer functions */
int64_t timer_ticks(void) {
    extern int64_t ticks;  // Declare the global `ticks` variable
    return ticks;
}

/* Initialization for the timer */
void timer_init(void) {
    ticks = 0;
    list_init(&sleepingList);
}

/* Increment timer ticks */
void timer_interrupt(struct intr_frame *args UNUSED) {
    ticks++;
    thread_tick();
}