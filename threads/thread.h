#ifndef THREADS_THREAD_H
#define THREADS_THREAD_H

#include "../lib/list.h"
#include "../lib/debug.h"
#include <stdint.h>

/* Thread states */
enum thread_status {
    THREAD_RUNNING,  /* Running thread */
    THREAD_READY,    /* Ready to run but not running */
    THREAD_BLOCKED,  /* Waiting for an event to trigger */
    THREAD_DYING     /* About to be destroyed */
};

/* Thread identifier type */
typedef int tid_t;
#define TID_ERROR ((tid_t) -1)  /* Error value for tid_t */

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

    /* Shared between thread.c and synch.c */
    struct list_elem elem;              /* List element */

    /* Tick to wake up the thread */
    int64_t wakeup_tick;                /* Tick at which the thread should wake up */

#ifdef USERPROG
    /* Owned by userprog/process.c */
    uint32_t *pagedir;                  /* Page directory */
#endif

    /* Additional data members can be added here */

    unsigned magic;                     /* Detects stack overflow */
};

/* If false (default), use round-robin scheduler.
   If true, use multi-level feedback queue scheduler.
   Controlled by kernel command-line option "-o mlfqs". */
extern bool thread_mlfqs;

/* Thread functions */
void thread_init(void);
void thread_start(void);

void thread_tick(void);
void thread_print_stats(void);

typedef void thread_func(void *aux);
tid_t thread_create(const char *name, int priority, thread_func *, void *);

void thread_block(void);
void thread_unblock(struct thread *);

struct thread *thread_current(void);
tid_t thread_tid(void);
const char *thread_name(void);

void thread_exit(void) NO_RETURN;
void thread_yield(void);

/* Thread comparison functions for wakeup tick */
bool thread_wakeup_compare(const struct list_elem *a, const struct list_elem *b, void *aux);

/* For advanced scheduler */
int thread_get_priority(void);
void thread_set_priority(int);
int thread_get_nice(void);
void thread_set_nice(int);
int thread_get_recent_cpu(void);
int thread_get_load_avg(void);

#endif /* THREADS_THREAD_H */