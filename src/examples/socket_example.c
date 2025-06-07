// Include
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// socket submodule
#include <core/socket.h>

// Preprocessor macros
#define MAX_BUFFER_LEN 65536

int accept_callback ( socket_tcp _socket_tcp, unsigned long ip_address, unsigned short port )
{

    // Uninitialized data
    char buffer[MAX_BUFFER_LEN];

    // Initialized data
    unsigned char a = (ip_address & 0xff000000) >> 24,
                  b = (ip_address & 0x00ff0000) >> 16,
                  c = (ip_address & 0x0000ff00) >> 8,
                  d = (ip_address & 0x000000ff) >> 0;
    
    // Log the IP
    printf("%d.%d.%d.%d:%d\n", a, b, c, d, port);

    // Receive data from the socket
    socket_tcp_receive(_socket_tcp, buffer, MAX_BUFFER_LEN);

    // Success
    return 1;
}

// Entry point
int main ( int argc, const char *argv[] )
{

    // Unused
    (void) argc;
    (void) argv;

    // Initialized data
    socket_tcp _tcp_socket = 0;
    socket_ip_address g10_app = 0;

    // Resolve "g10.app"
    socket_resolve_host(&g10_app, "g10.app");

    // Initialized data
    unsigned char a = (g10_app & 0xff000000) >> 24,
                  b = (g10_app & 0x00ff0000) >> 16,
                  c = (g10_app & 0x0000ff00) >> 8,
                  d = (g10_app & 0x000000ff) >> 0;
    
    // Log the IP
    printf("%d.%d.%d.%d\n", a, b, c, d);

    socket_tcp_connect(&_tcp_socket, socket_address_family_ipv4, g10_app, 6710);

    socket_tcp_destroy(&_tcp_socket);

    // Success
    return EXIT_SUCCESS;
}