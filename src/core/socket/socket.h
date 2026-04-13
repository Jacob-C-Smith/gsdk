/** !
 * Include header for socket library
 * 
 * @file src/core/socket/socket.h 
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

// enumeration definitions
enum socket_address_family_e 
{
    socket_address_family_ipv4 = 2, 
    socket_address_family_ipv6 = 10,
};
enum socket_protocol_e 
{
    socket_type_tcp = 1,
    socket_type_udp = 2,
};

// structure definitions
struct socket_ip_address_s
{
    enum socket_address_family_e _type;
    union
    {
        unsigned long ipv4;
        unsigned char ipv6[16];
    } _address;
};

// type definitions
typedef struct socket_ip_address_s socket_ip_address;
typedef unsigned short socket_port;

// function declarations
/// resolve
/** !
 * Get IP addresses from a hostname
 * 
 * @param p_ip_addresses result
 * @param limit          the maximum quantity of IP addresses to return
 * @param p_hostname     the name of the host
 * 
 * @return the quantity of IP addresses found on success, 0 on error
 */
int socket_resolve_host ( socket_ip_address *p_ip_addresses, size_t limit, const char *restrict p_hostname );

/// print
/** !
 * Print an IP address
 * 
 * @param ip_address the IP address
 * 
 * @return 1 on success, 0 on error
 */
int socket_ip_address_print ( socket_ip_address ip_address );