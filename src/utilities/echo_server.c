/** !
 * Echo Server
 *
 * @file src/utilities/echo_server.c
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

int connection_callback ( socket_udp socket, socket_ip_address ip, socket_port port, void *const p_parameter )
{

    // initialized data
    char _buf[1024] = { 0 };
    int len = 0;
    unsigned short nlen = 0;

    // logs
    printf("Accepted connection from "), 
    socket_ip_address_print(ip);

    // receive the packet
    len = socket_udp_receive_from(socket, _buf, sizeof(_buf), NULL, NULL);

    // error check
    if ( len <= 0 ) return 0;

    // send the message length (2 bytes)
    nlen = (unsigned short)len;
    socket_udp_send_to(socket, &nlen, 2, ip, port);

    // echo the message back to the client
    socket_udp_send_to(socket, _buf, (size_t)len, ip, port);

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
    socket_udp server_socket = 0;

    // create a udp socket on port 3000
    socket_udp_create(&server_socket, socket_address_family_ipv4, 3000);

    // logs
    log_info("Server listening on port %hu\n", 3000);

    // listen for connections
    while ( socket_udp_listen(server_socket, connection_callback, NULL) ) ;

    // logs
    log_info("Shutting down server...\n");

    // destroy the server socket
    socket_udp_destroy(&server_socket);

    // success
    return EXIT_SUCCESS;
}
