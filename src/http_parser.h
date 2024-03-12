#include <sys/socket.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "http_types.h"

//US-ASCII characters
#define LF 10
#define CR 13
#define SP 32

//basic message parsing logic
typedef enum PARSER_STATE {
    EXPECTING_START_LINE,
    PARSING_START_LINE,
    EXPECTING_FIELD_LINE,
    PARSING_FIELD_LINE,
    EXPECTING_BODY,
    PARSING_BODY,
    DONE,
} PARSER_STATE;

typedef struct connection_data_t{
    int connection;
    struct sockaddr connection_data;
    socklen_t connection_data_size;
} connection_data_t;

int parse_http_data(connection_data_t *conn_data, http_data_t *http_data);

//start-line parsing
int parse_start_line(char *start, char *end, http_start_line *start_line);
void parse_start_line_type(char *start, char *end, http_start_line *start_line);
int parse_http_version(char *start, http_version *version);
 
//utility functions
int check_for_crlf(int *crlf, char *c);
void check_malloc(void *p);
void unknown_start_line(char *start, char *end);

