/** ! 
 * Example base64 program
 * 
 * @file src/examples/base64_example.c
 * 
 * @author Jacob Smith
 */

// standard library
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <time.h>

// gsdk
/// core
#include <core/log.h>

/// reflection
#include <reflection/base64.h>

// forward declarations
/// logs
int checkpoint ( const char *p_event );

// data
// working data
const char _data[] = "Hello, World!";

/// encode buffer
char _encoded[32] = { 0 };

/// decode buffer
char _decoded[32] = { 0 };

// entry point
int main ( int argc, const char *argv[] )
{

    // unused 
    (void) argc;
    (void) argv;
    
    // #0 - start
    checkpoint("start");

    // #1 - base64 encoding 
    {

        // encode the data
        base64_encode(_encoded, _data, sizeof(_data)-1);

        // checkpoint
        checkpoint("encoding");

        // print the base64
        log_error("base64_encode(\"%s\") ", _data),
        putchar('='),
        log_info(" %s\n", _encoded);
    }

    // #2 - base64 decoding
    {

        // decode the data
        base64_decode(_decoded, _encoded, strlen(_encoded));

        // checkpoint
        checkpoint("decoding");

        // print the base64
        log_info("base64_decode(\"%s\") ", _encoded),
        putchar('='),
        log_error(" %s\n", _decoded);
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
  