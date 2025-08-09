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

// core 
#include <core/stream.h>

// entry point
int main ( int argc, const char *argv[] )
{

    // Unused
    (void) argc;
    (void) argv;

    // initialized data
    stream *p_readme_stream  = NULL;
    stream *p_writeme_stream = NULL;
    stream *p_memory_stream  = NULL;
    char buffer[256] = { 0 };
    size_t bytes_read = 0;
    char c = 0;

    // construct a read stream from a path
    if ( 0 == stream_from_path(&p_readme_stream, "README.md") ) goto failed_to_open_stream;
    
    // read from the stream

    // tee 1000 bytes from the readme to the writeme and standard out
    for (size_t i = 0; i < 1000; i++)
    {

        // read from the stream
        bytes_read = stream_read(p_readme_stream, &c, 1);

        // error check
        if ( bytes_read != 1 ) goto failed_to_read_stream;

        // write to file stream
        stream_write(p_writeme_stream, &c, 1);
    }

    // success
    return EXIT_SUCCESS;

    // error handling
    {

        // stream errors
        {
            failed_to_open_stream:
                #ifndef NDEBUG
                    log_error("Error: Failed to open stream\n");
                #endif

                // error
                return EXIT_FAILURE;

            failed_to_read_stream:
                #ifndef NDEBUG
                    log_error("Error: Failed to read from stream\n");
                #endif  
                
                // error
                return EXIT_FAILURE;
        }
    }
}
