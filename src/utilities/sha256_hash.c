/** ! 
 * Secure Hash Algorithm Example
 * 
 * @file sha_example.c
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
#include <core/sha.h>
#include <core/stream.h>

// data
/// file for reflection
FILE *p_f = NULL;

/// hash
sha256_hash hash = { 0 };

/// working hasher
sha256_state _sha256_state = { 0 };
size_t file_len = 0;

// entry point
int main ( int argc, const char *argv[] )
{

    // unused 
    (void) argc;
    (void) argv;

    // initialized data
    unsigned char buffer[4096] = { 0 };
    size_t bytes_read = 0;
    
    // construct a sha256 hasher
    sha256_construct(&_sha256_state);

    // read from standard input in chunks
    while ((bytes_read = fread(buffer, 1, sizeof(buffer), stdin)) > 0)
        
        // feed hasher
        sha256_update(&_sha256_state, buffer, bytes_read);

    // digest it
    sha256_final(&_sha256_state, hash);

    // print the hash
    sha256_print(hash);
    
    // formatting
    printf("\n");
    
    // success
    return EXIT_SUCCESS;
}
