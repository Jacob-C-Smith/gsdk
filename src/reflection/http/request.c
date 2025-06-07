#include <http/http.h>

int http_serialize_request (
    char *p_request_text,
    http_message *p_http_request
)
{

    // Argument check
    if ( p_request_text == (void *) 0 ) goto no_request_text;
    if ( p_http_request == (void *) 0 ) goto no_request_parser;

    // Write the request headers
    if ( p_http_request->request.accept_headers.accept )   p_request_text += sprintf(p_request_text, "Accept: %s\n", p_http_request->request.accept_headers.accept);
    if ( p_http_request->request.accept_headers.language ) p_request_text += sprintf(p_request_text, "Accept-Language: %s\n", p_http_request->request.accept_headers.language);
    if ( p_http_request->request.accept_headers.encoding ) p_request_text += sprintf(p_request_text, "Accept-Encoding: %s\n", p_http_request->request.accept_headers.encoding);
    if ( p_http_request->request.host )                    p_request_text += sprintf(p_request_text, "Host: %s\n", p_http_request->request.host);
    if ( p_http_request->request.user_agent )              p_request_text += sprintf(p_request_text, "User-Agent: %s\n", p_http_request->request.user_agent);
    if ( p_http_request->request.referrer )                p_request_text += sprintf(p_request_text, "Referer: %s\n", p_http_request->request.referrer);
    if ( p_http_request->request.connection )              p_request_text += sprintf(p_request_text, "Connection: %s\n", p_http_request->request.connection);

    // Write the response headers
    if ( p_http_request->response.content.date ) p_request_text += sprintf(p_request_text, "Date: %s\n", p_http_request->response.content.date);
    if ( p_http_request->response.cookie.set )   p_request_text += sprintf(p_request_text, "Set-Cookie: %s\n", p_http_request->response.cookie.set);
 
    // Write the representation headers
    if ( p_http_request->representation.type )     p_request_text += sprintf(p_request_text, "Content-Type: %s\n", p_http_request->representation.type);
    if ( p_http_request->representation.encoding ) p_request_text += sprintf(p_request_text, "Content-Encoding: %s\n", p_http_request->representation.encoding);
    if ( p_http_request->representation.language ) p_request_text += sprintf(p_request_text, "Content-Language: %s\n", p_http_request->representation.language);
    if ( p_http_request->representation.location ) p_request_text += sprintf(p_request_text, "Content-Location: %s\n", p_http_request->representation.location);
    if ( p_http_request->representation.length )   p_request_text += sprintf(p_request_text, "Content-Length: %s\n", p_http_request->representation.length);
    if ( p_http_request->representation.range )    p_request_text += sprintf(p_request_text, "Content-Range: %s\n", p_http_request->representation.range);

    // Write the payload headers
    if ( p_http_request->payload.trailer )           p_request_text += sprintf(p_request_text, "Trailer: %s\n", p_http_request->payload.trailer);
    if ( p_http_request->payload.transfer_encoding ) p_request_text += sprintf(p_request_text, "Transfer-Encoding: %s\n", p_http_request->payload.transfer_encoding);

    // Success
    return 1;

    // Error handling
    {

        // Argument errors
        {
            no_request_text:
                #ifndef NDEBUG
                    log_error("[http] Null pointer provided for parameter \"request_text\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;

            no_request_parser:
                #ifndef NDEBUG
                    log_error("[http] Null pointer provided for parameter \"pfn_request_parser\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }
    }
}

int http_parse_request (
    const char *p_request_text,
    http_message *p_http_request
)
{

    // Initialized data
    char *http_request_after_type = 0,
         *http_request_after_path = 0;
    dict *p_request_headers       = 0;

    printf("%s\n",p_request_text);

    // Get the request type
    http_request_after_type = strchr(p_request_text, ' '),
    
    // Insert a null terminator
    *http_request_after_type = '\0';

    // Increment the pointer past the null terminator
    http_request_after_type++;

    // Get the request path
    http_request_after_path = strchr(http_request_after_type, ' ');

    // Insert a null terminator
    *http_request_after_path = '\0';

    // Increment the pointer past the null terminator
    http_request_after_path++;
    
    // Return the request type to the caller
    //*p_request_type = http_request_type_hash_table_mmh64[crypto_mmh64(request_text, strlen(request_text)) % HTTP_REQUEST_TYPE_MMH64_HASH_TABLE_COUNT];

    // Return the requested path to the caller
    //*pp_path = http_request_after_type;

    //dict_construct(&p_request_headers, 32, 0);
    
    /*
    char *m = http_request_after_path;

    // Main response parser
    while (*m != '\0')
    {
        char *val = strchr(m, ":");
        char *ln = strchr(val, '\n');
        *ln = '\0';
        *val = '\0';
        ln++;
        dict_add(p_request_headers, m, val);
        m=ln+1;
    }*/

    // Success
    return 1;
}
