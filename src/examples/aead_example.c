/** !
 * Example AEAD program
 * 
 * @file src/examples/aead_example.c
 * 
 * @author Jacob Smith
 */

// standard library
#include <stdio.h>
#include <stdlib.h>

// gsdk
/// core
#include <core/log.h>
#include <core/chacha20.h>
#include <core/poly1305.h>
#include <core/aead.h>

// preprocessor definitions
#define BYTES_PER_ROW 32

// forward declarations
/// logs
int checkpoint ( const char *p_event );
int hex_print ( char *p, int len );

// data
/// aead
aead *p_aead = NULL;
char _aad[] = 
{ 
    0x50, 0x51, 0x52, 0x53,
    0xc0, 0xc1, 0xc2, 0xc3,
    0xc4, 0xc5, 0xc6, 0xc7,
};

/// chacha20
chacha20_key _key = 
{
    0x83828180, 0x87868584, 0x8b8a8988, 0x8f8e8d8c,
    0x93929190, 0x97969594, 0x9b9a9998, 0x9f9e9d9c,    
};
chacha20_nonce _nonce =  { 0x00000007, 0x43424140, 0x47464544, };

/// poly1305
poly1305_tag _tag = { 0 };

/// plain text, encrypted text, and decrypted text
char _x[114] = { 0 },
     _y[114] = { 0 },
     _z[114] = { 0 };

// entry point
int main ( int argc, const char *argv[] )
{

    // unused
    (void) argc;
    (void) argv;

    // #0 - start
    checkpoint("start");

    // #1 - initial
    {
        
        // construct an aead
        aead_construct
        (
            &p_aead,
            _key,
            _nonce
        );

        // copy plaintext
        strncpy(_x, "Ladies and Gentlemen of the class of '99: If I could offer you only one tip for the future, sunscreen would be it.", sizeof(_x));

        // checkpoint
        checkpoint("after construct");
    }

    // #2 - encrypt
    {
        
        // encrypt
        aead_encrypt
        (
            _y, p_aead,
            _tag,
            _aad, sizeof(_aad),
            _x, sizeof(_x)
        );

        // checkpoint
        checkpoint("after encrypt");
    }

    // #3 - reset sequence
    {

        // seek start
        aead_sequence_set(p_aead, 0);

        // checkpoint
        checkpoint("after sequence set");
    }

    // #4 - decrypt
    {

        // decrypt
        aead_decrypt
        (
            _z, p_aead, 
            _tag,
            _aad, sizeof(_aad), 
            _y, sizeof(_y)
        );

        // checkpoint
        checkpoint("after decrypt");
    }

    // #5 - done
    checkpoint("done");
 
    // success
    return EXIT_SUCCESS;
}

int hex_print ( char *p, int len )
{

    // print full lines
    for (int i = 0; i < len/BYTES_PER_ROW; i++)
    {

        // print offset
        printf("%03d  ", i*BYTES_PER_ROW);

        // print hex
        for (int j = 0; j < BYTES_PER_ROW; j++)
            printf("%02hhx ", p[i*BYTES_PER_ROW+j]);

        // formatting
        putchar(' ');

        // print text
        for (int j = 0; j < BYTES_PER_ROW; j++)
            putchar(isprint(p[i*BYTES_PER_ROW+j]) ? p[i*BYTES_PER_ROW+j] : '.');
        
        // formatting
        putchar('\n');
    }

    // print residual lines
    if ( len % BYTES_PER_ROW )
    {
        
        // print offset
        printf("%03d  ", len-len%BYTES_PER_ROW);
        
        // print text
        for (int j = 0; j < len%BYTES_PER_ROW; j++)
        printf("%02hhx ", p[len-len%BYTES_PER_ROW+j]);
        
        // padding
        for (int i = 0; i < BYTES_PER_ROW-len%BYTES_PER_ROW; i++)
            putchar(' '), putchar(' '),  putchar(' ');
        
        // formatting
        putchar(' ');
        
        // print hex
        for (int j = 0; j < len%BYTES_PER_ROW; j++)
            putchar(isprint(p[len-len%BYTES_PER_ROW+j])?p[len-len%BYTES_PER_ROW+j]:'.');
        
        // formatting
        putchar('\n');
    }

    // success
    return 1;
}

int checkpoint ( const char *p_event )
{
    
    // static data
    static int step = 0;

    // print the event
    log_info("#%d - aead %s\n", step, p_event);

    // print the one time key
    printf("- one time key - \n"), 
    hex_print((char *)_key, sizeof(_key));

    // print the tag
    printf("\n - tag - \n"), 
    hex_print((char *)_tag, sizeof(_tag));

    // print the aad
    printf("\n - aad - \n"), 
    hex_print(_aad, sizeof(_aad));

    // print the plain text
    log_info("\n - plain text - \n"), 
    hex_print(_x, sizeof(_x));

    // print the encrypted text
    log_warning("\n - encrypted text - \n"), 
    hex_print(_y, sizeof(_y));
    
    // print the decrypted text
    log_error("\n - decrypted text - \n", _z), 
    hex_print(_z, sizeof(_z));

    // formatting
    putchar('\n');

    // increment counter
    step++;
    
    // success
    return 1;
}
