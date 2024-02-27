#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>
#include <stdlib.h>

int main(int argc, char *argv[]);

void manageConnection(
        int connection,
        struct sockaddr *connection_data,
        socklen_t *connection_data_size);

