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
/** !
 * Run the RPC server
 * 
 * @param argc the argument count
 * @param argv the argument vector
 * 
 * @return EXIT_SUCCESS on success, EXIT_FAILURE on failure
 */
int run_server ( int argc, const char *argv[] );

/** !
 * Run the RPC client
 * 
 * @param argc the argument count
 * @param argv the argument vector
 * 
 * @return EXIT_SUCCESS on success, EXIT_FAILURE on failure
 */
int run_client ( int argc, const char *argv[] );

/** !
 * Echo rpc handler
 * 
 * @param p_connection the connection object
 * @param p_input      the request data
 * 
 * @return the response data
 */
void *echo_handler ( connection *p_connection, void *p_input );

/** !
 * Add rpc handler
 * 
 * @param p_connection the connection object
 * @param p_input      the request data
 * 
 * @return the response data
 */
void *add_handler ( connection *p_connection, void *p_input );

/** !
 * Print a usage message to standard out
 * 
 * @param argv0 the 0'th element of the argv parameter of the entry point
 * 
 * @return void
 */
void print_usage ( const char *argv0 );

// pack/unpack
/// string
int string_pack   ( void *p_buffer, const void *const p_value );
int string_unpack ( void *const p_value, void *p_buffer );

// two number
int two_number_pack   ( void *p_buffer, const void *const p_value );
int two_number_unpack ( void *const p_value, void *p_buffer );

// number
int number_pack   ( void *p_buffer, const void *const p_value );
int number_unpack ( void *const p_value, void *p_buffer );

// data
unsigned short port = 6710;

// entry point
int main ( int argc, const char *argv[] )
{

    // register echo rpc
    rpc_register(
        "echo", echo_handler,       // rpc info
        string_pack, string_unpack, // request reflection
        string_pack, string_unpack  // response reflection
    );

    // register add rpc
    rpc_register(
        "add", add_handler,                 // rpc info
        two_number_pack, two_number_unpack, // request reflection
        number_pack, number_unpack          // response reflection
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

        // initialized data
        const char *message = "Hello";
        char response[100] = { 0 };
        size_t response_size = sizeof(response);

        // invoke the remote procedure call
        rpc_call(
            p_connection,    // connection
            "echo",          // RPC name
            (void *)message, // RPC request 
            response,        // RPC response 
            &response_size   // sizeof(response)
        );

        // log the result
        log_info("Result of rpc -> \"%s\"\n", response);
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

void *echo_handler ( connection *p_connection, void *p_input )
{
    return (char *)p_input;
}

void *add_handler ( connection *p_connection, void *p_input )
{

    // initialized data
    void   **pp_args = (void **)p_input;
    size_t   a       = (size_t)pp_args[0],
             b       = (size_t)pp_args[1];

    // success
    return (void *)(a + b);
}

int string_pack ( void *p_buffer, const void *const p_value )
{
    return pack_pack(p_buffer, "%s", p_value);
}

int string_unpack ( void *const p_value, void *p_buffer )
{
    return pack_unpack(p_buffer, "%s", p_value);
}

int two_number_pack ( void *p_buffer, const void *const p_value )
{

    // initialized data
    size_t a = (size_t)((void **)p_value)[0];
    size_t b = (size_t)((void **)p_value)[1];

    // pack
    return pack_pack(p_buffer, "%2i64", a, b);
}

int two_number_unpack ( void *const p_value, void *p_buffer )
{

    // initialized data
    size_t *p_args = (size_t *)p_value;

    // unpack
    return pack_unpack(p_buffer, "%2i64", p_args[0], p_args[1]);
}

int number_pack ( void *p_buffer, const void *const p_value )
{

    // initialized data
    size_t a = (size_t)p_value;

    // pack
    return pack_pack(p_buffer, "%i64", a);
}

int number_unpack ( void *const p_value, void *p_buffer )
{

    // initialized data
    size_t *p_args = (size_t *)p_value;

    // unpack
    return pack_unpack(p_buffer, "%i64", p_value);
}