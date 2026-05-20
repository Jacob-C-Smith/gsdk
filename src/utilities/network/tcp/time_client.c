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
#include <core/pack.h>
#include <core/socket.h>
#include <core/stream.h>
#include <core/tcp.h>

// entry point
int main ( int argc, const char *argv[] )
{

    // unused parameters
    (void)argc;
    (void)argv;

    // initialized data
    socket_tcp client_socket = 0;
    socket_ip_address server_ip = { 0 };
    stream *p_stream = NULL;
    short len = 0;
    char _buf[1024] = { 0 };

    // create a TCP socket
    socket_tcp_create(&client_socket, socket_address_family_ipv4, 0);

    // resolve server address
    socket_resolve_host(&server_ip, 1, "127.0.0.1");

    // connect to the server
    socket_tcp_connect(&client_socket, socket_address_family_ipv4, server_ip, 3000);

    // construct a stream from the socket
    stream_from_tcp_socket(&p_stream, client_socket);
    
    // receive message length
    stream_read(p_stream, &len, 2);

    // receive message
    stream_read(p_stream, &_buf, len);
    
    // print the message
    log_info("%s", _buf);

    // destroy the stream
    stream_destroy(&p_stream);

    // close the socket
    socket_tcp_destroy(&client_socket);
    
    // done
    return EXIT_SUCCESS;
}
