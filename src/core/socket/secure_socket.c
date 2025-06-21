// /** !
//  * Secure socket
//  * 
//  * @file src/core/socket/secure_socket.c 
//  * 
//  * @author Jacob Smith
//  */

// // header 
// #include <core/secure_socket.h>

// int secure_socket_create ( secure_socket **pp_secure_socket, enum socket_address_family_e address_family, unsigned short port_number )
// {

//     // argument check
//     if ( pp_secure_socket == (void *) 0 ) goto no_secure_socket; 

//     secure_socket _secure_socket = -1;
//     int option = 1;
//     struct sockaddr_in socket_address =
//     {
//         .sin_family = (unsigned short) address_family,
//         .sin_addr.s_addr = htonl(INADDR_ANY),
//         .sin_port = htons(port_number),
//         .sin_zero = { 0 }
//     };

//     // Create the socket
//     _secure_socket = socket(address_family, socket_type_tcp, IPPROTO_TCP);

//     // error check
//     if ( _secure_socket == -1 ) goto failed_to_create_socket;
    
//     // set socket options
//     if ( setsockopt(_secure_socket, SOL_SOCKET, SO_REUSEADDR, &option, sizeof(option)) == -1 ) goto failed_to_set_socket_option;

//     // bind the socket to the port
//     if ( bind(_secure_socket,(struct sockaddr*) &socket_address, sizeof(socket_address)) == -1 ) goto failed_to_bind_socket;

//     // return a pointer to the caller
//     *pp_secure_socket = p_secure_socket;

//     // success
//     return 1;

//     // error handling
//     {
        
//         // argument errors
//         {
//             no_secure_socket:
//                 #ifndef NDEBUG
//                     printf("[secure socket] Null pointer provided for parameter \"pp_secure_socket\" in call to function \"%s\"\n", __FUNCTION__);
//                 #endif

//                 // error
//                 return 0;
//         }

//         // Socket errors
//         {
//             failed_to_create_socket:
//                 #ifndef NDEBUG
//                     printf("[secure socket] Call to function \"socket\" returned an erroneous value in call to function \"%s\"\n", __FUNCTION__);
//                 #endif

//                 // error
//                 return 0;

//             failed_to_set_socket_option:
//                 #ifndef NDEBUG
//                     printf("[secure socket] Call to function \"setsockopt\" returned an erroneous value in call to function \"%s\"\n", __FUNCTION__);
//                 #endif

//                 // error
//                 return 0;
            
//             failed_to_bind_socket:
//                 #ifndef NDEBUG
//                     printf("[secure socket] Call to function \"bind\" returned an erroneous value in call to function \"%s\"\n", __FUNCTION__);
//                 #endif

//                 // error
//                 return 0;

//         }
//     }
// }

// int secure_socket_listen ( secure_socket _secure_socket, fn_secure_socket_accept pfn_tcp_accept_callback, void *const p_tcp_accept_callback_parameter )
// {

//     // initialized data
//     secure_socket new_socket = 0;
//     struct sockaddr_in peer_addr = {0};
//     unsigned int addr_len = sizeof(peer_addr);

//     // Listen for connections
//     if ( listen(_secure_socket, 1) == -1 ) goto failed_to_listen;

//     // Accept a new connection
//     new_socket = accept(_secure_socket, (struct sockaddr *) &peer_addr, &addr_len);

//     // error check
//     if ( new_socket == -1 ) goto failed_to_connect;
    
//     // Callback
//     pfn_tcp_accept_callback(new_socket, ntohl(peer_addr.sin_addr.s_addr), (unsigned short) ntohs(peer_addr.sin_port), p_tcp_accept_callback_parameter);

//     // success
//     return 1;

//     // error handling
//     {

//         // Socket errors
//         {
//             failed_to_listen:
//                 #ifndef NDEBUG
//                     printf("[secure socket] Call to function \"listen\" returned an erroneous value in call to function \"%s\"\n", __FUNCTION__);
//                 #endif

//                 // error
//                 return 0;
            
//             failed_to_connect:
//                 #ifndef NDEBUG
//                     printf("[secure socket] Call to function \"connect\" returned an erroneous value in call to function \"%s\"\n", __FUNCTION__);
//                 #endif

//                 // error
//                 return 0;
//         }
//     }
// }

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

// int secure_socket_connect ( secure_socket *const p_secure_socket, enum socket_address_family_e address_family, socket_ip_address ip_address, socket_port port_number )
// {

//     // argument check
//     if ( p_secure_socket == (void *) 0 ) goto no_secure_socket; 

//     // Platform specific initialized data
//     #ifdef _WIN64
//         secure_socket _secure_socket = INVALID_SOCKET;
//     #else
//         secure_socket _secure_socket = -1;
//         struct sockaddr_in serv_addr =
//         {
//             .sin_family = (unsigned short) address_family,
//             .sin_addr.s_addr = (unsigned int) htonl((unsigned int)ip_address),
//             .sin_port = htons(port_number),
//             .sin_zero = { 0 }
//         };
//     #endif

//     // Platform specific implementation
//     #ifdef _WIN64


//     #elif __APPLE__

//         // Create the socket
//         _secure_socket = socket((int) address_family, socket_type_tcp, IPPROTO_TCP);

//         // error check
//         if ( _secure_socket == -1 ) goto failed_to_create_socket;
        
//         // Connect to the socket
//         if ( connect(_secure_socket, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) ) goto failed_to_connect; 
//     #else

//         // Create the socket
//         _secure_socket = socket((int) address_family, socket_type_tcp, IPPROTO_TCP);

//         // error check
//         if ( _secure_socket == -1 ) goto failed_to_create_socket;
        
//         // Connect to the socket
//         if ( connect(_secure_socket, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) ) goto failed_to_connect; 
//     #endif

//     // Return a socket to the caller
//     *p_secure_socket = _secure_socket;

//     // success
//     return 1;

//     // error handling
//     {
        
//         // argument errors
//         {
//             no_secure_socket:
//                 #ifndef NDEBUG
//                     printf("[secure socket] Null pointer provided for parameter \"p_secure_socket\" in call to function \"%s\"\n", __FUNCTION__);
//                 #endif

//                 // error
//                 return 0;
//         }

//         // Socket errors
//         {
//             failed_to_create_socket:
//                 #ifndef NDEBUG
//                     printf("[secure socket] Call to function \"socket\" returned an erroneous value in call to function \"%s\"\n", __FUNCTION__);
//                 #endif

//                 // error
//                 return 0;

//             failed_to_connect:
//                 #ifndef NDEBUG
//                     printf("[secure socket] Call to function \"connect\" returned an erroneous value in call to function \"%s\"\n", __FUNCTION__);
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
