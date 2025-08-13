/** !
 * Example distribute program
 * 
 * @file distribute_example.c
 * 
 * @author Jacob Smith
 */

// standard library
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// core
#include <core/log.h>
#include <core/hash.h>

// performance
#include <performance/connection.h>
#include <performance/rpc.h>

// forward declarations
int run_server ( int argc, const char *argv[] );
int run_client ( int argc, const char *argv[] );

void *echo_handler ( connection *p_connection, void *p_input );

/** !
 * Print a usage message to standard out
 * 
 * @param argv0 the 0'th element of the argv parameter of the entry point
 * 
 * @return void
 */
void print_usage ( const char *argv0 );

// string
int    string_pack   ( void *p_buffer, const void *const p_value );
int    string_unpack ( void *const p_value, void *p_buffer );
hash64 string_hash   ( const void *const string, unsigned long long unused );

// data
unsigned short port = 4009;

// entry point
int main ( int argc, const char *argv[] )
{

    // register RPC handlers
    rpc_register(
        "echo", 
        echo_handler,
        string_pack,  
        string_unpack
    );

    // run the server
    if      ( 0 == strcmp(argv[1], "server") ) return run_server(argc, argv);
    else if ( 0 == strcmp(argv[1], "client") ) return run_client(argc, argv);
    else goto usage;

    // success
    return EXIT_SUCCESS;

    // error handling
    {
        usage:
            
            // print a usage message
            print_usage(argv[0]);

            // abort
            exit(EXIT_FAILURE);
    }
}

void *echo_handler ( connection *p_connection, void *p_input )
{
    return (char *)p_input;
}

int string_pack ( void *p_buffer, const void *const p_value )
{
    return pack_pack(p_buffer, "%s", p_value);
}

int string_unpack ( void *const p_value, void *p_buffer )
{
    return pack_unpack(p_buffer, "%s", p_value);
}









void print_usage ( const char *argv0 )
{

    // print a usage message to standard out
    printf("Usage: %s [server | client]\n", argv0);

    // done
    return;
}

int run_server ( int argc, const char *argv[] )
{

    // log
    log_info("Starting RPC server...\n");

    // start the rpc handler
    if ( 0 == rpc_server_listen(port) ) goto failed_to_start_rpc_server;

    // error
    return EXIT_SUCCESS;

    // error handling
    {
        failed_to_start_rpc_server:

            // print the error
            log_error("RPC server failed to start.\n");

            // error
            return 0;
    }
}

int run_client ( int argc, const char *argv[] )
{

    // initialized data
    connection *p_connection = NULL;

    // log
    log_info("Starting RPC client...\n");

    // construct a connection
    if ( 0 == connection_construct(&p_connection, "127.0.0.1", port) ) goto failed_to_connect;

    // log
    log_info("Calling 'echo' with \"Hello\"\n");

    // test 'echo' RPC
    {
        const char *message = "Hello";
        char response[100] = {0};
        size_t response_size = sizeof(response);

        if ( 0 == rpc_call(
            p_connection, 
            "echo", 
            
            (void *)message, 
            strlen(message) + 1, 
            
            response, 
            &response_size)
        ) goto failed_to_call_rpc;

        
        log_info("Result of echo: '%s'\n", response);

    }

    // cleanup
    connection_destroy(&p_connection);

    // success
    return EXIT_SUCCESS;

    // error handling
    {
        failed_to_connect:

            // print the error
            log_error("Failed to connect to server.\n");

            // destroy the connection
            connection_destroy(&p_connection);

            // error
            return EXIT_FAILURE;
        failed_to_call_rpc:

            // print the error
            log_error("RPC 'echo' failed!\n");

            // destroy the connection
            connection_destroy(&p_connection);

            // error
            return EXIT_FAILURE;
    }
}

hash64 string_hash ( const void *const string, unsigned long long unused )
{

    // unused
    (void)unused;

    // done
    return hash_crc64(string, strlen(string));
}
