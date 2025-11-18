/** !
 * Implementation of cache
 *
 * @file cache.c
 *
 * @author Jacob Smith
 */

// header
#include <data/cache.h>

// core
#include <core/interfaces.h>
#include <core/pack.h>

int cache_create ( cache **const pp_cache )
{

    // argument check
    if ( pp_cache == (void *) 0 ) goto no_cache;

    // initialized data
    cache *p_cache = default_allocator(0, sizeof(cache));

    // error check
    if ( p_cache == (void *) 0 ) goto no_mem;

    // Initialize data
    memset(p_cache, 0, sizeof(cache));

    // return a pointer to the caller
    *pp_cache = p_cache;

    // success
    return 1;

    // error handling
    {

        // argument errors
        {
            no_cache:
                #ifndef NDEBUG
                    log_error("[cache] Null pointer provided for parameter \"pp_cache\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }

        // standard library errors
        {
            no_mem:
                #ifndef NDEBUG
                    log_error("[cache] Failed to allocate memory in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
    }
}

int cache_construct
(
    cache    **const  pp_cache,
    size_t            size,

    fn_equality      *pfn_equality,
    fn_key_accessor  *pfn_key_get
)
{

    // argument check
    if ( pp_cache == (void *) 0 ) goto no_cache;
    if ( size     ==          0 ) goto invalid_size;

    // initialized data
    cache *p_cache = (void *) 0;

    // Allocate memory for the cache
    if ( cache_create(&p_cache) == 0 ) goto failed_to_allocate_cache;

    // Populate the cache
    *p_cache = (cache)
    {
        .properties = 
        {
            .count = 0,
            .max = size,
            .pp_data = default_allocator(0, sizeof(void *) * size)
        },
        .pfn_equality = (pfn_equality ? pfn_equality :     (fn_equality *) default_equality),
        .pfn_key_get  = (pfn_key_get  ? pfn_key_get  : (fn_key_accessor *) default_key_accessor)
    };

    // error check
    if ( p_cache->properties.pp_data == (void *) 0 ) goto no_mem;

    // return a pointer to the caller
    *pp_cache = p_cache;

    // success
    return 1;

    // error handling
    {

        // argument errors
        {
            no_cache:
                #ifndef NDEBUG
                    log_error("[cache] Null pointer provided for parameter \"pp_cache\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
                
            invalid_size:
                #ifndef NDEBUG
                    log_error("[cache] Parameter \"size\" must be greater than zero in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }

        // Hash cache errors
        {
            failed_to_allocate_cache:
                #ifndef NDEBUG
                    log_error("[cache] Failed to allocate caches in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }

        // standard library errors
        {
            no_mem:
                #ifndef NDEBUG
                    log_error("[cache] Failed to allocate memory in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
    }
}

int cache_find ( cache *p_cache, const void *const p_key, void **const pp_result )
{

    // argument check
    if ( p_cache == (void *) 0 ) goto no_cache;
    if ( p_key   == (void *) 0 ) goto no_key;

    // linear search
    for (size_t i = 0; i < p_cache->properties.count; i++)
    {
        // if the keys match...
        if ( p_cache->pfn_equality(p_cache->pfn_key_get(p_cache->properties.pp_data[i]), p_key) )
        {
            // initialized data
            void *p_temp = p_cache->properties.pp_data[i];
            
            // store the result
            if ( pp_result )
                *pp_result = p_temp;

            // shift elements
            for (size_t j = i; j > 0; j--)
                p_cache->properties.pp_data[j] = p_cache->properties.pp_data[j-1];

            // move found element to the front
            p_cache->properties.pp_data[0] = p_temp;
            
            // Hit
            return 1;
        }
    }

    if ( pp_result ) *pp_result = NULL;

    // Miss
    return 0;

    // error handling
    {

        // argument errors
        {
            no_cache:
                #ifndef NDEBUG
                    log_error("[cache] Null pointer provided for parameter \"p_cache\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;

            no_key:
                #ifndef NDEBUG
                    log_error("[cache] Null pointer provided for parameter \"p_key\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
    }
}

int cache_insert ( cache *p_cache, const void *const p_key, const void *const p_value )
{

    // argument check
    if ( p_cache == (void *) 0 ) goto no_cache;
    if ( p_key   == (void *) 0 ) goto no_key;
    if ( p_value == (void *) 0 ) goto no_value;

    // Check if key exists
    for (size_t i = 0; i < p_cache->properties.count; i++)
    {
        if (p_cache->pfn_equality(p_cache->pfn_key_get(p_cache->properties.pp_data[i]), p_key))
        {
            
            // Key found, replace value and move to front
            
            // shift elements
            for (size_t j = i; j > 0; j--)
                p_cache->properties.pp_data[j] = p_cache->properties.pp_data[j-1];

            // move found element to the front
            p_cache->properties.pp_data[0] = (void *)p_value;

            // success
            return 1;
        }
    }

    // Key not found, insert at front. Evict LRU if full.
    
    // Determine the number of elements to shift
    size_t elements_to_shift = (p_cache->properties.count < p_cache->properties.max) ? p_cache->properties.count : p_cache->properties.max - 1;

    // Shift elements to the right
    for (size_t i = elements_to_shift; i > 0; i--)
    {
        p_cache->properties.pp_data[i] = p_cache->properties.pp_data[i-1];
    }

    // Insert the new value at the front
    p_cache->properties.pp_data[0] = (void *)p_value;

    // Increment count if cache is not full
    if (p_cache->properties.count < p_cache->properties.max)
    {
        p_cache->properties.count++;
    }

    // success
    return 1;

    // error handling
    {

        // argument errors
        {
            no_cache:
                #ifndef NDEBUG
                    log_error("[cache] Null pointer provided for parameter \"p_cache\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;

            no_key:
                #ifndef NDEBUG
                    log_error("[cache] Null pointer provided for parameter \"p_key\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;

            no_value:
                #ifndef NDEBUG
                    log_error("[cache] Null pointer provided for parameter \"p_value\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
    }
}

int cache_remove ( cache *p_cache, const void *const p_key, void **const pp_result )
{
    // argument check
    if ( p_cache == (void *) 0 ) goto no_cache;
    if ( p_key   == (void *) 0 ) goto no_key;

    // linear search
    for (size_t i = 0; i < p_cache->properties.count; i++)
    {
        // if the keys match...
        if ( p_cache->pfn_equality(p_cache->pfn_key_get(p_cache->properties.pp_data[i]), p_key) )
        {
            // store the result
            if ( pp_result )
                *pp_result = p_cache->properties.pp_data[i];

            // shift elements left
            for (size_t j = i; j < p_cache->properties.count - 1; j++)
                p_cache->properties.pp_data[j] = p_cache->properties.pp_data[j+1];

            // decrement count
            p_cache->properties.count--;
            
            // success
            return 1;
        }
    }

    // not found
    if ( pp_result )
        *pp_result = NULL;

    // success
    return 1;

    // error handling
    {
        // argument errors
        {
            no_cache:
                #ifndef NDEBUG
                    log_error("[cache] Null pointer provided for parameter \"p_cache\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;

            no_key:
                #ifndef NDEBUG
                    log_error("[cache] Null pointer provided for parameter \"p_key\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
    }
}

int cache_map ( cache *const p_cache, fn_map *pfn_map, fn_allocator *pfn_allocator )
{

    // argument check
    if ( NULL == p_cache ) goto no_cache;
    if ( NULL == pfn_map ) goto no_fn_map;

    // state check
    if ( pfn_allocator ) goto map_with_allocator;

    // iterate through each element in the cache
    for (size_t i = 0; i < p_cache->properties.count; i++)

        // update
        p_cache->properties.pp_data[i] = pfn_map(p_cache->properties.pp_data[i]);

    done:

    // success
    return 1;

    // this branch handles map calls that invoke the allocator
    map_with_allocator:
    {

        // iterate through each element in the cache
        for (size_t i = 0; i < p_cache->properties.count; i++)
        {

            // initialized data
            void *p_old = p_cache->properties.pp_data[i],
                 *p_new = NULL;

            // update
            p_new = pfn_map(p_cache->properties.pp_data[i]),
            p_cache->properties.pp_data[i] = p_new;

            // release
            if ( p_old != p_new && pfn_allocator )
                pfn_allocator(p_cache, 0);
        }

        // done
        goto done;
    }

    // error handling
    {
        
        // argument errors
        {
            no_cache:
                #ifndef NDEBUG
                    log_error("[cache] Null pointer provided for \"p_cache\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
            
            no_fn_map:
                #ifndef NDEBUG
                    log_error("[cache] Null pointer provided for \"pfn_map\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
    }
}

int cache_fori ( cache *p_cache, fn_fori pfn_fori )
{

    // argument check
    if ( NULL == p_cache  ) goto no_cache;
    if ( NULL == pfn_fori ) goto no_fori;

    // iterate through the properties
    for (size_t i = 0; i < p_cache->properties.count; i++)
        pfn_fori(p_cache->properties.pp_data[i], i);    

    // success
    return 1;

    // error handling
    {

        // argument errors
        {
            no_cache:
                #ifndef NDEBUG
                    log_error("[cache] Null pointer provided for parameter \"p_cache\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;

            no_fori:
                #ifndef NDEBUG
                    log_error("[cache] Null pointer provided for parameter \"pfn_fori\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
    }
}

int cache_for_each ( cache *p_cache, fn_foreach pfn_foreach )
{

    // argument check
    if ( NULL == p_cache     ) goto no_cache;
    if ( NULL == pfn_foreach ) goto no_foreach;

    // iterate through the properties
    for (size_t i = 0; i < p_cache->properties.count; i++)
        pfn_foreach(p_cache->properties.pp_data[i]);    

    // success
    return 1;

    // error handling
    {

        // argument errors
        {
            no_cache:
                #ifndef NDEBUG
                    log_error("[cache] Null pointer provided for parameter \"p_cache\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;

            no_foreach:
                #ifndef NDEBUG
                    log_error("[cache] Null pointer provided for parameter \"pfn_foreach\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
    }
}

int cacheee_destroy ( cache **const pp_cache )
{

    // argument check
    if ( pp_cache == (void *) 0 ) goto no_cache; 

    // initialized data
    cache *p_cache = *pp_cache;

    // No more pointer for caller
    *pp_cache = NULL;
    
    // Free the cache contents
    p_cache->properties.pp_data = default_allocator(p_cache->properties.pp_data, 0);
    p_cache = default_allocator(p_cache, 0);

    // success
    return 1;

    // error handling
    {

        // argument errors
        {
            no_cache:
                #ifndef NDEBUG
                    log_error("[cache] Null pointer provided for parameter \"p_cache\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
    }
}

int cache_pack ( void *p_buffer, cache *p_cache, fn_pack *pfn_element )
{
    
    // argument check
    if ( p_cache     == (void *) 0 ) goto no_cache;
    if ( pfn_element == (void *) 0 ) return 0;

    // initialized data 
    char *p = p_buffer;

    // Pack the length
    p += pack_pack(p, "%i64", p_cache->properties.count);

    // iterate through the cache
    for (size_t i = p_cache->properties.count; i-- > 0; )

        p += pfn_element(p, p_cache->properties.pp_data[i]);

    // success
    return p - (char *)p_buffer;

    // error handling
    {
        
        // argument errors
        {
            no_cache:
                #ifndef NDEBUG
                    log_error("[cache] Null pointer provided for \"p_cache\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
    }
}

int cache_unpack ( cache **pp_cache, void *p_buffer, fn_unpack *pfn_element )
{
    
    // argument check
    if ( pp_cache    == (void *) 0 ) goto no_cache;
    if ( pfn_element == (void *) 0 ) return 0;

    // initialized data
    cache *p_cache = NULL;
    char *p = p_buffer;
    size_t len = 0;

    // Unpack the length
    p += pack_unpack(p, "%i64", &len);

    // Construct an cache
    cache_construct(&p_cache, len, 0, 0);

    for (size_t i = 0; i < len; i++)
    {
        
        // initialized data
        char _result[1024] = { 0 };
        void *p_element = NULL;
        size_t len_result = pfn_element(_result, p);

        // Advance the buffer
        p += len_result;

        // Allocate memory for the element
        p_element = default_allocator(0, len_result),

        // Copy the memory
        memcpy(p_element, _result, len_result),
        
        // Add the element to the cache
        cache_insert(p_cache, p_element, p_element);
    }

    // Return the cache to the caller
    *pp_cache = p_cache;

    // success
    return 1;
    
    // error handling
    {
        
        // argument errors
        {
            no_cache:
                #ifndef NDEBUG
                    log_error("[cache] Null pointer provided for \"p_cache\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
    }
}
