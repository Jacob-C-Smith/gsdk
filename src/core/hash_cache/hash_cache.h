/** !
 * Include header for hash cache library
 * 
 * @file hash_cache/hash_cache.h 
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

// Platform dependent macros

// Memory management macro
#ifndef HASH_CACHE_REALLOC
#define HASH_CACHE_REALLOC(p, sz) realloc(p,sz)
#endif

// Type definitions
typedef unsigned long long hash64;

typedef hash64 (fn_hash64)                  ( const void *const k, size_t l );
typedef hash64 (fn_hash_cache_hash_index)   ( const void *const k, size_t l, size_t i );
typedef int    (fn_hash_cache_equality)     ( const void *const p_a, const void *const p_b );
typedef void  *(fn_hash_cache_key_accessor) ( const void *const p_value );
typedef void   (fn_hash_cache_free)         ( void *p_property );
typedef int    (fn_hash_cache_property)     ( void *p_property );
typedef int    (fn_hash_cache_property_i)   ( void *p_property, size_t i );

// Function declarations 

// Initializer
/** !
 * This gets called at runtime before main. 
 * 
 * @param void
 * 
 * @return void
 */
void hash_cache_init ( void ) __attribute__((constructor));

// Comparator
/** !
 * Default comparator
 * 
 * @param p_a A
 * @param p_b B
 * 
 * @return 0 if A == B else 1
 */
int hash_cache_equals ( const void *const p_a, const void *p_b );

// Key accessors
/** !
 * Default key accessor
 * 
 * @param p_value the value
 * 
 * @return p_value
 */
void *hash_cache_key_accessor ( const void *const p_value );

// Cleanup
/** !
 * This gets called at runtime after main
 * 
 * @param void
 * 
 * @return void
 */
void hash_cache_exit ( void ) __attribute__((destructor));
