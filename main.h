#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdlib.h>

#include "datastructures/stack.h"

struct thread_args {
         int connection;
         struct sockaddr connection_data;
         socklen_t connection_data_size;
         struct stack *free_sockets;
         pthread_t *thread_self;
};

int main(int argc, char *argv[]);

void *httpConnection(struct thread_args *args);

