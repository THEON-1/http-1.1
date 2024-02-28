#include <pthread.h>

#include "stack.h"

struct threadsafe_stack {
    struct stack s;
    pthread_mutex_t mutex;
};

struct threadsafe_stack *threadsafe_stack_initialize();
void threadsafe_stack_push(struct threadsafe_stack *s, void *data);
void *threadsafe_stack_pop(struct threadsafe_stack *s);
void threadsafe_stack_free(struct threadsafe_stack *s);

