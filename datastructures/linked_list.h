#include <stdlib.h>
#include <stdio.h>

struct list_element {
    void *data;
    struct list_element *next;
    struct list_element *prev;
};

struct list {
    struct list_element *head;
    struct list_element *tail;
    int size;
};

struct list *list_initilaize();

void list_append(struct list *l, void *data);
void list_prepend(struct list *l, void *data);
void list_insert(struct list *l, void *data, int position);

void *list_get(struct list *l, int position);
void *list_remove(struct list *l, int position);

void list_free(struct list *l);

