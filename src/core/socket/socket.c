/** !
 * Cross platform socket primitives 
 * 
 * @file src/core/socket/socket.c 
 * 
 * @author Jacob Smith
 */

#define _POSIX_C_SOURCE 200112L

// System headers
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>

// Project header
#include <core/socket.h>

#include <stdio.h>
#include <string.h>

int socket_resolve_host ( socket_ip_address *p_ip_addresses, size_t limit, const char *restrict p_hostname )
{

    // initialized data
    struct addrinfo *res   = NULL,
                    *rp    = NULL,
                     hints = (struct addrinfo)
                     {
                        .ai_family   = AF_UNSPEC,
                        .ai_socktype = SOCK_STREAM,
                        .ai_flags    = AI_ALL | AI_V4MAPPED
                     };
    int count = 0;

    if ( 0 != getaddrinfo(p_hostname, NULL, &hints, &res) ) return 0;

    // walk the results
    for (rp = res; rp && count < (int)limit; rp = rp->ai_next)
    {

        // ipv4
        if ( rp->ai_family == AF_INET )
        {

            // initialized data
            struct sockaddr_in *a = (struct sockaddr_in *)rp->ai_addr;

            // store the result
            p_ip_addresses[count] = (socket_ip_address)
            {
                ._type = socket_address_family_ipv4,
                ._address.ipv4 = ntohl(a->sin_addr.s_addr)
            };
        }

        // ipv6
        else if (rp->ai_family == AF_INET6)
        {

            // initialized data
            struct sockaddr_in6 *a = (struct sockaddr_in6 *)rp->ai_addr;

            // ignore ipv6-mapped ipv4 addresses
            if (IN6_IS_ADDR_V4MAPPED(&a->sin6_addr)) continue;

            // store the type
            p_ip_addresses[count]._type = socket_address_family_ipv6;

            // copy the ipv6 address
            memcpy(
                p_ip_addresses[count]._address.ipv6,
                a->sin6_addr.s6_addr,
                16
            );
        }

        // step
        count++;
    }

    // release the list
    freeaddrinfo(res);

    // success
    return count;
}

int socket_ip_address_print ( socket_ip_address ip_address )
{

    // strategy
    switch (ip_address._type)
    {
        case socket_address_family_ipv4:

            // print the ipv4 address
            printf(
                "%hhu.%hhu.%hhu.%hhu\n",
                (unsigned char) ((ip_address._address.ipv4 & 0xff000000) >> 24),
                (unsigned char) ((ip_address._address.ipv4 & 0x00ff0000) >> 16),
                (unsigned char) ((ip_address._address.ipv4 & 0x0000ff00) >> 8 ),
                (unsigned char) ((ip_address._address.ipv4 & 0x000000ff) >> 0 )
            );

            // done
            break;
        case socket_address_family_ipv6:

            // print the ipv6 address
            printf(
                "%02hhx%02hhx:%02hhx%02hhx:%02hhx%02hhx:%02hhx%02hhx:%02hhx%02hhx:%02hhx%02hhx:%02hhx%02hhx:%02hhx%02hhx\n",
                ip_address._address.ipv6[0] , ip_address._address.ipv6[1], 
                ip_address._address.ipv6[2] , ip_address._address.ipv6[3],
                ip_address._address.ipv6[4] , ip_address._address.ipv6[5], 
                ip_address._address.ipv6[6] , ip_address._address.ipv6[7],
                ip_address._address.ipv6[8] , ip_address._address.ipv6[9], 
                ip_address._address.ipv6[10], ip_address._address.ipv6[11],
                ip_address._address.ipv6[12], ip_address._address.ipv6[13], 
                ip_address._address.ipv6[14], ip_address._address.ipv6[15]
            );
            
            // done
            break;
    }

    // success
    return 1;
}
