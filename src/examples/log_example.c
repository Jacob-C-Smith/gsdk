/** !
 * Example log program
 * 
 * @file src/examples/log_example.c
 * 
 * @author Jacob Smith
 */

// standard library
#include <stdio.h>
#include <stdlib.h>

// core
#include <core/log.h>

// forward declarations
/// logs
int checkpoint ( const char *p_event );

// entry point
int main ( int argc, const char *argv[] )
{

    // unused
    (void) argc;
    (void) argv;

    // #0 - start
    checkpoint("start");

    // #1 - messages
    {

        // checkpoint
        checkpoint("messages");
        
        // log info
        log_info("This is some info\n");

        // log warning
        log_warning("This is a warning\n");

        // log error
        log_error("This is a error\n");
    }
    
    // #2 - tester
    {

        // checkpoint
        checkpoint("tester");
        
        // log scenario
        log_scenario("This is a test scenario\n");

        // log pass
        log_pass("This is a passing test\n");

        // log fail
        log_fail("This is a failing test\n");

    }
    
    // #3 - colorful
    {
        
        // checkpoint
        checkpoint("colorful");
        
        // colorful logs
        log_colorful(black  , "Hello, World!\n");
        log_colorful(red    , "Hello, World!\n");
        log_colorful(green  , "Hello, World!\n");
        log_colorful(yellow , "Hello, World!\n");
        log_colorful(blue   , "Hello, World!\n");
        log_colorful(magenta, "Hello, World!\n");
        log_colorful(cyan   , "Hello, World!\n");
        log_colorful(gray   , "Hello, World!\n");
    }

    // #4 - logging elsewhere
    {

        // checkpoint
        checkpoint("logging elsewhere");

        // update the log file
        log_update(stderr, false);

        // log some messages
        log_error("This is written to standard error!\n");

        // restore the original log file
        log_update(stdout, true);
    }

    // #5 - done
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

