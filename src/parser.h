#include <sys/socket.h>
#include <stdlib.h>
#include <stdio.h>

#define LF 10
#define CR 13
#define SP 32

typedef enum PARSER_STATE {
    EXPECTING_START_LINE,
    PARSING_START_LINE,
    EXPECTING_FIELD_LINE,
    PARSING_FIELD_LINE,
    EXPECTING_BODY,
    PARSING_BODY,
} PARSER_STATE;

typedef struct http_data_t{
} http_data_t;

typedef struct connection_data_t{
    int connection;
    struct sockaddr connection_data;
    socklen_t connection_data_size;
} connection_data_t;

int parse_http_data(connection_data_t *conn_data, http_data_t *http_data);

