/** !
 * Socket TCP implementation 
 * 
 * @file src/core/socket/tcp.c 
 * 
 * @author Jacob Smith
 */

// header
#include <core/tcp.h>

int socket_tcp_create ( socket_tcp *const p_socket_tcp, enum socket_address_family_e address_family, unsigned short port_number )
{

    // argument check
    if ( p_socket_tcp == (void *) 0 ) goto no_tcp_socket; 

    // platform specific initialized data
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

    // platform specific implementation
    #ifdef _WIN64

        // create the socket
        _socket_tcp = socket(address_family, socket_type_tcp, IPPROTO_TCP);

        // error check
        if ( _socket_tcp == INVALID_SOCKET ) goto failed_to_create_socket;

    #elif __APPLE__

        // create the socket
        _socket_tcp = socket(address_family, socket_type_tcp, IPPROTO_TCP);

        // error check
        if ( _socket_tcp == -1 ) goto failed_to_create_socket;
        
        // set options
        if ( setsockopt(_socket_tcp, SOL_SOCKET, SO_REUSEADDR, &option, sizeof(option)) == -1 ) goto failed_to_set_socket_option;

        // bind the socket to the port
        if ( bind(_socket_tcp,(struct sockaddr*) &socket_address, sizeof(socket_address)) == -1 ) goto failed_to_bind_socket;

    #else

        // create the socket
        _socket_tcp = socket((int)address_family, socket_type_tcp, IPPROTO_TCP);

        // error check
        if ( _socket_tcp == -1 ) goto failed_to_create_socket;
        
        // set socket options
        if ( setsockopt(_socket_tcp, SOL_SOCKET, SO_REUSEADDR, &option, sizeof(option)) == -1 ) goto failed_to_set_socket_option;

        // bind the socket to the port
        if ( bind(_socket_tcp,(struct sockaddr*) &socket_address, sizeof(socket_address)) == -1 ) goto failed_to_bind_socket;

    #endif

    // return a socket to the caller
    *p_socket_tcp = _socket_tcp;

    // success
    return 1;

    // error handling
    {
        
        // argument errors
        {
            no_tcp_socket:
                #ifndef NDEBUG
                    log_error("[socket] Null pointer provided for parameter \"p_tcp_socket\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }

        // socket errors
        {
            failed_to_create_socket:
                #ifndef NDEBUG
                    log_error("[socket] Call to function \"socket\" returned an erroneous value in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;

            failed_to_set_socket_option:
                #ifndef NDEBUG
                    log_error("[socket] Call to function \"setsockopt\" returned an erroneous value in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
            
            failed_to_bind_socket:
                #ifndef NDEBUG
                    log_error("[socket] Call to function \"bind\" returned an erroneous value in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;

        }
    }
}

int socket_tcp_listen ( socket_tcp _socket_tcp, fn_socket_tcp_accept pfn_tcp_accept_callback, void *const p_tcp_accept_callback_parameter )
{

    // initialized data
    socket_tcp      new_socket = 0;
    struct sockaddr_storage peer_addr  = { 0 };
    socklen_t       addr_len   = sizeof(peer_addr);
    socket_ip_address ip_address = { 0 };

    // listen for connections
    if ( listen(_socket_tcp, 1) == -1 ) goto failed_to_listen;

    // accept a new connection
    new_socket = accept(_socket_tcp, (struct sockaddr *)&peer_addr, &addr_len);

    // error check
    if ( new_socket == -1 ) goto failed_to_connect;
    
    // store the type
    ip_address._type = (enum socket_address_family_e) peer_addr.ss_family;

    // store the address
    if ( peer_addr.ss_family == AF_INET )
    {
        struct sockaddr_in *s = (struct sockaddr_in *)&peer_addr;
        ip_address._address.ipv4 = ntohl(s->sin_addr.s_addr);
    }
    else if ( peer_addr.ss_family == AF_INET6 )
    {
        struct sockaddr_in6 *s = (struct sockaddr_in6 *)&peer_addr;
        memcpy(ip_address._address.ipv6, s->sin6_addr.s6_addr, 16);
    }

    // callback
    pfn_tcp_accept_callback(
        new_socket, 
        ip_address, 
        (unsigned short)ntohs(((struct sockaddr_in *)&peer_addr)->sin_port),
        p_tcp_accept_callback_parameter
    );

    // success
    return 1;

    // error handling
    {

        // socket errors
        {
            failed_to_listen:
                #ifndef NDEBUG
                    log_error("[socket] Call to function \"listen\" returned an erroneous value in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
            
            failed_to_connect:
                #ifndef NDEBUG
                    log_error("[socket] Call to function \"connect\" returned an erroneous value in call to function \"%s\"\n", __FUNCTION__);
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
    int r = (int) recv(_socket_tcp, p_buffer, buffer_len, 0);

    // error check
    if ( r < 0 ) goto failed_to_recv;

    // success
    return r;

    // error handling
    {

        // argument errors
        {
            no_buffer:
                #ifndef NDEBUG
                    log_error("[socket] Null pointer provided for parameter \"p_buffer\" in call to function \"%s\"\n", __FUNCTION__);
                #endif 

                // error
                return 0;
        }

        // socket errors
        {
            failed_to_recv:
                #ifndef NDEBUG
                    log_error("[socket] Call to \"recv\" returned an erroneous value in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
    }
}

int socket_tcp_send ( socket_tcp _socket_tcp, const void *const p_buffer, size_t buffer_len )
{

    // argument check
    if ( p_buffer == (void *) 0 ) goto no_buffer;

    // initialized data
    int result = -1;

    // send data to the TCP socket
    result = send(_socket_tcp, p_buffer, buffer_len, 0);
    if ( result == -1 ) goto failed_to_send;

    // success
    return result;

    // error handling
    {

        // argument errors
        {
            no_buffer:
                #ifndef NDEBUG
                    log_error("[socket] Null pointer provided for parameter \"p_buffer\" in call to function \"%s\"\n", __FUNCTION__);
                #endif 

                // error
                return 0;
        }

        // socket errors
        {
            failed_to_send:
                #ifndef NDEBUG
                    log_error("[socket] Call to \"send\" returned an erroneous value in call to function \"%s\"\n", __FUNCTION__);
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

    // platform specific initialized data
    #ifdef _WIN64
        socket_tcp _socket_tcp = INVALID_SOCKET;
    #else
        socket_tcp              _socket_tcp = -1;
        struct sockaddr_storage serv_addr   = { 0 };
        socklen_t               addr_len    = 0;

        if ( ip_address._type == socket_address_family_ipv4 )
        {
            struct sockaddr_in *s = (struct sockaddr_in *)&serv_addr;
            s->sin_family      = AF_INET;
            s->sin_port        = htons(port_number);
            s->sin_addr.s_addr = htonl(ip_address._address.ipv4);
            addr_len           = sizeof(struct sockaddr_in);
        }
        else if ( ip_address._type == socket_address_family_ipv6 )
        {
            struct sockaddr_in6 *s = (struct sockaddr_in6 *)&serv_addr;
            s->sin6_family = AF_INET6;
            s->sin6_port   = htons(port_number);
            memcpy(s->sin6_addr.s6_addr, ip_address._address.ipv6, 16);
            addr_len       = sizeof(struct sockaddr_in6);
        }
    #endif

    // platform specific implementation
    #ifdef _WIN64


    #else

        // create the socket
        _socket_tcp = socket((int) address_family, socket_type_tcp, IPPROTO_TCP);

        // error check
        if ( _socket_tcp == -1 ) goto failed_to_create_socket;
        
        // connect to the socket
        if ( connect(_socket_tcp, (struct sockaddr *) &serv_addr, addr_len) ) goto failed_to_connect; 
    #endif

    // return a socket to the caller
    *p_socket_tcp = _socket_tcp;

    // success
    return 1;

    // error handling
    {
        
        // argument errors
        {
            no_tcp_socket:
                #ifndef NDEBUG
                    log_error("[socket] Null pointer provided for parameter \"p_tcp_socket\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }

        // socket errors
        {
            failed_to_create_socket:
                #ifndef NDEBUG
                    log_error("[socket] Call to function \"socket\" returned an erroneous value in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;

            failed_to_connect:
                #ifndef NDEBUG
                    log_error("[socket] Call to function \"connect\" returned an erroneous value in call to function \"%s\"\n", __FUNCTION__);
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

    // no more pointer for caller
    *p_socket_tcp = 0;

    // platform dependent implementation
    #ifdef _WIN64

        // close the socket
        if ( closesocket(_socket_tcp) == -1 ) goto failed_to_close_socket;

    #else

        // close the socket
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
                    log_error("[socket] Null pointer provided for parameter \"p_socket_tcp\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }

        // socket errors
        {
            failed_to_close_socket:
                #ifndef NDEBUG
                    log_error("[socket] Call to function \"close\" returned an erroneous value in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
                
        }
    }
}
