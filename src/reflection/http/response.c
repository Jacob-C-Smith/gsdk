#include <http/http.h>

int http_serialize_response ( const char *p_response_text, http_message *p_http_response )
{

    // Argument check
    if ( p_response_text == (void *) 0 ) goto no_response_text;
    if ( p_http_response == (void *) 0 ) goto no_response_parser;

    // Write the response line
    p_response_text += sprintf(p_response_text, "%d %s\r\n", http_response_status_codes[p_http_response->response.status_code], http_response_status_phrases[p_http_response->response.status_code]);

    // Write the response headers
    if ( p_http_response->response.content.date ) p_response_text += sprintf(p_response_text, "Date: %s\n");
    if ( p_http_response->response.cookie.set )   p_response_text += sprintf(p_response_text, "Set-Cookie: %s\n");
 
    // Write the representation headers
    if ( p_http_response->representation.type )     p_response_text += sprintf(p_response_text, "Content-Type: %s\n");
    if ( p_http_response->representation.encoding ) p_response_text += sprintf(p_response_text, "Content-Encoding: %s\n");
    if ( p_http_response->representation.language ) p_response_text += sprintf(p_response_text, "Content-Language: %s\n");
    if ( p_http_response->representation.location ) p_response_text += sprintf(p_response_text, "Content-Location: %s\n");
    if ( p_http_response->representation.length )   p_response_text += sprintf(p_response_text, "Content-Length: %s\n");
    if ( p_http_response->representation.range )    p_response_text += sprintf(p_response_text, "Content-Range: %s\n");

    // Write the payload headers
    if ( p_http_response->payload.trailer )           p_response_text += sprintf(p_response_text, "Trailer: %s\n");
    if ( p_http_response->payload.transfer_encoding ) p_response_text += sprintf(p_response_text, "Transfer-Encoding: %s\n");

    // Success
    return 1;

    // Error handling
    {

        // Argument errors
        {
            no_response_text:
                #ifndef NDEBUG
                    log_error("[http] Null pointer provided for parameter \"response_text\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;

            no_response_parser:
                #ifndef NDEBUG
                    log_error("[http] Null pointer provided for parameter \"pfn_response_parser\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }
    }
}
