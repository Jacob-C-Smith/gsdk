/** !
 * Socket UDP implementation 
 * 
 * @file src/core/socket/udp.c 
 * 
 * @author Jacob Smith
 */

// header
#include <core/udp.h>

int socket_udp_create ( socket_udp *const p_socket_udp, enum socket_address_family_e address_family, socket_port port_number )
{

    // argument check
    if ( p_socket_udp == (void *) 0 ) goto no_udp_socket; 

    // platform specific initialized data
    #ifdef _WIN64
        socket_udp _socket_udp = INVALID_SOCKET;
    #else
        socket_udp              _socket_udp = -1;
        int                     option      = 1;
        int                     af          = ( address_family == socket_address_family_ipv4 ) ? AF_INET : AF_INET6;
    #endif

    // platform specific implementation
    #ifdef _WIN64

        // create the socket
        _socket_udp = socket(address_family, SOCK_DGRAM, IPPROTO_UDP);

        // error check
        if ( _socket_udp == INVALID_SOCKET ) goto failed_to_create_socket;

    #else

        // create the socket
        _socket_udp = socket(af, SOCK_DGRAM, IPPROTO_UDP);

        // error check
        if ( _socket_udp == -1 ) goto failed_to_create_socket;
        
        // set socket options
        if ( setsockopt(_socket_udp, SOL_SOCKET, SO_REUSEADDR, &option, sizeof(option)) == -1 ) goto failed_to_set_socket_option;

        // bind the socket to the port
        if ( address_family == socket_address_family_ipv4 )
        {
            struct sockaddr_in socket_address =
            {
                .sin_family = AF_INET,
                .sin_addr.s_addr = htonl(INADDR_ANY),
                .sin_port = htons(port_number)
            };

            // bind the socket
            if ( bind(_socket_udp,(struct sockaddr*) &socket_address, sizeof(socket_address)) == -1 ) goto failed_to_bind_socket;
        }
        else 
        {
            struct sockaddr_in6 socket_address =
            {
                .sin6_family = AF_INET6,
                .sin6_addr   = in6addr_any,
                .sin6_port   = htons(port_number)
            };

            // bind the socket
            if ( bind(_socket_udp,(struct sockaddr*) &socket_address, sizeof(socket_address)) == -1 ) goto failed_to_bind_socket;
        }

    #endif

    // return a socket to the caller
    *p_socket_udp = _socket_udp;

    // success
    return 1;

    // error handling
    {
        
        // argument errors
        {
            no_udp_socket:
                #ifndef NDEBUG
                    printf("[socket] Null pointer provided for parameter \"p_udp_socket\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }

        // socket errors
        {
            failed_to_create_socket:
                #ifndef NDEBUG
                    printf("[socket] Call to function \"socket\" returned an erroneous value in call to function \"%s\": %s (%d)\n", __FUNCTION__, strerror(errno), errno);
                #endif

                // error
                return 0;

            failed_to_set_socket_option:
                #ifndef NDEBUG
                    printf("[socket] Call to function \"setsockopt\" returned an erroneous value in call to function \"%s\": %s (%d)\n", __FUNCTION__, strerror(errno), errno);
                #endif

                // error
                return 0;
            
            failed_to_bind_socket:
                #ifndef NDEBUG
                    printf("[socket] Call to function \"bind\" returned an erroneous value in call to function \"%s\": %s (%d)\n", __FUNCTION__, strerror(errno), errno);
                #endif

                // error
                return 0;

        }
    }
}

int socket_udp_listen ( socket_udp _socket_udp, fn_socket_udp_accept pfn_udp_accept_callback, void *const p_udp_accept_callback_parameter )
{

    // initialized data
    struct sockaddr_storage peer_addr  = { 0 };
    socklen_t       addr_len   = sizeof(peer_addr);
    socket_ip_address ip_address = { 0 };
    char            peek_buffer[1];

    // wait for a packet
    if ( recvfrom(_socket_udp, peek_buffer, 1, MSG_PEEK, (struct sockaddr *)&peer_addr, &addr_len) == -1 ) goto failed_to_receive;

    // store the type
    ip_address._type = (enum socket_address_family_e) ( ( peer_addr.ss_family == AF_INET ) ? socket_address_family_ipv4 : socket_address_family_ipv6 );

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
    pfn_udp_accept_callback(
        _socket_udp, 
        ip_address, 
        (unsigned short)ntohs(((struct sockaddr_in *)&peer_addr)->sin_port),
        p_udp_accept_callback_parameter
    );

    // success
    return 1;

    // error handling
    {

        // socket errors
        {
            failed_to_receive:
                #ifndef NDEBUG
                    printf("[socket] Call to function \"recvfrom\" returned an erroneous value in call to function \"%s\": %s (%d)\n", __FUNCTION__, strerror(errno), errno);
                #endif

                // error
                return 0;
        }
    }
}

int socket_udp_receive_from ( socket_udp _socket_udp, void *const p_buffer, size_t buffer_len, socket_ip_address *const p_ip_address, socket_port *const p_port )
{

    // argument check
    if ( p_buffer == (void *) 0 ) goto no_buffer;

    // initialized data
    struct sockaddr_storage peer_addr = { 0 };
    socklen_t               addr_len  = sizeof(peer_addr);
    int                     r         = (int) recvfrom(_socket_udp, p_buffer, buffer_len, 0, (struct sockaddr *)&peer_addr, &addr_len);

    // error check
    if ( r < 1 ) goto failed_to_recv;

    // store the address
    if ( p_ip_address )
    {
        p_ip_address->_type = (enum socket_address_family_e) ( ( peer_addr.ss_family == AF_INET ) ? socket_address_family_ipv4 : socket_address_family_ipv6 );
        if ( peer_addr.ss_family == AF_INET )
        {
            struct sockaddr_in *s = (struct sockaddr_in *)&peer_addr;
            p_ip_address->_address.ipv4 = ntohl(s->sin_addr.s_addr);
        }
        else if ( peer_addr.ss_family == AF_INET6 )
        {
            struct sockaddr_in6 *s = (struct sockaddr_in6 *)&peer_addr;
            memcpy(p_ip_address->_address.ipv6, s->sin6_addr.s6_addr, 16);
        }
    }

    // store the port
    if ( p_port ) *p_port = (unsigned short)ntohs(((struct sockaddr_in *)&peer_addr)->sin_port);

    // success
    return r;

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

        // socket errors
        {
            failed_to_recv:
                #ifndef NDEBUG
                    printf("[socket] Call to \"recvfrom\" returned an erroneous value in call to function \"%s\": %s (%d)\n", __FUNCTION__, strerror(errno), errno);
                #endif

                // error
                return 0;
        }
    }
}

int socket_udp_send_to ( socket_udp _socket_udp, const void *const p_buffer, size_t buffer_len, socket_ip_address ip_address, socket_port port )
{

    // argument check
    if ( p_buffer == (void *) 0 ) goto no_buffer;

    // initialized data
    struct sockaddr_storage serv_addr = { 0 };
    socklen_t               addr_len  = 0;
    int                     result    = -1;

    if ( ip_address._type == socket_address_family_ipv4 )
    {
        struct sockaddr_in *s = (struct sockaddr_in *)&serv_addr;
        s->sin_family      = AF_INET;
        s->sin_port        = htons(port);
        s->sin_addr.s_addr = htonl(ip_address._address.ipv4);
        addr_len           = sizeof(struct sockaddr_in);
    }
    else if ( ip_address._type == socket_address_family_ipv6 )
    {
        struct sockaddr_in6 *s = (struct sockaddr_in6 *)&serv_addr;
        s->sin6_family = AF_INET6;
        s->sin6_port   = htons(port);
        memcpy(s->sin6_addr.s6_addr, ip_address._address.ipv6, 16);
        addr_len       = sizeof(struct sockaddr_in6);
    }

    // send to
    result = sendto(_socket_udp, p_buffer, buffer_len, 0, (struct sockaddr *)&serv_addr, addr_len);
    if ( result == -1 ) goto failed_to_send;

    // success
    return result;

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

        // socket errors
        {
            failed_to_send:
                #ifndef NDEBUG
                    printf("[socket] Call to \"sendto\" returned an erroneous value in call to function \"%s\": %s (%d)\n", __FUNCTION__, strerror(errno), errno);
                #endif

                // error
                return 0;
        }
    }
}

