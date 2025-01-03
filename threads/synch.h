#ifndef THREADS_SYNCH_H
#define THREADS_SYNCH_H

#include "../lib/list.h"
#include "../lib/debug.h"
#include <stdbool.h>

/* A counting semaphore. */
struct semaphore {
    unsigned value;              /* Current value. */
    struct list waiters;         /* List of waiting threads. */
};

/* Lock */
struct lock {
    struct thread *holder;       /* Thread holding lock (for debugging). */
    struct semaphore semaphore;  /* Binary semaphore controlling access. */
};

/* Condition variable */
struct condition {
    struct list waiters;         /* List of waiting threads. */
};

/* Semaphore-related functions */
void sema_init(struct semaphore *sema, unsigned value);
void sema_down(struct semaphore *sema);
bool sema_try_down(struct semaphore *sema);
void sema_up(struct semaphore *sema);

/* Lock-related functions */
void lock_init(struct lock *lock);
void lock_acquire(struct lock *lock);
bool lock_try_acquire(struct lock *lock);
void lock_release(struct lock *lock);
bool lock_held_by_current_thread(const struct lock *lock);

/* Condition variable-related functions */
void cond_init(struct condition *cond);
void cond_wait(struct condition *cond, struct lock *lock);
void cond_signal(struct condition *cond, struct lock *lock);
void cond_broadcast(struct condition *cond, struct lock *lock);

#endif /* THREADS_SYNCH_H */