#include "threads/thread.h"
#include "threads/interrupt.h"
#include "threads/synch.h"
#include "../lib/list.h"
#include <stdint.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#ifndef UNUSED
#define UNUSED __attribute__((unused))
#endif

/* Global variables */
struct thread *idle_thread;            /* Pointer to the idle thread */
static struct list ready_list;         /* List of ready threads */
static struct list sleeping_list;      /* List of sleeping threads */
static int64_t ticks = 0;              /* Global timer ticks */

/* Function prototypes */
static struct thread *allocate_thread(void);
static void schedule(void);
static void idle(void *aux UNUSED);

/* Initialize the thread system */
void thread_init(void) {
    list_init(&ready_list);
    list_init(&sleeping_list);
}

/* Allocate memory for a new thread */
static struct thread *allocate_thread(void) {
    struct thread *t = malloc(sizeof(struct thread));
    ASSERT(t != NULL);
    memset(t, 0, sizeof *t);
    t->magic = THREAD_MAGIC;
    return t;
}

/* Create a new thread */
struct thread *thread_create(const char *name, int priority, thread_func *function, void *aux) {
    struct thread *t = allocate_thread();
    ASSERT(t != NULL);

    strncpy(t->name, name, sizeof t->name - 1);
    t->name[sizeof t->name - 1] = '\0';
    t->priority = priority;
    t->status = THREAD_READY;

    list_push_back(&ready_list, &t->elem);

    if (function != NULL) {
        function(aux);
    }

    return t;
}

/* Idle thread function */
static void idle(void *aux UNUSED) {
    while (true) {
        thread_block();
    }
}

/* Start the thread system */
void thread_start(void) {
    idle_thread = thread_create("idle", PRI_MIN, idle, NULL);
    ASSERT(idle_thread != NULL);
}

/* Get the current thread */
struct thread *thread_current(void) {
    return running_thread();
}

/* Block the current thread */
void thread_block(void) {
    ASSERT(!intr_context());

    struct thread *cur = thread_current();
    ASSERT(cur->status == THREAD_RUNNING);

    cur->status = THREAD_BLOCKED;
    schedule();
}

/* Unblock a thread */
void thread_unblock(struct thread *t) {
    ASSERT(is_thread(t));
    ASSERT(t->status == THREAD_BLOCKED);

    t->status = THREAD_READY;
    list_push_back(&ready_list, &t->elem);
}

/* Put the current thread to sleep */
void thread_sleep(int64_t wakeup_tick) {
    struct thread *cur = thread_current();
    ASSERT(cur != idle_thread);

    enum intr_level old_level = intr_disable();
    cur->wakeup_tick = wakeup_tick;
    list_insert_ordered(&sleeping_list, &cur->elem, thread_wakeup_compare, NULL);
    thread_block();
    intr_set_level(old_level);
}

/* Wake up threads whose wakeup_tick has passed */
void thread_wakeup(int64_t current_tick) {
    struct list_elem *e = list_begin(&sleeping_list);

    while (e != list_end(&sleeping_list)) {
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

/* Scheduler */
void schedule(void) {
    struct thread *current = thread_current();
    struct thread *next = NULL;

    ASSERT(intr_get_level() == INTR_OFF);

    if (current->status == THREAD_RUNNING) {
        current->status = THREAD_READY;
        list_push_back(&ready_list, &current->elem);
    }

    if (!list_empty(&ready_list)) {
        struct list_elem *next_elem = list_pop_front(&ready_list);
        next = list_entry(next_elem, struct thread, elem);
    } else {
        next = idle_thread;
    }

    next->status = THREAD_RUNNING;
    thread_switch(current, next);
}

/* Thread context switch function */
void thread_switch(struct thread *current, struct thread *next) {
    ASSERT(current != NULL);
    ASSERT(next != NULL);

    asm volatile (
        "push %%ebp\n"
        "mov %%esp, %[curr_esp]\n"
        "mov %%ebp, %[curr_ebp]\n"
        "mov %[next_esp], %%esp\n"
        "mov %[next_ebp], %%ebp\n"
        "pop %%ebp\n"
        "ret\n"
        :
        : [curr_esp] "m" (current->stack),
          [curr_ebp] "m" (current->ebp),
          [next_esp] "m" (next->stack),
          [next_ebp] "m" (next->ebp)
        : "memory"
    );
}

/* Timer ticks function */
int64_t timer_ticks(void) {
    return ticks;
}

/* Timer initialization */
void timer_init(void) {
    ticks = 0;
    list_init(&sleeping_list);
}

/* Timer interrupt handler */
void timer_interrupt(struct intr_frame *args UNUSED) {
    ticks++;
    thread_tick();
    thread_wakeup(ticks);
}

/* Tick handler for thread system */
void thread_tick(void) {
    int64_t current_tick = timer_ticks();
    thread_wakeup(current_tick);
}

/* Check if the struct is a valid thread */
bool is_thread(struct thread *t) {
    return t != NULL && t->magic == THREAD_MAGIC;
}

/* Architecture-specific implementation to get the running thread */
struct thread *running_thread(void) {
    return idle_thread;  // Placeholder; update as per your architecture
}