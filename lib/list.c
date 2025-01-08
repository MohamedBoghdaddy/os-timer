// list.c
#include "lib/list.h"
#include <stddef.h>

/* Initialize a list */
void list_init(struct list *list) {
    list->head.prev = NULL;
    list->head.next = &list->tail;
    list->tail.prev = &list->head;
    list->tail.next = NULL;
}

/* Insert an element into the list */
void list_insert(struct list_elem *before, struct list_elem *elem) {
    elem->prev = before->prev;
    elem->next = before;
    before->prev->next = elem;
    before->prev = elem;
}

/* Remove an element from the list */
void list_remove(struct list_elem *elem) {
    elem->prev->next = elem->next;
    elem->next->prev = elem->prev;
}

/* Get the first element of the list */
struct list_elem *list_begin(struct list *list) {
    return list->head.next;
}

/* Get the next element in the list */
struct list_elem *list_next(struct list_elem *elem) {
    return elem->next;
}

/* Get the end of the list */
struct list_elem *list_end(struct list *list) {
    return &list->tail;
}

/* Check if the list is empty */
bool list_empty(struct list *list) {
    return list->head.next == &list->tail;
}

/* Push an element to the back of the list */
void list_push_back(struct list *list, struct list_elem *elem) {
    list_insert(list_end(list), elem);
}

/* Push an element to the front of the list */
void list_push_front(struct list *list, struct list_elem *elem) {
    list_insert(list_begin(list), elem);
}

/* Pop the first element from the list */
struct list_elem *list_pop_front(struct list *list) {
    struct list_elem *front = list_begin(list);
    list_remove(front);
    return front;
}

/* Pop the last element from the list */
struct list_elem *list_pop_back(struct list *list) {
    struct list_elem *back = list->tail.prev;
    list_remove(back);
    return back;
}

/* Insert an element into the list in sorted order */
void list_insert_ordered(struct list *list, struct list_elem *elem,
                         bool (*less)(const struct list_elem *, const struct list_elem *, void *),
                         void *aux) {
    struct list_elem *e;

    for (e = list_begin(list); e != list_end(list); e = list_next(e)) {
        if (less(elem, e, aux)) {
            break;
        }
    }
    list_insert(e, elem);
}