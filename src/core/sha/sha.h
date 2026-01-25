/** ! 
 * SHA256 interface
 * 
 * @file core/sha256.h
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
struct sha256_state_s;
struct sha512_state_s;

// type definitions
typedef struct sha256_state_s sha256_state;
typedef unsigned char sha256_hash[32];

typedef struct sha512_state_s sha512_state;
typedef unsigned char sha512_hash[64];

// structure definitions
struct sha256_state_s
{
    unsigned int   _state[8];
    unsigned char _data[64];
    unsigned int  datalen;
    unsigned long bitlen;
};

struct sha512_state_s
{
    unsigned long long _state[8];
    unsigned char      _data[128];
    unsigned int       datalen;
    unsigned long long bitlen;
};

// function declarations
/// constructors
/** !
 * Construct a SHA256 hasher
 * 
 * @param p_sha256_state result
 * 
 * @return 1 on success, 0 on error
 */
int sha256_construct ( sha256_state *p_sha256_state );

/** !
 * Construct a SHA512 hasher
 * 
 * @param p_sha512_state result
 * 
 * @return 1 on success, 0 on error
 */
int sha512_construct ( sha512_state *p_sha512_state );

/// state
/** !
 * Feed the SHA256 hasher 
 * 
 * @param p_sha256_state the hasher
 * @param data           the data to feed
 * @param len            the length of the data
 * 
 * @return 1 on success, 0 on error
 */
int sha256_update ( sha256_state *p_sha256_state, const unsigned char *data, size_t len );

/** !
 * Feed the SHA512 hasher 
 * 
 * @param p_sha512_state the hasher
 * @param data           the data to feed
 * @param len            the length of the data
 * 
 * @return 1 on success, 0 on error
 */
int sha512_update ( sha512_state *p_sha512_state, const unsigned char *data, size_t len );

/** !
 * Produce a hash
 * 
 * @param p_sha256_state the hasher
 * @param hash           result
 * 
 * @return 1 on success, 0 on error
 */
int sha256_final ( sha256_state *p_sha256_state, unsigned char *hash );

/** !
 * Produce a hash
 * 
 * @param p_sha512_state the hasher
 * @param hash           result
 * 
 * @return 1 on success, 0 on error
 */
int sha512_final ( sha512_state *p_sha512_state, unsigned char *hash );

/// log
/** !
 * Print a SHA256 hash to standard output
 * 
 * @param _hash the hash 
 * 
 * @return 1 on success, 0 on error
 */
int sha256_print ( sha256_hash _hash );

/** !
 * Print a SHA512 hash to standard output
 * 
 * @param _hash the hash 
 * 
 * @return 1 on success, 0 on error
 */
int sha512_print ( sha512_hash _hash );

/// hash64
/** !
 * Compute the SHA256 hash of a key, and cast it to a 64-bit hash
 * 
 * @param k the key
 * @param l the length
 * 
 * @return the hash
 */
hash64 sha256_hash64 ( const void *const k, unsigned long long l );

/** !
 * Compute the SHA512 hash of a key, and cast it to a 64-bit hash
 * 
 * @param k the key
 * @param l the length
 * 
 * @return the hash
 */
hash64 sha512_hash64 ( const void *const k, unsigned long long l );

/// pack
/** !
 * Pack a SHA256 hash into a buffer
 * 
 * @param p_buffer the buffer to pack into
 * @param _hash    the SHA256 hash to pack
 * 
 * @return the number of bytes packed, or 0 on error
 */
int sha256_pack ( void *p_buffer, sha256_hash _hash );

/** !
 * Pack a SHA512 hash into a buffer
 * 
 * @param p_buffer the buffer to pack into
 * @param _hash    the SHA512 hash to pack
 * 
 * @return the number of bytes packed, or 0 on error
 */
int sha512_pack ( void *p_buffer, sha512_hash _hash );

/// unpack
/** !
 * Unpack a SHA256 hash from a buffer
 * 
 * @param p_buffer      the buffer to unpack from
 * @param p_sha256_hash the SHA256 hash to unpack
 * 
 * @return the number of bytes unpacked, or 0 on error
 */
int sha256_unpack ( sha256_hash *p_sha256_hash, void *p_buffer );

/** !
 * Unpack a SHA512 hash from a buffer
 * 
 * @param p_buffer      the buffer to unpack from
 * @param p_sha512_hash the SHA512 hash to unpack
 * 
 * @return the number of bytes unpacked, or 0 on error
 */
int sha512_unpack ( sha512_hash *p_sha512_hash, void *p_buffer );
