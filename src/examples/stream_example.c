/** !
 * Stream example
 *
 * @file src/examples/stream_example.c
 *
 * @author Jacob Smith
 */

// standard library
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// gsdk
/// core
#include <core/log.h>
#include <core/stream.h>

// forward declarations
/// logs
int checkpoint ( stream *p_stream, const char *p_event );

// preprocessor macros
#define HEX_DUMP_COLUMNS 16
#define HEX_DUMP_SPACING 8

// data
/// working stream
stream *p_stream = NULL;

// entry point
int main ( int argc, const char *argv[] )
{
    
    // unused
    (void) argc;
    (void) argv;

    char _buf[1024] = { 0 };

    // #0 - start
    checkpoint(p_stream, "start");

    // #1 - construct
    {

        // construct a stream
        stream_from_path(&p_stream, "resources/reflection/stream.txt");

        // checkpoint
        checkpoint(p_stream, "after construct"),
        putchar('\n');
    }

    // #2 - write
    {

        // write
        stream_write(p_stream, "Hello, World!", sizeof("Hello, World!"));

        // checkpoint
        checkpoint(p_stream, "after write"),
        putchar('\n');
    }

    // #3 - seek
    {

        // seek
        stream_seek(p_stream, 0, STREAM_SEEK_SET);

        // checkpoint
        checkpoint(p_stream, "after seek(0)"),
        putchar('\n');
    }

    // #4 - read
    {

        // read
        stream_read(p_stream, _buf, sizeof(_buf));

        // print the string
        printf("Read \"%s\"\n", _buf);

        // checkpoint
        checkpoint(p_stream, "after read"),
        putchar('\n');
    }

    // #5 - EOF
    {

        // initialized data
        bool eof = stream_eof(p_stream);

        // print the EOF 
        printf("EOF -> %s\n", eof ? "true" : "false");

        // checkpoint
        checkpoint(p_stream, "after EOF"),
        putchar('\n');
    }

    // #6 - seek
    {

        // seek
        stream_seek(p_stream, 0, STREAM_SEEK_SET);

        // checkpoint
        checkpoint(p_stream, "after seek(0)"),
        putchar('\n');
    }

    // #7 - copy
    {

        // initialized data
        stream *p_destination = NULL;

        // construct a stream
        stream_from_path(&p_destination, "resources/reflection/stream_copy.txt");

        // copy
        stream_copy(p_destination, p_stream, sizeof(_buf));

        // checkpoint
        checkpoint(p_destination, "destination after copy"),
        checkpoint(p_stream, "after copy"),
        putchar('\n');

        // destroy the transient stream
        stream_destroy(&p_destination);
    }

    // #8 - destroy
    {

        // destroy the stream
        stream_destroy(&p_stream);

        // checkpoint
        checkpoint(p_stream, "after destroy");
    }

    // #9 - end
    checkpoint(p_stream, "end");

    // success
    return EXIT_SUCCESS;
}

int checkpoint ( stream *p_stream, const char *p_event )
{

    // static data
    static int step = 0;

    // print the stream
    if ( NULL == p_stream )
        log_info("#%d - Stream %s: NULL\n", step, p_event);
    else
        log_info("#%d - Stream(%d/%d) %s\n", step, stream_tell(p_stream), stream_size(p_stream), p_event);

    // increment counter
    step++;

    // success
    return 1;
}
