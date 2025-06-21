/** ! 
 * AES (en/de)cryption 
 * 
 * @file aes.h
 * 
 * @author Jacob Smith
 */

// header guard
#pragma once

// standard library
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

// core
#include <core/log.h>

// structure declarations
struct aes256_s;

// type definitions
typedef struct aes256_s aes256;
typedef unsigned char aes256_key[32];

// structure definitions
struct aes256_s
{
    aes256_key _key;
};

// external declarations
extern int aes256_key_expansion ( const unsigned char *key     , unsigned char       *enc_keys );
extern int aes256_invert_keys   ( unsigned char       *dec_keys, const unsigned char *enc_keys );
extern int aes256_encrypt_block ( unsigned char       *block   , const unsigned char *enc_keys );
extern int aes256_decrypt_block ( unsigned char       *block   , const unsigned char *dec_keys );

// function declarations
int aes256_construct ( aes256 **pp_aes256, aes256_key _key );
int aes256_encrypt ( void *p_value, size_t len, const unsigned char *enc_keys );
int aes256_decrypt ( void *p_value, size_t len, const unsigned char *dec_keys );
