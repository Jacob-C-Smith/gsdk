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
#include <limits.h>
#include <time.h>

// core
#include <core/log.h>
#include <core/socket.h>

// performance
#include <performance/parallel.h>
#include <performance/thread_pool.h>

// function declarations
int callback ( socket_tcp _socket_tcp, socket_ip_address ip_address, socket_port port_number, void *const p_parameter );

// data
thread_pool *p_thread_pool = NULL;

// function definitions
// entry point
int main ( int argc, const char *argv[] )
{

    // unused
    (void) argc;
    (void) argv;

    // initialized data
    socket_tcp _socket = { 0 };

    thread_pool_construct(&p_thread_pool, 4);

    socket_tcp_create(&_socket, socket_address_family_ipv4, 3000);

    socket_tcp_listen(_socket, callback, NULL);

    // success
    return EXIT_SUCCESS;
}

int callback ( socket_tcp _socket_tcp, socket_ip_address ip_address, socket_port port_number, void *const p_parameter )
{
    // log connection
    log_info("Accepted connection from %lu:%hu", ip_address, port_number);

    // success
    return 1;
}

int echo ( socket_tcp _socket_tcp )
{

    // initialized data
    char _buffer[1024] = { 0 };

    socket_tcp_receive(_socket_tcp, _buffer, sizeof(_buffer));

    socket_tcp_send(_socket_tcp, _buffer, strlen(_buffer));

    // success
    return 1;
}