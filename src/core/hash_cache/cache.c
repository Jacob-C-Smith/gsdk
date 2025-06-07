/** !
 * Implementation of cache
 *
 * @file cache.c
 *
 * @author Jacob Smith
 */

// Headers
#include <core/cache.h>

int cache_create ( cache **const pp_cache )
{

    // Argument check
    if ( pp_cache == (void *) 0 ) goto no_cache;

    // Initialized data
    cache *p_cache = HASH_CACHE_REALLOC(0, sizeof(cache));

    // Error check
    if ( p_cache == (void *) 0 ) goto no_mem;

    // Initialize data
    memset(p_cache, 0, sizeof(cache));

    // Return a pointer to the caller
    *pp_cache = p_cache;

    // Success
    return 1;

    // Error handling
    {

        // Argument errors
        {
            no_cache:
                #ifndef NDEBUG
                    log_error("[hash cache] Null pointer provided for parameter \"pp_cache\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }

        // standard library errors
        {
            no_mem:
                #ifndef NDEBUG
                    log_error("[hash cache] Failed to allocate memory in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }
    }
}

int cache_construct (
    cache               **const  pp_cache,
    size_t                       size,
    fn_hash_cache_equality      *pfn_equality,
    fn_hash_cache_key_accessor    *pfn_key_get
)
{

    // Argument check
    if ( pp_cache == (void *) 0 ) goto no_cache;
    if ( size     ==          0 ) goto invalid_size;

    // Initialized data
    cache *p_cache = (void *) 0;

    // Allocate memory for the cache
    if ( cache_create(&p_cache) == 0 ) goto failed_to_allocate_cache;

    // Cache starts empty
    p_cache->properties.count = 0;

    // Store the size
    p_cache->properties.max = size;

    // Set the equality function
    p_cache->pfn_equality = pfn_equality ? pfn_equality : (fn_hash_cache_equality *) hash_cache_equals;

    // Set the key getter function
    p_cache->pfn_key_get = pfn_key_get ? pfn_key_get : (fn_hash_cache_key_accessor *) hash_cache_key_accessor;

    // Allocate memory for the cache
    p_cache->properties.pp_data = HASH_CACHE_REALLOC(0, sizeof(void *) * size);

    // Error check
    if ( p_cache->properties.pp_data == (void *) 0 ) goto no_mem;

    // Return a pointer to the caller
    *pp_cache = p_cache;

    // Success
    return 1;

    // Error handling
    {

        // Argument errors
        {
            no_cache:
                #ifndef NDEBUG
                    log_error("[hash cache] Null pointer provided for parameter \"pp_cache\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
                
            invalid_size:
                #ifndef NDEBUG
                    log_error("[hash cache] Parameter \"size\" must be greater than zero in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }

        // Hash cache errors
        {
            failed_to_allocate_cache:
                #ifndef NDEBUG
                    log_error("[hash cache] Failed to allocate caches in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }

        // standard library errors
        {
            no_mem:
                #ifndef NDEBUG
                    log_error("[hash cache] Failed to allocate memory in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }
    }
}

int cache_get ( const cache *const p_cache, const void *const p_key, void **const pp_result )
{

    // Argument check
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

            // Success
            return 1;
        }
    }

    // Miss
    return 0;

    // Error handling
    {

        // Argument errors
        {
            no_cache:
                #ifndef NDEBUG
                    log_error("[hash cache] Null pointer provided for parameter \"p_cache\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;

            no_key:
                #ifndef NDEBUG
                    log_error("[hash cache] Null pointer provided for parameter \"p_key\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }
    }
}

int cache_insert ( cache *const p_cache, const void *const p_key, const void *const p_value )
{

    // Argument check
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
    p_cache->properties.pp_data[p_cache->properties.count] = (void *) p_value;

    // Increment the quantity of entries
    p_cache->properties.count++;

    // Success
    return 1;

    // Error handling
    {

        // Argument errors
        {
            no_cache:
                #ifndef NDEBUG
                    log_error("[hash cache] Null pointer provided for parameter \"p_cache\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;

            no_key:
                #ifndef NDEBUG
                    log_error("[hash cache] Null pointer provided for parameter \"p_key\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;

            no_value:
                #ifndef NDEBUG
                    log_error("[hash cache] Null pointer provided for parameter \"p_value\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }
    }
}

int cache_remove ( cache *const p_cache, const void *const p_key, void **const pp_result )
{

    // Argument check
    if ( p_cache == (void *) 0 ) goto no_cache;
    if ( p_key   == (void *) 0 ) goto no_key;
    
    // Unused
    (void) pp_result;

    /*
    // Initialized data
    size_t i = 0;
    void *p_value = (void *) 0;

    // Search the cache
    for (; i < p_cache->entries; i++)
        
        // Compare
        if ( p_cache->pfn_equals(p_cache->pfn_key(p_cache->data[i]), p_key ) )
        {

            // Move the property to the front of the cache 

            // Return the value to the caller 
            p_value = p_cache->data[i];
        }

    // If the cache hit
    if ( p_value )
    {

        // Remove the element from the cache

        // Decrement the property count

        // Return a pointer to the caller
        if ( *pp_result ) *pp_result = p_value;

        // Success
        return 1;
    }*/

    // Miss
    return 0;

    // Error handling
    {

        // Argument errors
        {
            no_cache:
                #ifndef NDEBUG
                    log_error("[hash cache] Null pointer provided for parameter \"p_cache\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;

            no_key:
                #ifndef NDEBUG
                    log_error("[hash cache] Null pointer provided for parameter \"p_key\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }
    }
}

int cache_clear ( cache *p_cache, fn_hash_cache_free *pfn_free )
{

    // Argument check
    if ( p_cache == (void *) 0 ) goto no_cache;

    // Free each property
    if ( pfn_free )
        
        // Iterate through the contents of the cache
        for (size_t i = 0; i < p_cache->properties.count; i++)
        
            // Free the property
            pfn_free(p_cache->properties.pp_data[i]);

    // Clear the property counter
    p_cache->properties.count = 0;

    // Success
    return 1;

    // Error handling
    {

        // Argument errors
        {
            no_cache:
                #ifndef NDEBUG
                    log_error("[hash cache] Null pointer provided for parameter \"p_cache\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }
    }
}

int cache_for_i ( const cache *const p_cache, fn_hash_cache_property_i pfn_function )
{
    
    // Argument check
    if ( p_cache      == (void *) 0 ) goto no_cache;
    if ( pfn_function == (void *) 0 ) goto no_function;

    // Iterate through the cache
    for (size_t i = 0; i < p_cache->properties.count; i++)

        // Call the function
        pfn_function(p_cache->properties.pp_data[i], i);        

    // Success
    return 1;

    // Error handling
    {

        // Argument errors
        {
            no_cache:
                #ifndef NDEBUG
                    log_error("[hash cache] Null pointer provided for parameter \"p_cache\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;

            no_function:
                #ifndef NDEBUG
                    log_error("[hash cache] Null pointer provided for parameter \"pfn_function\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }
    }
}

int cache_for_each ( const cache *const p_cache, fn_hash_cache_property pfn_function )
{
    
    // Argument check
    if ( p_cache      == (void *) 0 ) goto no_cache;
    if ( pfn_function == (void *) 0 ) goto no_function;

    // Iterate through the cache
    for (size_t i = 0; i < p_cache->properties.count; i++)

        // Call the function
        pfn_function(p_cache->properties.pp_data[i]);

    // Success
    return 1;

    // Error handling
    {

        // Argument errors
        {
            no_cache:
                #ifndef NDEBUG
                    log_error("[hash cache] Null pointer provided for parameter \"p_cache\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;

            no_function:
                #ifndef NDEBUG
                    log_error("[hash cache] Null pointer provided for parameter \"pfn_function\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }
    }
}

int cache_info ( const cache *const p_cache );

int cache_destroy ( cache **const pp_cache, fn_hash_cache_free *pfn_cache_free )
{

    // Argument check
    if ( pp_cache == (void *) 0 ) goto no_cache; 

    // Initialized data
    cache *p_cache = *pp_cache;

    // No more pointer for caller
    *pp_cache = (void *) 0;

    // Clear the cache
    cache_clear(p_cache, pfn_cache_free);

    // Free the cache contents
    if ( HASH_CACHE_REALLOC(p_cache->properties.pp_data, 0) ) goto failed_to_free;

    // Free the cache
    if ( HASH_CACHE_REALLOC(p_cache, 0) ) goto failed_to_free;

    // Success
    return 1;

    // Error handling
    {

        // Argument errors
        {
            no_cache:
                #ifndef NDEBUG
                    log_error("[hash cache] Null pointer provided for parameter \"p_cache\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }

        // standard library errors
        {
            failed_to_free:
                #ifndef NDEBUG
                    log_error("[standard library] Failed to free memory in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }
    }
}
