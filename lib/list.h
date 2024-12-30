#ifndef LIB_LIST_H
#define LIB_LIST_H

#include <stdbool.h>
#include <stddef.h>

/* List element structure. */
struct list_elem {
    struct list_elem *prev;  /* Previous element. */
    struct list_elem *next;  /* Next element. */
};

struct list {
    struct list_elem head;  /* Head of the list (dummy element). */
    struct list_elem tail;  /* Tail of the list (dummy element). */
};


void list_init(struct list *);

void list_insert(struct list_elem *, struct list_elem *);

void list_remove(struct list_elem *);


struct list_elem *list_begin(struct list *);
struct list_elem *list_next(struct list_elem *);
struct list_elem *list_end(struct list *);


bool list_empty(struct list *);

#define list_entry(LIST_ELEM, STRUCT, MEMBER) \
    ((STRUCT *) ((uint8_t *) (LIST_ELEM) - offsetof(STRUCT, MEMBER)))

#endif 
