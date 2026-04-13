/** !
 * Echo Client
 *
 * @file src/utilities/echo_client.c
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
#include <core/udp.h>
#include <core/pack.h>

// entry point
int main ( int argc, const char *argv[] )
{

    // unused parameters
    (void)argc;
    (void)argv;

    // initialized data
    socket_udp client_socket = 0;
    socket_ip_address server_ip = { 0 };
    unsigned short len = 0;
    char _buf[1024] = { 0 };
    const char *p_msg = "Hello, World!";

    // create a UDP socket
    socket_udp_create(&client_socket, socket_address_family_ipv4, 0);

    // resolve server address
    socket_resolve_host(&server_ip, 1, "127.0.0.1");

    // send message to the server
    socket_udp_send_to(client_socket, p_msg, strlen(p_msg) + 1, server_ip, 3000);

    // receive message length (2 bytes)
    socket_udp_receive_from(client_socket, &len, 2, NULL, NULL);
    
    // receive the message
    socket_udp_receive_from(client_socket, _buf, (size_t)len, NULL, NULL);

    // print the message
    log_info("%s\n", _buf);

    // close the socket
    socket_udp_destroy(&client_socket);
    
    // done
    return EXIT_SUCCESS;
}
