/** ! 
 * RSA (en/de)cryption with C23 BitInt
 * 
 * @file rsa.h
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
int enc ( void *p_x, void *p_y, public_key *p_public_key );

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
int dec ( void *p_y, void *p_z, public_key *p_public_key, private_key *p_private_key );
