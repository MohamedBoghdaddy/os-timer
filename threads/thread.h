#ifndef THREADS_THREAD_H
#define THREADS_THREAD_H

#include <stdbool.h>
#include <stdint.h>
#include "../lib/list.h"
#include "../lib/debug.h"

/* Thread states */
enum thread_status {
    THREAD_RUNNING,     /* Running thread. */
    THREAD_READY,       /* Not running but ready to run. */
    THREAD_BLOCKED,     /* Waiting for an event to trigger. */
    THREAD_DYING        /* About to be destroyed. */
};

/* Thread identifier type */
typedef int tid_t;
#define TID_ERROR ((tid_t)-1)

/* Thread priorities */
#define PRI_MIN 0
#define PRI_DEFAULT 31
#define PRI_MAX 63

/* Magic number for thread validation */
#define THREAD_MAGIC 0xcd6abf4b

/* Function pointer type for thread functions */
typedef void thread_func(void *aux);

/* A kernel thread or user process */
struct thread {
    tid_t tid;                      /* Thread identifier. */
    enum thread_status status;      /* Thread state. */
    char name[16];                  /* Name (for debugging purposes). */
    uint8_t *stack;                 /* Saved stack pointer. */
    int priority;                   /* Priority. */
    struct list_elem allelem;       /* List element for all threads list. */
    struct list_elem elem;          /* List element. */
    int64_t wakeup_tick;            /* Tick at which the thread should wake up. */
    unsigned magic;                 /* Detects stack overflow. */
};

/* External idle thread declaration */
extern struct thread *idle_thread;

/* Function prototypes */
void thread_init(void);
void thread_start(void);
struct thread *thread_current(void);
struct thread *thread_create(const char *name, int priority, thread_func *function, void *aux);
void thread_block(void);
void thread_unblock(struct thread *t);
void thread_tick(void);
void thread_sleep(int64_t ticks);
void thread_wakeup(int64_t current_tick);
bool thread_wakeup_compare(const struct list_elem *a, const struct list_elem *b, void *aux);
void thread_switch(struct thread *current, struct thread *next);

#endif /* THREADS_THREAD_H */