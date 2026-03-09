/** !
 * dictionary implementation
 *
 * @file src/data/dict/dict.h
 *
 * @author Jacob Smith
 */

// header file
#include <data/dict.h>

// structure declarations
struct dict_item_s;

// type definitions
typedef struct dict_item_s dict_item;

// structure definitions
struct dict_item_s
{
    void      *value; 
    dict_item *next;  
};

struct dict_s
{
    size_t      max, count; // quantity of slots and  elements
    dict_item **data;       // contents
    mutex       _lock;      // lock

    fn_allocator    *pfn_allocator;     // pointer to allocator function
    fn_key_accessor *pfn_key_accessor;  // pointer to key accessor function 
    fn_hash64       *pfn_hash_function; // pointer to hash function
};

int dict_construct
(
    dict **const pp_dict,
    size_t size,

    fn_allocator    *pfn_allocator,
    fn_key_accessor *pfn_key_accessor,
    fn_hash64       *pfn_hash64
)
{

    // argument check
    if ( NULL == pp_dict ) goto no_dict;
    if ( 0    ==    size ) goto zero_size;

    // initialized data
    dict *p_dict = NULL;

    // allocate a dictionary
    p_dict = default_allocator(p_dict, sizeof(dict));
    if ( NULL == p_dict ) goto no_mem;

    // populate fields
    *p_dict = (dict)
    {
        .max   = size, .count = 0,
        .data  = NULL,
        ._lock = (mutex) { 0 },

        .pfn_allocator     = pfn_allocator,
        .pfn_key_accessor  = pfn_key_accessor ? pfn_key_accessor : default_key_accessor,
        .pfn_hash_function = pfn_hash64       ? pfn_hash64       : hash_crc64,
    };
    
    // allocate properties
    p_dict->data = default_allocator(0, size * sizeof(dict_item *));
    if ( NULL == p_dict->data ) 
    {
        
        // release the dictionary
        default_allocator(p_dict, 0);

        // error
        goto no_mem;
    }

    // zero the slots
    memset(p_dict->data, 0, size * sizeof(dict_item *));

    // create a lock
    if ( mutex_create(&p_dict->_lock) == 0 ) goto failed_to_create_mutex;

    // return a pointer to the caller
    *pp_dict = p_dict;

    // success
    return 1;

    // error handling
    {

        // argument errors
        {
            no_dict:
                #ifndef NDEBUG
                    log_error("[dict] Null pointer provided for parameter \"pp_dict\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;

            zero_size:
                #ifndef NDEBUG
                    log_error("[dict] Zero provided for parameter \"size\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;

        }

        // dictionary errors
        {
            failed_to_create_mutex:
                #ifndef NDEBUG
                    log_error("[dict] Failed to create mutex in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // release the slots
                default_allocator(p_dict->data, 0),

                // release the dictionary
                default_allocator(p_dict, 0);

                // error
                return 0;
        }

        // standard library errors
        {
            no_mem:
                #ifndef NDEBUG
                    log_error("[standard library] Failed to allocate memory in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
    }
}

int dict_get ( dict *const p_dict, const char *const p_key, void **pp_value )
{
    
    // argument check
    if ( NULL == p_dict ) goto no_dict;
    if ( NULL ==  p_key ) goto no_value;

    // initialized data
    hash64     h      = 0;
    dict_item *p_item = NULL;

    // lock
    mutex_lock(&p_dict->_lock);

    // compute the hash of the key
    h = p_dict->pfn_hash_function(p_key, strlen(p_key)),

    // store the first item in the chain
    p_item = p_dict->data[h % p_dict->max];

    // walk the chain
    for (; NULL != p_item; p_item = p_item->next)
    {

        // initialized data
        const char *p_item_key = p_dict->pfn_key_accessor(p_item->value);
        
        // skip
        if ( strcmp(p_key, p_item_key) ) continue;

        // return a pointer to the caller
        if ( pp_value )
            *pp_value = p_item->value;

        // unlock
        mutex_unlock(&p_dict->_lock);

        // success
        return 1;
    }
    
    // unlock
    mutex_unlock(&p_dict->_lock);

    // error
    return 0;

    // error handling
    {

        // argument errors
        {
            no_dict:
                #ifndef NDEBUG
                    log_error("[dict] Null pointer provided for parameter \"p_dict\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;

            no_value:
                #ifndef NDEBUG
                    log_error("[dict] Null pointer provided for parameter \"p_value\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
    }
}

int dict_values ( dict *const p_dict, void *pp_values[], size_t limit )
{

    // argument check
    if ( NULL ==    p_dict ) goto no_dict;
    if ( NULL == pp_values ) goto no_values;

    // initialized data
    size_t i = 0;
    
    // lock
    mutex_lock(&p_dict->_lock);

    // iterate over dictionary items
    for (size_t j = 0; j < p_dict->max; j++)
    {

        // initialized data
        dict_item *p_item = p_dict->data[j];

        // walk the chain
        while ( p_item )
        {

            // store the value
            pp_values[i] = p_item->value;

            // step
            p_item = p_item->next, i++;

            // bail
            if ( i >= limit ) goto done;
        }
    }

    done:

    // unlock
    mutex_unlock(&p_dict->_lock);

    // success
    return 1;

    // error handling
    {

        // argument errors
        {
            no_dict:
                #ifndef NDEBUG
                    log_error("[dict] Null pointer provided for parameter \"p_dict\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;

            no_values:
                #ifndef NDEBUG
                    log_error("[dict] Null pointer provided for parameter \"pp_values\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
    }
}

int dict_size ( dict *const p_dict, size_t *p_result )
{

    // argument check
    if ( p_dict   == NULL ) goto no_dict;
    if ( p_result == NULL ) goto no_result;

    // lock
    mutex_lock(&p_dict->_lock);

    // return a pointer to the caller
    *p_result = p_dict->count;

    // unlock
    mutex_unlock(&p_dict->_lock);

    // success
    return 1;

    // error handling
    {

        // argument errors
        {
            no_dict:
                #ifndef NDEBUG
                    log_error("[dict] Null pointer provided for parameter \"p_dict\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;

            no_result:
                #ifndef NDEBUG
                    log_error("[dict] Null pointer provided for parameter \"p_result\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
    }
}

int dict_add ( dict *const p_dict, const void *const p_value )
{

    // argument check
    if ( NULL ==  p_dict ) goto no_dict;
    if ( NULL == p_value ) goto no_value;

    // initialized data
    hash64      h      = 0;
    const char *p_key  = NULL;
    dict_item  *p_item = NULL;

    // lock
    mutex_lock(&p_dict->_lock);

    // store the key
    p_key = p_dict->pfn_key_accessor(p_value),

    // compute the hash of the key
    h = p_dict->pfn_hash_function(p_key, strlen(p_key)),

    // store the first item in the chain
    p_item = p_dict->data[h % p_dict->max];

    // walk the chain
    for (; NULL != p_item; p_item = p_item->next)
    {

        // initialized data
        const char *p_item_key = p_dict->pfn_key_accessor(p_item->value);
        
        // stop if key exists
        if ( 0 == strcmp(p_key, p_item_key) ) break;
    }

    // existing property?
    if ( p_item )
    {

        // initialized data
        void *p_old = p_item->value;

        // same pointer?
        if ( p_old == p_value ) goto done;

        // update the value
        p_item->value = (void *) p_value;

        // release the old value
        if ( p_dict->pfn_allocator )
            p_dict->pfn_allocator(p_old, 0);
        
        // done
        goto done;
    }

    // allocate a new item
    p_item = default_allocator(0, sizeof(dict_item));
    if ( NULL == p_item ) goto no_mem;

    // zero set
    memset(p_item, 0, sizeof(dict_item));

    // set the value
    p_item->value = (void *) p_value;

    // repair the list
    p_item->next = p_dict->data[(h % p_dict->max)];
    
    // insert the item
    p_dict->data[(h % p_dict->max)] = p_item;

    // increment the entry counter
    p_dict->count++;
    
    done:

    // unlock
    mutex_unlock(&p_dict->_lock);

    // success
    return 1;

    // error handling
    {

        // argument errors
        {
            no_dict:
                #ifndef NDEBUG
                    log_error("[dict] Null pointer provided for parameter \"p_dict\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;

            no_value:
                #ifndef NDEBUG
                    log_error("[dict] Null pointer provided for parameter \"p_value\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }

        // standard library errors
        {
            no_mem:
                #ifndef NDEBUG
                    log_error("[standard library] Failed to allocate memory in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // unlock
                mutex_unlock(&p_dict->_lock);

                // error
                return 0;
        }
    }
}

int dict_pop ( dict *const p_dict, const char *const p_key, const void **const pp_value )
{

    // argument check
    if ( NULL == p_dict ) goto no_dict;
    if ( NULL ==  p_key ) goto no_key;

    // initialized data
    hash64      h           = 0;
    const char *p_item_key  = NULL;
    dict_item  *p_item      = NULL;
    dict_item  *p_last = NULL;

    // lock
    mutex_lock(&p_dict->_lock);

    // compute the hash of the key
    h = p_dict->pfn_hash_function(p_key, strlen(p_key)),

    // store the first item in the chain
    p_item = p_dict->data[h % p_dict->max];

    // walk the chain
    for (; NULL != p_item; p_item = p_item->next)
    {

        // store the key of the item
        p_item_key = p_dict->pfn_key_accessor(p_item->value);
        
        // stop if key exists
        if ( 0 == strcmp(p_key, p_item_key) )
        {

            // initialized data
            dict_item *p_next = p_item->next;

            // repair the list
            if ( p_last )
                p_last->next = p_next;
            else
                p_dict->data[h % p_dict->max] = p_next;

            // decrement the entry counter
            p_dict->count--;

            // return a pointer to the caller
            if ( pp_value )
                *pp_value = p_item->value;

            // release the item
            default_allocator(p_item, 0);

            // unlock
            mutex_unlock(&p_dict->_lock);

            // success
            return 1;
        }

        // save last
        p_last = p_item;
    }

    // unlock
    mutex_unlock(&p_dict->_lock);
    
    // error
    return 0;

    // error handling
    {

        // argument errors
        {
            no_dict:
                #ifndef NDEBUG
                    log_error("[dict] Null pointer provided for parameter \"p_dict\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;

            no_key:
                #ifndef NDEBUG
                    log_error("[dict] Null pointer provided for parameter \"p_key\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
    }
}

int dict_foreach ( dict *const p_dict, fn_foreach *pfn_foreach )
{

    // argument check
    if ( NULL ==      p_dict ) goto no_dict;
    if ( NULL == pfn_foreach ) goto no_foreach;

    // lock
    mutex_lock(&p_dict->_lock);

    // iterate over dictionary items
    for (size_t i = 0; i < p_dict->max; i++)
    {

        // initialized data
        dict_item *p_item = p_dict->data[i];

        // walk the chain
        while ( p_item )
        {

            // call the foreach function
            pfn_foreach(p_item->value);

            // step
            p_item = p_item->next;
        }
    }

    // unlock
    mutex_unlock(&p_dict->_lock);

    // success
    return 1;

    // error handling
    {

        // argument errors
        {
            no_dict:
                #ifndef NDEBUG
                    log_error("[dict] Null pointer provided for parameter \"p_dict\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;

            no_foreach:
                #ifndef NDEBUG
                    log_error("[dict] Null pointer provided for parameter \"pfn_foreach\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
    }
}

int dict_pack ( void *const p_buffer, dict *const p_dict, fn_pack *pfn_element )
{

    // argument check
    if ( NULL ==    p_buffer ) goto no_buffer;
    if ( NULL ==      p_dict ) goto no_dict;
    if ( NULL == pfn_element ) goto no_pack;

    // initialized data
    char *p = p_buffer;

    // lock
    mutex_lock(&p_dict->_lock);

    // pack the size and count
    p += pack_pack(p, "%2i64",
        p_dict->max,
        p_dict->count
    );

    // iterate over dictionary items
    for (size_t i = 0; i < p_dict->max; i++)
    {

        // initialized data
        dict_item *p_item = p_dict->data[i];

        // walk the chain
        while ( p_item )
        {

            // call the pack function
            p += pfn_element(p, p_item->value);

            // step
            p_item = p_item->next;
        }
    }

    // unlock
    mutex_unlock(&p_dict->_lock);

    // success
    return p - (char *)p_buffer;

    // error handling
    {

        // argument errors
        {
            no_buffer:
                #ifndef NDEBUG
                    log_error("[dict] Null pointer provided for parameter \"p_buffer\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;

            no_dict:
                #ifndef NDEBUG
                    log_error("[dict] Null pointer provided for parameter \"p_dict\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;

            no_pack:
                #ifndef NDEBUG
                    log_error("[dict] Null pointer provided for parameter \"pfn_element\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
    }
}

int dict_unpack
(
    dict      **pp_dict,
    void       *p_buffer,
    fn_unpack  *pfn_element,

    fn_allocator    *pfn_allocator,
    fn_key_accessor *pfn_key_accessor,
    fn_hash64       *pfn_hash64
)
{

    // argument check
    if ( NULL ==     pp_dict ) goto no_dict;
    if ( NULL ==    p_buffer ) goto no_buffer;
    if ( NULL == pfn_element ) goto no_unpack;
    
    // initialized data
    dict   *p_dict = NULL;
    char   *p      = p_buffer;
    size_t  size   = 0,
            count  = 0;
    int     result = 0;

    // unpack the size and count
    p += pack_unpack(p, "%2i64",
        &size,
        &count
    );

    // construct a dictionary
    result = dict_construct
    (
        &p_dict,
        size,

        pfn_allocator,
        pfn_key_accessor,
        pfn_hash64
    );
    if ( 0 == result ) goto failed_to_construct_dictionary;
    
    // iterate through values
    for (size_t i = 0; i < count; i++)
    {
        
        // initialized data
        void *p_value = NULL;

        // parse the value
        p += pfn_element(&p_value, p);

        // add the value to the dictionary
        dict_add(p_dict, p_value);
    }

    // return a pointer to the caller
    *pp_dict = p_dict;

    // success
    return p - (char *)p_buffer;

    // error handling
    {

        // argument errors
        {
            no_buffer:
                #ifndef NDEBUG
                    log_error("[dict] Null pointer provided for parameter \"p_buffer\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;

            no_dict:
                #ifndef NDEBUG
                    log_error("[dict] Null pointer provided for parameter \"p_dict\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;

            no_unpack:
                #ifndef NDEBUG
                    log_error("[dict] Null pointer provided for parameter \"pfn_element\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }

        // dictionary errors
        {
            failed_to_construct_dictionary:
                #ifndef NDEBUG
                    log_error("[dict] Failed to create dictionary in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
    }
}

hash64 dict_hash ( dict *const p_dict, fn_hash64 *pfn_element )
{

    // argument check
    if ( NULL ==      p_dict ) goto no_dict;
    if ( NULL == pfn_element ) goto no_hash;

    // initialized data
    hash64 result = 0;

    // lock
    mutex_lock(&p_dict->_lock);

    // iterate over dictionary items
    for (size_t i = 0; i < p_dict->max; i++)
    {

        // initialized data
        dict_item *p_item = p_dict->data[i];

        // walk the chain
        while ( p_item )
        {

            // call the pack function
            result ^= pfn_element(p_item->value, sizeof(p_item->value));

            // step
            p_item = p_item->next;
        }
    }

    // unlock
    mutex_unlock(&p_dict->_lock);

    // success
    return result;

    // error handling
    {

        // argument errors
        {
            no_dict:
                #ifndef NDEBUG
                    log_error("[dict] Null pointer provided for parameter \"p_dict\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;

            no_hash:
                #ifndef NDEBUG
                    log_error("[dict] Null pointer provided for parameter \"pfn_element\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
    }
}

int dict_destroy ( dict **const pp_dict, fn_allocator *pfn_allocator )
{

    // argument check
    if ( NULL == pp_dict ) goto no_dict;

    // initialized data
    dict *p_dict = *pp_dict;

    // lock
    mutex_lock(&p_dict->_lock);

    // no more pointer for caller
    *pp_dict = NULL;

    // unlock
    mutex_unlock(&p_dict->_lock);

    // iterate over dictionary items
    for (size_t i = 0; i < p_dict->max; i++)
    {

        // initialized data
        dict_item *p_item = p_dict->data[i];

        // walk the chain
        while ( p_item )
        {

            // initialized data
            dict_item *p_next = p_item->next;

            // call the allocator function
            if ( pfn_allocator )
                pfn_allocator(p_item->value, 0);

            // release the item
            default_allocator(p_item, 0);

            // step
            p_item = p_next;
        }
    }

    // release dictionary items
    default_allocator(p_dict->data, 0);

    // destroy lock
    mutex_destroy(&p_dict->_lock);

    // release the dictionary
    default_allocator(p_dict, 0);

    // success
    return 1;

    // error handling
    {

        // argument errors
        {
            no_dict:
                #ifndef NDEBUG
                    log_error("[dict] Null pointer provided for parameter \"pp_dict\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
    }
}
