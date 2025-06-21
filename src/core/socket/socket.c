/** !
 * Cross platform socket primitives 
 * 
 * @file socket.c 
 * 
 * @author Jacob Smith
 */

// Include 
#include <core/socket.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>

int socket_tcp_create ( socket_tcp *const p_socket_tcp, enum socket_address_family_e address_family, unsigned short port_number )
{

    // argument check
    if ( p_socket_tcp == (void *) 0 ) goto no_tcp_socket; 

    // Platform specific initialized data
    #ifdef _WIN64
        socket_tcp _socket_tcp = INVALID_SOCKET;
    #else
        socket_tcp _socket_tcp = -1;
        int option = 1;
        struct sockaddr_in socket_address =
        {
            .sin_family = (unsigned short) address_family,
            .sin_addr.s_addr = htonl(INADDR_ANY),
            .sin_port = htons(port_number),
            .sin_zero = { 0 }
        };
    #endif

    // Platform specific implementation
    #ifdef _WIN64

        // Create the socket
        _socket_tcp = socket(address_family, socket_type_tcp, IPPROTO_TCP);

        // error check
        if ( _socket_tcp == INVALID_SOCKET ) goto failed_to_create_socket;

    #elif __APPLE__

        // Create the socket
        _socket_tcp = socket(address_family, socket_type_tcp, IPPROTO_TCP);

        // error check
        if ( _socket_tcp == -1 ) goto failed_to_create_socket;
        
        // Set socket options
        if ( setsockopt(_socket_tcp, SOL_SOCKET, SO_REUSEADDR, &option, sizeof(option)) == -1 ) goto failed_to_set_socket_option;

        // Bind the socket to the port
        if ( bind(_socket_tcp,(struct sockaddr*) &socket_address, sizeof(socket_address)) == -1 ) goto failed_to_bind_socket;

    #else

        // Create the socket
        _socket_tcp = socket((int)address_family, socket_type_tcp, IPPROTO_TCP);

        // error check
        if ( _socket_tcp == -1 ) goto failed_to_create_socket;
        
        // Set socket options
        if ( setsockopt(_socket_tcp, SOL_SOCKET, SO_REUSEADDR, &option, sizeof(option)) == -1 ) goto failed_to_set_socket_option;

        // Bind the socket to the port
        if ( bind(_socket_tcp,(struct sockaddr*) &socket_address, sizeof(socket_address)) == -1 ) goto failed_to_bind_socket;

    #endif

    // Return a socket to the caller
    *p_socket_tcp = _socket_tcp;

    // success
    return 1;

    // error handling
    {
        
        // argument errors
        {
            no_tcp_socket:
                #ifndef NDEBUG
                    printf("[socket] Null pointer provided for parameter \"p_tcp_socket\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }

        // Socket errors
        {
            failed_to_create_socket:
                #ifndef NDEBUG
                    printf("[socket] Call to function \"socket\" returned an erroneous value in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;

            failed_to_set_socket_option:
                #ifndef NDEBUG
                    printf("[socket] Call to function \"setsockopt\" returned an erroneous value in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
            
            failed_to_bind_socket:
                #ifndef NDEBUG
                    printf("[socket] Call to function \"bind\" returned an erroneous value in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;

        }
    }
}

int socket_tcp_listen ( socket_tcp _socket_tcp, fn_socket_tcp_accept pfn_tcp_accept_callback, void *const p_tcp_accept_callback_parameter )
{

    // initialized data
    socket_tcp new_socket = 0;
    struct sockaddr_in peer_addr = {0};
    unsigned int addr_len = sizeof(peer_addr);

    // Listen for connections
    if ( listen(_socket_tcp, 1) == -1 ) goto failed_to_listen;

    // Accept a new connection
    new_socket = accept(_socket_tcp, (struct sockaddr *) &peer_addr, &addr_len);

    // error check
    if ( new_socket == -1 ) goto failed_to_connect;
    
    // Callback
    pfn_tcp_accept_callback(new_socket, ntohl(peer_addr.sin_addr.s_addr), (unsigned short) ntohs(peer_addr.sin_port), p_tcp_accept_callback_parameter);

    // success
    return 1;

    // error handling
    {

        // Socket errors
        {
            failed_to_listen:
                #ifndef NDEBUG
                    printf("[socket] Call to function \"listen\" returned an erroneous value in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
            
            failed_to_connect:
                #ifndef NDEBUG
                    printf("[socket] Call to function \"connect\" returned an erroneous value in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
    }
}

int socket_tcp_receive ( socket_tcp _socket_tcp, void *p_buffer, size_t buffer_len )
{

    // argument check
    if ( p_buffer == (void *) 0 ) goto no_buffer;

    // initialized data
    size_t r = (size_t) recv(_socket_tcp, p_buffer, buffer_len, 0);

    // error check
    if ( r < 1 ) goto failed_to_recv;

    // success
    return 1;

    // error handling
    {

        // argument errors
        {
            no_buffer:
                #ifndef NDEBUG
                    printf("[socket] Null pointer provided for parameter \"p_buffer\" in call to function \"%s\"\n", __FUNCTION__);
                #endif 

                // error
                return 0;
        }

        // Socket errors
        {
            failed_to_recv:
                #ifndef NDEBUG
                    printf("[socket] Call to \"recv\" returned an erroneous value in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
    }
}

int socket_tcp_send ( socket_tcp _socket_tcp, const void *const p_buffer, size_t buffer_len )
{

    // argument check
    if ( p_buffer   == (void *) 0 ) goto no_buffer;

    // Send data to the TCP socket
    if ( send(_socket_tcp, p_buffer, buffer_len, 0) == -1 ) goto failed_to_send;

    // success
    return 1;

    // error handling
    {

        // argument errors
        {
            no_buffer:
                #ifndef NDEBUG
                    printf("[socket] Null pointer provided for parameter \"p_buffer\" in call to function \"%s\"\n", __FUNCTION__);
                #endif 

                // error
                return 0;
        }

        // Socket errors
        {
            failed_to_send:
                #ifndef NDEBUG
                    printf("[socket] Call to \"send\" returned an erroneous value in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
    }
}

int socket_tcp_connect ( socket_tcp *const p_socket_tcp, enum socket_address_family_e address_family, socket_ip_address ip_address, socket_port port_number )
{

    // argument check
    if ( p_socket_tcp == (void *) 0 ) goto no_tcp_socket; 

    // Platform specific initialized data
    #ifdef _WIN64
        socket_tcp _socket_tcp = INVALID_SOCKET;
    #else
        socket_tcp _socket_tcp = -1;
        struct sockaddr_in serv_addr =
        {
            .sin_family = (unsigned short) address_family,
            .sin_addr.s_addr = (unsigned int) htonl((unsigned int)ip_address),
            .sin_port = htons(port_number),
            .sin_zero = { 0 }
        };
    #endif

    // Platform specific implementation
    #ifdef _WIN64


    #elif __APPLE__

        // Create the socket
        _socket_tcp = socket((int) address_family, socket_type_tcp, IPPROTO_TCP);

        // error check
        if ( _socket_tcp == -1 ) goto failed_to_create_socket;
        
        // Connect to the socket
        if ( connect(_socket_tcp, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) ) goto failed_to_connect; 
    #else

        // Create the socket
        _socket_tcp = socket((int) address_family, socket_type_tcp, IPPROTO_TCP);

        // error check
        if ( _socket_tcp == -1 ) goto failed_to_create_socket;
        
        // Connect to the socket
        if ( connect(_socket_tcp, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) ) goto failed_to_connect; 
    #endif

    // Return a socket to the caller
    *p_socket_tcp = _socket_tcp;

    // success
    return 1;

    // error handling
    {
        
        // argument errors
        {
            no_tcp_socket:
                #ifndef NDEBUG
                    printf("[socket] Null pointer provided for parameter \"p_tcp_socket\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }

        // Socket errors
        {
            failed_to_create_socket:
                #ifndef NDEBUG
                    printf("[socket] Call to function \"socket\" returned an erroneous value in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;

            failed_to_connect:
                #ifndef NDEBUG
                    printf("[socket] Call to function \"connect\" returned an erroneous value in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
    }
}

int socket_resolve_host ( socket_ip_address *p_ip_address, const char *restrict p_hostname )
{

    // initialized data
    struct addrinfo  hints = { 0 },
                    *addr_result = 0;
    struct sockaddr_in *ipv4 = (void *) 0;
    int result = 0;

    // Initialize the hints structure
    hints = (struct addrinfo)
    {
        .ai_family   = AF_INET,
        .ai_socktype = SOCK_STREAM
    };

    // Get address info
    result = getaddrinfo(p_hostname, NULL, &hints, &addr_result);

    // error check
    if ( result ) goto failed_to_resolve_address;

    // Store the address
    ipv4 = (struct sockaddr_in *)addr_result->ai_addr;

    *p_ip_address = ntohl(ipv4->sin_addr.s_addr);

    // success
    return 1;

    // error handling
    {

        // Socket errors
        {
            failed_to_resolve_address:
                #ifndef NDEBUG
                    printf("[socket] Failed to resolve hostname \"%s\" in call to function \"%s\". Network database says: %s", p_hostname, __FUNCTION__, gai_strerror(result));
                #endif

                // error
                return 0;
        }
    }
}

int socket_tcp_destroy ( socket_tcp *p_socket_tcp )
{

    // argument check
    if ( p_socket_tcp == (void *) 0 ) goto no_socket;

    // initialized data
    socket_tcp _socket_tcp = *p_socket_tcp;

    // No more pointer for caller
    *p_socket_tcp = 0;

    // Platform dependent implementation
    #ifdef _WIN64

        // Shutdown the socket
        if ( shutdown(_socket_tcp, SD_SEND) == -1 ) goto failed_to_shutdown_socket;
        
        // Close the socket
        if ( closesocket(_socket_tcp) == -1 ) goto failed_to_close_socket;

    #else

        socket_tcp_send(_socket_tcp, "", 0);

        // Shutdown the socket
        if ( shutdown(_socket_tcp, SHUT_RDWR) == -1 ) goto failed_to_shutdown_socket;
        
        // Close the socket
        if ( close(_socket_tcp) == -1 ) goto failed_to_close_socket;

    #endif

    // success
    return 1;

    // error handling
    {

        // argument errors
        {
            no_socket:
                #ifndef NDEBUG
                    printf("[socket] Null pointer provided for parameter \"p_socket_tcp\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }

        // Socket errors
        {
            failed_to_shutdown_socket:
                #ifndef NDEBUG
                    printf("[socket] Call to function \"shutdown\" returned an erroneous value in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
            
            failed_to_close_socket:
                #ifndef NDEBUG
                    printf("[socket] Call to function \"close\" returned an erroneous value in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
                
        }
    }
}
