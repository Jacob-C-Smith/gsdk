/** ! 
 * Ed25519 digital signature scheme
 * 
 * @file src/core/ed25519/ed25519.h
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
#include <core/sha.h>

// type definitions
typedef unsigned char ed25519_public_key  [32];
typedef unsigned char ed25519_private_key [32];
typedef unsigned char ed25519_signature   [64];

// function declarations
/// constructors
/** !
 * Construct an Ed25519 key pair
 * 
 * @param pp_public_key  result
 * @param pp_private_key result
 * 
 * @return 1 on success, 0 on error
 */
int ed25519_key_pair_construct 
(
    ed25519_public_key  *p_public_key, 
    ed25519_private_key *p_private_key
);

/// sign
/** !
 * Sign a message
 * 
 * @param p_signature   result
 * @param p_message     the message to sign
 * @param message_len   length of the message
 * @param p_public_key  the public key
 * @param p_private_key the private key
 * 
 * @return 1 on success, 0 on error
 */
int ed25519_sign
(
    ed25519_signature         *p_signature, 
    const unsigned char       *p_message, 
    size_t                     message_len, 
    const ed25519_public_key  *p_public_key, 
    const ed25519_private_key *p_private_key
);

/// verify
/** !
 * Verify a signature
 * 
 * @param p_signature  the signature to verify
 * @param p_message    the message
 * @param message_len  length of the message
 * @param p_public_key the public key
 * 
 * @return 1 on success, 0 on error
 */
int ed25519_verify
(
    const ed25519_signature  *p_signature,
    const unsigned char      *p_message,
    size_t                    message_len,
    const ed25519_public_key *p_public_key
);

/// print
/** !
 * Print a public key
 * 
 * @param p_public_key the public key
 * 
 * @return 1 on success, 0 on error
 */
int ed25519_public_key_print ( ed25519_public_key *p_public_key );

/** !
 * Print a private key
 * 
 * @param p_private_key the private key
 * 
 * @return 1 on success, 0 on error
 */
int ed25519_private_key_print ( ed25519_private_key *p_private_key );

/** !
 * Print a signature
 * 
 * @param p_signature the signature
 * 
 * @return 1 on success, 0 on error
 */
int ed25519_signature_print ( ed25519_signature *p_signature );

/// reflection
/** !
 * Pack a public key into a buffer
 * 
 * @param p_buffer     the buffer
 * @param p_public_key the public key 
 * 
 * @return 1 on success, 0 on error
 */
int ed25519_public_key_pack ( void *p_buffer, ed25519_public_key *p_public_key );

/** !
 * Unpack a buffer into a public key
 * 
 * @param p_public_key result
 * @param p_buffer     the buffer
 * 
 * @return 1 on success, 0 on error
 */
int ed25519_public_key_unpack ( ed25519_public_key *p_public_key, void *p_buffer );

/** !
 * Pack a private key into a buffer
 * 
 * @param p_buffer      the buffer
 * @param p_private_key the private key 
 * 
 * @return 1 on success, 0 on error
 */
int ed25519_private_key_pack ( void *p_buffer, ed25519_private_key *p_private_key );

/** !
 * Unpack a buffer into a private key
 * 
 * @param p_private_key result
 * @param p_buffer      the buffer
 * 
 * @return 1 on success, 0 on error
 */
int ed25519_private_key_unpack ( ed25519_private_key *p_private_key, void *p_buffer );

/** !
 * Pack an ed25519 key pair into a buffer
 * 
 * @param p_buffer      the buffer
 * @param p_public_key  the public key 
 * @param p_private_key the private key 
 * 
 * @return 1 on success, 0 on error
 */
int ed25519_key_pair_pack ( void *p_buffer, ed25519_public_key *p_public_key, ed25519_private_key *p_private_key );

/** !
 * Unpack a buffer into an ed25519 key pair
 * 
 * @param p_public_key  result
 * @param p_private_key result
 * @param p_buffer      the buffer
 * 
 * @return 1 on success, 0 on error
 */
int ed25519_key_pair_unpack ( ed25519_public_key *p_public_key, ed25519_private_key *p_private_key, void *p_buffer );

/** !
 * Pack a signature into a buffer
 * 
 * @param p_buffer    the buffer
 * @param p_signature the signature 
 * 
 * @return 1 on success, 0 on error
 */
int ed25519_signature_pack ( void *p_buffer, ed25519_signature *p_signature );

/** !
 * Unpack a buffer into a signature
 * 
 * @param p_signature result
 * @param p_buffer    the buffer
 * 
 * @return 1 on success, 0 on error
 */
int ed25519_signature_unpack ( ed25519_signature *p_signature, void *p_buffer );
