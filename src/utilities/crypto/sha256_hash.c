/** ! 
 * SHA256 utility
 * 
 * @file src/utilities/sha256_hash.c
 * 
 * @author Jacob Smith
 */

// standard library
#include <stdio.h>

// gsdk
/// core
#include <core/log.h>
#include <core/sha.h>

// entry point
int main ( int argc, const char *argv[] )
{

    // unused 
    (void) argc;
    (void) argv;

    // initialized data
    sha256_state _sha256_state = { 0 };
    sha256_hash hash = { 0 };
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
    
    // flush stdout
    putchar('\n');
    
    // success
    return EXIT_SUCCESS;
}
