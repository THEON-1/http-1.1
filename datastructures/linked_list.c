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
    struct list_element *e;
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
    struct list_element *e;
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
    struct list_element *e;
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
        struct list_element *f = l->tail;
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
        struct list_element *f = l->head;
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

void *list_get(struct list *l, int position) {
    if (position > l->size || position < 1) {
        fprintf(stderr, "list index out of bounds: %i in %i", position, l->size);
        exit(1);
    }

    struct list_element *e;
    if (position > (l->size)/2) {
        for (int i = l->size; i > position; i--) {
            e = e->prev;
        }
        return e->data;
    }
    else {
        for (int i = 1; i < position; i++) {
            e = e->next;
        }
        return e->data;
    }
}

void *list_remove(struct list *l, int position) {
    if (position > l->size || position < 1) {
        fprintf(stderr, "list index out of bounds: %i in %i", position, l->size);
        exit(1);
    }

    struct list_element *e;
    void *data;
    if (position == 1) {
        e = l->head;
        l->head = e->next;
        l->head->prev = NULL;
    }
    else if (position == l->size) {
        e = l->tail;
        l->tail = e->prev;
        l->tail->next = NULL;
    }
    else if (position > (l->size)/2) {
        for (int i = l->size; i > position; i--) {
            e = e->prev;
        }
        e->prev->next = e->next;
        e->next->prev = e->prev;
    }
    else {
        for (int i = 1; i < position; i++) {
            e = e->next;
        }
        e->prev->next = e->next;
        e->next->prev = e->prev;
    }
    data = e->data;
    free(e);
    return data;
}

void list_free(struct list *l) {
    struct list_element *e;
    while ((e = l->head) != NULL) {
        l->head = e->next;
        free(e->data);
        free(e);
    }
    free(l);
}

