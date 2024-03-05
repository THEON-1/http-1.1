#include "parser.h"

int parse_http_data(connection_data_t *conn_data, http_data_t *http_data) {
    int status, buffer_size;
    char *msg;

    PARSER_STATE state = EXPECTING_START_LINE;
    char *header_start, *header_end;
    int crlf = 0;

    buffer_size = 1024;
    msg = malloc(sizeof(char) * buffer_size);
    while ((status = recv(conn_data->connection, (void *)msg, buffer_size, 0)) > 0) {
        fprintf(stdout, "%s\n", msg);
        for (char *c = msg; c < msg + status; c++) {
            switch (state) {
                case EXPECTING_START_LINE:
                    header_start = c;
                    state = PARSING_START_LINE;
                case PARSING_START_LINE:
                    if (crlf > 0) {
                        if (*c == LF) {
                            header_end = c;
                            state = EXPECTING_FIELD_LINE;
                            printf("header line received:\n%.*s\n", (int)(header_end - header_start + 1), header_start);
                        }
                        else {
                            printf("unexpected character recieved after CR: %i", *c);
                            exit(1);
                        }
                    }
                    else if (*c == CR) {
                        crlf = 1;
                    }
                    break;
                default:
                    break;
            }
        }
    }

    return status;
}

