/** ! 
 * ExampleSecure Hash Algorithm program
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

// forward declarations
/// logs
int checkpoint ( const char *p_event );

// data
// working data
const char _data[] = "Hello, World!";

/// hash
sha256_hash hash_256 = { 0 };
sha512_hash hash_512 = { 0 };

/// working hasher
sha256_state _sha256_state = { 0 };
sha512_state _sha512_state = { 0 };
size_t file_len = 0;

// entry point
int main ( int argc, const char *argv[] )
{

    // unused 
    (void) argc;
    (void) argv;
    
    // #0 - start
    checkpoint("start");

    // #1 - sha256 example
    {

        // construct a sha256 hasher
        sha256_construct(&_sha256_state);

        // feed it
        sha256_update(&_sha256_state, _data, sizeof(_data)-1);

        // digest it
        sha256_final(&_sha256_state, hash_256);

        // checkpoint
        checkpoint("SHA-256");

        // print the hash
        log_info("SHA256(\"%s\")", _data),
        printf(" = 0x"),
        sha256_print(hash_256);
        
        // formatting
        printf("\n");
    }

    // #2 - sha512 example
    {

        // construct a sha512 hasher
        sha512_construct(&_sha512_state);

        // feed it
        sha512_update(&_sha512_state, _data, sizeof(_data)-1);

        // digest it
        sha512_final(&_sha512_state, hash_512);

        // checkpoint
        checkpoint("SHA-512");

        // print the hash
        log_info("SHA512(\"%s\")", _data),
        printf(" = 0x"),
        sha512_print(hash_512);
        
        // formatting
        printf("\n");
    }

    // #3 - done
    checkpoint("done");

    // success
    return EXIT_SUCCESS;
}

int checkpoint ( const char *p_event )
{
    
    // static data
    static int step = 0;
    
    // print the event
    log_info("#%d - %s\n", step, p_event),
    
    // increment counter
    step++;
    
    // success
    return 1;
}
  