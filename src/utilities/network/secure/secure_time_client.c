/** !
 * Secure Time Client
 *
 * @file src/utilities/network/secure/secure_time_client.c
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

    // unused parameters
    (void)argc;
    (void)argv;

    // initialized data
    secure_socket *p_secure_socket = NULL;
    socket_ip_address server_ip = { 0 };
    char _buf[1024] = { 0 };
    char _time[26] = { 0 };
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
    if ( 0 == secure_socket_connect(&p_secure_socket, server_ip, 3000, p_intermediate, NULL) ) goto failed_to_connect;

    // destroy the intermediate
    if ( 0 == certificate_destroy(&p_intermediate) ) goto failed_to_destroy_certificate;

    // receive message
    size_t len = secure_socket_receive(p_secure_socket, &_buf, 1024);
    if ( 0 == len ) goto failed_to_receive;
    
    // unpack the time string
    stream_from_buffer(&p_stream, _buf, len);
    pack_unpack(p_stream, "%s", &_time);
    stream_destroy(&p_stream);

    // print the message
    log_info("%s", _time);

    // close the socket
    if ( 0 == secure_socket_destroy(&p_secure_socket) ) goto failed_to_destroy_secure_socket;
    
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
            failed_to_receive:
                log_error("Error: Failed to receive message!\n");
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
