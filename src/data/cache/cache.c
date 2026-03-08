/** !
 * Cache implementation
 *
 * @file src/data/cache/cache.c
 *
 * @author Jacob Smith
 */

// header
#include <data/cache.h>

// structure definitions
struct cache_s
{
    struct
    {
        void   **pp_data;
        size_t   count, max;
    } properties;

    mutex _lock;

    fn_equality     *pfn_equality;
    fn_key_accessor *pfn_key_accessor;
    fn_allocator    *pfn_allocator;
};

// function definitions
int cache_construct
(
    cache    **const  pp_cache,
    size_t            size,

    fn_equality      *pfn_equality,
    fn_key_accessor  *pfn_key_accessor,
    fn_allocator     *pfn_allocator
)
{

    // argument check
    if ( NULL == pp_cache ) goto no_cache;
    if ( 0    ==     size ) goto invalid_size;

    // initialized data
    cache *p_cache = NULL;

    // allocate memory for the cache
    p_cache = default_allocator(0, sizeof(cache));
    if ( NULL == p_cache ) goto no_mem;
    
    // populate the cache fields
    *p_cache = (cache)
    {
        .properties = 
        {
            .count   = 0,
            .max     = size,
            .pp_data = NULL
        },
        .pfn_equality      = (pfn_equality     ? pfn_equality     : default_equality),
        .pfn_key_accessor  = (pfn_key_accessor ? pfn_key_accessor : default_key_accessor),
        .pfn_allocator     = pfn_allocator
    };

    // allocate memory for the values
    p_cache->properties.pp_data = default_allocator(0, sizeof(void *) * size);
    if ( NULL == p_cache->properties.pp_data )
    {

        // release the cache
        p_cache = default_allocator(p_cache, 0);

        // error
        goto no_mem;
    }

    // construct a lock
    mutex_create(&p_cache->_lock);

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
    if ( NULL == p_cache ) goto no_cache;
    if ( NULL ==   p_key ) goto no_key;

    // lock
    mutex_lock(&p_cache->_lock);

    // linear search
    for (size_t i = 0; i < p_cache->properties.count; i++)
    {

        // if the keys match...
        if ( p_cache->pfn_equality(p_cache->pfn_key_accessor(p_cache->properties.pp_data[i]), p_key) )
        {

            // initialized data
            void *p_temp = p_cache->properties.pp_data[i];
            
            // return a pointer to the caller
            if ( pp_result )
                *pp_result = p_temp;

            // shift elements
            for (size_t j = i; j > 0; j--)
                p_cache->properties.pp_data[j] = p_cache->properties.pp_data[j-1];

            // move found element to the front
            p_cache->properties.pp_data[0] = p_temp;
                    
            // unlock
            mutex_unlock(&p_cache->_lock);
            
            // hit
            return 1;
        }
    }

    // return a NULL to the caller
    if ( pp_result ) *pp_result = NULL;
        
    // unlock
    mutex_unlock(&p_cache->_lock);
    
    // miss
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

int cache_peek ( cache *p_cache, const void *const p_key, void **const pp_result )
{

    // argument check
    if ( NULL == p_cache ) goto no_cache;
    if ( NULL ==   p_key ) goto no_key;

    // lock
    mutex_lock(&p_cache->_lock);

    // linear search
    for (size_t i = 0; i < p_cache->properties.count; i++)
    {

        // if the keys match...
        if ( p_cache->pfn_equality(p_cache->pfn_key_accessor(p_cache->properties.pp_data[i]), p_key) )
        {

            // initialized data
            void *p_temp = p_cache->properties.pp_data[i];
            
            // return a pointer to the caller
            if ( pp_result )
                *pp_result = p_temp;

            // unlock
            mutex_unlock(&p_cache->_lock);
            
            // hit
            return 1;
        }
    }

    // return a NULL to the caller
    if ( pp_result ) *pp_result = NULL;
        
    // unlock
    mutex_unlock(&p_cache->_lock);
    
    // miss
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

size_t cache_size ( cache *p_cache )
{

    // argument check
    if ( NULL == p_cache ) goto no_cache;

    // initialized data
    size_t result = 0;

    // lock
    mutex_lock(&p_cache->_lock);

    // store the result
    result = p_cache->properties.count;

    // unlock
    mutex_unlock(&p_cache->_lock);

    // success
    return result;

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
    }
}

int cache_insert ( cache *p_cache, const void *const p_value, fn_allocator *pfn_allocator )
{

    // argument check
    if ( NULL == p_cache ) goto no_cache;
    if ( NULL == p_value ) goto no_value;
    
    // initialized data
    void *p_key = NULL;

    // lock
    mutex_lock(&p_cache->_lock);

    // store the key
    p_key = p_cache->pfn_key_accessor(p_value);

    // check if key exists
    for (size_t i = 0; i < p_cache->properties.count; i++)
    {

        // test the key
        if (
            p_cache->pfn_equality(
                p_cache->pfn_key_accessor(p_cache->properties.pp_data[i]), 
                p_key
            )
        )
        {
            
            // initialized data
            void *p_old = p_cache->properties.pp_data[i];

            // shift elements
            for (size_t j = i; j > 0; j--)
                p_cache->properties.pp_data[j] = p_cache->properties.pp_data[j-1];

            // move found element to the front
            p_cache->properties.pp_data[0] = (void *)p_value;

            // release the old value
            if ( p_old != p_value && p_cache->pfn_allocator )
                p_cache->pfn_allocator(p_old, 0);

            // unlock
            mutex_unlock(&p_cache->_lock);

            // success
            return 1;
        }
    }
    
    // handle eviction if the cache is full
    if ( p_cache->properties.count == p_cache->properties.max )
    {

        // initialized data
        void *p_evicted = p_cache->properties.pp_data[p_cache->properties.max - 1];

        // shift elements to the right
        for (size_t i = p_cache->properties.max - 1; i > 0; i--)
            p_cache->properties.pp_data[i] = p_cache->properties.pp_data[i-1];

        // insert the new value at the front
        p_cache->properties.pp_data[0] = (void *)p_value;

        // release the evicted value
        if ( p_evicted != p_value && pfn_allocator )
            pfn_allocator(p_evicted, 0);
    }

    // otherwise, just insert the new value
    else
    {

        // shift elements to the right
        for (size_t i = p_cache->properties.count; i > 0; i--)
            p_cache->properties.pp_data[i] = p_cache->properties.pp_data[i-1];

        // insert the new value at the front
        p_cache->properties.pp_data[0] = (void *)p_value;

        // increment count
        p_cache->properties.count++;
    }

    // unlock
    mutex_unlock(&p_cache->_lock);

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
    if ( NULL == p_cache ) goto no_cache;
    if ( NULL ==   p_key ) goto no_key;

    // lock
    mutex_lock(&p_cache->_lock);

    // iterate through each element in the cache
    for (size_t i = 0; i < p_cache->properties.count; i++)
    {

        // if the keys match...
        if ( p_cache->pfn_equality(p_cache->pfn_key_accessor(p_cache->properties.pp_data[i]), p_key) )
        {

            // store the result
            if ( pp_result )
                *pp_result = p_cache->properties.pp_data[i];

            // shift elements left
            for (size_t j = i; j < p_cache->properties.count - 1; j++)
                p_cache->properties.pp_data[j] = p_cache->properties.pp_data[j+1];

            // decrement count
            p_cache->properties.count--;
            
            // unlock
            mutex_unlock(&p_cache->_lock);
            
            // success
            return 1;
        }
    }

    // not found
    if ( pp_result )
        *pp_result = NULL;
            
    // unlock
    mutex_unlock(&p_cache->_lock);
    
    // error
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

int cache_map ( cache *const p_cache, fn_map *pfn_map, fn_allocator *pfn_allocator )
{

    // argument check
    if ( NULL == p_cache ) goto no_cache;
    if ( NULL == pfn_map ) goto no_fn_map;

    // lock
    mutex_lock(&p_cache->_lock);

    // state check
    if ( pfn_allocator ) goto map_with_allocator;

    // iterate through each element in the cache
    for (size_t i = 0; i < p_cache->properties.count; i++)

        // update
        p_cache->properties.pp_data[i] = pfn_map(p_cache->properties.pp_data[i]);

    done:
            
    // unlock
    mutex_unlock(&p_cache->_lock);
    
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
                pfn_allocator(p_old, 0);
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

    // lock
    mutex_lock(&p_cache->_lock);

    // iterate through the properties
    for (size_t i = 0; i < p_cache->properties.count; i++)
        pfn_fori(p_cache->properties.pp_data[i], i);    
            
    // unlock
    mutex_unlock(&p_cache->_lock);
    
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

    // lock
    mutex_lock(&p_cache->_lock);

    // iterate through the properties
    for (size_t i = 0; i < p_cache->properties.count; i++)
        pfn_foreach(p_cache->properties.pp_data[i]);    

    // unlock
    mutex_unlock(&p_cache->_lock);
    
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

int cache_pack ( void *p_buffer, cache *p_cache, fn_pack *pfn_element )
{
    
    // argument check
    if ( NULL ==    p_buffer ) goto no_buffer;
    if ( NULL ==     p_cache ) goto no_cache;
    if ( NULL == pfn_element ) goto no_pack;

    // initialized data 
    char *p = p_buffer;

    // lock
    mutex_lock(&p_cache->_lock);

    // pack the length
    p += pack_pack(p, "%2i64", 
        p_cache->properties.count,
        p_cache->properties.max
    );

    // iterate through the cache
    for (size_t i = p_cache->properties.count; i-- > 0; )
        p += pfn_element(p, p_cache->properties.pp_data[i]);
    
    // unlock
    mutex_unlock(&p_cache->_lock);
    
    // success
    return p - (char *)p_buffer;

    // error handling
    {
        
        // argument errors
        {
            no_buffer:
                #ifndef NDEBUG
                    log_error("[cache] Null pointer provided for \"p_buffer\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;

            no_cache:
                #ifndef NDEBUG
                    log_error("[cache] Null pointer provided for \"p_cache\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;

            no_pack:
                #ifndef NDEBUG
                    log_error("[cache] Null pointer provided for \"pfn_element\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
    }
}

int cache_unpack
(
    cache **pp_cache,
    void *p_buffer,
    fn_unpack *pfn_element,

    fn_equality      *pfn_equality,
    fn_key_accessor  *pfn_key_accessor,
    fn_allocator     *pfn_allocator
)
{
    
    // argument check    
    if ( NULL ==    pp_cache ) goto no_cache;
    if ( NULL ==    p_buffer ) goto no_buffer;
    if ( NULL == pfn_element ) goto no_unpack;

    // initialized data
    cache  *p_cache = NULL;
    char   *p       = p_buffer;
    size_t  count   = 0;
    size_t  max     = 0;

    // unpack the length
    p += pack_unpack(p, "%2i64",
        &count,
        &max
    );

    // construct an cache
    if ( 0 == cache_construct(&p_cache, max, pfn_equality, pfn_key_accessor, pfn_allocator) ) goto failed_to_construct_cache;

    // iterate through each element in the cache
    for (size_t i = 0; i < count; i++)
    {
        
		// initialized data
		void *p_element = NULL;

		// call the unpack function
		p += pfn_element(&p_element, p);
        
        // Add the element to the cache
        cache_insert(p_cache, p_element, NULL);
    }

    // return the cache to the caller
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

            no_buffer:
                #ifndef NDEBUG
                    log_error("[cache] Null pointer provided for \"p_buffer\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;

            no_unpack:
                #ifndef NDEBUG
                    log_error("[cache] Null pointer provided for \"pfn_element\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }

        // cache errors
        {
            failed_to_construct_cache:
                #ifndef NDEBUG
                    log_error("[cache] Failed to construct cache in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
    }
}

hash64 cache_hash ( cache *p_cache, fn_hash64 *pfn_element )
{

    // argument check
    if ( NULL == p_cache ) goto no_cache;

    // initialized data
    hash64     result     = 0;
    fn_hash64 *pfn_hash64 = (pfn_element) ? pfn_element : hash_crc64;

    // lock
    mutex_lock(&p_cache->_lock);

    // iterate through each element in the cache
    for (size_t i = 0; i < p_cache->properties.count; i++)
        result ^= pfn_hash64(p_cache->properties.pp_data[i], sizeof(void *));

    // unlock
    mutex_unlock(&p_cache->_lock);

    // success
    return result;

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

int cache_destroy ( cache **const pp_cache )
{

    // argument check
    if ( NULL == pp_cache ) goto no_cache; 

    // initialized data
    cache *p_cache = *pp_cache;
    fn_allocator *pfn_allocator = NULL;

    // lock
    mutex_lock(&p_cache->_lock);

    // no more pointer for caller
    *pp_cache = NULL;
    
    // unlock
    mutex_unlock(&p_cache->_lock);
    
    // store the allocator provided in the constructor
    pfn_allocator = p_cache->pfn_allocator;

    // release contents
    if ( pfn_allocator )
        for (size_t i = 0; i < p_cache->properties.count; i++)
            pfn_allocator(p_cache->properties.pp_data[i], 0);

    // release the cache contents
    p_cache->properties.pp_data = default_allocator(p_cache->properties.pp_data, 0);

    // destroy the lock
    mutex_destroy(&p_cache->_lock);

    // release the cache
    p_cache = default_allocator(p_cache, 0);

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
    }
}
