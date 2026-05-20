/** !
 * Time Server
 *
 * @file src/utilities/network/tcp/time_server.c
 *
 * @author Jacob Smith
 */

// standard library
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

// gsdk
/// core
#include <core/log.h>
#include <core/pack.h>
#include <core/socket.h>
#include <core/stream.h>
#include <core/sync.h> 
#include <core/tcp.h>

int connection_callback ( socket_tcp socket, socket_ip_address ip, socket_port port, void *const p_parameter )
{

    // unused
    (void) port;
    (void) p_parameter;
    
    // initialized data
    struct tm* ptr = NULL;
    time_t lt = 0;
    stream *p_stream = NULL;
    char _buf[1024] = { 0 };
    size_t len = 0;

    // logs
    printf("Accepted connection from "), 
    socket_ip_address_print(ip);

    // construct a stream 
    stream_from_tcp_socket(&p_stream, socket);

    // store the current time
    lt = time(NULL);

    // compute the localized time
    ptr = localtime(&lt);

    // pack the time string into a buffer
    len = pack_pack(_buf, "%s", asctime(ptr));

    // send the localized time to the client
    stream_write(p_stream, _buf, len);

    // destroy the stream
    stream_destroy(&p_stream);

    // clean up
    socket_tcp_destroy(&socket); 

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

    // create a tcp socket on port 3000
    socket_tcp_create(&server_socket, socket_address_family_ipv4, 3000);

    // logs
    log_info("Server listening on port %hu\n", 3000);

    // listen for connections
    while ( true )
        socket_tcp_listen(server_socket, connection_callback, NULL);

    // logs
    log_info("Shutting down server...\n");

    // destroy the server socket
    socket_tcp_destroy(&server_socket);

    // success
    return EXIT_SUCCESS;
}
