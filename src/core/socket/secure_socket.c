/** !
 * Secure socket
 * 
 * @file src/core/socket/secure_socket.c 
 * 
 * @author Jacob Smith
 */

// header 
#include <core/secure_socket.h>

struct secure_socket_s
{
    bool server_flag;
    socket_tcp _socket;
    union 
    {
        struct 
        {
            public_key  *p_public_key;
            private_key *p_private_key;
        } server;

        struct 
        {
            struct
            {
                public_key  *p_public_key;
                private_key *p_private_key;
            } client_pair;
            public_key _server_public_key;
        } client;
    };
};

int secure_socket_create ( secure_socket **pp_secure_socket, enum socket_address_family_e address_family, unsigned short port_number )
{

    // unused 
    (void)address_family;
    (void)port_number;

    // argument check
    if ( pp_secure_socket == (void *) 0 ) goto no_secure_socket; 

    // initialized data
    secure_socket *p_secure_socket = default_allocator(0, sizeof(secure_socket));

    // error check
    if ( NULL == p_secure_socket ) goto no_mem;

    // return a pointer to the caller
    *pp_secure_socket = p_secure_socket;

    // success
    return 1;

    // error handling
    {
        
        // argument errors
        {
            no_secure_socket:
                #ifndef NDEBUG
                    printf("[secure socket] Null pointer provided for parameter \"pp_secure_socket\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
    
        // standard library errors
        {
            no_mem:
                #ifndef NDEBUG
                    log_error("[socket] Failed to allocate memory in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
    }
}

int secure_socket_listen ( secure_socket *p_secure_socket, fn_secure_socket_accept pfn_tcp_accept_callback, void *const p_tcp_accept_callback_parameter )
{

    // unused
    (void)pfn_tcp_accept_callback;
    (void)p_tcp_accept_callback_parameter;

    // initialized data
    socket_tcp new_socket = 0;
    struct sockaddr_in peer_addr = {0};
    unsigned int addr_len = sizeof(peer_addr);

    // make a keypair
    key_pair_construct
    (
        &p_secure_socket->server.p_public_key,
        &p_secure_socket->server.p_private_key    
    );

    // listen for connections
    if ( listen(p_secure_socket->_socket, 1) == -1 ) goto failed_to_listen;

    // accept a new connection
    new_socket = accept(p_secure_socket->_socket, (struct sockaddr *) &peer_addr, &addr_len);

    // error check
    if ( new_socket == -1 ) goto failed_to_connect;
    
    // handshake
    {
    
        // initialized data
        unsigned char _server_hello[4096] = { 0 };

        // pack the server's public key
        public_key_pack(_server_hello, p_secure_socket->server.p_public_key);

        // send the server hello
        socket_tcp_send
        (
            new_socket,
            _server_hello,
            4096
        );
    }

    // callback
    // pfn_tcp_accept_callback(new_socket, ntohl(peer_addr.sin_addr.s_addr), (unsigned short) ntohs(peer_addr.sin_port), p_tcp_accept_callback_parameter);

    // success
    return 1;

    // error handling
    {

        // Socket errors
        {
            failed_to_listen:
                #ifndef NDEBUG
                    printf("[secure socket] Call to function \"listen\" returned an erroneous value in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
            
            failed_to_connect:
                #ifndef NDEBUG
                    printf("[secure socket] Call to function \"connect\" returned an erroneous value in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
    }
}

int secure_socket_connect ( secure_socket *p_secure_socket, enum socket_address_family_e address_family, socket_ip_address ip_address, socket_port port_number )
{

    // argument check
    if ( p_secure_socket == (void *) 0 ) goto no_secure_socket; 

    struct sockaddr_in serv_addr =
    {
        .sin_family = (unsigned short) address_family,
        .sin_addr.s_addr = (unsigned int) htonl((unsigned int)ip_address),
        .sin_port = htons(port_number),
        .sin_zero = { 0 }
    };

    // create the socket
    p_secure_socket->_socket = socket((int) address_family, socket_type_tcp, IPPROTO_TCP);

    // error check
    if ( p_secure_socket->_socket == -1 ) goto failed_to_create_socket;
    
    // connect to the socket
    if ( connect(p_secure_socket->_socket, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) ) goto failed_to_connect; 
    
    // handshake
    {

        // initialized data
        unsigned char _server_hello[4096] = { 0 };
        unsigned char _client_response_plain[4096] = { 0 };
        unsigned char _client_response[4096] = { 0 };

        // hello
        {

            // read the server hello
            socket_tcp_receive
            (
                p_secure_socket->_socket,
                _server_hello,
                sizeof(_server_hello)
            );

            // unpack the server hello
            public_key_unpack
            (
                &p_secure_socket->client._server_public_key, 
                &_server_hello                               
            );

            // print the server's public key
            print_public_key(&p_secure_socket->client._server_public_key);

            // pack the client's private key using the server's public key
            private_key_pack
            (
                _client_response_plain,                           
                p_secure_socket->client.client_pair.p_private_key 
            );

            // encrypt the client's private key
            enc
            (
                _client_response_plain,
                _client_response,
                &p_secure_socket->client._server_public_key
            );

            // send the client's private key to the server
            socket_tcp_send(p_secure_socket->_socket, _client_response, sizeof(_client_response));
        }

        // 
        {

            // initialized data
            // aes256 *p_aes256 = NULL;

            // construct a block cipher
            // aes256_construct(&p_aes256, (unsigned char *)p_secure_socket->client.client_pair.p_private_key);
            
            // send it 
            // socket_tcp_send(
            //     p_secure_socket->_socket,
            //     aes256_encrypt
            //     (
            //         p_aes256,
            //         "Hello, World!",
            //         14
            //     )
            // );

            
        }
    }
    
    // success
    return 1;

    // error handling
    {
        
        // argument errors
        {
            no_secure_socket:
                #ifndef NDEBUG
                    printf("[secure socket] Null pointer provided for parameter \"p_secure_socket\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }

        // Socket errors
        {
            failed_to_create_socket:
                #ifndef NDEBUG
                    printf("[secure socket] Call to function \"socket\" returned an erroneous value in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;

            failed_to_connect:
                #ifndef NDEBUG
                    printf("[secure socket] Call to function \"connect\" returned an erroneous value in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
    }
}

// int secure_socket_receive ( secure_socket _secure_socket, void *p_buffer, size_t buffer_len )
// {

//     // argument check
//     if ( p_buffer == (void *) 0 ) goto no_buffer;

//     // initialized data
//     size_t r = (size_t) recv(_secure_socket, p_buffer, buffer_len, 0);

//     // error check
//     if ( r < 1 ) goto failed_to_recv;

//     // success
//     return 1;

//     // error handling
//     {

//         // argument errors
//         {
//             no_buffer:
//                 #ifndef NDEBUG
//                     printf("[secure socket] Null pointer provided for parameter \"p_buffer\" in call to function \"%s\"\n", __FUNCTION__);
//                 #endif 

//                 // error
//                 return 0;
//         }

//         // Socket errors
//         {
//             failed_to_recv:
//                 #ifndef NDEBUG
//                     printf("[secure socket] Call to \"recv\" returned an erroneous value in call to function \"%s\"\n", __FUNCTION__);
//                 #endif

//                 // error
//                 return 0;
//         }
//     }
// }

// int secure_socket_send ( secure_socket _secure_socket, const void *const p_buffer, size_t buffer_len )
// {

//     // argument check
//     if ( p_buffer   == (void *) 0 ) goto no_buffer;

//     // Send data to the TCP socket
//     if ( send(_secure_socket, p_buffer, buffer_len, 0) == -1 ) goto failed_to_send;

//     // success
//     return 1;

//     // error handling
//     {

//         // argument errors
//         {
//             no_buffer:
//                 #ifndef NDEBUG
//                     printf("[secure socket] Null pointer provided for parameter \"p_buffer\" in call to function \"%s\"\n", __FUNCTION__);
//                 #endif 

//                 // error
//                 return 0;
//         }

//         // Socket errors
//         {
//             failed_to_send:
//                 #ifndef NDEBUG
//                     printf("[secure socket] Call to \"send\" returned an erroneous value in call to function \"%s\"\n", __FUNCTION__);
//                 #endif

//                 // error
//                 return 0;
//         }
//     }
// }

// int socket_resolve_host ( socket_ip_address *p_ip_address, const char *restrict p_hostname )
// {

//     // initialized data
//     struct addrinfo  hints = { 0 },
//                     *addr_result = 0;
//     struct sockaddr_in *ipv4 = (void *) 0;
//     int result = 0;

//     // Initialize the hints structure
//     hints = (struct addrinfo)
//     {
//         .ai_family   = AF_INET,
//         .ai_socktype = SOCK_STREAM
//     };

//     // Get address info
//     result = getaddrinfo(p_hostname, NULL, &hints, &addr_result);

//     // error check
//     if ( result ) goto failed_to_resolve_address;

//     // Store the address
//     ipv4 = (struct sockaddr_in *)addr_result->ai_addr;

//     *p_ip_address = ntohl(ipv4->sin_addr.s_addr);

//     // success
//     return 1;

//     // error handling
//     {

//         // Socket errors
//         {
//             failed_to_resolve_address:
//                 #ifndef NDEBUG
//                     printf("[secure socket] Failed to resolve hostname \"%s\" in call to function \"%s\". Network database says: %s", p_hostname, __FUNCTION__, gai_strerror(result));
//                 #endif

//                 // error
//                 return 0;
//         }
//     }
// }

// int secure_socket_destroy ( secure_socket *p_secure_socket )
// {

//     // argument check
//     if ( p_secure_socket == (void *) 0 ) goto no_socket;

//     // initialized data
//     secure_socket _secure_socket = *p_secure_socket;

//     // No more pointer for caller
//     *p_secure_socket = 0;

//     // Platform dependent implementation
//     #ifdef _WIN64

//         // Shutdown the socket
//         if ( shutdown(_secure_socket, SD_SEND) == -1 ) goto failed_to_shutdown_socket;
        
//         // Close the socket
//         if ( closesocket(_secure_socket) == -1 ) goto failed_to_close_socket;

//     #else

//         secure_socket_send(_secure_socket, "", 0);

//         // Shutdown the socket
//         if ( shutdown(_secure_socket, SHUT_RDWR) == -1 ) goto failed_to_shutdown_socket;
        
//         // Close the socket
//         if ( close(_secure_socket) == -1 ) goto failed_to_close_socket;

//     #endif

//     // success
//     return 1;

//     // error handling
//     {

//         // argument errors
//         {
//             no_socket:
//                 #ifndef NDEBUG
//                     printf("[secure socket] Null pointer provided for parameter \"p_secure_socket\" in call to function \"%s\"\n", __FUNCTION__);
//                 #endif

//                 // error
//                 return 0;
//         }

//         // Socket errors
//         {
//             failed_to_shutdown_socket:
//                 #ifndef NDEBUG
//                     printf("[secure socket] Call to function \"shutdown\" returned an erroneous value in call to function \"%s\"\n", __FUNCTION__);
//                 #endif

//                 // error
//                 return 0;
            
//             failed_to_close_socket:
//                 #ifndef NDEBUG
//                     printf("[secure socket] Call to function \"close\" returned an erroneous value in call to function \"%s\"\n", __FUNCTION__);
//                 #endif

//                 // error
//                 return 0;
                
//         }
//     }
// }
