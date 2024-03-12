#include <netdb.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>

#include "http_parser.h"

typedef struct thread_args {
  connection_data_t data;
  int *free_threads;
  pthread_mutex_t *mutex;
} thread_args;

int main(int argc, char *argv[]);

void *httpConnection(void *void_args);
