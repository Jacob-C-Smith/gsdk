/** !
 * Secure Echo Client
 *
 * @file src/utilities/network/secure/secure_echo_client.c
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
#include <core/socket.h>
#include <core/tcp.h>
#include <core/pack.h>
#include <core/stream.h>

/// crypto
#include <crypto/secure_socket.h>

// entry point
int main ( int argc, const char *argv[] )
{

    // unused 
    (void) argc;
    (void) argv;

    // initialized data
    stream *p_stream_socket = NULL;
    socket_ip_address server_ip = { 0 };
    char _msg[1024] = { 0 };
    char _out[1024] = { 0 };
    certificate *p_root = NULL;
    certificate *p_intermediate = NULL;
    stream *p_stream = NULL;

    // load certificates
    {
        
        // load the root
        {
            if ( 0 == stream_from_path(&p_stream, "root.cer") ) goto failed_to_open_file;
            certificate_unpack(&p_root, p_stream);
            stream_destroy(&p_stream);
        }

        // load the intermediate
        {
            if ( 0 == stream_from_path(&p_stream, "inter.cer") ) goto failed_to_open_file;
            certificate_unpack(&p_intermediate, p_stream);
            stream_destroy(&p_stream);
        }
    }

    // verify root and intermediate
    if ( 0 == certificate_verify(p_intermediate, p_root) ) goto failed_to_verify_intermediate;

    // destroy the root
    if ( 0 == certificate_destroy(&p_root) ) goto failed_to_destroy_certificate;

    // resolve server address
    if ( 0 == socket_resolve_host(&server_ip, 1, "127.0.0.1") ) goto failed_to_resolve_host;

    // create a TCP socket
    if ( 0 == secure_socket_connect(&p_stream_socket, server_ip, 3000, p_intermediate, NULL) ) goto failed_to_connect;

    // destroy the intermediate
    if ( 0 == certificate_destroy(&p_intermediate) ) goto failed_to_destroy_certificate;

    // repl
    while ( 1 )
    {

        putchar('<'),
        putchar(' ');
        
        // read 
        fgets(_out, 1023, stdin);

        if ( feof(stdin) ) break;

        _out[strlen(_out) - 1] = '\0';

        // pack the string directly to the secure stream
        pack_pack(p_stream_socket, "%s", &_out);
        stream_flush(p_stream_socket);

        // unpack the string directly from the secure stream
        pack_unpack(p_stream_socket, "%s", &_msg);

        // print the message
        log_info("> %s\n", &_msg);
    }

    // logs
    log_info("Bye bye!\n");

    // close the socket
    if ( 0 == stream_destroy(&p_stream_socket) ) goto failed_to_destroy_secure_socket;
    
    // success
    return EXIT_SUCCESS;

    // error handling
    {

        // certificate errors
        {
            failed_to_verify_intermediate:
                log_error("Error: Failed to verify intermediate certificate!\n");
                return EXIT_FAILURE;
            failed_to_destroy_certificate:
                log_error("Error: Failed to destroy certificate!\n");
                return EXIT_FAILURE;
        }
        
        // socket errors
        {
            failed_to_resolve_host:
                log_error("Error: Failed to resolve host!\n");
                return EXIT_FAILURE;
        }

        // secure socket errors
        {
            failed_to_connect:
                log_error("Error: Failed to connect to server!\n");
                return EXIT_FAILURE;
            failed_to_destroy_secure_socket:
                log_error("Error: Failed to destroy secure socket!\n");
                return EXIT_FAILURE;
        }

        // standard library errors
        {
            failed_to_open_file:
                log_error("Error: Failed to open file!\n");
                return EXIT_FAILURE;
        }
    }
}
