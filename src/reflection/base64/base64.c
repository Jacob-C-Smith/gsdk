/** !
 * Base 64 encoding and decoding
 * 
 * @file base64.c 
 * 
 * @author Jacob Smith
 */

// Include 
#include <reflection/base64.h>

// Data
static const char base_64_encoding_characters[] = 
{
    'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', // 0   - 7
    'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', // 8   - 15
    'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', // 16  - 23
    'Y', 'Z', 'a', 'b', 'c', 'd', 'e', 'f', // 24  - 31
    'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', // 32  - 39
    'o', 'p', 'q', 'r', 's', 't', 'u', 'v', // 40  - 47
    'w', 'x', 'y', 'z', '0', '1', '2', '3', // 48  - 55
    '4', '5', '6', '7', '8', '9', '+', '/'  // 56  - 63
};
static const char base_64_decoding_characters[] = 
{
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // 0   - 7
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // 8   - 15
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // 16  - 23
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // 24  - 31
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // 32  - 39
    0x00, 0x00, 0x00, 0x3E, 0x00, 0x00, 0x00, 0x3F, // 40  - 47
    0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x3A, 0x3B, // 48  - 55
    0x3C, 0x3D, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // 56  - 63
    0x00, 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, // 64  - 71
    0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, // 72  - 79
    0x0F, 0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, // 80  - 87
    0x17, 0x18, 0x19, 0x00, 0x00, 0x00, 0x00, 0x00, // 88  - 95
    0x00, 0x1A, 0x1B, 0x1C, 0x1D, 0x1E, 0x1F, 0x20, // 96  - 103
    0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28, // 104 - 111
    0x29, 0x2A, 0x2B, 0x2C, 0x2D, 0x2E, 0x2F, 0x30, // 112 - 119
    0x31, 0x32, 0x33, 0x00, 0x00, 0x00, 0x00, 0x00  // 120 - 127
};

int base64_encode ( const void *const p_data, size_t len, char *const p_output )
{

    // Argument errors
    if ( p_data   == (void *) 0 ) goto no_data;
    if ( len      ==          0 ) goto no_len;
    if ( p_output == (void *) 0 ) goto no_output;

    // Static constant data
    static const unsigned char remainders[3] = { 0, 2, 1 };

    // Iterate through len bytes of p_data
    for (size_t i = 0, j = 0; i < len; i+=3, j+=4)
    {

        // Initialized data
        unsigned long part = (unsigned long)(((i   < len) ? ((unsigned char*)p_data)[i]   : 0) << 0x10) | 
                             (unsigned long)(((i+1 < len) ? ((unsigned char*)p_data)[i+1] : 0) << 0x08) |
                             (unsigned long)( (i+2 < len) ? ((unsigned char*)p_data)[i+2] : 0);

        // Write the part to the output
        p_output[j]   = base_64_encoding_characters[(part >> 3 * 6) & 0x3F],
        p_output[j+1] = base_64_encoding_characters[(part >> 2 * 6) & 0x3F],
        p_output[j+2] = base_64_encoding_characters[(part >> 1 * 6) & 0x3F],
        p_output[j+3] = base_64_encoding_characters[(part >> 0 * 6) & 0x3F];
    }

    // Insert a null terminator in the correct position
    p_output[( 4 * ( ( len + 2 ) / 3 ) ) - remainders[len % 3]] = '\0';
    
    no_len:
    
    // Success
    return 1;

    // Error handling
    {

        // Argument errors
        {
            no_data:
                #ifndef NDEBUG
                    log_error("[base64] Null pointer provided for parameter \"p_data\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;

            no_output:
                #ifndef NDEBUG
                    log_error("[base64] Null pointer provided for parameter \"p_output\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }
    }
}

int base64_decode ( const char *const p_data, size_t len, void *const p_output )
{

    // Argument errors
    if ( p_data   == (void *) 0 ) goto no_data;
    if ( len      ==          0 ) goto no_len;
    if ( p_output == (void *) 0 ) goto no_output;

    // Iterate through len bytes of p_data
    for (size_t i = 0, j = 0; i < (( (len+5) * 3 ) / 4 ); i+=4, j+=3)
    {

        // Initialized data
        unsigned long long part = (unsigned long long)(base_64_decoding_characters[(size_t)p_data[i+0]] << 3 * 6) + 
                                  (unsigned long long)(base_64_decoding_characters[(size_t)p_data[i+1]] << 2 * 6) +
                                  (unsigned long long)(base_64_decoding_characters[(size_t)p_data[i+2]] << 1 * 6) +
                                  (unsigned long long)(base_64_decoding_characters[(size_t)p_data[i+3]] << 0 * 6);

        // Write the part to the output
        ((unsigned char *const)p_output)[j+0] = (part >> 2 * 8) & 0xFF,
        ((unsigned char *const)p_output)[j+1] = (part >> 1 * 8) & 0xFF,
        ((unsigned char *const)p_output)[j+2] = (part >> 0 * 8) & 0xFF;
    }

    no_len:

    // Success
    return 1;

    // Error handling
    {

        // Argument errors
        {
            no_data:
                #ifndef NDEBUG
                    log_error("[base64] Null pointer provided for parameter \"p_data\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;

            no_output:
                #ifndef NDEBUG
                    log_error("[base64] Null pointer provided for parameter \"p_output\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }
    }
}
