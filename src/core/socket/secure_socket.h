/** !
 * Include header for securesocket library
 * 
 * @file core/secure_socket.h
 * 
 * @author Jacob Smith 
 */

// header guard
#pragma once

// standard library
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>

// core
#include <core/log.h>
#include <core/rsa.h>
#include <core/aes.h>
#include <core/pack.h>
#include <core/socket.h>

// structure declarations
struct secure_socket_s;

// type definitions
typedef struct secure_socket_s secure_socket;
typedef enum socket_address_family_e socket_address_family;

typedef unsigned long socket_ip_address;
typedef unsigned short socket_port;

enum socket_address_family_e
{
    socket_address_family_ipv4 = AF_INET,
    socket_address_family_ipv6 = AF_INET6,
};
enum socket_protocol_e
{
    socket_type_tcp = SOCK_STREAM,
    socket_type_udp = SOCK_DGRAM
};

// type definitions
typedef int(*fn_secure_socket_accept)( secure_socket *p_secure_socket, socket_ip_address ip_address, socket_port port_number, void *const p_parameter );

/** !
 * Create a secure secure socket
 * 
 * @param p_secure_socket result
 * @param address_family socket_address_family_ipv4 -or- socket_address_family_ipv6
 * @param port_number the port number
 * 
 * @sa secure_socket_bind
 * 
 * @return 1 on success, 0 on error
 */
int secure_socket_create
(
    secure_socket                **pp_secure_socket,
    enum socket_address_family_e   address_family,
    socket_port                    port_number
);

/** !
 * Block and listen for a connection on a secure socket, then call pfn_tcp_accept_callback.
 * 
 * @param _secure_socket the secure socket
 * @param pfn_tcp_accept_callback a callback function parameterized with the new socket, the IP address, and the port number
 * 
 * @sa tcp_socket_bind
 * 
 * @return 1 on success, 0 on error
 */
int secure_socket_listen
( 
    secure_socket *p_secure_socket, 
    fn_secure_socket_accept pfn_tcp_accept_callback, 
    void *const p_tcp_accept_callback_parameter
);

/** !
 * Receive data from a secure socket, and store in a buffer
 * 
 * @param _secure_socket the secure socket
 * @param p_buffer the buffer
 * @param buffer_len the maximum number of bytes to store in p_buffer
 * 
 * @sa secure_socket_send
 * 
 * @return 1 on success, 0 on error
 */
int secure_socket_receive
(
    secure_socket *p_secure_socket,
    void          *p_buffer,
    size_t         buffer_len
);

/** !
 * Send data to a secure socket
 * 
 * @param p_secure_socket the secure socket
 * @param p_buffer        the data to send
 * @param buffer_len      the size of the data in bytes
 * 
 * @sa secure_socket_receive
 * 
 * @return 1 on success, 0 on error
 */
int secure_socket_send 
(
    secure_socket *p_secure_socket,
    const void    *p_buffer,
    size_t         buffer_len
);

/** !
 * Connect to a secure socket
 * 
 * @param _secure_socket the secure socket
 * @param ip_address the IP address of the server
 * @param port_number the port number
 * 
 * @sa secure_socket_receive
 * 
 * @return 1 on success, 0 on error
 */
int secure_socket_connect 
(
    secure_socket *const  p_secure_socket,
    socket_address_family address_family,
    socket_ip_address     ip_address,
    socket_port           port_number
);

/** !
 * Destroy a secure socket
 * 
 * @param p_secure_socket pointer to the secure socket
 * 
 * @sa secure_socket_create
 * 
 * @return 1 on success, 0 on error
 */
int secure_socket_destroy ( secure_socket *p_secure_socket );
