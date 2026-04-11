/** ! 
 * ChaCha20 interface
 * 
 * @file src/core/chacha20/chacha20.h
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

// preprocessor definitions
// #define CHACHA20_DEBUG

// structure declarations
struct chacha20_s;

// type definitions
typedef unsigned int  chacha20_key    [8];
typedef unsigned int  chacha20_nonce  [3];
typedef unsigned int  chacha20_block  [16];
typedef unsigned int  chacha20_state  [16];

typedef unsigned char poly1305_one_time_key[32];
typedef unsigned char poly1305_tag         [16];

typedef struct chacha20_s chacha20;

// function declarations
/// constructors
/** !
 * Construct a chacha20 cipher
 * 
 * @param p_chacha20 result
 * @param key        the key
 * @param nonce      the nonce
 * @param block      the block counter
 * 
 * @return 1 on success, 0 on error
 */
int chacha20_construct
(
    chacha20       **pp_chacha20, 
    chacha20_key     key, 
    chacha20_nonce   nonce,
    int              block
);

/// encrypt
/** !
 * Encrypt data with a chacha20 cipher
 * 
 * @param p_ciphertext result
 * @param p_chacha20   the chacha cipher
 * @param p_plaintext  pointer to plaintext
 * @param len          the quantity of bytes to encrypt
 * 
 * @return the quantity of bytes encrypted on success, 0 on error
 */
int chacha20_encrypt ( void *p_ciphertext, chacha20 *p_chacha20, void *p_plaintext, size_t len );

/// decrypt
/** !
 * Decrypt data with a chacha20 cipher
 * 
 * @param p_plaintext  result
 * @param p_chacha20   the chacha cipher
 * @param p_ciphertext pointer to ciphertext
 * @param len          the quantity of bytes to decrypt
 * 
 * @return the quantity of bytes decrypted on success, 0 on error
 */
int chacha20_decrypt ( void *p_plaintext, chacha20 *p_chacha20, void *p_ciphertext, size_t len );

/// seek
/** !
 * Set the chacha20 block counter
 * 
 * @param p_chacha20 the chacha cipher
 * @param block      the block counter
 * 
 * @return 1 on success, 0 on error
 */
int chacha20_seek ( chacha20 *p_chacha20, unsigned int block );

/// tell
/** !
 * Get the chacha20 block counter
 * 
 * @param p_chacha20 the chacha cipher
 * @param p_block    result
 * 
 * @return 1 on success, 0 on error
 */
int chacha20_tell ( chacha20 *p_chacha20, unsigned int *p_block );

/// destroy
/** !
 * Destory a chacha20 cipher
 * 
 * @param pp_chacha20 pointer to chacha cipher pointer
 * 
 * @return 1 on success, 0 on error
 */
int chacha20_destroy ( chacha20 **pp_chacha20 );
