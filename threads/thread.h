#ifndef THREADS_THREAD_H
#define THREADS_THREAD_H

#include <stdbool.h>
#include <stdint.h>
#include "../lib/list.h"
#include "../lib/debug.h"

/* Thread states */
enum thread_status {
    THREAD_RUNNING,  /* Running thread */
    THREAD_READY,    /* Ready to run but not running */
    THREAD_BLOCKED,  /* Waiting for an event to trigger */
    THREAD_DYING     /* About to be destroyed */
};

/* Thread identifier type */
typedef int tid_t;
#define TID_ERROR ((tid_t)-1) /* Error value for tid_t */

/* Thread priorities */
#define PRI_MIN 0       /* Lowest priority */
#define PRI_DEFAULT 31  /* Default priority */
#define PRI_MAX 63      /* Highest priority */

/* A kernel thread or user process */
struct thread {
    tid_t tid;                          /* Thread identifier */
    enum thread_status status;          /* Thread state */
    char name[16];                      /* Name (for debugging purposes) */
    uint8_t *stack;                     /* Saved stack pointer */
    int priority;                       /* Priority */
    struct list_elem allelem;           /* List element for all threads */
    struct list_elem elem;              /* List element for run queue or sleep queue */
    int64_t wakeup_tick;                /* Tick to wake up */

    unsigned magic;                     /* Detects stack overflow */
};

/* External idle thread declaration */
extern struct thread *idle_thread;

/* Function prototypes */
void thread_init(void);                                     /* Initialize the thread system */
void thread_start(void);                                    /* Start the threading system */
struct thread *thread_current(void);                       /* Get the currently running thread */
void thread_block(void);                                    /* Block the current thread */
void thread_unblock(struct thread *t);                     /* Unblock the given thread */
void thread_tick(void);                                     /* Handle per-tick thread processing */
void thread_sleep(int64_t ticks);                          /* Put the current thread to sleep */
void thread_wakeup(int64_t current_tick);                  /* Wake up threads at the given tick */
bool thread_wakeup_compare(const struct list_elem *a,      /* Compare wakeup ticks for sorting */
                           const struct list_elem *b, 
                           void *aux);
#endif /* THREADS_THREAD_H */
