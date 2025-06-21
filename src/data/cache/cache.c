/** !
 * Implementation of cache
 *
 * @file cache.c
 *
 * @author Jacob Smith
 */

// headers
#include <core/cache.h>

int cache_create ( cache **const pp_cache )
{

    // argument check
    if ( pp_cache == (void *) 0 ) goto no_cache;

    // initialized data
    cache *p_cache = realloc(0, sizeof(cache));

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
                    log_error("[hash cache] Null pointer provided for parameter \"pp_cache\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }

        // standard library errors
        {
            no_mem:
                #ifndef NDEBUG
                    log_error("[hash cache] Failed to allocate memory in call to function \"%s\"\n", __FUNCTION__);
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
            .pp_data = realloc(0, sizeof(void *) * size)
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
                    log_error("[hash cache] Null pointer provided for parameter \"pp_cache\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
                
            invalid_size:
                #ifndef NDEBUG
                    log_error("[hash cache] Parameter \"size\" must be greater than zero in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }

        // Hash cache errors
        {
            failed_to_allocate_cache:
                #ifndef NDEBUG
                    log_error("[hash cache] Failed to allocate caches in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }

        // standard library errors
        {
            no_mem:
                #ifndef NDEBUG
                    log_error("[hash cache] Failed to allocate memory in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
    }
}

int cache_get ( const cache *const p_cache, const void *const p_key, void **const pp_result )
{

    // argument check
    if ( p_cache == (void *) 0 ) goto no_cache;
    if ( p_key   == (void *) 0 ) goto no_key;

    // Search the cache
    for (size_t i = 0; i < p_cache->properties.count; i++)
    {
        
        // Compare
        if ( p_cache->pfn_equality(p_cache->pfn_key_get(p_cache->properties.pp_data[i]), p_key ) == 0 )
        {

            // Move the property to the front of the cache 
            void *tmp = p_cache->properties.pp_data[i];
            p_cache->properties.pp_data[i] = p_cache->properties.pp_data[0];
            p_cache->properties.pp_data[0] = tmp;

            // Return the value to the caller 
            *pp_result = tmp;

            // success
            return 1;
        }
    }

    // Miss
    return 0;

    // error handling
    {

        // argument errors
        {
            no_cache:
                #ifndef NDEBUG
                    log_error("[hash cache] Null pointer provided for parameter \"p_cache\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;

            no_key:
                #ifndef NDEBUG
                    log_error("[hash cache] Null pointer provided for parameter \"p_key\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
    }
}

int cache_insert ( cache *const p_cache, const void *const p_key, const void *const p_value )
{

    // argument check
    if ( p_cache == (void *) 0 ) goto no_cache;
    if ( p_key   == (void *) 0 ) goto no_key;
    if ( p_value == (void *) 0 ) goto no_value;

    // If the cache is full ...
    if ( p_cache->properties.count == p_cache->properties.max )
    {
        
        // ... remove the last object in the cache ...
        //

        // ... and decrement the quantity of entries
        p_cache->properties.count--;
    }

    // Add the object to the cache
    p_cache->properties.pp_data[p_cache->properties.count++] = (void *) p_value;

    // success
    return 1;

    // error handling
    {

        // argument errors
        {
            no_cache:
                #ifndef NDEBUG
                    log_error("[hash cache] Null pointer provided for parameter \"p_cache\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;

            no_key:
                #ifndef NDEBUG
                    log_error("[hash cache] Null pointer provided for parameter \"p_key\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;

            no_value:
                #ifndef NDEBUG
                    log_error("[hash cache] Null pointer provided for parameter \"p_value\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
    }
}

int cache_destroy ( cache **const pp_cache )
{

    // argument check
    if ( pp_cache == (void *) 0 ) goto no_cache; 

    // initialized data
    cache *p_cache = *pp_cache;

    // No more pointer for caller
    *pp_cache = (void *) 0;
    
    // Free the cache contents
    p_cache->properties.pp_data = realloc(p_cache->properties.pp_data, 0);
    p_cache = realloc(p_cache, 0);

    // success
    return 1;

    // error handling
    {

        // argument errors
        {
            no_cache:
                #ifndef NDEBUG
                    log_error("[hash cache] Null pointer provided for parameter \"p_cache\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
    }
}
