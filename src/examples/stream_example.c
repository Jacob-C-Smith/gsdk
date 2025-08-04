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
    stream *p_readme_stream = NULL;
    stream *p_writeme_stream = NULL;
    stream *p_socket_stream = NULL;
    socket_tcp _socket = 0; 

    // construct a read stream from a path
    if ( 0 == stream_from_path(&p_readme_stream, "README.md") ) goto failed_to_open_stream;

    // construct a write stream from a path
    if ( 0 == stream_from_path(&p_writeme_stream, "resources/core/WRITEME.md") ) goto failed_to_open_stream;

    // construct a socket
    socket_tcp_create(&_socket, socket_address_family_ipv4, 3000);
    if ( 0 == stream_from_socket_tcp(&p_socket_stream, _socket) ) goto failed_to_open_stream;

    // read from the stream
    char buffer[256] = { 0 };
    size_t bytes_read = 0;
    char c = 0;

    // tee 1000 bytes from the readme to the writeme and standard out
    for (size_t i = 0; i < 1000; i++)
    {

        // read from the stream
        bytes_read = stream_read(p_readme_stream, &c, 1);

        // error check
        if ( bytes_read != 1 ) goto failed_to_read_stream;

        // print the character
        printf("%c", c);

        // write to file stream
        stream_write(p_writeme_stream, &c, 1);

        // write to socket stream
        stream_write(p_socket_stream, &c, 1);
    }

    // close the streams
    // stream_close(&p_readme_stream);
    // stream_close(&p_writeme_stream);
    // stream_close(&p_socket_stream);

    // clean up
    socket_tcp_destroy(&_socket);

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
