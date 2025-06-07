// Header
#include <http/http.h>

// Preprocessor definitions
#define HTTP_HEADER_MAP_SIZE   49
#define HTTP_REQUEST_MAP_SIZE  13
#define HTTP_RESPONSE_MAP_SIZE 126
// Data
static bool initialized = false;
enum http_headers_e         _http_headers[HTTP_HEADER_MAP_SIZE]             = { 0 };
enum http_response_status_e _http_response_statuses[HTTP_RESPONSE_MAP_SIZE] = { 0 };
enum http_request_type_e    _http_request_types[HTTP_REQUEST_MAP_SIZE]      = { 0 };

void http_init ( void )
{

    // State check
    if ( initialized == true ) return;

    // Initialize the log library
    log_init();

    // Initialize the sync library
    sync_init();

    // Initialize the hash cache library
    hash_cache_init();

    // Initialize the maps
    memset(&_http_headers          , -1, sizeof(_http_headers));
    memset(&_http_request_types    , -1, sizeof(_http_request_types));
    memset(&_http_response_statuses, -1, sizeof(_http_response_statuses));

    // Populate the headers map
    for (size_t i = 0; i < HTTP_HEADER_QUANTITY; i++)

        // Store the header enum
        _http_headers[ hash_fnv64(http_headers[i], strlen(http_headers[i])) % HTTP_HEADER_MAP_SIZE ] = i;

    // Populate the request map
    for (size_t i = 0; i < HTTP_REQUEST_QUANTITY; i++)

        // Store the request enum
        _http_request_types[ hash_fnv64(http_request_types[i], strlen(http_request_types[i])) % HTTP_REQUEST_MAP_SIZE ] = i;

    // Populate the response map
    for (size_t i = 0; i < HTTP_RESPONSE_STATUS_QUANTITY; i++)

        // Store the response enum
        _http_response_statuses[ hash_fnv64(http_response_status_phrases[i], strlen(http_response_status_phrases[i])) % HTTP_RESPONSE_MAP_SIZE ] = i;

    // Set the initialized flag
    initialized = true;

    // Done
    return;
}

void http_exit ( void )
{
    
    // State check
    if ( initialized == false ) return;

    // Clean up the log library
    log_exit();

    // Clean up the sync library
    sync_exit();

    // Clean up the hash cache library
    hash_cache_exit();

    // Clear the initialized flag
    initialized = false;

    // Done
    return;
}
