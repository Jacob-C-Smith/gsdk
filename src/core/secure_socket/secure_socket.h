/** !
 * Secure socket interface
 *
 * @file src/core/secure_socket/secure_socket.h
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

// core
#include <core/log.h>
#include <core/hash.h>
#include <core/interfaces.h>
#include <core/ed25519.h>
#include <core/x25519.h>
#include <core/sha.h>
#include <core/aead.h>
#include <core/pack.h>
#include <core/socket.h>
#include <core/tcp.h>
#include <core/certificate.h>

// preprocessor definitions
// #define SECURE_SOCKET_DEBUG

// structure declarations
struct secure_socket_s;

// type definitions
typedef struct secure_socket_s secure_socket;
typedef int(*fn_secure_socket_accept)( secure_socket *p_secure_socket, socket_ip_address ip_address, socket_port port_number, void *const p_parameter );

// function declarations
/// construct
/** !
 * Construct a secure socket from an existing TCP socket.
 * Performs a TLS-like handshake using X25519 for key exchange and AEAD for encryption.
 *
 * @param pp_secure_socket  result
 * @param tcp_socket        the established TCP socket
 * @param is_server         true if server, false if client
 * @param p_certificate     the local certificate
 * @param p_private_key     the local private key for handshake signing
 *
 * @return 1 on success, 0 on error
 */
int secure_socket_construct
(
    secure_socket       **pp_secure_socket,
    socket_tcp            tcp_socket,
    bool                  is_server,
    certificate          *p_certificate,
    ed25519_private_key  *p_private_key
);

/// connect
/** !
 * Establish a secure connection to a server.
 *
 * @param pp_secure_socket result
 * @param ip_address       server IP
 * @param port             server port
 * @param p_certificate    the local certificate
 * @param p_private_key    the local private key for handshake signing IF NOT NULL ELSE unused
 *
 * @return 1 on success, 0 on error
 */
int secure_socket_connect
(
    secure_socket       **pp_secure_socket,
    socket_ip_address     ip_address,
    socket_port           port,
    certificate          *p_certificate,
    ed25519_private_key  *p_private_key
);

/// listen
/** !
 * Listen on a secure socket
 *
 * @param tcp_server_socket raw tcp socket
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
    fn_secure_socket_accept  pfn_callback,
    certificate             *p_certificate,
    ed25519_private_key     *p_private_key,
    void                    *p_parameter
);

/// send 
/** !
 * Send encrypted data over the secure socket.
 *
 * @param p_secure_socket the secure socket
 * @param p_data          the plaintext data
 * @param len             the quantity of bytes
 *
 * @return bytes sent on success, 0 on error
 */
int secure_socket_send ( secure_socket *p_secure_socket, const void *p_data, size_t len );

/** !
 * Receive and decrypt data from the secure socket.
 *
 * @param p_secure_socket the secure socket
 * @param p_buffer        the buffer to store decrypted data
 * @param buffer_len      the quantity of bytes
 *
 * @return bytes received on success, 0 on error
 */
int secure_socket_receive ( secure_socket *p_secure_socket, void *p_buffer, size_t buffer_len );

/** !
 * Release the secure socket and underlying resources.
 *
 * @param pp_secure_socket pointer to the secure socket pointer
 *
 * @return 1 on success, 0 on error
 */
int secure_socket_destroy ( secure_socket **pp_secure_socket );
