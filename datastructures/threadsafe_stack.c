#include "threadsafe_stack.h"

struct threadsafe_stack *threadsafe_stack_initialize() {
    struct threadsafe_stack *s;
    s = malloc(sizeof *s);
    s->s.head = NULL;
    s->s.size = 0;
    pthread_mutex_init(&s->mutex, NULL);
    return s;
}

void threadsafe_stack_push(struct threadsafe_stack *s, void *data) {
    pthread_mutex_lock(&s->mutex);
    stack_push((struct stack*)s, data);
    pthread_mutex_unlock(&s->mutex);
}

void *threadsafe_stack_pop(struct threadsafe_stack *s) {
    void *data;
    pthread_mutex_lock(&s->mutex);
    data = stack_pop((struct stack *)s);
    pthread_mutex_unlock(&s->mutex);
    return data;
}

void threadsafe_stack_free(struct threadsafe_stack *s) {
    struct stack_element* e;
    struct stack *t = (struct stack *)s;
    while ((e = t->head) != NULL) {
        t->head = e->next;
        free(e->data);
        free(e);
    }
    free(s);
}

