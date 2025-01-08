#ifndef LIB_LIST_H
#define LIB_LIST_H

#include <stdbool.h>
#include <stddef.h>

/* List element structure. */
struct list_elem {
    struct list_elem *prev;  /* Previous element. */
    struct list_elem *next;  /* Next element. */
};

/* List structure. */
struct list {
    struct list_elem head;  /* Head of the list (dummy element). */
    struct list_elem tail;  /* Tail of the list (dummy element). */
};

/* Function prototypes */
void list_init(struct list *);
void list_insert(struct list_elem *, struct list_elem *);
void list_remove(struct list_elem *);
struct list_elem *list_begin(struct list *);
struct list_elem *list_next(struct list_elem *);
struct list_elem *list_end(struct list *);
bool list_empty(struct list *);

/* Additional list functions */
void list_push_back(struct list *, struct list_elem *);
void list_push_front(struct list *, struct list_elem *);
struct list_elem *list_pop_front(struct list *);
struct list_elem *list_pop_back(struct list *);
void list_insert_ordered(struct list *, struct list_elem *, 
                         bool (*less)(const struct list_elem *, const struct list_elem *, void *), 
                         void *aux);

/* Macro to get the containing struct from a list element */
#define list_entry(LIST_ELEM, STRUCT, MEMBER) \
    ((STRUCT *) ((uint8_t *) (LIST_ELEM) - offsetof(STRUCT, MEMBER)))

#endif /* LIB_LIST_H */