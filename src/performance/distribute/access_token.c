/** ! 
 * Access token implementation
 *
 * @file src/performance/distribute/access_token.c
 *
 * @author Jacob Smith
 */

// header
#include <performance/access_token.h>

// function definitions
int access_token_construct
(
    access_token **pp_access_token,
    public_key    *p_public_key,
    private_key   *p_private_key,
    json_value    *p_header_value,
    json_value    *p_payload_value
)
{

    // argument check
    if ( NULL == pp_access_token ) goto no_access_token;
    if ( NULL ==    p_public_key ) goto no_public_key;
    if ( NULL ==   p_private_key ) goto no_private_key;
    if ( NULL ==  p_header_value ) goto no_header_value;
    if ( NULL == p_payload_value ) goto no_payload_value;

    // initialized data
    access_token *p_access_token       = default_allocator(0, sizeof(access_token));
    char          _buffer       [8192] = { 0 },
                  _back_buffer  [4096] = { 0 };
    char         *p_buffer             = _buffer;
    size_t        len                  = 0;
    sha256_state _sha_state            = { 0 };

    // error check
    if ( NULL == p_access_token ) goto no_mem;

    // initialize access token
    *p_access_token = (access_token)
    {
        .p_public_key    = p_public_key,
        .p_header_value  = p_header_value,
        .p_payload_value = p_payload_value
    };

    // serialize the header
    p_buffer += base64_encode
    (
        _back_buffer,
        json_value_serialize
        (
            p_header_value,
            _back_buffer
        ),
        p_buffer
    ),
    
    // serialize the payload
    p_buffer += base64_encode
    (
        _back_buffer,
        json_value_serialize
        (
            p_payload_value,
            _back_buffer
        ),
        p_buffer
    );

    // compute the size of the access token
    len = p_buffer - _buffer;

    // construct the hasher
    sha256_construct(&_sha_state),

    // feed the hasher
    sha256_update(&_sha_state, p_buffer, len),

    // digest the input
    sha256_final(&_sha_state, &p_access_token->_sha_hash);

    // return a pointer to the caller
    *pp_access_token = p_access_token;

    // success
    return 1;

    // error handling
    {

        // argument errors
        {
            no_access_token:
                #ifndef NDEBUG
                    log_error("[distribute] [access token] Null pointer provided for parameter \"pp_access_token\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;

            no_public_key:
                #ifndef NDEBUG
                    log_error("[distribute] [access token] Null pointer provided for parameter \"p_public_key\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;

            no_private_key:
                #ifndef NDEBUG
                    log_error("[distribute] [access token] Null pointer provided for parameter \"p_private_key\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;

            no_header_value:
                #ifndef NDEBUG
                    log_error("[distribute] [access token] Null pointer provided for parameter \"p_header_value\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;

            no_payload_value:
                #ifndef NDEBUG
                    log_error("[distribute] [access token] Null pointer provided for parameter \"p_payload_value\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }

        // standard library errors
        {
            no_mem:
                #ifndef NDEBUG
                    log_error("[standard library] Failed to allocate memory in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
    }
}

int access_token_verify
(
    const char    *token_string,
    public_key    *p_public_key,
    access_token **pp_access_token
);

int access_token_destroy
(
    access_token **pp_access_token
);
