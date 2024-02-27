#include "main.h"

#define PROTOCOL_IP 0

int main (int argc, char *argv[]) {
    int sockfd, status, yes, connection;
    struct addrinfo hints, *res, *p, *ipv4;
    socklen_t connection_data_size;
    struct sockaddr connection_data;

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

    if ((status = listen(sockfd, 10)) != 0) {
        perror("listen");
        exit(1);
    }

    connection_data_size = sizeof connection_data;
    if ((connection = accept(sockfd, &connection_data, &connection_data_size)) == -1) {
        perror("accept");
        exit(1);
    }

    manageConnection(connection, &connection_data, &connection_data_size);
    
    freeaddrinfo(res);
}

void manageConnection(
        int connection,
        struct sockaddr *connection_data,
        socklen_t *connection_data_size) {
    int status, buffer_size;
    char *msg;

    buffer_size = 1024;
    msg = malloc(sizeof(char) * buffer_size);

    while ((status = recv(connection, (void *)msg, buffer_size, 0)) > 0) {
        fprintf(stdout, "%s\n", msg);
    }
    if (status < 0) {
        perror("recv");
    }
}

