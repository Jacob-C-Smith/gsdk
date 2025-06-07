/** !
 * Header for hashing functions
 * 
 * @file hash_cache/hash.h 
 * 
 * @author Jacob Smith 
 */

// Include guard
#pragma once

// standard library
#include <stdio.h>
#include <stddef.h>

// sync module
#include <core/sync.h>

// log module
#include <core/log.h>

// hash cache
#include <core/hash_cache.h>

// Function declarations 

// Hashing
/** !
 * Compute a 64-bit hash using the Fowler–Noll–Vo hash function
 * 
 * @param k pointer to data to be hashed
 * @param l number of bytes to hash
 * 
 * @return the 64-bit hash on success, 0 on error
*/
hash64 hash_fnv64 ( const void *const k, size_t l );

/** !
 * Compute a 64-bit hash using the MurMur hash function
 * 
 * @param k pointer to data to be hashed
 * @param l number of bytes to hash
 * 
 * @return the 64-bit hash on success, 0 on error
*/
hash64 hash_mmh64 ( const void *const k, size_t l );

/** !
 * Compute a 64-bit hash using the xxHash hash function
 * 
 * @param k pointer to data to be hashed
 * @param l number of bytes to hash
 * 
 * @return the 64-bit hash on success, 0 on error
*/
hash64 hash_xxh64 ( const void *const k, size_t l );

/** !
 * Compute a 64-bit hash using the CRC hash function
 * 
 * @param k pointer to data to be hashed
 * @param l number of bytes to hash
 * 
 * @return the 64-bit hash on success, 0 on error
*/
hash64 hash_crc64 ( const void *const k, size_t l );
