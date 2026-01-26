/** ! 
 * Ed25519 digital signature scheme
 * 
 * @file core/ed25519.h
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
#include <core/sha.h>

// type definitions
typedef struct ed25519_public_key_s  ed25519_public_key;
typedef struct ed25519_private_key_s ed25519_private_key;
typedef struct ed25519_signature_s   ed25519_signature;

// structure definitions
struct ed25519_public_key_s
{
    unsigned char data[32];
};

struct ed25519_private_key_s
{
    unsigned char data[32];
};

struct ed25519_signature_s
{
    unsigned char data[64];
};

// function declarations
/** !
 * Construct an Ed25519 key pair
 * 
 * @param pp_public_key  result
 * @param pp_private_key result
 * 
 * @return 1 on success, 0 on error
 */
int ed25519_key_pair_construct ( ed25519_public_key **pp_public_key, ed25519_private_key **pp_private_key );

/** !
 * Construct an Ed25519 key pair from a seed (private key)
 * 
 * @param pp_public_key  result
 * @param pp_private_key result
 * @param seed           32-byte seed
 * 
 * @return 1 on success, 0 on error
 */
int ed25519_key_pair_from_seed ( ed25519_public_key **pp_public_key, ed25519_private_key **pp_private_key, const unsigned char *seed );

/** !
 * Sign a message
 * 
 * @param p_signature    result
 * @param p_message      the message to sign
 * @param message_len    length of the message
 * @param p_public_key   the public key
 * @param p_private_key  the private key
 * 
 * @return 1 on success, 0 on error
 */
int ed25519_sign ( ed25519_signature *p_signature, const unsigned char *p_message, size_t message_len, const ed25519_public_key *p_public_key, const ed25519_private_key *p_private_key );

/** !
 * Verify a signature
 * 
 * @param p_signature    the signature to verify
 * @param p_message      the message
 * @param message_len    length of the message
 * @param p_public_key   the public key
 * 
 * @return 1 if valid, 0 if invalid or error
 */
int ed25519_verify ( const ed25519_signature *p_signature, const unsigned char *p_message, size_t message_len, const ed25519_public_key *p_public_key );
