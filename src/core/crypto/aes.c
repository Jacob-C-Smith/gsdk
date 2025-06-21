/** ! 
 * AES (en/de)cryption 
 * 
 * @file aes.h
 * 
 * @author Jacob Smith
 */

// header
#include <core/aes.h>

// function definitions
int aes256_encrypt ( void *p_value, size_t len, const unsigned char *enc_keys )
{

    // unused
    (void) p_value;
    (void) len;
    (void) enc_keys;

    // success
    return 1;
}

int aes256_decrypt ( void *p_value, size_t len, const unsigned char *dec_keys )
{

    // unused
    (void) p_value;
    (void) len;
    (void) dec_keys;

    // success
    return 1;
}
