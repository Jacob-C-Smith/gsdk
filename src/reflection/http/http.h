/** !
 * Include header for http library
 * 
 * @file http/http.h 
 * 
 * @author Jacob Smith
 */

// header guard
#pragma once

// standard library
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stdbool.h>
#include <float.h>
#include <errno.h>
#include <ctype.h>

// core
#include <core/log.h>
#include <core/sync.h>

// data
#include <data/dict.h>

// Enumerations
enum http_headers_e
{
    HTTP_HEADER_INVALID           = -1,
    HTTP_HEADER_HEADER_DATE       =  0,
    HTTP_HEADER_HEADER_SET_COOKIE =  1,
    HTTP_HEADER_CONTENT_TYPE      =  2,
    HTTP_HEADER_CONTENT_ENCODING  =  3,
    HTTP_HEADER_CONTENT_LENGUAGE  =  4,
    HTTP_HEADER_CONTENT_LOCATION  =  5,
    HTTP_HEADER_CONTENT_LENGTH    =  6,
    HTTP_HEADER_CONTENT_RANGE     =  7,
    HTTP_HEADER_TRAILER           =  8,
    HTTP_HEADER_TRANSFER_ENCODING =  9,
    HTTP_HEADER_ACCEPT            =  10,
    HTTP_HEADER_ACCEPT_LANGUAGE   =  11,
    HTTP_HEADER_ACCEPT_ENCODING   =  12,
    HTTP_HEADER_HOST              =  13,
    HTTP_HEADER_USER_AGENT        =  14,
    HTTP_HEADER_REFERER           =  15,
    HTTP_HEADER_CONNECTION        =  16,
    HTTP_HEADER_QUANTITY          =  17
};

enum http_request_type_e
{
    HTTP_REQUEST_INVALID  = -1,
    HTTP_REQUEST_GET      =  0,
    HTTP_REQUEST_HEAD     =  1,
    HTTP_REQUEST_POST     =  2,
    HTTP_REQUEST_PUT      =  3,
    HTTP_REQUEST_DELETE   =  4,
    HTTP_REQUEST_CONNECT  =  5,
    HTTP_REQUEST_OPTIONS  =  6,
    HTTP_REQUEST_TRACE    =  7,
    HTTP_REQUEST_PATCH    =  8,
    HTTP_REQUEST_QUANTITY =  9
};

enum http_response_status_e
{
    HTTP_RESPONSE_INVALID                    = -1,
    HTTP_RESPONSE_CONTINUE                   =  0,
    HTTP_RESPONSE_SWITCHING_PROTOCOLS        =  1,
    HTTP_RESPONSE_OK                         =  2,
    HTTP_RESPONSE_CREATED                    =  3,
    HTTP_RESPONSE_NO_CONTENT                 =  4,
    HTTP_RESPONSE_RESET_CONTENT              =  5,
    HTTP_RESPONSE_PARTIAL_CONTENT            =  6,
    HTTP_RESPONSE_MOVED_PERMANENTLY          =  7,
    HTTP_RESPONSE_FOUND                      =  8,
    HTTP_RESPONSE_NOT_MODIFIED               =  9,
    HTTP_RESPONSE_TEMPORARY_REDIRECT         =  10,
    HTTP_RESPONSE_PERMANENT_REDIRECT         =  11,
    HTTP_RESPONSE_BAD_REQUEST                =  12,
    HTTP_RESPONSE_UNAUTHORIZED               =  13,
    HTTP_RESPONSE_FORBIDDEN                  =  14,
    HTTP_RESPONSE_NOT_FOUND                  =  15,
    HTTP_RESPONSE_METHOD_NOT_ALLOWED         =  15,
    HTTP_RESPONSE_LENGTH_REQUIRED            =  17,
    HTTP_RESPONSE_PAYLOAD_TOO_LARGE          =  18,
    HTTP_RESPONSE_URI_TOO_LONG               =  19,
    HTTP_RESPONSE_RANGE_NOT_SATISFIABLE      =  20,
    HTTP_RESPONSE_UPGRADE_REQUIRED           =  21,
    HTTP_RESPONSE_NOT_TOO_MANY_REQUESTS      =  22,
    HTTP_RESPONSE_INTERNAL_SERVER_ERROR      =  23,
    HTTP_RESPONSE_NOT_IMPLEMENTED            =  24,
    HTTP_RESPONSE_SERVICE_UNAVAILABLE        =  25,
    HTTP_RESPONSE_HTTP_VERSION_NOT_SUPPORTED =  26,
    HTTP_RESPONSE_STATUS_QUANTITY            =  27
};

