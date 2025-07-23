/** !
 * tuple library
 * 
 * @file tuple.c 
 * 
 * @author Jacob Smith
 */

// headers
#include <data/tuple.h>

// core
#include <core/log.h>
#include <core/pack.h>
#include <core/sync.h>

// structure definitions
struct tuple_s
{
    size_t   element_count; // Quantity of elements
    void   *_p_elements[];  // Tuple contents
};

// Data
static bool initialized = false;

void tuple_init ( void ) 
{

    // State check
    if ( initialized == true ) return;

    // Initialize the log library
    log_init();

    // Set the initialized flag
    initialized = true;

    // Done
    return;
}

// function declarations
int tuple_create ( tuple **const pp_tuple )
{

    // argument check
    if ( pp_tuple == (void *) 0 ) goto no_tuple;

    // Allocate memory for a tuple
    tuple *p_tuple = default_allocator(0, sizeof(tuple));

    // error checking
    if ( p_tuple == (void *) 0 ) goto no_mem;

    // Zero set
    memset(p_tuple, 0, sizeof(tuple));

    // Return the allocated memory
    *pp_tuple = p_tuple;

    // success
    return 1;

    // error handling
    {

        // argument errors
        {
            no_tuple:
                #ifndef NDEBUG
                    log_error("[tuple] Null pointer provided for parameter \"pp_tuple\" in call to function \"%s\"\n", __FUNCTION__);
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

int tuple_construct ( tuple **const pp_tuple, size_t size )
{

    // argument check
    if ( pp_tuple == (void *) 0 ) goto no_tuple;

    // initialized data
    tuple *p_tuple = 0;

    // Allocate a tuple
    if ( tuple_create(&p_tuple) == 0 ) goto failed_to_create_tuple;

    // Set the count and iterator max
    p_tuple->element_count = size;

    // Grow the allocation
    p_tuple = default_allocator(p_tuple, sizeof(tuple) + ( size * sizeof(void *) ) );

    // error checking
    if ( p_tuple == (void *) 0 ) goto no_mem;

    // return a pointer to the caller
    *pp_tuple = p_tuple;

    // success
    return 1;

    // error handling
    {

        // argument errors
        {
            no_tuple:
                #ifndef NDEBUG
                    log_error("[tuple] Null pointer provided for parameter \"pp_tuple\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error 
                return 0;                
        }

        // Tuple errors
        {
            failed_to_create_tuple:
                #ifndef NDEBUG
                    log_error("[tuple] Failed to create tuple in call to function \"%s\"\n", __FUNCTION__);
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

int tuple_from_elements ( tuple **const pp_tuple, void *const *const elements, size_t size )
{

    // argument check
    if ( pp_tuple == (void *) 0 ) goto no_tuple;
    if ( elements == (void *) 0 ) goto no_elements;

    // initialized data
    tuple *p_tuple = 0;

    // Allocate a tuple
    if ( tuple_construct(&p_tuple, size) == 0 ) goto failed_to_allocate_tuple;        

    // Iterate over each key
    for (size_t i = 0; elements[i]; i++)

        // Add the key to the tuple
        p_tuple->_p_elements[i] = elements[i];

    // Set the quantity of elements
    p_tuple->element_count = size;

    // Return
    *pp_tuple = p_tuple;

    // success
    return 1;

    // error handling
    {

        // argument errors
        {
            no_tuple:
                #ifndef NDEBUG
                    log_error("[tuple] Null pointer provided for parameter \"pp_tuple\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error 
                return 0;

            no_elements:
                #ifndef NDEBUG
                    log_error("[tuple] Null pointer provided for parameter \"keys\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error 
                return 0;
        }

        // Tuple errors
        {
            failed_to_allocate_tuple:
                #ifndef NDEBUG
                    log_error("[tuple] Call to \"tuple_construct\" returned an erroneous value in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
    }
}

int tuple_from_arguments ( tuple **const pp_tuple, size_t element_count, ... )
{

    // argument check
    if ( pp_tuple      == (void *) 0 ) goto no_tuple;
    if ( element_count ==          0 ) goto no_elements;

    // uninitialized data
    va_list list;

    // initialized data
    tuple *p_tuple = 0;

    // Initialize the variadic list
    va_start(list, element_count);

    // Allocate a tuple
    if ( tuple_construct(&p_tuple, element_count) == 0 ) goto failed_to_allocate_tuple;        

    // Iterate over each key
    for (size_t i = 0; i < element_count; i++)

        // Add the key to the tuple
        p_tuple->_p_elements[i] = va_arg(list, void *);
    
    // End the variadic list
    va_end(list);

    // Return
    *pp_tuple = p_tuple;

    // success
    return 1;

    // error handling
    {

        // argument errors
        {
            no_tuple:
                #ifndef NDEBUG
                    log_error("[tuple] Null pointer provided for parameter \"pp_tuple\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error 
                return 0;

            no_elements:
                #ifndef NDEBUG
                    log_error("[tuple] Parameter \"element_count\" must be greater than zero in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error 
                return 0;
        }

        // Tuple errors
        {
            failed_to_allocate_tuple:
                #ifndef NDEBUG
                    log_error("[tuple] Call to \"tuple_construct\" returned an erroneous value in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
    }
}

int tuple_index ( const tuple *const p_tuple, signed long long index, void **const pp_value )
{

    // argument check
    if ( p_tuple                == (void *) 0 ) goto no_tuple;
    if ( p_tuple->element_count ==          0 ) goto no_elements;
    if ( pp_value               == (void *) 0 ) goto no_value;

    // error check
    if ( p_tuple->element_count == (size_t) llabs(index) ) goto bounds_error;

    // Positive index
    if ( index >= 0 )
        *pp_value = p_tuple->_p_elements[index];

    // Negative numbers
    else 
        *pp_value = p_tuple->_p_elements[p_tuple->element_count - (size_t) ( index * -1 )];

    // success
    return 1;

    // error handling
    {
        no_tuple:
            #ifndef NDEBUG
                log_error("[tuple] Null pointer provided for parameter \"p_tuple\" in call to function \"%s\"\n", __FUNCTION__);
            #endif

            // error
            return 0;

        no_value:
            #ifndef NDEBUG
                log_error("[tuple] Null pointer provided for parameter \"pp_vale\" in call to function \"%s\"\n", __FUNCTION__);
            #endif

            // error
            return 0;

        no_elements:
            #ifndef NDEBUG
                log_error("[tuple] Can not index an empty tuple in call to function \"%s\"\n", __FUNCTION__);
            #endif

            // error 
            return 0;
        
        bounds_error:
            #ifndef NDEBUG
                log_error("[tuple] Index out of bounds in call to function \"%s\"\n", __FUNCTION__);
            #endif

            // error
            return 0;
    }
}

int tuple_slice ( const tuple *const p_tuple, const void **const pp_elements, signed long long lower_bound, signed long long  upper_bound )
{

    // argument check
    if ( p_tuple == (void *) 0 ) goto no_tuple;
    if ( pp_elements == 0 ) goto no_elemenets;
    if ( lower_bound < 0 ) goto erroneous_lower_bound;
    if ( p_tuple->element_count < (size_t) upper_bound ) goto erroneous_upper_bound;

    // Return the elements
    memcpy(pp_elements, &p_tuple->_p_elements[lower_bound], sizeof(void *) * (size_t) ( upper_bound - lower_bound + 1 ) );

    // success
    return 1;

    // error handling
    {

        // argument errors
        {
            no_tuple:
                #ifndef NDEBUG
                    log_error("[tuple] Null pointer provided for parameter \"p_tuple\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error 
                return 0;

            erroneous_lower_bound:
                #ifndef NDEBUG
                    log_error("[tuple] Parameter \"lower_bound\" must be greater than zero in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error 
                return 0;
                
            erroneous_upper_bound:
                #ifndef NDEBUG
                    log_error("[tuple] Parameter \"upper_bound\" must be less than or equal to tuple size in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error 
                return 0;

            no_elemenets:
                #ifndef NDEBUG
                    log_error("[tuple] Null pointer provided for parameter \"p_tuple\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error 
                return 0;
        }
    }
}

bool tuple_is_empty ( const tuple *const p_tuple )
{

    // argument check
    if ( p_tuple == (void *) 0 ) goto no_tuple;

    // success
    return ( p_tuple->element_count == 0 );

    // error handling
    {

        // argument errors
        {
            no_tuple:
                #ifndef NDEBUG
                    log_error("[tuple] Null pointer provided for parameter \"p_tuple\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
    }
}

size_t tuple_size ( const tuple *const p_tuple )
{

    // argument check
    if ( p_tuple == (void *) 0 ) goto no_tuple;

    // success
    return p_tuple->element_count;

    // error handling
    {

        // argument errors
        {
            no_tuple:
                #ifndef NDEBUG
                    log_error("[tuple] Null pointer provided for parameter \"p_tuple\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
    }
}

int tuple_foreach ( const tuple *const p_tuple, void (*const pfn_function)(void *const value, size_t index) )
{

    // argument check
    if ( p_tuple      == (void *) 0 ) goto no_tuple;
    if ( pfn_function == (void *) 0 ) goto no_func;

    // Iterate over each element in the tuple
    for (size_t i = 0; i < p_tuple->element_count; i++)
        
        // Call the function
        pfn_function(p_tuple->_p_elements[i], i);

    // success
    return 1;

    // error handling
    {
        
        // argument errors
        {
            no_tuple:
                #ifndef NDEBUG
                    log_error("[tuple] Null pointer provided for parameter \"p_tuple\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
            
            no_func:
                #ifndef NDEBUG
                    log_error("[tuple] Null pointer provided for parameter \"pfn_function\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
    }
}

int tuple_pack ( tuple *p_tuple, void *p_buffer, fn_pack *pfn_element )
{
    
    // argument check
    if ( p_tuple     == (void *) 0 ) goto no_tuple;
    if ( pfn_element == (void *) 0 ) return 0;

    // initialized data 
    char *p = p_buffer;

    // Pack the length
    p += pack_pack(p, "%i64", p_tuple->element_count);

    // Iterate through the tuple
    for (size_t i = 0; i < p_tuple->element_count; i++)
        p += pfn_element(p, p_tuple->_p_elements[i]);

    // success
    return p - (char *)p_buffer;

    // error handling
    {
        
        // argument errors
        {
            no_tuple:
                #ifndef NDEBUG
                    log_error("[tuple] Null pointer provided for \"p_tuple\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
    }
}

int tuple_unpack ( tuple **pp_tuple, void *p_buffer, fn_unpack *pfn_element )
{
    
    // argument check
    if ( pp_tuple    == (void *) 0 ) goto no_tuple;
    if ( pfn_element == (void *) 0 ) return 0;

    // initialized data
    tuple *p_tuple = NULL;
    char *p = p_buffer;
    size_t len = 0;

    // Unpack the length
    p += pack_unpack(p, "%i64", &len);

    // Construct an tuple
    tuple_construct(&p_tuple, len);

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
        
        // Add the element to the tuple
        p_tuple->_p_elements[i] = p_element;
    }

    // Return the tuple to the caller
    *pp_tuple = p_tuple;

    // success
    return 1;
    
    // error handling
    {
        
        // argument errors
        {
            no_tuple:
                #ifndef NDEBUG
                    printf("[tuple] Null pointer provided for \"p_tuple\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
    }
}

int tuple_destroy ( tuple **const pp_tuple )
{

    // argument check
    if ( pp_tuple == (void *) 0 ) goto no_tuple;

    // initialized data
    tuple *p_tuple = *pp_tuple;

    // No more pointer for caller
    *pp_tuple = (void *) 0;

    // Free the tuple
    p_tuple = default_allocator(p_tuple, 0);
    
    // success
    return 1;

    // error handling
    {

        // argument errors
        {
            no_tuple:
                #ifndef NDEBUG
                    log_error("[tuple] Null pointer provided for parameter \"pp_tuple\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
    }
}

void tuple_exit ( void ) 
{

    // State check
    if ( initialized == false ) return;

    // Clean up log
    log_exit();

    // TODO: Anything else?
    // 

    // Clear the initialized flag
    initialized = false;

    // Done
    return;
}
