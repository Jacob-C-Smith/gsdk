/** !
 * Example Poly1305 program
 * 
 * @file src/examples/poly1305_example.c
 * 
 * @author Jacob Smith
 */

// standard library
#include <stdio.h>
#include <stdlib.h>

// gsdk
/// core
#include <core/log.h>
#include <core/poly1305.h>

// preprocessor definitions
#define BYTES_PER_ROW 16

// forward declarations
/// logs
int checkpoint ( const char *p_event );
int hex_print ( char *p, size_t len );

// data
/// poly1305
poly1305_one_time_key _one_time_key = 
{ 
    0x85, 0xd6, 0xbe, 0x78, 0x57, 0x55, 0x6d, 0x33,
    0x7f, 0x44, 0x52, 0xfe, 0x42, 0xd5, 0x06, 0xa8, 
    0x01, 0x03, 0x80, 0x8a, 0xfb, 0x0d, 0xb2, 0xfd, 
    0x4a, 0xbf, 0xf6, 0xaf, 0x41, 0x49, 0xf5, 0x1b,
};
chacha20_key _key = 
{
    0x83828180, 0x87868584, 0x8b8a8988, 0x8f8e8d8c,
    0x93929190, 0x97969594, 0x9b9a9998, 0x9f9e9d9c,    
};
chacha20_nonce _nonce =  { 0x00000000, 0x03020100, 0x07060504, };
poly1305_tag _tag = { 0 };
char _message[34] = "Cryptographic Forum Research Group";

// entry point
int main ( int argc, const char *argv[] )
{

    // unused
    (void) argc;
    (void) argv;

    // #0 - start
    checkpoint("start");

    // #1 - message authentication code
    {

        // compute the message authentication code
        poly1305_mac(_message, sizeof(_message), _tag, _one_time_key);

        // checkpoint
        checkpoint("after computing message authentication code");
    }
    
    // #2 - key generation
    {

        // clear message and tag
        memset(_message, 0, sizeof(_message)),
        memset(_tag, 0, sizeof(_tag));
        
        // generate a key
        poly1305_key_generate(&_one_time_key, _key, _nonce, 0);

        // checkpoint
        checkpoint("key generation");
    }

    // #3 - done
    checkpoint("done");
 
    // success
    return EXIT_SUCCESS;
}

int hex_print ( char *p, size_t len )
{

    // print full lines
    for (size_t i = 0; i < len/BYTES_PER_ROW; i++)
    {

        // print offset
        printf("%03d  ", i*BYTES_PER_ROW);

        // print hex
        for (size_t j = 0; j < BYTES_PER_ROW; j++)
            printf("%02hhx ", p[i*BYTES_PER_ROW+j]);

        // formatting
        putchar(' ');

        // print text
        for (size_t j = 0; j < BYTES_PER_ROW; j++)
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
        for (size_t j = 0; j < len%BYTES_PER_ROW; j++)
        printf("%02hhx ", p[len-len%BYTES_PER_ROW+j]);
        
        // padding
        for (size_t i = 0; i < BYTES_PER_ROW-len%BYTES_PER_ROW; i++)
            putchar(' '), putchar(' '),  putchar(' ');
        
        // formatting
        putchar(' ');
        
        // print hex
        for (size_t j = 0; j < len%BYTES_PER_ROW; j++)
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
    log_info("#%d - poly1305 %s\n", step, p_event);

    // print the plain text
    log_info("- one time key - \n"), 
    hex_print(_one_time_key, sizeof(_one_time_key));

    // print the encrypted text
    log_warning("\n - message - \n"), 
    hex_print(_message, sizeof(_message));
    
    // print the decrypted text
    log_error("\n - tag - \n"), 
    hex_print(_tag, sizeof(_tag));

    // formatting
    putchar('\n');

    // increment counter
    step++;
    
    // success
    return 1;
}
