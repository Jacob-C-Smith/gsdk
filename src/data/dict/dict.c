/** !
 * dict library
 *
 * @file dict.c
 *
 * @author Jacob Smith
 */

// headers
#include <data/dict.h>

// core
#include <core/log.h>
#include <core/hash.h>

// reflection
#include <core/pack.h>

// Internal type definitions
typedef struct dict_item_s dict_item;

// structure definitions
struct dict_item_s
{
    char   *key;   // The key
    void   *value; // The value
    size_t  index; // The index in the iterable lists

    dict_item *next; // The next entry
};

struct dict_s
{

    struct
    {
        dict_item **data;  // Hash table contents
        size_t      max,   // Hash table elements
                    count; // Entries
    } entries;

    struct
    {
        char   **keys;   // Iterable keys
        void   **values; // Iterable values
        size_t   max;    // Iterable dict bound
    } iterable;

    fn_hash64 *pfn_hash_function; // Pointer to the hash function

    mutex _lock; // locked when writing values
};

int dict_create ( dict **const pp_dict )
{

    // argument check
    if ( pp_dict == (void *) 0 ) goto no_dictionary;

    // Allocate memory for a dictionary
    dict *p_dict = default_allocator(0, sizeof(dict));

    // error checking
    if ( p_dict == (void *) 0 ) goto no_mem;

    // Zero set
    memset(p_dict, 0, sizeof(dict));

    // Return the allocated memory
    *pp_dict = p_dict;

    // success
    return 1;

    // error handling
    {

        // argument errors
        {
            no_dictionary:
                #ifndef NDEBUG
                    log_error("[dict] Null pointer provided for parameter \"pp_dict\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }

        // standard library errors
        {
            no_mem:
                #ifndef NDEBUG
                    log_error("[Standard Library] Failed to allocate memory in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
    }
}

int dict_construct ( dict **const pp_dict, size_t size, fn_hash64 pfn_hash_function )
{

    // argument check
    if ( pp_dict == (void *) 0 ) goto no_dictionary;
    if ( size    ==          0 ) goto zero_size;

    // initialized data
    dict *p_dict = 0;

    // Allocate a dictionary
    if ( dict_create(pp_dict) == 0 ) goto failed_to_create_dict;

    // Get a pointer to the allocated dictionary
    p_dict = *pp_dict;

    // Set the count and iterator max
    p_dict->entries.max  = size;
    p_dict->iterable.max = 1;

    // Allocate "size" number of properties
    p_dict->entries.data = default_allocator(0, size * sizeof(dict_item *));

    // Allocate key and value lists
    p_dict->iterable.keys   = default_allocator(0, sizeof(char *));
    p_dict->iterable.values = default_allocator(0, sizeof(void *));

    // Zero set the allocated memory
    memset(p_dict->entries.data, 0, size * sizeof(dict_item *));
    memset(p_dict->iterable.keys, 0, sizeof(char *));
    memset(p_dict->iterable.values, 0, sizeof(char *));

    // Create a mutex
    if ( mutex_create(&p_dict->_lock) == 0 ) goto failed_to_create_mutex;

    // Set the hash function
    if ( pfn_hash_function ) 
        p_dict->pfn_hash_function = pfn_hash_function;

    // Default
    else
        p_dict->pfn_hash_function = hash_crc64;

    // error checking
    if ( p_dict->entries.data    == (void *) 0 ) goto no_mem;
    if ( p_dict->iterable.keys   == (void *) 0 ) goto no_mem;
    if ( p_dict->iterable.values == (void *) 0 ) goto no_mem;

    // success
    return 1;

    // error handling
    {

        // argument errors
        {
            no_dictionary:
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

        // Dictionary errors
        {
            failed_to_create_dict:
                #ifndef NDEBUG
                    log_error("[dict] Failed to create dictionary in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;

            failed_to_create_mutex:
                #ifndef NDEBUG
                    log_error("[dict] Failed to create mutex in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }

        // standard library errors
        {
            no_mem:
                #ifndef NDEBUG
                    log_error("[Standard Library] Failed to allocate memory in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
    }
}

int dict_from_keys ( dict **const pp_dict, const char **const keys, size_t size )
{

    // argument check
    if ( pp_dict == (void *) 0 ) goto no_dictionary;
    if ( keys    == (void *) 0 ) goto no_keys;

    // TODO: replace with a goto
    if ( size == 0 ) return 0;

    // initialized data
    dict *p_dict = 0;

    // Allocate a dictionary
    if ( dict_construct(&p_dict, size, 0) == 0 ) goto failed_to_construct_dict;

    // Iterate over each key
    for (size_t i = 0; keys[i]; i++)

        // Add the key to the dictionary
        dict_add(p_dict, keys[i], (void *)0);

    // Return
    *pp_dict = p_dict;

    // success
    return 1;

    // error handling
    {

        // argument errors
        {
            no_dictionary:
                #ifndef NDEBUG
                    log_error("[dict] Null pointer provided for parameter \"pp_dict\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;

            no_keys:
                #ifndef NDEBUG
                    log_error("[dict] Null pointer provided for parameter \"keys\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;

        }

        // dict errors
        {
            failed_to_construct_dict:
                #ifndef NDEBUG
                    log_error("[dict] Call to \"dict_construct\" returned an erroneous value in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
    }
}

const void *dict_get ( dict *const p_dict, const char *const key )
{

    // argument check
    if ( p_dict == (void *) 0 ) goto no_dictionary;
    if ( key    == (void *) 0 ) goto no_name;

    // lock
    mutex_lock(&p_dict->_lock);

    // initialized data
    dict_item *ret = p_dict->entries.data[p_dict->pfn_hash_function(key, strlen(key)) % p_dict->entries.max];
    void      *val = 0;

    // Walk the list
    while ( ret != 0 )
    {

        // Is this the right entry?
        if ( strcmp(key, ret->key) == 0 )
            break;

        // Iterate
        ret = ret->next;
    }

    // If the walk yielded a property, extract the value of the property, else value = 0
    val = (ret) ? ret->value : (void *)0;

    // unlock
    mutex_unlock(&p_dict->_lock);

    // Return the value if it exists, otherwise null pointer
    return val;

    // error handling
    {

        // argument errors
        {
            no_dictionary:
                #ifndef NDEBUG
                    log_error("[dict] Null pointer provided for parameter \"p_dict\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;

            no_name:
                #ifndef NDEBUG
                    log_error("[dict] Null pointer provided for parameter \"key\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
    }
}

size_t dict_values ( dict *const p_dict, void **const values )
{

    // argument check
    if ( p_dict == (void *) 0 ) goto no_dictioanry;

    // lock
    mutex_lock(&p_dict->_lock);

    // initialized data
    size_t entry_count = p_dict->entries.count;

    // error check
    if (values == 0)
    {

        // unlock
        mutex_unlock(&p_dict->_lock);

        // success
        return entry_count;
    }

    // Copy memory
    memcpy(values, p_dict->iterable.values, entry_count * sizeof(void *));

    // unlock
    mutex_unlock(&p_dict->_lock);

    // success
    return 1;

    // error handling
    {

        // argument errors
        {
            no_dictioanry:
                #ifndef NDEBUG
                    log_error("[dict] Null pointer provided for parameter \"p_dict\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
    }
}

size_t dict_keys ( dict *const p_dict, const char **const keys )
{

    // argument check
    if ( p_dict == (void *) 0 ) goto no_dictioanry;

    // lock
    mutex_lock(&p_dict->_lock);

    // initialized data
    size_t entry_count = p_dict->entries.count;

    // Counting branch?
    if ( keys == 0 )
    {

        // unlock
        mutex_unlock(&p_dict->_lock);

        // Return
        return entry_count;
    }

    // Copy memory
    memcpy(keys, p_dict->iterable.keys, entry_count * sizeof(char *));

    // unlock
    mutex_unlock(&p_dict->_lock);

    // success
    return 1;

    // error handling
    {

        // argument errors
        {
            no_dictioanry:
                #ifndef NDEBUG
                    log_error("[dict] Null pointer provided for parameter \"p_dict\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }

    }
}

int dict_add ( dict *const p_dict, const char *const key,   void * const p_value )
{

    // argument check
    if ( p_dict == (void *) 0 ) goto no_dictionary;
    if ( key    == (void *) 0 ) goto no_name;

    // lock
    mutex_lock(&p_dict->_lock);

    // initialized data
    unsigned long long  h        = p_dict->pfn_hash_function((void *)key, strlen(key));
    dict_item          *property = p_dict->entries.data[h % p_dict->entries.max];

    // Find the key in the hash table

    // Walk the linked list
    for (; property != 0; property = property->next)

        // Is this the correct entry?
        if ( strcmp(key, property->key) == 0 )

            // Stop looking
            break;

    // Make a new property
    if ( property == (void *) 0 )
    {

        // Resize iterable max?
        if ( p_dict->entries.count >= p_dict->iterable.max )
        {

            // Double the size
            p_dict->iterable.max *= 2;

            // Reallocate iterable dicts
            p_dict->iterable.keys   = default_allocator(p_dict->iterable.keys  , p_dict->iterable.max * sizeof(char *));
            p_dict->iterable.values = default_allocator(p_dict->iterable.values, p_dict->iterable.max * sizeof(void *));

            // error checking
            if ( p_dict->iterable.keys   == (void *) 0 ) goto no_mem;
            if ( p_dict->iterable.values == (void *) 0 ) goto no_mem;
        }

        // Allocate a new dict_item
        property = default_allocator(0, sizeof(dict_item));

        // Zero
        memset(property, 0, sizeof(dict_item));

        // error checking
        if ( property == (void *) 0 ) goto no_mem;

        // Set the property
        property->key = (char *) key;
        property->value = p_value;

        // Insert the hash
        property->next = p_dict->entries.data[(h % p_dict->entries.max)];
        property->index = p_dict->entries.count;
        p_dict->entries.data[(h % p_dict->entries.max)] = property;

        // Update the iterables
        p_dict->iterable.keys[p_dict->entries.count] = (char *) key;
        p_dict->iterable.values[p_dict->entries.count] = p_value;

        // Increment the entry counter
        p_dict->entries.count++;
    }

    // Update an existing property
    else
    {

        // Update the property
        property->value = p_value;

        // Update the iterable
        p_dict->iterable.values[property->index] = property->value;
    }

    // unlock
    mutex_unlock(&p_dict->_lock);

    // success
    return 1;

    // error handling
    {

        // argument errors
        {
            no_dictionary:
                #ifndef NDEBUG
                    log_error("[dict] Null pointer provided for parameter \"p_dict\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;

            no_name:
                #ifndef NDEBUG
                    log_error("[dict] Null pointer provided for parameter \"key\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }

        // standard library errors
        {
            no_mem:
                #ifndef NDEBUG
                    log_error("[Standard library] Failed to allocate memory in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // unlock
                mutex_unlock(&p_dict->_lock);

                // error
                return 0;
        }
    }
}

int dict_pop ( dict *const p_dict, const char *const key, const void **const pp_value )
{

    // argument check
    if ( p_dict == (void *) 0 ) goto no_dictionary;
    if ( key    == (void *) 0 ) goto no_name;

    // lock
    mutex_lock(&p_dict->_lock);

    // initialized data
    unsigned long long  h = p_dict->pfn_hash_function(key, strlen(key));
    dict_item          *i = p_dict->entries.data[h % p_dict->entries.max],
                       *k = 0;

    // error check
    if ( i == 0 ) goto no_item;

    // Check the head
    if ( strcmp(key, i->key) == 0 )
    {

        // initialized data
        dict_item *j = i->next;

        // Get a dict item
        i = p_dict->entries.data[h % p_dict->entries.max];

        p_dict->entries.data[h % p_dict->entries.max] = j;

        goto free_item;
    }

    while (i->next)
    {

        if (strcmp(key, i->next->key) == 0)
        {
            dict_item *j = i->next->next;
            k=i->next;
            // Stitch up the linked list
            i->next = j;
            goto free_item;
        }

        i = i->next;
    }

    goto no_item;

    free_item:

    if ( strcmp(key, i->key) == 0 )
        k=i;

    // Return the value
    if (pp_value)
        *pp_value = k->value;

    // Clean up the iterables
    {

        // initialized data
        size_t              idx       = k->index;
        char               *swap_key  = p_dict->iterable.keys[p_dict->entries.count-1];
        unsigned long long  swap_hash = p_dict->pfn_hash_function(swap_key, strlen(swap_key));
        dict_item          *swap_item = p_dict->entries.data[swap_hash % p_dict->entries.max];

        if ( swap_key == (void *) 0 ) goto no_swap_key;

        if (idx == p_dict->entries.count-1)
        {
            p_dict->iterable.keys[p_dict->entries.count-1] = 0;
            p_dict->iterable.values[p_dict->entries.count-1] = 0;
            goto done;
        }

        // Clean up key iterable
        p_dict->iterable.keys[idx] = p_dict->iterable.keys[p_dict->entries.count-1];
        p_dict->iterable.keys[p_dict->entries.count-1] = 0;

        // Clean up value iterable
        p_dict->iterable.values[idx] = p_dict->iterable.values[p_dict->entries.count-1];
        p_dict->iterable.values[p_dict->entries.count-1] = 0;

        if ( swap_item )
            swap_item->index = idx;
    }

    done:

    // Free the pop'd dict_item
    if ( default_allocator(k, 0) ) goto failed_to_free;

    // Decrement entries
    p_dict->entries.count--;

    // Resize iterable max?
    if ( p_dict->entries.count > p_dict->iterable.max / 2)
    {

        // Double the size
        p_dict->iterable.max /= 2;

        // Reallocate iterable dicts
        p_dict->iterable.keys   = default_allocator(p_dict->iterable.keys  , p_dict->iterable.max * sizeof(void *));
        p_dict->iterable.values = default_allocator(p_dict->iterable.values, p_dict->iterable.max * sizeof(void *));

        // error checking
        if ( p_dict->iterable.keys == (void *) 0 ) goto no_mem;
        if ( p_dict->iterable.keys == (void *) 0 ) goto no_mem;
    }

    // unlock
    mutex_unlock(&p_dict->_lock);

    // success
    return 1;

    no_swap_key:
    return 0;

    // error handling
    {

        // argument error
        {
            no_dictionary:
                #ifndef NDEBUG
                    log_error("[dict] Null pointer provided for parameter \"p_dict\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;

            no_name:
                #ifndef NDEBUG
                    log_error("[dict] Null pointer provided for parameter \"key\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }

        // Failed to find item
        {
            no_item:
                #ifndef NDEBUG
                    log_warning("[dict] Failed to find property from key \"%s\" in call to function \"%s\"\n", key, __FUNCTION__);
                #endif

                // unlock
                mutex_unlock(&p_dict->_lock);

                // error
                return 0;
        }

        // standard library errors
        {
            no_mem:
                #ifndef NDEBUG
                    log_warning("[Standard library] Failed to allocate memory in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // unlock
                mutex_unlock(&p_dict->_lock);

                // error
                return 0;
                
            failed_to_free:
                #ifndef NDEBUG
                    printf("[Standard Library] Call to \"default_allocator\" returned an erroneous value in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
    }
}

int dict_foreach ( dict *const p_dict, void (*function)(const void * const, size_t) )
{

    // argument check
    if ( p_dict                == (void *) 0 ) goto no_dictionary;
    if ( function              == (void *) 0 ) goto no_function;
    if ( p_dict->entries.count ==          0 ) return 1;

    // lock
    mutex_lock(&p_dict->_lock);

    // Iterate over each hash table item
    for (size_t i = 0; i < p_dict->entries.count; i++)

        // Call the function on the item
        function(p_dict->iterable.values[i], i);

    // unlock
    mutex_unlock(&p_dict->_lock);

    // success
    return 1;

    // error handling
    {

        // argument errors
        {
            no_dictionary:
                #ifndef NDEBUG
                    log_warning("[dict] Null pointer provided for parameter \"p_dict\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;

            no_function:
                #ifndef NDEBUG
                    log_warning("[dict] Null pointer provided for parameter \"function\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
    }
}

int dict_copy ( dict *const p_dict, dict **const pp_dict )
{

    // argument check
    if ( p_dict  == (void *) 0 ) goto no_dictionary;
    if ( pp_dict == (void *) 0 ) goto no_target;

    // initialized data
    dict  *i_dict = 0;
    const char **keys   = default_allocator(0, p_dict->entries.max * sizeof(char *));
    void **values = default_allocator(0, p_dict->entries.max * sizeof(void *));

    // error checking
    if ( keys   == (void *) 0 ) goto no_mem;
    if ( values == (void *) 0 ) goto no_mem;

    // Zero set
    memset(keys  , 0, p_dict->entries.max * sizeof(char *));
    memset(values, 0, p_dict->entries.max * sizeof(char *));

    // Construct a new dictionary of the same size
    dict_construct(pp_dict, p_dict->entries.max, 0);

    // Get the keys and values out of the first dictionary
    dict_keys(p_dict, keys);
    dict_values(p_dict, values);

    // Iterate over each value
    for (size_t i = 0; i < p_dict->entries.count && keys[i]; i++)

        // Insert each value
        dict_add(*pp_dict, keys[i], values[i]);

    // Free the lists
    // Free the keys
    if ( default_allocator(keys, 0) ) goto failed_to_free;

    // Free the values
    if ( default_allocator(values, 0) ) goto failed_to_free;

    *pp_dict = i_dict;

    // success
    return 1;

    // error handling
    {

        // argument errors
        {
            no_dictionary:
                #ifndef NDEBUG
                    log_warning("[dict] Null pointer provided for parameter \"p_dict\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
            no_target:
                #ifndef NDEBUG
                    log_warning("[dict] Null pointer provided for parameter \"pp_dict\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }

        // standard library errors
        {
            no_mem:
                #ifndef NDEBUG
                    log_warning("[Standard Library] Failed to allocate memory in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // unlock
                mutex_unlock(&p_dict->_lock);

                // error
                return 0;
        }

        // standard library errors
        {
            failed_to_free:
                #ifndef NDEBUG
                    printf("[Standard Library] Call to \"default_allocator\" returned an erroneous value in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
    }
}
int dict_pack ( dict *p_dict, void *p_buffer, fn_pack *pfn_element )
{
    
    // argument check
    if ( p_dict      == (void *) 0 ) goto no_dict;
    if ( p_buffer    == (void *) 0 ) return 0;
    if ( pfn_element == (void *) 0 ) return 0;

    // initialized data 
    char *p = p_buffer;

    // lock
    mutex_lock(&p_dict->_lock);

    // Pack the quantity of properties
    p += pack_pack(p, "%i64", p_dict->entries.count);

    // Iterate through the dict
    for (size_t i = 0; i < p_dict->entries.max; i++)
    {

        // initialized data
        dict_item *p_dict_item = p_dict->entries.data[i];

        // walk the list
        while (p_dict_item)
        {
            
            // pack the property
            p += pack_pack(p, "%s", p_dict_item->key),
            p += pfn_element(p, p_dict_item->value);
            
            // step
            p_dict_item = p_dict_item->next;
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
            no_dict:
                #ifndef NDEBUG
                    log_error("[dict] Null pointer provided for \"p_dict\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
    }
}

int dict_unpack ( dict **pp_dict, void *p_buffer, fn_unpack *pfn_element )
{
    
    // argument check
    if ( pp_dict    == (void *) 0 ) goto no_dict;
    if ( pfn_element == (void *) 0 ) return 0;

    // initialized data
    dict *p_dict = NULL;
    char *p = p_buffer;
    size_t len = 0;

    // Unpack the length
    p += pack_unpack(p, "%i64", &len);

    // Construct an dict
    dict_construct(&p_dict, len, 0);

    // Return the dict to the caller
    *pp_dict = p_dict;

    // success
    return 1;
    
    // error handling
    {
        
        // argument errors
        {
            no_dict:
                #ifndef NDEBUG
                    log_error("[dict] Null pointer provided for \"p_dict\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
    }
}

int dict_clear ( dict *const p_dict )
{

    // argument check
    if ( p_dict                == (void *) 0 ) goto no_dictionary;
    if ( p_dict->entries.count ==          0 ) return 1;

    // lock
    mutex_lock(&p_dict->_lock);

    // Iterate over each hash table item
    for (size_t i = 0; i < p_dict->entries.max; i++)
    {

        // Is there a valid entry at the index?
        if (p_dict->entries.data[i])
        {

            // initialized data
            dict_item *i_di = p_dict->entries.data[i];

            // Iterate through linked list
            while ( i_di )
            {

                // initialized data
                dict_item *n = i_di->next;

                // Free the item
                i_di = default_allocator(i_di, 0);
                
                // Iterate
                i_di = n;
            }

            // Prevent double free
            p_dict->entries.data[i] = 0;
        }
    }

    // Clear iterables

    // If there are no iterables, there is nothing to do
    if ( p_dict->iterable.keys == 0 && p_dict->iterable.values == 0 ) goto done;

    // Check for a valid pointer
    if ( p_dict->iterable.keys )

        // Iterate over each key
        for (size_t i = 0; i < p_dict->entries.count; i++)

            // Zero set the key
            p_dict->iterable.keys[i] = 0;

    // Check for a valid pointer
    if ( p_dict->iterable.values )

        // Iterate over each value
        for (size_t i = 0; i < p_dict->entries.count; i++)

            // Zero the value
            p_dict->iterable.values[i] = 0;

    // Zero the count
    p_dict->entries.count = 0;

    done:

    // unlock
    mutex_unlock(&p_dict->_lock);

    // success
    return 1;

    // error handling
    {

        // argument errors
        {
            no_dictionary:
                #ifndef NDEBUG
                    log_warning("[dict] Null pointer provided for parameter \"p_dict\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
    }
}

int dict_free_clear ( dict *const p_dict, void (*const free_func)(const void *const) )
{

    // argument check
    if ( p_dict                == (void *) 0 ) goto no_dictionary;
    if ( free_func             == (void *) 0 ) goto no_free_func;
    if ( p_dict->entries.count ==          0 ) return 1;
    
    // lock
    mutex_lock(&p_dict->_lock);

    // Iterate over each hash table item
    for (size_t i = 0; i < p_dict->entries.max; i++)
    {

        // Is there a valid entry at the index?
        if (p_dict->entries.data[i])
        {

            // Initialzied data
            dict_item *i_di = p_dict->entries.data[i];

            // Iterate through linked list
            while ( i_di )
            {
                dict_item *n = i_di->next;

                // Call the specified deallocator
                free_func(i_di->value);

                // Free the item
                i_di = default_allocator(i_di, 0);
                
                // Iterate
                i_di = n;
            }

            // Prevent double free
            p_dict->entries.data[i] = 0;
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
            no_dictionary:
                #ifndef NDEBUG
                    log_warning("[dict] Null pointer provided for parameter \"p_dict\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;

            no_free_func:
                #ifndef NDEBUG
                    log_warning("[dict] Null pointer provided for parameter \"free_func\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
    }
}

int dict_destroy ( dict **const pp_dict )
{

    // argument check
    if ( pp_dict  == (void *) 0 ) goto no_dictionary;
    if ( *pp_dict == (void *) 0 ) goto pp_dict_null;

    // initialized data
    dict *p_dict = *pp_dict;

    // lock
    mutex_lock(&p_dict->_lock);

    // No more pointer for end user
    *pp_dict = (dict *) 0;

    // unlock
    mutex_unlock(&p_dict->_lock);

    // Remove all the dictionary properties
    if ( dict_clear(p_dict) == 0 ) goto failed_to_clear;

    // Free the hash table
    p_dict->entries.data = default_allocator(p_dict->entries.data, 0);

    // Free the iterables
    p_dict->iterable.keys   = default_allocator(p_dict->iterable.keys, 0),
    p_dict->iterable.values = default_allocator(p_dict->iterable.values, 0);

    // Destroy the mutex
    mutex_destroy(&p_dict->_lock);

    // Free the dictionary
    p_dict = default_allocator(p_dict, 0);

    // success
    return 1;

    // error handling
    {

        // argument errors
        {
            no_dictionary:
                #ifndef NDEBUG
                    log_warning("[dict] Null pointer provided for parameter \"pp_dict\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;

            pp_dict_null:
                #ifndef NDEBUG
                    log_warning("[dict] Parameter \"pp_dict\" points to null pointer in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }

        // dict errors
        {
            failed_to_clear:
                #ifndef NDEBUG
                    log_warning("[dict] Call to \"dict_clear\" returned an erroneous value in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
    }
}
