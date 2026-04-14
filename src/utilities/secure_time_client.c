/** !
 * Secure Time Client
 *
 * @file src/utilities/secure_time_client.c
 *
 * @author Jacob Smith
 */

// standard library
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// gsdk
// core
#include <core/log.h>
#include <core/socket.h>
#include <core/secure_socket.h>
#include <core/tcp.h>
#include <core/pack.h>

// entry point
int main ( int argc, const char *argv[] )
{

    // unused parameters
    (void)argc;
    (void)argv;

    // initialized data
    socket_tcp client_socket = 0;
    secure_socket *p_secure_socket = NULL;
    socket_ip_address server_ip = { 0 };
    short len = 0;
    char _buf[1024] = { 0 };
    char _time[26] = { 0 };
    certificate *p_root = NULL;
    certificate *p_intermediate = NULL;
    ed25519_public_key public_key = { 0 };
    ed25519_private_key private_key = { 0 };
    FILE *p_f = NULL;

    // load certificates
    {
        
        // initialized data
        char _buffer[160] = { 0 };

        // load the root
        {
            p_f = fopen("root.cer", "rb");
            if ( NULL == p_f ) goto failed_to_open_file;

            fread(_buffer, 1, sizeof(_buffer), p_f);
            certificate_unpack(&p_root, _buffer);
            fclose(p_f);
        }

        // load the intermediate
        {
            p_f = fopen("inter.cer", "rb");
            if ( NULL == p_f ) goto failed_to_open_file;

            fread(_buffer, 1, sizeof(_buffer), p_f);
            certificate_unpack(&p_intermediate, _buffer);
            fclose(p_f);
        }
    }

    // verify root and intermediate
    if ( 0 == certificate_verify(p_intermediate, p_root) ) goto failed_to_verify_intermediate;

    // destroy the root
    certificate_destroy(&p_root);

    // resolve server address
    socket_resolve_host(&server_ip, 1, "127.0.0.1");

    // create a TCP socket
    secure_socket_connect(&p_secure_socket, server_ip, 3000, p_intermediate, NULL);

    // destroy the intermediate
    certificate_destroy(&p_intermediate);

    // receive message
    secure_socket_receive(p_secure_socket, &_buf, 1024);
    
    // unpack the time string
    pack_unpack(_buf, "%s", &_time);

    // print the message
    log_info("%s", _time);

    // close the socket
    secure_socket_destroy(&p_secure_socket);
    
    // done
    return EXIT_SUCCESS;

    // error handling
    {

        // certificate errors
        {
            failed_to_verify_intermediate:
                log_error("Error: Failed to verify intermediate certificate!\n");
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
