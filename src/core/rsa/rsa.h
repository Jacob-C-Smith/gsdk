/** ! 
 * RSA (en/de)cryption with C23 BitInt
 * 
 * @file src/core/rsa/rsa.h
 * 
 * @author Jacob Smith
 */

// header guard
#pragma once

// standard library
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <time.h>

// core
#include <core/log.h>
#include <core/hash.h>
#include <core/sha.h>
#include <core/interfaces.h>
#include <core/pack.h>

// structure declarations
struct public_key_s;
struct private_key_s;

// type definitions
typedef _BitInt(2048) i2048;

typedef struct public_key_s  public_key;
typedef struct private_key_s private_key;

// structure definitions
struct public_key_s
{
    i2048 n, a;
};

struct private_key_s
{
    i2048 p, q, b;
};

// function declarations
/** !
 * Construct a public private key pair from /dev/urandom
 * 
 * @param pp_public_key result
 * @param pp_public_key result
 * 
 * @return 1 on success, 0 on error
 */
int key_pair_construct ( public_key **pp_public_key, private_key **pp_private_key );

/** !
 * Construct a public private key pair from a file
 * 
 * @param pp_public_key      result
 * @param pp_public_key      result
 * @param p_public_key_path  path to public key file
 * @param p_private_key_path path to private key file 
 *
 * @return 1 on success, 0 on error
 */
int key_pair_from_files
(
    public_key  **pp_public_key,
    private_key **pp_private_key,
    const char   *p_public_key_path,
    const char   *p_private_key_path 
);

/** !
 * Print 256 hex nibbles 
 * 
 * @param a the data
 * 
 * @return 1 on success, 0 on error
 */
int print_n_bit_int ( i2048 a );

/** !
 * Print a public key
 * 
 * @param p_public_key the public key
 * 
 * @return 1 on success, 0 on error
 */
int print_public_key ( public_key *p_public_key );

/** !
 * Print a shortened representation of a public key
 * 
 * @param p_public_key the public key
 * 
 * @return 1 on success, 0 on error
 */
int print_public_key_short ( public_key *p_public_key );

/** !
 * Print a private key
 * 
 * @param p_private_key the private key
 * 
 * @return 1 on success, 0 on error
 */
int print_private_key ( private_key *p_private_key );

/** !
 * Pack a public key into a buffer
 * 
 * @param p_buffer     the buffer
 * @param p_public_key the public key 
 * 
 * @return 1 on success, 0 on error
 */
int public_key_pack ( void *p_buffer, public_key *p_public_key );

/** !
 * Unpack a buffer into a public key
 * 
 * @param p_public_key result
 * @param p_buffer     the buffer
 * 
 * @return 1 on success, 0 on error
 */
int public_key_unpack ( public_key *p_public_key, void *p_buffer );

/** !
 * Pack a private key into a buffer
 * 
 * @param p_buffer      the buffer
 * @param p_private_key the private key 
 * 
 * @return 1 on success, 0 on error
 */
int private_key_pack ( void *p_buffer, private_key *p_private_key );

/** !
 * Unpack a buffer into a private key
 * 
 * @param p_private_key result
 * @param p_buffer      the buffer
 * 
 * @return 1 on success, 0 on error
 */
int private_key_unpack ( private_key *p_private_key, void *p_buffer );

/** ! 
 * Encrypt a block
 * 
 * @param p_x          pointer to plaintext block
 * @param p_y          result
 * @param p_public_key the public key
 * 
 * @return 1 on success, 0 on error
 */
int rsa_encrypt ( void *p_x, void *p_y, public_key *p_public_key );

/** ! 
 * Decrypt a block
 * 
 * @param p_y           pointer to ciphertext block
 * @param p_z           result
 * @param p_public_key  the public key
 * @param p_private_key the private key
 * 
 * @return 1 on success, 0 on error
 */
int rsa_decrypt ( void *p_y, void *p_z, public_key *p_public_key, private_key *p_private_key );

/** !
 * RSAES-OAEP-ENCRYPT using SHA-256 and MGF1
 *
 * @param p_public_key the public key
 * @param p_msg        plaintext buffer
 * @param msg_len      plaintext length in bytes
 * @param p_label      optional label (can be NULL)
 * @param label_len    label length in bytes (0 if p_label is NULL)
 * @param p_out        output buffer for ciphertext (k bytes)
 * @param p_out_len    in: size of p_out; out: bytes written (k)
 *
 * @return 1 on success, 0 on error
 */
int rsa_oaep_encrypt
(
    public_key *p_public_key,
    const unsigned char *p_msg  , size_t msg_len,
    const unsigned char *p_label, size_t label_len,
    unsigned char       *p_out  , size_t *p_out_len
);

/** !
 * RSAES-OAEP-DECRYPT using SHA-256 and MGF1
 *
 * @param p_public_key  the public key (for n)
 * @param p_private_key the private key
 * @param p_ct          ciphertext buffer (k bytes)
 * @param ct_len        ciphertext length in bytes (must equal k)
 * @param p_label       optional label (must match encryption)
 * @param label_len     label length
 * @param p_out         output buffer for plaintext
 * @param p_out_len     in: capacity of p_out; out: bytes written
 *
 * @return 1 on success, 0 on error
 */
int rsa_oaep_decrypt 
(
    public_key          *p_public_key , private_key *p_private_key,
    const unsigned char *p_cipher_text, size_t       cipher_text_len,
    const unsigned char *p_label      , size_t       label_len,
    unsigned char       *p_out        , size_t      *p_out_len
);
