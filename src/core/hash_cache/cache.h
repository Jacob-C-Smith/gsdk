/** !
 * Header for cache
 * 
 * @file hash_cache/cache.h 
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
#include <core/hash.h>

// Platform dependent macros

// Memory management macro
#ifndef HASH_CACHE_REALLOC
#define HASH_CACHE_REALLOC(p, sz) realloc(p,sz)
#endif

// Structure declarations
struct cache_s;

// Type definitions
typedef struct cache_s cache;

// Structure definitions
struct cache_s
{
    struct
    {
        void   **pp_data;
        size_t   count, max;
    } properties;
    fn_hash_cache_equality   *pfn_equality;
    fn_hash_cache_key_accessor *pfn_key_get;
};

// Function declarations 

// Allocators
/** !
 * Allocate memory for a cache
 * 
 * @param pp_cache result
 * 
 * @return 1 on success, 0 on error
 */
int cache_create ( cache **const pp_cache );

// Constructors
/** !
 * Construct a cache
 * 
 * @param pp_cache        result
 * @param size            the maximum quantity of properties the cache can fit
 * @param pfn_equality    pointer to a equality function, or 0 for default
 * @param pfn_key_get     pointer to a key getter, or 0 for key == value
 * 
 * @return 1 on success, 0 on error
 */
int cache_construct (
    cache               **const  pp_cache,
    size_t                       size,
    fn_hash_cache_equality      *pfn_equality,
    fn_hash_cache_key_accessor    *pfn_key_get
);

// Accessors
/** !
 * Search a cache for a value using a key
 * 
 * @param p_cache   the cache
 * @param p_key     the key
 * @param pp_result return
 * 
 * @return 1 on success, 0 on error
 */
int cache_get ( const cache *const p_cache, const void *const p_key, void **const pp_result );

// Mutators
/** !
 * Add a property to a cache
 * 
 * @param p_cache the cache
 * @param p_key   the key of the property
 * @param p_value the value of the property
 * 
 * @return 1 on success, 0 on error
 */
int cache_insert ( cache *const p_cache, const void *const p_key, const void *const p_value );

/** !
 * Remove a property from the cache
 * 
 * @param p_cache   the cache
 * @param p_key     the key of the property
 * @param pp_result return if not null pointer else value is discarded
 * 
 * @return 1 on success, 0 on error
 */
int cache_remove ( cache *const p_cache, const void *const p_key, void **const pp_result );

/** !
 * Clear the cache of all properties
 * 
 * @param p_cache        the cache
 * @param pfn_cache_free the property deallocator if not null pointer else nothing 
 * 
 * @return 1 on success, 0 on error
 */
int cache_clear ( cache *p_cache, fn_hash_cache_free *pfn_free );

// Iterators
/** !
 * Call a function on each element of the cache
 * 
 * @param p_cache      the cache
 * @param pfn_function pointer to the function
 * 
 * @return 1 on success, 0 on error
 */
int cache_for_i ( const cache *const p_cache, fn_hash_cache_property_i pfn_function );

/** !
 * Call a function on each element of the cache
 * 
 * @param p_cache      the cache
 * @param pfn_function pointer to the function
 * 
 * @return 1 on success, 0 on error
 */
int cache_for_each ( const cache *const p_cache, fn_hash_cache_property pfn_function );

// Destructors
/** !
 * Release a cache and all its allocations
 * 
 * @param p_cache        the cache
 * @param pfn_cache_free the property deallocator if not null pointer else nothing 
 * 
 * @return 1 on success, 0 on error
 */
int cache_destroy ( cache **const pp_cache, fn_hash_cache_free *pfn_cache_free );
