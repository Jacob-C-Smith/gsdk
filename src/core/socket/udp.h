/** !
 * Socket UDP interface 
 * 
 * @file src/core/socket/udp.h 
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
typedef int socket_udp;
typedef int(*fn_socket_udp_accept)( socket_udp _socket_udp, socket_ip_address ip_address, socket_port port_number, void *const p_parameter );

/// construct
/** !
 * Create a UDP socket
 * 
 * @param p_socket_udp   return
 * @param address_family socket_address_family_ipv4 -or- socket_address_family_ipv6
 * @param port_number    the port number
 * 
 * @return 1 on success, 0 on error
 */
int socket_udp_create ( socket_udp *const p_socket_udp, enum socket_address_family_e address_family, socket_port port_number );

/// listen
/** !
 * Block and listen for a connection on a UDP socket, then call pfn_udp_accept_callback.
 * 
 * @param _socket_udp             the UDP socket
 * @param pfn_udp_accept_callback a callback function parameterized with the new socket, the IP address, and the port number
 * 
 * @return 1 on success, 0 on error
 */
int socket_udp_listen ( socket_udp _socket_udp, fn_socket_udp_accept pfn_udp_accept_callback, void *const p_parameter );

/// receive
/** !
 * Receive data from a UDP socket
 * 
 * @param _socket_udp  the UDP socket
 * @param p_buffer     the buffer
 * @param buffer_len   the maximum number of bytes to store in p_buffer
 * @param p_ip_address result
 * @param p_port       result
 * 
 * @return the quantity of bytes received on success, 0 on error
 */
int socket_udp_receive_from ( socket_udp _socket_udp, void *const p_buffer, size_t buffer_len, socket_ip_address *const p_ip_address, socket_port *const p_port );

/// send
/** !
 * Send data to a UDP socket
 * 
 * @param _socket_udp the UDP socket
 * @param p_buffer    the data to send
 * @param buffer_len  the size of the data in bytes
 * @param ip_address  the IP address to send to
 * @param port        the port number to send to
 * 
 * @return 1 on success, 0 on error
 */
int socket_udp_send_to ( socket_udp _socket_udp, const void *const p_buffer, size_t buffer_len, socket_ip_address ip_address, socket_port port );

/// connect
/** !
 * Connect to a UDP socket
 * 
 * @param _socket_udp the UDP socket
 * @param ip_address the IP address of the server
 * @param port_number the port number
 * 
 * @return 1 on success, 0 on error
 */
int socket_udp_connect ( socket_udp *const p_socket_udp, enum socket_address_family_e address_family, socket_ip_address ip_address, socket_port port_number );

/// destroy
/** !
 * Destroy a UDP socket
 * 
 * @param p_socket_udp pointer to the UDP socket
 * 
 * @return 1 on success, 0 on error
 */
int socket_udp_destroy ( socket_udp *p_socket_udp );
