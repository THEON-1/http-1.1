#include "http_parser.h"
#include <cstring>

int parse_http_data(connection_data_t *conn_data, http_data_t *http_data) {
    int message_size, buffer_size;
    char *msg;

    PARSER_STATE state = EXPECTING_START_LINE;

    char *header_start, *header_end;
    char *field_start, *field_end;
    int *body_length = 0;
    int crlf = 0;

    buffer_size = 1024;
    msg = malloc(sizeof(char) * buffer_size);
    while ((message_size =
        recv(conn_data->connection, (void *)msg, buffer_size, 0)) > 0) {
        printf("%s\n\n", msg);
        for (char *c = msg; c < msg + message_size; c++) {
            switch (state) {
                case EXPECTING_START_LINE:
                    header_start = c;
                    state = PARSING_START_LINE;
                case PARSING_START_LINE:
                    if (check_for_crlf(&crlf, c)) {
                        header_end = c;
                        state = EXPECTING_FIELD_LINE;
                        // -1 instead of +1 to end before crlf
                        printf("header line received:\n%.*s\n", (int)(header_end - header_start - 1), header_start);
                    }
                    break;
                case EXPECTING_FIELD_LINE:
                    if (check_for_crlf(&crlf, c)) {
                        state = EXPECTING_BODY;
                        break;
                    } else if (crlf > 0) {
                        break;
                    } else {
                        field_start = c;
                        state = PARSING_FIELD_LINE;
                    }
                case PARSING_FIELD_LINE:
                    if (check_for_crlf(&crlf, c)) {
                        field_end = c;
                        state = EXPECTING_FIELD_LINE;
                        printf("field line received:\n%.*s\n", (int)(field_end - field_start - 1), field_start);
                    }
                    break;
                case EXPECTING_BODY:
                    printf("remaining data: %i characters\n",
                           (int)(msg - c + message_size - 1));
                    state = DONE;
                    break;
                case DONE:
                    break;
            }
        }
    }

    return message_size;
}

int parse_start_line(char *start, char *end, http_start_line *start_line) {
    char *space_1 = NULL;
    char *space_2 = NULL;
    for (char *c = start; c <= end; c++) {
        if (*c == SP) {
            if (space_1 == NULL) {
                space_1 = c;
            } else if (space_2 == NULL) {
                space_2 = c;
            } else {
                puts("Unexpected space in Header start line");
                exit(1);
            }
        }
    }
    if (space_2 == NULL) {
        puts("Expected 2 spaces in start line!");
        exit(1);
    }

    parse_start_line_type(start, space_1 - 1, start_line);

    if (start_line->type == REQUEST_LINE) {
        char *request_target_start = space_1 + 1;
        char *request_target_end = space_2 - 1;
        int request_target_size = request_target_end - request_target_start + 1;

        switch (*request_target_start) {
            case '/':
                start_line->request.request_target_type = ORIGIN_FORM;
                start_line->request.request_taget = malloc(sizeof(char) * request_target_size);
                strncpy(start_line->request.request_taget, request_target_start, request_target_size);
                start_line->request.request_target_size = request_target_size;
                break;
            case 'h':
                if (!strncmp(request_target_start, "http://", 7)) {

                }
        }
    }
}

void parse_start_line_type(char *start, char *end, http_start_line *start_line) {
    int length = end - start + 1;
    switch (*start) {
        case 'G':
            if (length == 3 && !strncmp(start, "GET", 3)) {
                start_line->type = REQUEST_LINE;
                start_line->request.method = GET;
                return;
            } 
            break;
        case 'H':
            switch (length) {
                case 4:
                    if (!strncmp(start, "HEAD", 4)) {
                        start_line->type = REQUEST_LINE;
                        start_line->request.method = HEAD;
                        return;
                    }
                    break;
                case 8:
                    if (parse_http_version(start, &start_line->status.version)) {
                        start_line->type = STATUS_LINE;
                    }
                    break;
            }
            break;
        case 'P':
            switch (length) {
                case 4:
                    if (!strncmp(start, "POST", 4)) {
                        start_line->type = REQUEST_LINE;
                        start_line->request.method = POST;
                        return;
                    }
                    break;
                case 3:
                    if (!strncmp(start, "PUT", 3)) {
                        start_line->type = REQUEST_LINE;
                        start_line->request.method = PUT;
                        return;
                    }
                    break;
            }
            break;
        case 'D':
            if (length == 6 && !strncmp(start, "DELETE", 6)) {
                start_line->type = REQUEST_LINE;
                start_line->request.method = DELETE;
                return;
            }
            break;
        case 'C':
            if (length == 7 && !strncmp(start, "CONNECT", 7)) {
                start_line->type = REQUEST_LINE;
                start_line->request.method = CONNECT;
                return;
            }
            break;
        case 'O':
            if (length == 7 && !strncmp(start, "OPTIONS", 7)) {
                start_line->type = REQUEST_LINE;
                start_line->request.method = OPTIONS;
                return;
            }
            break;
        case 'T':
            if (length == 5 && !strncmp(start, "TRACE", 5)) {
                start_line->type = REQUEST_LINE;
                start_line->request.method = TRACE;
                return;
            }
            break;
    }
    fprintf(stderr, "unknown start line: %.*s", length, start);
}

int parse_http_version(char *start, http_version *version) {
    char *dot = start + 6;
    char *digit_1 = start + 5;
    char *digit_2 = start + 7;
    if (*dot == '.' && isdigit(*digit_1) && isdigit(*digit_2) && !strncmp(start, "HTTP/", 5)) {
        version->major = *digit_1 - 48;
        version->minor = *digit_2 - 48;
        return 1;
    }
    return 0;
}

int check_for_crlf(int *crlf, char *c) {
    if (*crlf > 0) {
        if (*c == LF) {
            *crlf = 0;
            return 1;
        } else {
            printf("unexpected character received after CR: %i\n", *c);
            exit(1);
        }
    } else if (*c == CR) {
        *crlf = 1;
    }
    return 0;
}

void check_malloc(void *p) {
    if (p == NULL) {
        fputs("memory allocation failed!", stderr);
        exit(1);
    }
}

void unknown_start_line(char *start, char *end) {
}
