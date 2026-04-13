/** !
 * Socket TCP interface 
 * 
 * @file src/core/socket/tcp.h 
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

// posix
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>

// gsdk
/// core
#include <core/log.h>
#include <core/socket.h>

// type definitions
typedef int socket_tcp;
typedef int(*fn_socket_tcp_accept)( socket_tcp _socket_tcp, socket_ip_address ip_address, socket_port port_number, void *const p_parameter );

/// construct
/** !
 * Create a TCP socket
 * 
 * @param p_socket_tcp   return
 * @param address_family socket_address_family_ipv4 -or- socket_address_family_ipv6
 * @param port_number    the port number
 * 
 * @return 1 on success, 0 on error
 */
int socket_tcp_create ( socket_tcp *const p_socket_tcp, enum socket_address_family_e address_family, socket_port port_number );

/// listen
/** !
 * Block and listen for a connection on a TCP socket, then call pfn_tcp_accept_callback.
 * 
 * @param _socket_tcp             the TCP socket
 * @param pfn_tcp_accept_callback a callback function parameterized with the new socket, the IP address, and the port number
 * 
 * @return 1 on success, 0 on error
 */
int socket_tcp_listen ( socket_tcp _socket_tcp, fn_socket_tcp_accept pfn_tcp_accept_callback, void *const p_parameter );

/// receive
/** !
 * Receive data from a TCP socket, and store in a buffer
 * 
 * @param _socket_tcp the TCP socket
 * @param p_buffer the buffer
 * @param buffer_len the maximum number of bytes to store in p_buffer
 * 
 * @sa socket_tcp_send
 * 
 * @return 1 on success, 0 on error
 */
int socket_tcp_receive ( socket_tcp _socket_tcp, void *const p_buffer, size_t buffer_len );

/// send
/** !
 * Send data to a TCP socket
 * 
 * @param _socket_tcp the TCP socket
 * @param p_buffer the data to send
 * @param buffer_len the size of the data in bytes
 * 
 * @sa socket_tcp_receive
 * 
 * @return 1 on success, 0 on error
 */
int socket_tcp_send ( socket_tcp _socket_tcp, const void *const p_buffer, size_t buffer_len );

/// connect
/** !
 * Connect to a TCP socket
 * 
 * @param _socket_tcp the TCP socket
 * @param ip_address the IP address of the server
 * @param port_number the port number
 * 
 * @sa socket_tcp_receive
 * 
 * @return 1 on success, 0 on error
 */
int socket_tcp_connect ( socket_tcp *const p_socket_tcp, enum socket_address_family_e address_family, socket_ip_address ip_address, socket_port port_number );

/// destroy
/** !
 * Destroy a TCP socket
 * 
 * @param p_socket_tcp pointer to the TCP socket
 * 
 * @sa socket_tcp_create
 * 
 * @return 1 on success, 0 on error
 */
int socket_tcp_destroy ( socket_tcp *p_socket_tcp );
