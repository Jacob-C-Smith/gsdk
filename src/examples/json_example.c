/** !
 * Example program for json module
 * 
 * @file src/examples/json_example.c
 * 
 * @author Jacob Smith
 */

// standard library
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

// gsdk
/// core
#include <core/hash.h>
#include <core/interfaces.h>
#include <core/log.h>
#include <core/pack.h>
#include <core/stream.h>
#include <core/sync.h>

/// data
#include <data/array.h>
#include <data/dict.h>

/// reflection
#include <reflection/json.h>

// function declarations

// entry point
int main ( int argc, const char* argv[] )
{

    // argument check
    if ( 2 != argc ) goto no_arguments;
    
    // initialized data
    stream     *p_stdout = NULL;
    stream     *p_stdin  = NULL;
    json_value *p_value  = NULL;

    // construct a stream on stdin
    stream_from_path(&p_stdin, argv[1]);

    // construct a stream on stdout
    stream_from_file(&p_stdout, stdout);

    // parse json 
    if ( 0 == json_parse(&p_value, p_stdin, NULL) ) return EXIT_FAILURE;

    // serialize json 
    printf("\n[%d bytes]\n", json_serialize(p_stdout, p_value));

    // release the json 
    if ( p_value )
        p_value = default_allocator(p_value, 0);

    // release streams
    stream_destroy(&p_stdin),
    stream_destroy(&p_stdout);

    // success
    return EXIT_SUCCESS;

    // error handling
    {
        no_arguments:

            // print a usage statement
            printf("Usage: %s file.json\n", argv[0]);

            // error
            return EXIT_FAILURE;    
    }
}
