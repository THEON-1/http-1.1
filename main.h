#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdlib.h>

struct thread_args {
    int connection;
    struct sockaddr connection_data;
    socklen_t connection_data_size;
    int *free_threads;
    pthread_mutex_t *mutex;
};

int main(int argc, char *argv[]);

void *httpConnection(void *void_args);

