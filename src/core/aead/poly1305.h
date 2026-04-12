/** ! 
 * Poly1305 interface
 * 
 * @file src/core/aead/poly1305.h
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
#include <core/chacha20.h>
#include <core/log.h>
#include <core/interfaces.h>
#include <core/pack.h>

// preprocessor definitions
// #define POLY1305_DEBUG

// type definitions
typedef unsigned char poly1305_one_time_key[32];
typedef unsigned char poly1305_tag         [16];

// function declarations
/// message
/** !
 * Compute a message authentication code  
 * 
 * @param p_message the message
 * @param len       the quantity of bytes to authenticate
 * @param tag       result
 * @param key       the key
 * 
 * @return 1 on success, 0 on error
 */
int poly1305_mac 
(
    const unsigned char   *p_message, 
    size_t                 len, 
    poly1305_tag           tag, 
    poly1305_one_time_key  key
);

/// key generation
/** !
 * Generate a poly1305 key  
 * 
 * @param p_key result
 * @param key   the key
 * @param nonce the nonce
 * @param block the block counter
 * 
 * @return 1 on success, 0 on error
 */
int poly1305_key_generate
(
    poly1305_one_time_key *p_key, 
    chacha20_key           key, 
    chacha20_nonce         nonce,
    int                    block
);
