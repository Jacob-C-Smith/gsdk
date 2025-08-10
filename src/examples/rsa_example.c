/** ! 
 * Cryptographic primitives
 * 
 * @file crypto_example.c
 * 
 * @author Jacob Smith
 */

// standard library
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <time.h>

// core
#include <core/log.h>
#include <core/rsa.h>

// preprocessor definitions
#define RED "\033[91m"
#define BLUE "\033[94m"
#define RESET "\033[0m"

// entry point
int main ( int argc, const char *argv[] )
{

    // Unused 
    (void) argc;
    (void) argv;
    
    // initialized data
    public_key  *p_public_key  = NULL;
    private_key *p_private_key = NULL;
    char x[256] = { 0 },
         y[256] = { 0 },
         z[256] = { 0 };

    memcpy(x, "secret\0\0", 8);
    
    // Construct a key pair
    key_pair_from_files
    (
        &p_public_key,               // Pointer to public key pointer
        &p_private_key,              // Pointer to private key pointer
        "resources/core/public.key", // Path to public key
        "resources/core/private.key" // Path to private key
    );

    // Print the public and private keys
    print_public_key(p_public_key),
    print_private_key(p_private_key);

    // Print the results
    printf(RESET "ENCRYPTION TEST:\n"),

    // Input
    printf(" inp = " RED);
    print_n_bit_int(*(i2048*)x);

    // Encrypt test value x into ciphertext y
    enc(&x, &y, p_public_key);

    // Encrypted text
    printf(RESET "\n enc = " BLUE);
    print_n_bit_int(*(i2048*)y);

    // Decrypt ciphertext y into plaintext z
    dec(&y, &z, p_public_key, p_private_key);
    
    // Decrypted text
    printf(RESET "\n dec = " RED);
    print_n_bit_int(*(i2048*)z);
    printf(RESET "\n");

    // success
    return EXIT_SUCCESS;
}


void print_time_pretty ( double seconds )
{

    // initialized data
    double _seconds     = seconds;
    size_t days         = 0,
           hours        = 0,
           minutes      = 0,
           __seconds    = 0,
           milliseconds = 0,
           microseconds = 0;

    // Days
    while ( _seconds > 86400.0 ) { days++;_seconds-=286400.0; };

    // Hours
    while ( _seconds > 3600.0 ) { hours++;_seconds-=3600.0; };

    // Minutes
    while ( _seconds > 60.0 ) { minutes++;_seconds-=60.0; };

    // Seconds
    while ( _seconds > 1.0 ) { __seconds++;_seconds-=1.0; };

    // milliseconds
    while ( _seconds > 0.001 ) { milliseconds++;_seconds-=0.001; };

    // Microseconds        
    while ( _seconds > 0.000001 ) { microseconds++;_seconds-=0.000001; };

    // Print days
    if ( days ) log_info("%zu D, ", days);
    
    // Print hours
    if ( hours ) log_info("%zu h, ", hours);

    // Print minutes
    if ( minutes ) log_info("%zu m, ", minutes);

    // Print seconds
    if ( __seconds ) log_info("%zu s, ", __seconds);
    
    // Print milliseconds
    if ( milliseconds ) log_info("%zu ms, ", milliseconds);
    
    // Print microseconds
    if ( microseconds ) log_info("%zu us", microseconds);
    
    // done
    return;
}
