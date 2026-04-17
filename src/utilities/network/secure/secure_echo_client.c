/** !
 * Secure Echo Client
 *
 * @file src/utilities/secure_echo_client.c
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
#include <core/secure_socket.h>
#include <core/tcp.h>
#include <core/pack.h>

// entry point
int main ( int argc, const char *argv[] )
{

    // unused 
    (void) argc;
    (void) argv;

    // initialized data
    secure_socket *p_secure_socket = NULL;
    socket_ip_address server_ip = { 0 };
    short len = 0;
    char _in[1024] = { 0 };
    char _msg[1024] = { 0 };
    char _out[1024] = { 0 };
    certificate *p_root = NULL;
    certificate *p_intermediate = NULL;
    FILE *p_f = NULL;

    // load certificates
    {
        
        // initialized data
        char _buffer[1024] = { 0 };

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
    if ( 0 == certificate_destroy(&p_root) ) goto failed_to_destroy_certificate;

    // resolve server address
    if ( 0 == socket_resolve_host(&server_ip, 1, "127.0.0.1") ) goto failed_to_resolve_host;

    // create a TCP socket
    if ( 0 == secure_socket_connect(&p_secure_socket, server_ip, 3000, p_intermediate, NULL) ) goto failed_to_connect;

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

        // pack the string into a buffer
        len = pack_pack(_msg, "%s", &_out);

        // send the message 
        secure_socket_send(p_secure_socket, _msg, len);

        // receive message
        len = secure_socket_receive(p_secure_socket, &_in, 1024);

        // unpack the string
        pack_unpack(&_in, "%s", &_msg);

        // print the message
        log_info("> %s\n", &_msg);
    }

    // logs
    log_info("Bye bye!\n");

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
