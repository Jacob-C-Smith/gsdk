/** !
 * Cache interface
 * 
 * @file src/data/cache/cache.h 
 *
 * @author Jacob Smith
 */

// header guard
#pragma once

// standard library
#include <stdio.h>
#include <stddef.h>

// gsdk
/// core
#include <core/hash.h>
#include <core/interfaces.h>
#include <core/log.h>
#include <core/pack.h>
#include <core/sync.h>

// preprocessor definitions
// NOTE: MacOS wierdness
#define cache_destroy cacheee_destroy
#define cache_remove cacheee_remove

// structure declarations
struct cache_s;

// type definitions
typedef struct cache_s cache;

// function declarations 
/// constructors
/** !
 * Construct a cache
 * 
 * @param pp_cache result
 * @param size     the maximum quantity of values the cache can fit
 * 
 * @param pfn_equality     pointer to equality function IF NOT NULL ELSE default
 * @param pfn_key_accessor pointer to key accessor function IF NOT NULL ELSE default
 * @param pfn_allocator    pointer to allocator function IF NOT NULL ELSE default
 * 
 * @return 1 on success, 0 on error
 */
int cache_construct
(
    cache    **const  pp_cache,
    size_t            size,

    fn_equality      *pfn_equality,
    fn_key_accessor  *pfn_key_accessor,
    fn_allocator     *pfn_allocator
);

/// accessors
/** !
 * Search a cache for a value using a key
 * 
 * @param p_cache   the cache
 * @param p_key     the key
 * @param pp_result return
 * 
 * @return 1 on success, 0 on error
 */
int cache_find ( cache *p_cache, const void *const p_key, void **const pp_result );

/** !
 * Search a cache for a value using a key, without updating the order of the cache
 * 
 * @param p_cache   the cache
 * @param p_key     the key
 * @param pp_result return
 * 
 * @return 1 on success, 0 on error
 */
int cache_peek ( cache *p_cache, const void *const p_key, void **const pp_result );

/** !
 * Get the size of a cache
 * 
 * @param p_cache the cache
 * 
 * @return the size of the cache
 */
size_t cache_size ( cache *p_cache );

/// mutators
/** !
 * Add a value to a cache, optionally deallocating the overflowed element
 * 
 * @param p_cache       the cache
 * @param p_value       the value
 * @param pfn_allocator pointer to allocator function IF NOT NULL ELSE default
 * 
 * @return 1 on success, 0 on error
 */
int cache_insert ( cache *p_cache, const void *const p_value, fn_allocator *pfn_allocator );

/** !
 * Remove a value from the cache
 * 
 * @param p_cache   the cache
 * @param p_key     the key of the value
 * @param pp_result return IF NOT NULL ELSE value is discarded
 * 
 * @note IF pp_result IS NULL, you are responsible for releasing the element.
 * 
 * @return 1 on success, 0 on error
 */
int cache_remove ( cache *p_cache, const void *const p_key, void **const pp_result );

/// map
/** !
 * Apply an operation to each element in a cache,
 * optionally updating elements
 *
 * @param p_cache       the cache
 * @param pfn_map       pointer to map function
 * @param pfn_allocator pointer to allocator function
 * 
 * @return 1 on success, 0 on error
 */
int cache_map ( cache *const p_cache, fn_map *pfn_map, fn_allocator *pfn_allocator );

/// iterators
/** !
 * Call a function on each element of a cache
 * 
 * @param p_cache  the cache
 * @param pfn_fori pointer to the function
 * 
 * @return 1 on success, 0 on error
 */
int cache_fori ( cache *p_cache, fn_fori pfn_fori );

/** !
 * Call a function on each element of a cache
 * 
 * @param p_cache      the cache
 * @param pfn_function pointer to the function
 * 
 * @return 1 on success, 0 on error
 */
int cache_for_each ( cache *p_cache, fn_foreach pfn_foreach );

/// reflection
/** !
 * Pack a cache into a buffer
 * 
 * @param p_buffer     the buffer
 * @param p_cache      the cache
 * @param pfn_elemenet pointer to pack function IF not null ELSE default
 * 
 * @return 1 on success, 0 on error
 */
int cache_pack ( void *p_buffer, cache *p_cache, fn_pack *pfn_element );

/** !
 * Unpack a buffer into a cache
 * 
 * @param pp_cache     result
 * @param p_buffer     the buffer
 * @param pfn_elemenet pointer to unpack function IF not null ELSE default
 * 
 * @param pfn_equality  pointer to equality function IF NOT NULL ELSE default
 * @param pfn_key_get   pointer to key accessor function IF NOT NULL ELSE default
 * @param pfn_allocator pointer to allocator function IF NOT NULL ELSE default
 *  
 * @return 1 on success, 0 on error
 */
int cache_unpack
(
    cache **pp_cache,
    void *p_buffer,
    fn_unpack *pfn_element,

    fn_equality      *pfn_equality,
    fn_key_accessor  *pfn_key_accessor,
    fn_allocator     *pfn_allocator
);

/// hash
/** !
 * Compute a 64-bit hash of a cache
 * 
 * @param p_cache     the cache
 * @param pfn_element pointer to hashing function
 * 
 * @return hash on success, 0 on error
 */
hash64 cache_hash ( cache *p_cache, fn_hash64 *pfn_element );

/// destructors
/** !
 * Release a cache and all its allocations
 * 
 * @param pp_cache pointer to cache pointer
 * 
 * @return 1 on success, 0 on error
 */
int cache_destroy ( cache **const pp_cache );
