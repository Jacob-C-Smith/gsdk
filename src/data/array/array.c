/** !
 * array library
 * 
 * @file array.c 
 * 
 * @author Jacob Smith
 */

// header
#include <data/array.h>

// core
#include <core/log.h>
#include <core/hash.h>
#include <core/pack.h>
#include <core/sync.h>

// structure definitions
struct array_s
{
    size_t   count,         // quantity of elements in an array
             max;           // maximum quantity of elements in an array 
    mutex    _lock;         // lock
    void    **p_p_elements; // elements
};

int array_construct ( array **pp_array, size_t size )
{

    // argument check
    if ( pp_array == (void *) 0 ) goto no_array;
    if ( size     == 0          ) goto zero_size;

    // initialized data
    array *p_array = default_allocator(0, sizeof(array));

    // error checking
    if ( NULL == p_array ) goto no_mem;

    // initialize the array
    p_array->count = 0,
    p_array->max   = size,

    // allocate memory for the array contents
    p_array->p_p_elements = default_allocator(0, p_array->max * sizeof(void *));

    // error checking
    if ( p_array->p_p_elements == (void *) 0 ) goto no_mem;

    // create a mutex
    if ( mutex_create(&p_array->_lock) == 0 ) goto failed_to_create_mutex;

    // return a pointer to the caller
    *pp_array = p_array;

    // success
    return 1;

    // error handling
    {

        // argument errors
        {
            no_array:
                #ifndef NDEBUG
                    log_error("[array] Null pointer provided for parameter \"pp_array\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error 
                return 0;

            zero_size:
                #ifndef NDEBUG
                    log_error("[array] Zero provided for parameter \"size\" in call to function \"%s\"\n", __FUNCTION__);
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

        // sync errors
        {
            failed_to_create_mutex:
                #ifndef NDEBUG
                    log_error("[array] Failed to create mutex in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error 
                return 0;
        }
    }
}

int array_from_elements ( array **pp_array, void *_p_elements[], size_t size )
{

    // argument check
    if ( NULL == pp_array    ) goto no_array;
    if ( NULL == _p_elements ) goto no_elements;
    if ( 0    == size        ) goto zero_size;

    // initialized data
    array *p_array = NULL;

    // allocate an array
    if ( array_construct(&p_array, size) == 0 ) goto failed_to_allocate_array;        

    // copy each element
    for (size_t i = 0; _p_elements[i]; i++)
        array_add(p_array, _p_elements[i]);

    // return a pointer to the caller
    *pp_array = p_array;

    // success
    return 1;

    // error handling
    {

        // argument errors
        {
            no_array:
                #ifndef NDEBUG
                    log_error("[array] Null pointer provided for \"pp_array\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error 
                return 0;

            no_elements:
                #ifndef NDEBUG
                    log_error("[array] Null pointer provided for \"_p_elements\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error 
                return 0;

            zero_size:
                #ifndef NDEBUG
                    log_error("[array] Zero provided for parameter \"size\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error 
                return 0;   
        }

        // array errors
        {
            failed_to_allocate_array:
                #ifndef NDEBUG
                    log_error("[array] Call to \"array_construct\" returned an erroneous value in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
    }
}

int array_from_arguments ( array **pp_array, size_t size, size_t count, ... )
{

    // argument check
    if ( pp_array == (void *) 0 ) goto no_array;
    if ( 0        >= count      ) goto negative_count;

    // uninitialized data
    va_list list;

    // initialized data
    array *p_array = NULL;

    // Initialize the variadic list
    va_start(list, count);

    // Allocate an array
    if ( array_construct(&p_array, size) == 0 ) goto failed_to_allocate_array;        

    // Iterate over each key
    for (size_t i = 0; i < count; i++)

        // Add the key to the array
        array_add(p_array, va_arg(list, void *));
    
    // Update the element count
    p_array->count = count;

    // End the variadic list
    va_end(list);

    // return a pointer to the caller
    *pp_array = p_array;

    // success
    return 1;

    // error handling
    {

        // argument errors
        {
            no_array:
                #ifndef NDEBUG
                    log_error("[array] Null pointer provided for \"pp_array\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error 
                return 0;
            
            negative_count:
                #ifndef NDEBUG
                    log_error("[array] Parameter \"count\" was negative in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error 
                return 0;
            
        }

        // array errors
        {
            failed_to_allocate_array:
                #ifndef NDEBUG
                    log_error("[array] Call to \"array_construct\" returned an erroneous value in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
    }
}

int array_index ( array *p_array, signed index, void **const pp_value )
{

    // argument errors
    if ( p_array        == (void *) 0 ) goto no_array;
    if ( p_array->count ==          0 ) goto no_elements;
    if ( pp_value       == (void *) 0 ) goto no_value;

    // lock
    mutex_lock(&p_array->_lock);

    // error check
    if ( p_array->count == (size_t) abs(index) ) goto bounds_error;

    // Positive index
    if ( index >= 0 )
        *pp_value = p_array->p_p_elements[index];

    // Negative numbers
    else 
        *pp_value = p_array->p_p_elements[p_array->count - (size_t) abs(index)];

    // unlock
    mutex_unlock(&p_array->_lock);

    // success
    return 1;

    // error handling
    {
        no_array:
            #ifndef NDEBUG
                log_error("[array] Null pointer provided for parameter \"p_array\" in call to function \"%s\"\n", __FUNCTION__);
            #endif

            // error
            return 0;

        no_value:
            #ifndef NDEBUG
                log_error("[array] Null pointer provided for parameter \"p_value\" in call to function \"%s\"\n", __FUNCTION__);
            #endif

            // error
            return 0;

        no_elements:
            #ifndef NDEBUG
                log_error("[array] Can not index an empty array in call to function \"%s\"\n", __FUNCTION__);
            #endif

            // error 
            return 0;
        
        bounds_error:
            #ifndef NDEBUG
                log_error("[array] Index out of bounds in call to function \"%s\"\n", __FUNCTION__);
            #endif

            // unlock
            mutex_unlock(&p_array->_lock);
            
            // error
            return 0;
    }
}

int array_get ( array *p_array, void **const pp_elements, size_t *const p_count )
{

    // argument check
    if ( p_array == (void *) 0 ) goto no_array;

    // lock
    mutex_lock(&p_array->_lock);

    // Return the elements
    if ( pp_elements )
        memcpy(pp_elements, p_array->p_p_elements, (sizeof(void *) * p_array->count) );
    
    // Return the count
    if ( p_count )
        *p_count = p_array->count;

    // unlock
    mutex_unlock(&p_array->_lock);

    // success
    return 1;

    // error handling
    {

        // argument errors
        {
            no_array:
                #ifndef NDEBUG
                    log_error("[array] Null pointer provided for \"p_array\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error 
                return 0;
        }
    }
}

int array_slice ( array *p_array, void *pp_elements[], signed lower_bound, signed upper_bound )
{

    // argument check
    if ( p_array == (void *) 0 ) goto no_array;
    if ( lower_bound < 0 ) goto erroneous_lower_bound;
    if ( p_array->count < (size_t) upper_bound ) goto erroneous_upper_bound;

    // lock
    mutex_lock(&p_array->_lock);

    // Return the elements
    if ( pp_elements )
        memcpy(pp_elements, &p_array->p_p_elements[lower_bound], sizeof(void *) * (size_t) ( upper_bound - lower_bound + 1LL ) );
    
    // unlock
    mutex_unlock(&p_array->_lock);

    // success
    return 1;

    // error handling
    {

        // argument errors
        {
            no_array:
                #ifndef NDEBUG
                    log_error("[array] Null pointer provided for \"p_array\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error 
                return 0;

            erroneous_lower_bound:
                #ifndef NDEBUG
                    log_error("[array] Parameter \"lower_bound\" must be greater than zero in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error 
                return 0;
                
            erroneous_upper_bound:
                #ifndef NDEBUG
                    log_error("[array] Parameter \"upper_bound\" must be less than or equal to array size in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error 
                return 0;
        }
    }
}

bool array_is_empty ( array *p_array )
{

    // argument check
    if ( p_array == (void *) 0 ) goto no_array;

    // initialized data
    bool ret = false;

    // lock
    mutex_lock(&p_array->_lock);

    // Is empty?
    ret = ( 0 == p_array->count );

    // unlock
    mutex_unlock(&p_array->_lock);

    // success
    return ret;

    // error handling
    {

        // argument errors
        {
            no_array:
                #ifndef NDEBUG
                    log_error("[array] Null pointer provided for parameter \"p_array\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
    }
}

size_t array_size ( array *p_array )
{

    // argument check
    if ( p_array == (void *) 0 ) goto no_array;

    // success
    return p_array->count;

    // error handling
    {

        // argument errors
        {
            no_array:
                #ifndef NDEBUG
                    log_error("[array] Null pointer provided for parameter \"p_array\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
    }
}

int array_add ( array *p_array, void *p_element )
{

    // argument check
    if ( p_array == (void *) 0 ) goto no_array;

    // lock
    mutex_lock(&p_array->_lock);

    // update the iterables
    p_array->p_p_elements[p_array->count] = p_element;

    // increment the entry counter
    p_array->count++;

    // resize iterable max?
    if ( p_array->count >= p_array->max )
    {
    
        // double the size
        p_array->max *= 2;
    
        // default_allocatorate iterable arrays
        p_array->p_p_elements = default_allocator(p_array->p_p_elements, p_array->max * sizeof(void *));
    
        // error checking
        if ( NULL == p_array->p_p_elements ) goto no_mem;
    }
    
    // unlock
    mutex_unlock(&p_array->_lock);

    // success
    return 1;

    // error handling
    {

        // argument errors
        {
            no_array:
                #ifndef NDEBUG
                    log_error("[array] Null pointer provided for \"p_array\" in call to function \"%s\"\n", __FUNCTION__);
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
                mutex_unlock(&p_array->_lock);

                // error
                return 0;
        }
    }
}

int array_set ( array *p_array, signed index, void *p_value )
{
    
    // argument check   
    if ( p_array == (void *) 0 ) goto no_array;

    // State check
    if ( p_array->count == 0 ) goto no_elements;
    
    // initialized data
    size_t _index = 0;

    // lock
    mutex_lock(&p_array->_lock);

    // error check
    if ( p_array->count == (size_t) abs(index) ) goto bounds_error;

    // Store the correct index
    _index = ( index >= 0 ) ? (size_t) index : (size_t) p_array->count - (size_t) abs(index);
    
    // Store the element
    p_array->p_p_elements[_index] = p_value;

    // unlock
    mutex_unlock(&p_array->_lock);

    // success
    return 1;

    // error handling
    {

        // argument errors
        {
            no_array:
                #ifndef NDEBUG
                    log_error("[array] Null pointer provided for \"p_array\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }

        // array errors
        {
            bounds_error:
                #ifndef NDEBUG
                    log_error("[array] Index out of bounds in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // unlock
                mutex_unlock(&p_array->_lock);

                // error
                return 0;

            no_elements:
                #ifndef NDEBUG
                    log_error("[array] Can not index an empty array in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error 
                return 0;
        
        }
    }
}

int array_remove ( array *p_array, signed index, void **const pp_value )
{

    // argument check
    if ( p_array == (void *) 0 ) goto no_array;

    // State check
    if ( p_array->count == 0 ) goto no_elements;
    
    // initialized data
    size_t _index = 0;

    // lock
    mutex_lock(&p_array->_lock);

    // error check
    if ( p_array->count == (size_t) abs(index) ) goto bounds_error;

    // Store the correct index
    _index = ( index >= 0 ) ? (size_t) index : (size_t) p_array->count - (size_t) abs(index);
    
    // Store the element
    if ( pp_value != (void *) 0 ) *pp_value = p_array->p_p_elements[_index];

    // Edge case
    if ( (size_t) index == p_array->count-1 ) goto done;

    // Iterate from the index of the removed element to the end of the array
    for (size_t i = _index; i < p_array->count-1; i++)
    
        // Shift elements
        p_array->p_p_elements[i] = p_array->p_p_elements[i+1];

    done:

    // Decrement the element counter
    p_array->count--;

    // unlock
    mutex_unlock(&p_array->_lock);

    // success
    return 1;

    // error handling
    {

        // argument errors
        {
            no_array:
                #ifndef NDEBUG
                    log_error("[array] Null pointer provided for \"p_array\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }

        // array errors
        {
            bounds_error:
                #ifndef NDEBUG
                    log_error("[array] Index out of bounds in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // unlock
                mutex_unlock(&p_array->_lock);

                // error
                return 0;

            no_elements:
                #ifndef NDEBUG
                    log_error("[array] Can not index an empty array in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error 
                return 0;
        
        }
    }
}

int array_sort ( array *p_array, fn_comparator *pfn_comparator )
{
    
    // argument check
    if ( NULL ==        p_array ) goto no_array;
    if ( NULL == pfn_comparator ) goto no_fn_comparator;

    // lock
    mutex_lock(&p_array->_lock);

    // sort
    qsort(
        p_array->p_p_elements, // array
        p_array->count,        // quantity
        sizeof(void *),        // size
        pfn_comparator         // comparator
    );

    // unlock
    mutex_unlock(&p_array->_lock);

    // success
    return 1;

    // error handling
    {
        
        // argument errors
        {
            no_array:
                #ifndef NDEBUG
                    log_error("[array] Null pointer provided for \"p_array\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
            
            no_fn_comparator:
                #ifndef NDEBUG
                    log_error("[array] Null pointer provided for \"pfn_comparator\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
    }
}

int array_map ( array *const p_array, fn_map *pfn_map, fn_allocator *pfn_allocator )
{

    // argument check
    if ( NULL == p_array ) goto no_array;
    if ( NULL == pfn_map ) goto no_fn_map;

    // lock
    mutex_lock(&p_array->_lock);

    // state check
    if ( pfn_allocator ) goto map_with_allocator;

    // iterate through each element in the array
    for (size_t i = 0; i < p_array->count; i++)

        // update
        p_array->p_p_elements[i] = pfn_map(p_array->p_p_elements[i]);

    done:

    // unlock
    mutex_unlock(&p_array->_lock);

    // success
    return 1;

    // this branch handles map calls that invoke the allocator
    map_with_allocator:
    {

        // iterate through each element in the array
        for (size_t i = 0; i < p_array->count; i++)
        {

            // initialized data
            void *p_old = p_array->p_p_elements[i],
                 *p_new = NULL;

            // update
            p_new = pfn_map(p_array->p_p_elements[i]),
            p_array->p_p_elements[i] = p_new;

            // release
            if ( p_old != p_new && pfn_allocator )
                pfn_allocator(p_array, 0);
        }

        // done
        goto done;
    }

    // error handling
    {
        
        // argument errors
        {
            no_array:
                #ifndef NDEBUG
                    log_error("[array] Null pointer provided for \"p_array\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
            
            no_fn_map:
                #ifndef NDEBUG
                    log_error("[array] Null pointer provided for \"pfn_map\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
    }
}

int array_fori ( array *p_array, fn_fori *pfn_fori ) 
{

    // argument check
    if ( NULL == p_array  ) goto no_array;
    if ( NULL == pfn_fori ) goto no_fn_fori;

    // lock
    mutex_lock(&p_array->_lock);

    // Iterate over each element in the array
    for (size_t i = 0; i < p_array->count; i++)
        
        // Call the function
        pfn_fori(p_array->p_p_elements[i], i);

    // unlock
    mutex_unlock(&p_array->_lock);

    // success
    return 1;

    // error handling
    {
        
        // argument errors
        {
            no_array:
                #ifndef NDEBUG
                    log_error("[array] Null pointer provided for \"p_array\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
            
            no_fn_fori:
                #ifndef NDEBUG
                    log_error("[array] Null pointer provided for \"pfn_fori\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
    }
}

int array_log ( array *p_array, void *pfn_next, const char *const restrict format, ... )
{

    // argument check
    if ( p_array  == (void *) 0 ) goto no_array;
    if ( pfn_next == (void *) 0 ) return 0;

    // Print the header
    log_info("=== %s : %p ===\n", format, p_array);

    // lock
    mutex_lock(&p_array->_lock);

    // Iterate over each element in the array
    for (size_t i = 0; i < p_array->count; i++)
        
        // TODO: Call the function
        ; 

    // unlock
    mutex_unlock(&p_array->_lock);

    // Print a newline
    putchar('\n');

    // success
    return 1;

    // error handling
    {
        
        // argument errors
        {
            no_array:
                #ifndef NDEBUG
                    log_error("[array] Null pointer provided for \"p_array\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
    }
}

int array_pack ( void *p_buffer, array *p_array, fn_pack *pfn_element )
{
    
    // argument check
    if ( p_array     == (void *) 0 ) goto no_array;
    if ( pfn_element == (void *) 0 ) return 0;

    // initialized data 
    char *p = p_buffer;

    // lock
    mutex_lock(&p_array->_lock);

    // Pack the length
    p += pack_pack(p, "%i64", p_array->count);

    // Iterate through the array
    for (size_t i = 0; i < p_array->count; i++)
        p += pfn_element(p, p_array->p_p_elements[i]);

    // unlock
    mutex_unlock(&p_array->_lock);

    // success
    return p - (char *)p_buffer;

    // error handling
    {
        
        // argument errors
        {
            no_array:
                #ifndef NDEBUG
                    log_error("[array] Null pointer provided for \"p_array\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
    }
}

int array_unpack ( array **pp_array, void *p_buffer, fn_unpack *pfn_element )
{
    
    // argument check
    if ( pp_array    == (void *) 0 ) goto no_array;
    if ( pfn_element == (void *) 0 ) return 0;

    // initialized data
    array *p_array = NULL;
    char *p = p_buffer;
    size_t len = 0;

    // Unpack the length
    p += pack_unpack(p, "%i64", &len);

    // Construct an array
    array_construct(&p_array, len);

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
        
        // Add the element to the array
        array_add(p_array, p_element);
    }

    // Return the array to the caller
    *pp_array = p_array;

    // success
    return 1;
    
    // error handling
    {
        
        // argument errors
        {
            no_array:
                #ifndef NDEBUG
                    log_error("[array] Null pointer provided for \"p_array\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
    }
}

hash64 array_hash ( array *p_array, fn_hash64 *pfn_element )
{

    // argument check
    if ( p_array == (void *) 0 ) goto no_array;

    // initialized data
    hash64     result     = 0;
    fn_hash64 *pfn_hash64 = (pfn_element) ? pfn_element : hash_crc64;

    // iterate through each element in the array
    for (size_t i = 0; i < p_array->count; i++)
        result ^= pfn_hash64(p_array->p_p_elements[i], 8);

    // success
    return result;

    // error handling
    {

        // argument errors
        {
            no_array:
                #ifndef NDEBUG
                    log_error("[array] Null pointer provided for \"pp_array\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
    }
}

int array_destroy ( array **pp_array )
{

    // argument check
    if ( pp_array == (void *) 0 ) goto no_array;

    // initialized data
    array *p_array = *pp_array;

    // lock
    mutex_lock(&p_array->_lock);

    // no more pointer for end user
    *pp_array = (array *) 0;

    // unlock
    mutex_unlock(&p_array->_lock);

    // destroy the mutex
    mutex_destroy(&p_array->_lock);

    // release the array
    p_array = default_allocator(p_array, 0);
    
    // success
    return 1;

    // error handling
    {

        // argument errors
        {
            no_array:
                #ifndef NDEBUG
                    log_error("[array] Null pointer provided for \"pp_array\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
    }
}
