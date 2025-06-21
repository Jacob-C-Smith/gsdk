/** !
 * Header for hashing functions
 * 
 * @file hash_cache/hash.h 
 * 
 * @author Jacob Smith 
 */

/** ! 
 * Commentary
 * 
 * Hashing functions use the hash in
 * 
 * - hash_fnv64
 * - hash_mmh64
 * - hash_xxh64
 * - hash_crc64
 */

// Include guard
#pragma once

// standard library
#include <stdio.h>
#include <stddef.h>

// core
#include <core/log.h>
#include <core/sync.h>

// hash cache
#include <core/interfaces.h>

// Function declarations 

// Hashing
/** !
 * Compute a 64-bit hash using the Fowler–Noll–Vo hash function
 * 
 * @param k pointer to data to be hashed
 * @param l number of bytes to hash
 * 
 * @interface fn_hash64 : hash64 (*) ( const void *const k, unsigned long long l )
 * 
 * @return the 64-bit hash on success, 0 on error
*/
hash64 hash_fnv64 ( const void *const k, unsigned long long l );

/** !
 * Compute a 64-bit hash using the MurMur hash function
 * 
 * @param k pointer to data to be hashed
 * @param l number of bytes to hash
 * 
 * @interface fn_hash64 : hash64 (*) ( const void *const k, unsigned long long l )
 * 
 * @return the 64-bit hash on success, 0 on error
*/
hash64 hash_mmh64 ( const void *const k, unsigned long long l );

/** !
 * Compute a 64-bit hash using the xxHash hash function
 * 
 * @param k pointer to data to be hashed
 * @param l number of bytes to hash
 * 
 * @interface fn_hash64 : hash64 (*) ( const void *const k, unsigned long long l )
 * 
 * @return the 64-bit hash on success, 0 on error
*/
hash64 hash_xxh64 ( const void *const k, unsigned long long l );

/** !
 * Compute a 64-bit hash using the CRC hash function
 * 
 * @param k pointer to data to be hashed
 * @param l number of bytes to hash
 * 
 * @interface fn_hash64 : hash64 (*) ( const void *const k, unsigned long long l )
 * 
 * @return the 64-bit hash on success, 0 on error
*/
hash64 hash_crc64 ( const void *const k, unsigned long long l );