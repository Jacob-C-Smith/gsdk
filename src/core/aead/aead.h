/** ! 
 * AEAD interface
 * 
 * @file src/core/aead/aead.h
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
#include <core/chacha20.h>
#include <core/poly1305.h>

// structure declarations
struct aead_s;

// type definitions
typedef struct aead_s aead;

// function declarations
/// constructors
/** !
 * Construct an AEAD
 * 
 * @param pp_aead result
 * @param key     the key
 * @param nonce   the nonce
 * 
 * @return 1 on success, 0 on error
 */
int aead_construct
(
    aead           **pp_aead,
    chacha20_key     key,
    chacha20_nonce   nonce
);

/// encrypt
/** !
 * Encrypt a message
 * 
 * @param p_ciphertext the ciphertext
 * @param p_aead       the aead
 * @param tag          result
 * @param p_aad        the aad
 * @param aad_length   the quantity of bytes of aad
 * @param p_plaintext  the plaintext
 * @param len          the quantity of bytes of plaintext        
 * 
 * @return 1 on success, 0 on error
 */
int aead_encrypt
(
    void *p_ciphertext,
    aead *p_aead,
    poly1305_tag tag,

    const void *p_aad,       size_t aad_len,
    const void *p_plaintext, size_t len
);

/// seek
int aead_sequence_set ( aead *p_aead, size_t sequence );

/// decrypt
/** !
 * Encrypt a message
 * 
 * @param p_plaintext  the plaintext
 * @param p_aead       the aead
 * @param tag          the poly1305 tag
 * @param p_aad        the aad
 * @param aad_length   the quantity of bytes of aad
 * @param p_ciphertext the ciphertext
 * @param len          the quantity of bytes of ciphertext        
 * 
 * @return 1 on success, 0 on error
 */
int aead_decrypt
( 
    void         *p_plaintext, 
    aead         *p_aead, 
    poly1305_tag  tag, 
    
    void *p_aad       , size_t aad_len, 
    void *p_ciphertext, size_t len
);

/// destructors
/** !
 * Release an AEAD
 * 
 * @param pp_aead pointer to aead pointer
 * 
 * @return 1 on success, 0 on error
 */
int aead_destroy ( aead **pp_aead );