int socket_udp_connect ( socket_udp *const p_socket_udp, enum socket_address_family_e address_family, socket_ip_address ip_address, socket_port port_number )
{

    // argument check
    if ( p_socket_udp == (void *) 0 ) goto no_udp_socket; 

    // initialized data
    socket_udp _socket_udp = *p_socket_udp;

    // platform specific implementation
    #ifdef _WIN64

        // ...

    #else

        // connect to the socket
        if ( ip_address._type == socket_address_family_ipv4 )
        {
            struct sockaddr_in serv_addr = 
            {
                .sin_family = AF_INET,
                .sin_port   = htons(port_number),
                .sin_addr.s_addr = htonl(ip_address._address.ipv4)
            };

            // connect
            if ( connect(_socket_udp, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) ) goto failed_to_connect; 
        }
        else 
        {
            struct sockaddr_in6 serv_addr = 
            {
                .sin6_family = AF_INET6,
                .sin6_port   = htons(port_number)
            };
            memcpy(serv_addr.sin6_addr.s6_addr, ip_address._address.ipv6, 16);

            // connect
            if ( connect(_socket_udp, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) ) goto failed_to_connect; 
        }
    #endif

    // success
    return 1;

    // error handling
    {
        
        // argument errors
        {
            no_udp_socket:
                #ifndef NDEBUG
                    printf("[socket] Null pointer provided for parameter \"p_socket_udp\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }

        // socket errors
        {
            failed_to_connect:
                #ifndef NDEBUG
                    printf("[socket] Call to function \"connect\" returned an erroneous value in call to function \"%s\": %s (%d)\n", __FUNCTION__, strerror(errno), errno);
                #endif

                // error
                return 0;
        }
    }
}

int socket_udp_destroy ( socket_udp *p_socket_udp )
{

    // argument check
    if ( p_socket_udp == (void *) 0 ) goto no_socket;

    // initialized data
    socket_udp _socket_udp = *p_socket_udp;

    // no more pointer for caller
    *p_socket_udp = 0;

    // platform dependent implementation
    #ifdef _WIN64

        // close the socket
        if ( closesocket(_socket_udp) == -1 ) goto failed_to_close_socket;

    #else

        // close the socket
        if ( close(_socket_udp) == -1 ) goto failed_to_close_socket;
    #endif

    // success
    return 1;

    // error handling
    {

        // argument errors
        {
            no_socket:
                #ifndef NDEBUG
                    printf("[socket] Null pointer provided for parameter \"p_socket_udp\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }

        // socket errors
        {
            failed_to_close_socket:
                #ifndef NDEBUG
                    printf("[socket] Call to function \"close\" returned an erroneous value in call to function \"%s\": %s (%d)\n", __FUNCTION__, strerror(errno), errno);
                #endif

                // error
                return 0;
                
        }
    }
}
