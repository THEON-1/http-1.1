
typedef struct http_data_t{
} http_data_t;

typedef struct http_version {
    int major;
    int minor;
} http_version;

typedef enum HTTP_START_LINE_TYPE {
    REQUEST_LINE,
    STATUS_LINE,
} HTTP_START_LINE_TYPE;

typedef enum HTTP_START_LINE_REQUEST_METHOD {
    GET,
    HEAD,
    POST,
    PUT,
    DELETE,
    CONNECT,
    OPTIONS,
    TRACE,
} HTTP_START_LINE_REQUEST_METHOD;

typedef enum HTTP_START_LINE_REQUEST_TARGET_TYPE {
    ORIGIN_FORM,
    ABSOLUTE_FORM,
    AUTHORITY_FORM,
    ASTERISK_FORM,
} HTTP_START_LINE_REQUEST_TARGET_TYPE;

typedef struct http_start_line_request {
    HTTP_START_LINE_REQUEST_METHOD method;
    HTTP_START_LINE_REQUEST_TARGET_TYPE request_target_type;
    int request_target_size;
    char *request_taget;
    http_version http_version;
} http_start_line_request;

typedef enum HTTP_STATUS_CODE {
    CONTINUE = 100,
    SWITCHING_PROTOCOLS = 101,
    OK = 200,
    CREATED = 201,
    ACCEPTED = 202,
    NON_AUTHOTITATIVE_INFORMATION = 203,
    NO_CONTENT = 204,
    RESET_CONTENT = 205,
    PARTIAL_CONTENT = 206,
    MULTIPLE_CHOICES = 300,
    MOVED_PERMANENTLY = 301,
    FOUND = 302,
    SEE_OTHER = 303,
    NOT_MODIFIED = 304,
    USE_PROXY = 305,
    TEMPORARY_REDIRECT = 307,
    PERMANENT_REDIRECT = 308,
    BAD_REQUEST = 400,
    UNAUTHORIZED = 401,
    PAYMENT_REQUIRED = 402,
    FORBIDDEN = 403,
    NOT_FOUND = 404,
    METHOD_NOT_AVAILABLE = 405,
    NOT_ACCEPTABLE = 406,
    PROXY_AUTHENTICATION_REQUIRED = 407,
    REQUEST_TIMEOUT = 408,
    CONFLICT = 409,
    GONE = 410,
    LENGTH_REQUIRED = 411,
    PRECONDITION_FAILED = 412,
    CONTENT_TOO_LARGE = 413,
    URO_TOO_LONG = 414,
    UNSUPORTED_MEDIA_TYPE = 415,
    RANGE_NOT_SATISFIABLE = 416,
    EXPECTATION_FAILED = 417,
    MISDIRCTED_REQUEST = 421,
    UNPROCESSABLE_CONTENT = 422,
    UPGRADE_REQUIRED = 426,
    INTERNAL_SERVER_ERROR = 500,
    NOT_IMPLEMENTED = 501,
    BAD_GATEWAY = 502,
    SERVICE_UNAVAILABLE = 503,
    GATEWAY_TIMEOUT = 504,
    HTTP_VERSION_NOT_SUPPORTED = 505,
} HTTP_STATUS_CODE;

typedef struct http_start_line_status {
    http_version version;
    HTTP_STATUS_CODE status_code;
    char *reason_phrase;
} http_start_line_status;

typedef struct http_start_line {
    HTTP_START_LINE_TYPE type;
    union {
        http_start_line_request request;
        http_start_line_status status;
    };
} http_start_line;

