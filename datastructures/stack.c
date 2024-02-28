#include "stack.h"

struct stack *stack_initialize() {
    struct stack *s;
    s = malloc(sizeof *s);
    s->head = NULL;
    s->size = 0;
    return s;
}

void stack_push(struct stack *s, void *data) {
    struct stack_element *e;
    e = malloc(sizeof *e);

    e->data = data;
    e->next = s->head;
    s->head = e;
    s->size++;
}

void *stack_pop(struct stack *s) {
    if (s->size == 0) {
        fprintf(stderr, "tried to pop empty stack");
        exit(1);
    }
    struct stack_element *e;
    e = s->head;
    s->head = e->next;
    s->size--;
    return e->data;
}

void stack_free(struct stack *s) {
    struct stack_element* e;
    while ((e = s->head) != NULL) {
        s->head = e->next;
        free(e->data);
        free(e);
    }
    free(s);
}

