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
    
    // construct a sha256 hasher
    sha256_construct(&_sha256_state);

    // feed it
    sha256_update(&_sha256_state, "abc", strlen("abc"));

    // digest it
    sha256_final(&_sha256_state, hash);

    // print the hash
    sha256_print(hash);
    
    // formatting
    printf("\n");
    
    // success
    return EXIT_SUCCESS;
}
