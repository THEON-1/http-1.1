#include "linked_list.h"

struct list *list_initialize() {
    struct list *l;
    l = (struct list *)malloc(sizeof *l);
    l->head = NULL;
    l->tail = NULL;
    l->size = 0;
    return l;
}

void list_append(struct list *l, void *data) {
    struct element *e;
    e = malloc(sizeof *e);

    e->data = data;
    e->next = NULL;
    if (l->size == 0) {
        e->prev = NULL;
        l->head = e;
        l->tail = e;
        l->size = 1;
    }
    else {
        e->prev = l->tail;
        l->tail->next = e;
        l->tail = e;
        (l->size)++;
    }
}

void list_prepend(struct list *l, void *data) {
    struct element *e;
    e = malloc(sizeof *e);

    e->prev = NULL;
    e->data = data;
    if (l->size == 0) {
        e->next = NULL;
        l->head = e;
        l->tail = e;
        l->size = 1;
    }
    else {
        e->next = l->head;
        l->head->prev = e;
        l->head = e;
        (l->size)++;
    }
}

void list_insert(struct list *l, void *data, int position) {
    if (position > l->size + 1 || position < 1) {
        fprintf(stderr, "list index out of bounds: %i in %i", position, l->size);
        exit(1);
    }
    struct element *e;
    e = malloc(sizeof *e);
    e->data = data;
    if (l->size == 0) {
        l->head = e;
        l->tail = e;
        l->size = 1;
        e->prev = NULL;
        e->next = NULL;
    }
    else if (position == 1) {
        list_prepend(l, data);
    }
    else if (position == l->size + 1) {
        list_append(l, data);
    }
    else if (position > (l->size+1)/2) {
        struct element *f = l->tail;
        for (int i = l->size; i >= position; i--) {
            f = f->prev;
        }
        e->next = f->next;
        e->prev = f;
        f->next = e;
        e->next->prev = e;
        (l->size)++;
    }
    else {
        struct element *f = l->head;
        for (int i = 1; i < position; i++) {
            f = f->next;
        }
        e->next = f;
        e->prev = f->prev;
        f->prev = e;
        e->prev->next = e;
        (l->size)++;
    }
}