// Data
static const char *http_headers [HTTP_HEADER_QUANTITY] = 
{
    "Date",
    "Set-Cookie",
    "Content-Type",
    "Content-Encoding",
    "Content-Language",
    "Content-Location",
    "Content-Length",
    "Content-Range",
    "Trailer",
    "Transfer-Encoding",
    "Accept",
    "Accept-Language",
    "Accept-Encoding",
    "Host",
    "User-Agent",
    "Referer",
    "Connection"
};

static const char *http_request_types [HTTP_REQUEST_QUANTITY] =
{
    "GET",
    "HEAD",
    "POST",
    "PUT",
    "DELETE",
    "CONNECT",
    "OPTIONS",
    "TRACE",
    "PATCH"
};

static const char *http_response_status_phrases [HTTP_RESPONSE_STATUS_QUANTITY] = 
{
    "Continue",
    "Switching Protocols",
    "OK",
    "Created",
    "No Content",
    "Reset Content",
    "Partial Content",
    "Moved Permanently",
    "Found",
    "Not Modified",
    "Temporary Redirect",
    "Permanent Redirect",
    "Bad Request",
    "Unauthorized",
    "Forbidden",
    "Not Found",
    "Method Not Allowed",
    "Length Required",
    "Payload Too Large",
    "URI Too Long",
    "Range Not Satifiable",
    "Upgrade Required",
    "Too Many Requests",
    "Internal Server Error",
    "Not Implemented",
    "Service Unavailable",
    "HTTP Version Not Supported"
};


static const short http_response_status_codes[HTTP_RESPONSE_STATUS_QUANTITY] = 
{
    100,
    101,
    200,
    201,
    204,
    205,
    206,
    301,
    302,
    304,
    307,
    308,
    400,
    401,
    403,
    404,
    405,
    411,
    413,
    414,
    416,
    426,
    429,
    500,
    501,
    503,
    505
};

// forward declarations
struct http_message_s;

// type definitions
typedef struct   http_message_s http_message;
typedef enum     http_request_type_e    http_request_type;
typedef enum     http_response_status_e http_response_status;
typedef int    (*http_request_response_fn)(http_message _http_request, http_message _http_response);

// structure definitions
struct http_message_s
{
    int keep_alive;
    char *body;
    bool status;

    struct
    {
        struct 
        {
            char *accept;
            char *language;
            char *encoding;
        } accept_headers;
        char *host;
        char *user_agent;
        char *referrer;
        char *connection;
        http_request_type type;
        char *path;
    } request;

    struct 
    {
        struct
        {
            char *encoding;
            char *type;
            char *date;
        } content;
        struct 
        {
            char *set;
        } cookie;
        http_response_status status_code;
    } response;

    struct 
    {
        char *type;
        char *encoding;
        char *language;
        char *location;
        char *length;
        char *range;
    } representation;

    struct
    {
        char *trailer;
        char *transfer_encoding;
    } payload;
};

// initializer
/** !
 * This gets called at runtime before main. 
 * 
 * @param void
 * 
 * @return void
 */
void http_init ( void ) __attribute__((constructor));

// cleanup
/** !
 * This gets called at runtime after main
 * 
 * @param void
 * 
 * @return void
 */
void http_exit ( void ) __attribute__((destructor));
