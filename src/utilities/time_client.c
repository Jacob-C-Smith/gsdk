/** !
 * Time Client
 *
 * @file src/utilities/time_client.c
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
    socket_ip_address server_ip = { 0 };
    short len = 0;
    char _buf[1024] = { 0 };

    // create a TCP socket
    socket_tcp_create(&client_socket, socket_address_family_ipv4, 0);

    // resolve server address
    socket_resolve_host(&server_ip, 1, "127.0.0.1");

    // connect to the server
    socket_tcp_connect(&client_socket, socket_address_family_ipv4, server_ip, 3000);

    // receive message length
    socket_tcp_receive(client_socket, &len, 2);

    // receive message
    socket_tcp_receive(client_socket, &_buf, len);
    
    // print the message
    log_info("%s", _buf);

    // close the socket
    socket_tcp_destroy(&client_socket);
    
    // done
    return EXIT_SUCCESS;
}
