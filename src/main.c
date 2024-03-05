#include "main.h"

#define PROTOCOL_IP 0
#define THREAD_MAX 3
#define CONNECTION_BACKLOG 10

int main (int argc, char *argv[]) {
    int sockfd, status, yes, free_connections;
    struct addrinfo hints, *res, *p, *ipv4;
    struct sockaddr connection_data;
    thread_args *args;
    pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
    pthread_t *free_thread = malloc(sizeof(pthread_t));

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
    free_connections = THREAD_MAX;
    fprintf(stdout, "free threads: %i\n", free_connections);
    while (1) {
        int connection;
        socklen_t connection_data_size;

        if (free_connections == 0) {
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

        pthread_mutex_lock(&mutex);
        free_connections--;
        fprintf(stdout, "free threads: %i\n", free_connections);
        pthread_mutex_unlock(&mutex);

        args->data = (connection_data_t){
            .connection = connection,
            .connection_data = connection_data,
            .connection_data_size = connection_data_size};
        args->free_threads = &free_connections;
        args->mutex = &mutex;

        pthread_create(free_thread, NULL, (void *)httpConnection, (void *)args);
    }
    
    free(args);
    freeaddrinfo(res);
    free(free_thread);
}

void *httpConnection(void *void_args) {
    thread_args *args = (thread_args *)void_args;
    connection_data_t *conn_data = &args->data;
    http_data_t *http_data;
    int status;

    http_data = malloc(sizeof(http_data_t));
    status = parse_http_data(conn_data, http_data);

    pthread_mutex_lock(args->mutex);
    (*args->free_threads)++;
    fprintf(stdout, "free threads: %i\n", *args->free_threads);
    pthread_mutex_unlock(args->mutex);

    if (status < 0) {
        perror("recv");
    }
    return 0;
}

