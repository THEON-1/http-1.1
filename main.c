#include "main.h"
#include <stdlib.h>

#define PROTOCOL_IP 0
#define THREAD_MAX 10
#define CONNECTION_BACKLOG 10

int main (int argc, char *argv[]) {
    int sockfd, status, yes, connection, free_threads;
    struct addrinfo hints, *res, *p, *ipv4;
    socklen_t connection_data_size;
    struct sockaddr connection_data;
    pthread_t new_connection;
    struct thread_args *args;

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;

    if ((status = getaddrinfo(NULL, "9999", &hints, &res)) != 0) {
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

    if ((status = bind (sockfd, ipv4->ai_addr, ipv4->ai_addrlen)) != 0) {
        perror("bind");
        exit(1);
    }

    args = malloc(sizeof *args);
    free_threads = THREAD_MAX;
    while (1) {
        if (free_threads == 0) {
            continue;
        }
        if ((status = listen(sockfd, CONNECTION_BACKLOG)) != 0) {
            perror("listen");
            exit(1);
        }

        (free_threads)--;
        fprintf(stdout, "free_threads: %i", free_threads);

        connection_data_size = sizeof connection_data;
        if ((connection = accept(sockfd, &connection_data, &connection_data_size)) == -1) {
            perror("accept");
            exit(1);
        }

        args->connection = connection;
        args->free_sockets = &free_threads;
        args->connection_data = connection_data;
        args->connection_data_size = connection_data_size;
        
        pthread_create(&new_connection, NULL, (void *)httpConnection, (void *)args);

    }
    
    free(args);
    freeaddrinfo(res);
}

void *httpConnection(struct thread_args *args) {
    int status, buffer_size;
    char *msg;

    buffer_size = 1024;
    msg = malloc(sizeof(char) * buffer_size);

    while ((status = recv(args->connection, (void *)msg, buffer_size, 0)) > 0) {
        fprintf(stdout, "%s\n", msg);
    }

    (args->free_sockets)++;

    if (status < 0) {
        perror("recv");
    }
    return 0;
}

