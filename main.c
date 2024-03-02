#include "main.h"

#define PROTOCOL_IP 0
#define THREAD_MAX 3
#define CONNECTION_BACKLOG 10

int main (int argc, char *argv[]) {
    int sockfd, status, yes, connection;
    struct addrinfo hints, *res, *p, *ipv4;
    socklen_t connection_data_size;
    struct sockaddr connection_data;
    struct thread_args *args;
    struct threadsafe_stack *free_threads;

    if (argc != 2) {
        fprintf(stderr, "invalid number of arguments given!");
        exit(1);
    }
    printf("port given: %s\n", argv[1]);

    memset(&hints, 0, sizeof hints);
   hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;
    if ((status = getaddrinfo(NULL, argv[1], &hints, &res)) != 0) {
        fprintf(stderr, "getaddressinfo error %s\n", gai_strerror(status));
        exit(1);
    }

    for (p = res; p != NULL; p = p->ai_next) {
        if (p->ai_family == AF_INET) {
            ipv4 = p;
        }
    }
    if (ipv4 == NULL) {
        fprintf(stderr, "getaddressinfo did not return an ipv4 configuration!");
        exit(1);
    }

    sockfd = socket(ipv4->ai_family, ipv4->ai_socktype, ipv4->ai_protocol);

    if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof yes) == -1) {
        perror("setsockopt");
        exit(1);
    }

    printf("binding to port %s\n", argv[1]);
    if ((status = bind (sockfd, ipv4->ai_addr, ipv4->ai_addrlen)) != 0) {
        perror("bind");
        exit(1);
    }

    args = malloc(sizeof *args);
    free_threads = threadsafe_stack_initialize();
    for (int i = 0; i < THREAD_MAX; i++){
        pthread_t *thread = malloc(sizeof(pthread_t));
        threadsafe_stack_push(free_threads, thread);
    }
    fprintf(stdout, "free threads: %i\n", free_threads->s.size);
    while (1) {
        pthread_t *free_thread;
        if (free_threads->s.size == 0) {
            continue;
        }
        if ((status = listen(sockfd, CONNECTION_BACKLOG)) != 0) {
            perror("listen");
            exit(1);
        }
        connection_data_size = sizeof connection_data;

        if ((connection = accept(sockfd, &connection_data, &connection_data_size)) == -1) {
            perror("accept");
            exit(1);
        }

        free_thread = (pthread_t *)threadsafe_stack_pop(free_threads);
        fprintf(stdout, "free threads: %i\n", free_threads->s.size);

        args->connection = connection;
        args->connection_data = connection_data;
        args->connection_data_size = connection_data_size;
        
        args->thread_self = free_thread;
        args->free_sockets = free_threads;
        
        pthread_create(free_thread, NULL, (void *)httpConnection, (void *)args);
    }
    
    free(args);
    freeaddrinfo(res);
}

void *httpConnection(void *void_args) {
    struct thread_args *args = (struct thread_args *)void_args;
    int status, buffer_size;
    char *msg;

    buffer_size = 1024;
    msg = malloc(sizeof(char) * buffer_size);

    while ((status = recv(args->connection, (void *)msg, buffer_size, 0)) > 0) {
        fprintf(stdout, "%s\n", msg);
    }

    threadsafe_stack_push(args->free_sockets, args->thread_self);
    fprintf(stdout, "free threads: %i\n", args->free_sockets->s.size);

    if (status < 0) {
        perror("recv");
    }
    return 0;
}

