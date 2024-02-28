#include <stdio.h>
#include <stdlib.h>

struct stack_element {
    void *data;
    struct stack_element *next;
};

struct stack {
    struct stack_element *head;
    int size;
};

struct stack *stack_initialize();
void stack_push(struct stack *s, void *data);
void *stack_pop(struct stack *s);
void stack_free(struct stack *s);

