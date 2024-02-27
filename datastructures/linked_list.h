#include <stdlib.h>
#include <stdio.h>

struct element {
    void *data;
    struct element *next;
    struct element *prev;
};

struct list {
    struct element *head;
    struct element *tail;
    int size;
};

struct list *list_initilaize();
void list_append(struct list *l, void *data);
void list_prepend(struct list *l, void *data);
void list_insert(struct list *l, void *data, int position);

