/** ! 
 * Secure Socket Client
 * 
 * @file src/utilities/secure_socket_client.c
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
#include <core/secure_socket.h>
#include <core/rsa.h>
// #include <core/aes.h>
#include <core/pack.h>

// data
#include <data/dict.h>

// reflection
#include <reflection/base64.h>
#include <reflection/json.h>

// entry point
int main ( int argc, const char *argv[] )
{
    
    // unused
    (void) argc;
    (void) argv;

    // initialized data
    secure_socket *p_secure_socket = NULL;
    socket_ip_address _ip = 0;

    // construct a secure socket
    secure_socket_create(&p_secure_socket, socket_address_family_ipv4, 3000);

    // resolve the ip of the server
    socket_resolve_host(&_ip, "localhost");

    // connect to the server
    secure_socket_connect(p_secure_socket, socket_address_family_ipv4, _ip, 3000);

    // start a secure connection
    {

    }

    // transmit a file confidentiality
    {

    }

    // success
    return EXIT_SUCCESS;
}
