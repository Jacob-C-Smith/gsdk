// header
#include <http/http.h>

// preprocessor definitions
#define HTTP_HEADER_MAP_SIZE   49
#define HTTP_REQUEST_MAP_SIZE  13
#define HTTP_RESPONSE_MAP_SIZE 126
// Data
static bool initialized = false;
enum http_headers_e         _http_headers[HTTP_HEADER_MAP_SIZE]             = { 0 };
enum http_response_status_e _http_response_statuses[HTTP_RESPONSE_MAP_SIZE] = { 0 };
enum http_request_type_e    _http_request_types[HTTP_REQUEST_MAP_SIZE]      = { 0 };
