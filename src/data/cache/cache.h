/** !
 * Header for cache
 * 
 * @file hash_cache/cache.h 
 *
 * @author Jacob Smith
 */

// header guard
#pragma once

// standard library
#include <stdio.h>
#include <stddef.h>

// core
#include <core/interfaces.h>
#include <core/log.h>
#include <core/sync.h>
#include <core/hash.h>

// preprocessor definitions
#define cache_destroy cacheee_destroy
#define cache_remove cacheee_remove

// structure declarations
struct cache_s;

// type definitions
typedef struct cache_s cache;

// structure definitions
struct cache_s
{
    fn_equality     *pfn_equality;
    fn_key_accessor *pfn_key_get;

    struct
    {
        void   **pp_data;
        size_t   count, max;
    } properties;
};

// function declarations 
/// constructors
/** !
 * Construct a cache
 * 
 * @param pp_cache        result
 * @param size            the maximum quantity of properties the cache can fit
 * @param pfn_equality    pointer to a equality function, NULL for default
 * @param pfn_key_get     pointer to a key getter, NULL for default
 * 
 * @return 1 on success, 0 on error
 */
int cache_construct (
    cache           **pp_cache,
    size_t            size,

    fn_equality      *pfn_equality,
    fn_key_accessor  *pfn_key_get
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

/// mutators
/** !
 * Add a property to a cache
 * 
 * @param p_cache the cache
 * @param p_key   the key of the property
 * @param p_value the value of the property
 * 
 * @return 1 on success, 0 on error
 */
int cache_insert ( cache *p_cache, const void *const p_key, const void *const p_value );

/** !
 * Remove a property from the cache
 * 
 * @param p_cache   the cache
 * @param p_key     the key of the property
 * @param pp_result return if not null pointer else value is discarded
 * 
 * @return 1 on success, 0 on error
 */
int cacheee_remove ( cache *p_cache, const void *const p_key, void **const pp_result );


/// map
/** !
 * Apply an operation to each element in a cache,
 * optionally releasing elements
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

// reflection
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
 * @return 1 on success, 0 on error
 */
int cache_unpack ( cache **pp_cache, void *p_buffer, fn_unpack *pfn_element );

/// destructors
/** !
 * Release a cache and all its allocations
 * 
 * @param p_cache the cache
 * 
 * @return 1 on success, 0 on error
 */
int cacheee_destroy ( cache **const pp_cache );
