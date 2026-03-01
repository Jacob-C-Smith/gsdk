/** !
 * base64 module implementation
 * 
 * @file src/reflection/base64/base64.c 
 * 
 * @author Jacob Smith
 */

// header
#include <reflection/base64.h>

// data
static const char base_64_encoding_characters[] = 
{
    'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 
    'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 
    'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 
    'Y', 'Z', 'a', 'b', 'c', 'd', 'e', 'f', 
    'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 
    'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 
    'w', 'x', 'y', 'z', '0', '1', '2', '3', 
    '4', '5', '6', '7', '8', '9', '+', '/'  
};
static const char base_64_decoding_characters[] = 
{
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x3E, 0x00, 0x00, 0x00, 0x3F, 
    0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x3A, 0x3B, 
    0x3C, 0x3D, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 
    0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 
    0x0F, 0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 
    0x17, 0x18, 0x19, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x1A, 0x1B, 0x1C, 0x1D, 0x1E, 0x1F, 0x20, 
    0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28, 
    0x29, 0x2A, 0x2B, 0x2C, 0x2D, 0x2E, 0x2F, 0x30, 
    0x31, 0x32, 0x33, 0x00, 0x00, 0x00, 0x00, 0x00  
};

int base64_encode ( char *const p_output, const void *const p_data, size_t len )
{

    // argument errors
    if ( NULL == p_output ) goto no_output;
    if ( NULL ==   p_data ) goto no_data;

    // initialized data
    size_t output_length = ( 4 * ( ( len + 2 ) / 3 ) );

    // iterate through len bytes of p_data
    for (size_t i = 0, j = 0; i < len; i += 3, j += 4)
    {

        // initialized data
        unsigned long part = (unsigned long)( ( (i + 0 < len) ? ( (unsigned char*)p_data)[i + 0] : 0 ) << 0x10 ) | 
                             (unsigned long)( ( (i + 1 < len) ? ( (unsigned char*)p_data)[i + 1] : 0 ) << 0x08 ) |
                             (unsigned long)( ( (i + 2 < len) ? ( (unsigned char*)p_data)[i + 2] : 0 ) << 0x00 ) ;

        // write this fragment to the output
        p_output[j + 0] =                 base_64_encoding_characters[(part >> (3 * 6)) & 0x3F],
        p_output[j + 1] =                 base_64_encoding_characters[(part >> (2 * 6)) & 0x3F],
        p_output[j + 2] = (i + 1 < len) ? base_64_encoding_characters[(part >> (1 * 6)) & 0x3F] : '=',
        p_output[j + 3] = (i + 2 < len) ? base_64_encoding_characters[(part >> (0 * 6)) & 0x3F] : '=';
    }

    // insert a null terminator at the end of the output
    p_output[output_length] = '\0';
    
    // success
    return 1;

    // error handling
    {

        // argument errors
        {
            no_data:
                #ifndef NDEBUG
                    log_error("[base64] Null pointer provided for parameter \"p_data\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;

            no_output:
                #ifndef NDEBUG
                    log_error("[base64] Null pointer provided for parameter \"p_output\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
    }
}

int base64_decode ( void *const p_output, const char *const p_data, size_t len )
{

    // argument errors
    if ( NULL == p_output ) goto no_output;
    if ( NULL ==   p_data ) goto no_data;

    // iterate through len bytes of p_data
    for ( size_t i = 0, j = 0; i < len; i+=4, j+=3 )
    {

        // initialized data
        unsigned long long part = (unsigned long long)( ( ( i + 0 < len ) && ( (unsigned char)p_data[i+0] < 128 ) ? base_64_decoding_characters[(unsigned char)p_data[i+0]] : 0 ) << 3 * 6 ) + 
                                  (unsigned long long)( ( ( i + 1 < len ) && ( (unsigned char)p_data[i+1] < 128 ) ? base_64_decoding_characters[(unsigned char)p_data[i+1]] : 0 ) << 2 * 6 ) +
                                  (unsigned long long)( ( ( i + 2 < len ) && ( (unsigned char)p_data[i+2] < 128 ) ? base_64_decoding_characters[(unsigned char)p_data[i+2]] : 0 ) << 1 * 6 ) +
                                  (unsigned long long)( ( ( i + 3 < len ) && ( (unsigned char)p_data[i+3] < 128 ) ? base_64_decoding_characters[(unsigned char)p_data[i+3]] : 0 ) << 0 * 6 );

        // write this fragment to the output
        if ( i + 1 < len ) ((unsigned char *const)p_output)[j+0] = (part >> 2 * 8) & 0xFF;
        if ( i + 2 < len && p_data[i+2] != '=' ) ((unsigned char *const)p_output)[j+1] = (part >> 1 * 8) & 0xFF;
        if ( i + 3 < len && p_data[i+3] != '=' ) ((unsigned char *const)p_output)[j+2] = (part >> 0 * 8) & 0xFF;
    }
    
    // success
    return 1;

    // error handling
    {

        // argument errors
        {
            no_data:
                #ifndef NDEBUG
                    log_error("[base64] Null pointer provided for parameter \"p_data\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;

            no_output:
                #ifndef NDEBUG
                    log_error("[base64] Null pointer provided for parameter \"p_output\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
    }
}
