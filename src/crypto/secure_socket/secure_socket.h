/** !
 * Secure socket interface
 *
 * @file src/crypto/secure_socket/secure_socket.h
 *
 * @author Jacob Smith
 */

// header guard
#pragma once

// standard library
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

// gsdk
/// core
#include <core/log.h>
#include <core/hash.h>
#include <core/interfaces.h>
#include <core/pack.h>
#include <core/socket.h>
#include <core/tcp.h>
#include <core/sync.h>

/// crypto
#include <crypto/ed25519.h>
#include <crypto/x25519.h>
#include <crypto/sha.h>
#include <crypto/aead.h>
#include <crypto/certificate.h>

// preprocessor definitions
#define SECURE_SOCKET_DEBUG

// type definitions
typedef int(fn_secure_socket_accept)( stream *p_stream, socket_ip_address ip_address, socket_port port_number, void *const p_parameter );

// function declarations
/// construct
/** !
 * Construct a secure stream from an existing TCP socket.
 * 
 * @param pp_stream         result
 * @param tcp_socket        the TCP socket
 * @param is_server         TRUE IF server ELSE client
 * @param p_certificate     the local certificate
 * @param p_private_key     the local private key for handshake signing
 *
 * @return 1 on success, 0 on error
 */
int secure_stream_construct
(
    stream              **pp_stream,
    socket_tcp            tcp_socket,
    bool                  is_server,
    certificate          *p_certificate,
    ed25519_private_key  *p_private_key
);

/// connect
/** !
 * Establish a secure connection to a server.
 *
 * @param pp_stream        result
 * @param ip_address       server IP
 * @param port             server port
 * @param p_certificate    the local certificate
 * @param p_private_key    the local private key for handshake signing IF NOT NULL ELSE unused
 *
 * @return 1 on success, 0 on error
 */
int secure_socket_connect
(
    stream              **pp_stream,
    socket_ip_address     ip_address,
    socket_port           port,
    certificate          *p_certificate,
    ed25519_private_key  *p_private_key
);

/// listen
/** !
 * Listen on a secure socket
 *
 * @param tcp_server_socket raw TCP socket
 * @param pfn_callback      pointer to callback function parameter
 * @param p_certificate     the certificate
 * @param p_private_key     the private key
 * @param p_parameter       callback parameter
 * 
 * @return 1 on success, 0 on error
 */
int secure_socket_listen
(
    socket_tcp               tcp_server_socket,
    fn_secure_socket_accept *pfn_callback,
    certificate             *p_certificate,
    ed25519_private_key     *p_private_key,
    void                    *p_parameter
);
