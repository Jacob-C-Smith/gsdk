/** !
 * Example hash program
 * 
 * @file src/examples/hash_example.c
 * 
 * @author Jacob Smith
 */

// standard library
#include <stdio.h>
#include <stdlib.h>

// core
#include <core/log.h>
#include <core/hash.h>

// forward declarations
/// logs
int checkpoint ( const char *p_event );

// data
const char _string[] = "Hi mom!";

// entry point
int main ( int argc, const char *argv[] )
{
    
    // unused
    (void) argc;
    (void) argv;

    // #0 - start
    checkpoint("start");

    // #1 - Cyclic Redundancy Check
    {
        
        // initialized data
        hash64 crc64 = hash_crc64(_string, sizeof(_string));

        // checkpoint
        checkpoint("Cyclic Redundancy Check");

        // print the hash
        printf("crc64(\"%s\") = 0x%016llX\n", _string, crc64);
    }

    // #2 - MurMur
    {
        
        // initialized data
        hash64 mmh64 = hash_mmh64(_string, sizeof(_string));

        // checkpoint
        checkpoint("MurMur");

        // print the hash
        printf("mmh64(\"%s\") = 0x%016llX\n", _string, mmh64);
    }

    // #3 - Fowler–Noll–Vo
    {
        
        // initialized data
        hash64 fvn64 = hash_fnv64(_string, sizeof(_string));

        // checkpoint
        checkpoint("Fowler-Noll-Vo");

        // print the hash
        printf("fvn64(\"%s\") = 0x%016llX\n", _string, fvn64);
    }

    // #4 - done
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
  