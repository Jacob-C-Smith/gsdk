/** !
 * An example program for the http server
 * 
 * @author Jacob Smith
 * 
 * @file main.c
*/

// standard library
#include <stdio.h>

// http server 
#include <web/server.h>

// entry point
int main ( int argc, const char *argv[] )
{

    // initialized data
    web_server *p_web_server = 0;

    // Construct an http server
    if ( web_server_construct(&p_web_server, "config.json") == 0 ) goto failed_to_construct_web_server;

    // Start the http server
    if ( web_server_start(p_web_server) == 0 )
        ; 

    // success
    return EXIT_SUCCESS;

    // error handling
    {

        failed_to_construct_web_server:

            // Write an error message to standard out
            printf("Failed to start HTTP server!\n");

            // error
            return EXIT_FAILURE;
    }
}
