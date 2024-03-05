#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>

#include "parser.h"

typedef struct thread_args{
    connection_data_t data;
    int *free_threads;
    pthread_mutex_t *mutex;
} thread_args;

int main(int argc, char *argv[]);

void *httpConnection(void *void_args);

