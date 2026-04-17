/** !
 * Example ChaCha20 program
 * 
 * @file src/examples/chacha20_example.c
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

// preprocessor definitions
#define BYTES_PER_ROW 32

// forward declarations
/// logs
int checkpoint ( const char *p_event );
int hex_print ( char *p, int len );

// data
/// chacha20
chacha20 *p_chacha20 = NULL;

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
        
        // construct a chacha20 stream cipher
        chacha20_construct(
            &p_chacha20,
            (chacha20_key) { 0x03020100, 0x07060504, 0x0b0a0908, 0x0f0e0d0c, 0x13121110, 0x17161514, 0x1b1a1918, 0x1f1e1d1c },
            (chacha20_nonce) { 0x00000000, 0x4a000000, 0x00000000 },
            1
        ); 

        // copy plaintext
        strncpy(_x, "Ladies and Gentlemen of the class of '99: If I could offer you only one tip for the future, sunscreen would be it.", sizeof(_x));
        
        // checkpoint
        checkpoint("after construct");
    }

    // #2 - encrypt 
    {

        // encrypt
        chacha20_encrypt(_y, p_chacha20, _x, sizeof(_x));

        // checkpoint
        checkpoint("after encrypt");
    }
    
    // #3 - decrypt
    {

        // seek
        chacha20_seek(p_chacha20, 1),

        // decrypt
        chacha20_decrypt(_z, p_chacha20, _y, sizeof(_y));

        // checkpoint
        checkpoint("after decrypt");
    }

    // #4 - destroy
    {

        // destroy the chacha20 cipher
        chacha20_destroy(&p_chacha20);

        // checkpoint
        checkpoint("after destroy");
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
    log_info("#%d - chacha20 %s\n", step, p_event);

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
