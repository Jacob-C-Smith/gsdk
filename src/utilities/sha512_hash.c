/** ! 
 * SHA512 utility
 * 
 * @file src/utilities/sha512_hash.c
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
    sha512_state _sha512_state = { 0 };
    sha512_hash hash = { 0 };
    unsigned char buffer[4096] = { 0 };
    size_t bytes_read = 0;
    
    // construct a sha512 hasher
    sha512_construct(&_sha512_state);

    // read from standard input in chunks
    while ((bytes_read = fread(buffer, 1, sizeof(buffer), stdin)) > 0)
        
        // feed hasher
        sha512_update(&_sha512_state, buffer, bytes_read);

    // digest it
    sha512_final(&_sha512_state, hash);

    // print the hash
    sha512_print(hash);
    
    // flush stdout
    putchar('\n');
    
    // success
    return EXIT_SUCCESS;
}
