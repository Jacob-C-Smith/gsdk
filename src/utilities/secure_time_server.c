/** !
 * Time Server
 *
 * @file src/utilities/time_server.c
 *
 * @author Jacob Smith
 */

// standard library
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

// gsdk
// core
#include <core/log.h>
#include <core/socket.h>
#include <core/secure_socket.h>
#include <core/tcp.h>
#include <core/pack.h>
#include <core/sync.h> 

int connection_callback ( secure_socket *p_secure_socket, socket_ip_address ip_address, socket_port port, void *const p_parameter )
{

    // unused
    (void) port;
    (void) p_parameter;

    // initialized data
    char _buf[1024] = { 0 };
    struct tm* ptr = NULL;
    time_t lt = 0;
    size_t len = 0;

    // logs
    printf("Accepted connection from "), 
    socket_ip_address_print(ip_address);

    // store the current time
    lt = time(NULL);

    // compute the localized time
    ptr = localtime(&lt);

    // pack the time string into a buffer
    len = pack_pack(_buf, "%s", asctime(ptr));

    // send the localized time to the client
    secure_socket_send(p_secure_socket, _buf, len);

    // clean up
    secure_socket_destroy(&p_secure_socket);

    // success
    return 1;
}

// entry point
int main ( int argc, const char *argv[] )
{

    // unused parameters
    (void)argc;
    (void)argv;

    // initialized data
    socket_tcp server_socket = 0;
    certificate *p_root = NULL;
    certificate *p_intermediate = NULL;
    certificate *p_leaf = NULL;
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

        // load the leaf
        {
            p_f = fopen("leaf.cer", "rb");
            if ( NULL == p_f ) goto failed_to_open_file;

            fread(_buffer, 1, sizeof(_buffer), p_f);
            certificate_unpack(&p_leaf, _buffer);
            fclose(p_f);
        }
    }

    // load keys
    {

        // initialized data
        char _buffer[64] = { 0 };

        // load the keys
        {
            p_f = fopen("leaf.key", "rb");
            if ( NULL == p_f ) goto failed_to_open_file;

            fread(_buffer, 1, sizeof(_buffer), p_f);
            ed25519_key_pair_unpack(&public_key, &private_key, _buffer);
            fclose(p_f);
        }
    }

    // create a tcp socket on port 3000
    socket_tcp_create(&server_socket, socket_address_family_ipv4, 3000);

    // logs
    log_info("Server listening on port %hu\n", 3000);

    // listen for connections
    while ( true )
        secure_socket_listen(server_socket, connection_callback, p_leaf, &private_key, NULL);

    // logs
    log_info("Shutting down server...\n");

    // destroy the server socket
    socket_tcp_destroy(&server_socket);

    // success
    return EXIT_SUCCESS;

    // error handling
    {

        // standard library errors
        {
            failed_to_open_file:
                log_error("Error: Failed to open file!\n");
                return EXIT_FAILURE;
        }
    }
}