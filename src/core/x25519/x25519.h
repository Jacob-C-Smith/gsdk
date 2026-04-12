/** ! 
 * X25519 interface
 * 
 * @file src/core/x25519/x25519.h
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
#include <core/interfaces.h>
#include <core/pack.h>

// type definitions
typedef unsigned char x25519_public_key    [32];
typedef unsigned char x25519_private_key   [32];
typedef unsigned char x25519_shared_secret [32];

// function declarations
/// constructors
/** !
 * Construct an X25519 key pair
 * 
 * @param p_public_key  result
 * @param p_private_key result
 * 
 * @return 1 on success, 0 on error
 */
int x25519_key_pair_construct 
(
    x25519_public_key  *p_public_key, 
    x25519_private_key *p_private_key
);

/// key derivation
/** !
 * Derive the public key from the private key
 * 
 * @param p_private_key the private key
 * @param p_public_key  result
 * 
 * @return 1 on success, 0 on error
 */
int x25519_public_key_derive 
(
    x25519_private_key *p_private_key, 
    x25519_public_key  *p_public_key
);

/// shared secret derivation
/** !
 * Compute a shared secret
 * 
 * @param p_private_key   the private key
 * @param p_public_key    the peer's public key
 * @param p_shared_secret result
 * 
 * @return 1 on success, 0 on error
 */
int x25519_shared_secret_derive
(
    x25519_private_key   *p_private_key, 
    x25519_public_key    *p_public_key, 
    x25519_shared_secret *p_shared_secret
);

/// print
/** !
 * Print a public key
 * 
 * @param p_public_key the public key
 * 
 * @return 1 on success, 0 on error
 */
int x25519_public_key_print ( x25519_public_key *p_public_key );

/** !
 * Print a private key
 * 
 * @param p_private_key the private key
 * 
 * @return 1 on success, 0 on error
 */
int x25519_private_key_print ( x25519_private_key *p_private_key );

/** !
 * Print a shared secret
 * 
 * @param p_shared_secret the shared secret
 * 
 * @return 1 on success, 0 on error
 */
int x25519_shared_secret_print (  x25519_shared_secret *p_shared_secret );

/// reflection
/** !
 * Pack a public key into a buffer
 * 
 * @param p_buffer     the buffer
 * @param p_public_key the public key 
 * 
 * @return 1 on success, 0 on error
 */
int x25519_public_key_pack ( void *p_buffer, x25519_public_key *p_public_key );

/** !
 * Unpack a buffer into a public key
 * 
 * @param p_public_key result
 * @param p_buffer     the buffer
 * 
 * @return 1 on success, 0 on error
 */
int x25519_public_key_unpack ( x25519_public_key *p_public_key, void *p_buffer );

/** !
 * Pack a private key into a buffer
 * 
 * @param p_buffer      the buffer
 * @param p_private_key the private key 
 * 
 * @return 1 on success, 0 on error
 */
int x25519_private_key_pack ( void *p_buffer, x25519_private_key *p_private_key );

/** !
 * Unpack a buffer into a private key
 * 
 * @param p_private_key result
 * @param p_buffer      the buffer
 * 
 * @return 1 on success, 0 on error
 */
int x25519_private_key_unpack ( x25519_private_key *p_private_key, void *p_buffer );

/** !
 * Pack an x25519 key pair into a buffer
 * 
 * @param p_buffer      the buffer
 * @param p_public_key  the public key 
 * @param p_private_key the private key 
 * 
 * @return 1 on success, 0 on error
 */
int x25519_key_pair_pack ( void *p_buffer, x25519_public_key *p_public_key, x25519_private_key *p_private_key );

/** !
 * Unpack a buffer into an x25519 key pair
 * 
 * @param p_public_key  result
 * @param p_private_key result
 * @param p_buffer      the buffer
 * 
 * @return 1 on success, 0 on error
 */
int x25519_key_pair_unpack ( x25519_public_key *p_public_key, x25519_private_key *p_private_key, void *p_buffer );

/** !
 * Pack a shared secret key into a buffer
 * 
 * @param p_buffer        the buffer
 * @param p_shared_secret the shared secret 
 * 
 * @return 1 on success, 0 on error
 */
int x25519_shared_secret_pack ( void *p_buffer, x25519_shared_secret *p_shared_secret );

/** !
 * Unpack a buffer into a shared secret
 * 
 * @param p_shared_secret result
 * @param p_buffer        the buffer
 * 
 * @return 1 on success, 0 on error
 */
int x25519_shared_secret_unpack ( x25519_shared_secret *p_shared_secret, void *p_buffer );
