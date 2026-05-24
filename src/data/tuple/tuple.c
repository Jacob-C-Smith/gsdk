/** !
 * Tuple implementation
 * 
 * @file src/data/tuple/tuple.c
 * 
 * @author Jacob Smith
 */

// header file
#include <data/tuple.h>

fn_it_done tuple_iterator_done;
fn_it_next tuple_iterator_next;
fn_it_item tuple_iterator_item;

// structure definitions
struct tuple_s
{
    size_t   element_count; // the quantity of elements
    void   *_p_elements[];  // the contents
};

// function declarations
/** !
 * Construct a tuple with a specific size
 *
 * @param pp_tuple result
 * @param size     number of elements in a tuple
 *
 * @sa tuple_create
 * @sa tuple_destroy
 *
 * @return 1 on success, 0 on error
 */
int tuple_construct ( tuple **const pp_tuple, size_t size );

// function definitions
int tuple_construct ( tuple **const pp_tuple, size_t size )
{

    // argument check
    if ( NULL == pp_tuple ) goto no_tuple;

    // initialized data
    tuple *p_tuple = NULL;

    // allocate a tuple
    p_tuple = default_allocator(p_tuple, sizeof(tuple) + ( size * sizeof(void *) ) );
    if ( NULL == p_tuple ) goto no_mem;

    // store the size
    p_tuple->element_count = size;
    
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

int tuple_from_elements ( tuple **const pp_tuple, void *const *const elements, size_t size )
{

    // argument check
    if ( NULL == pp_tuple ) goto no_tuple;
    if ( NULL == elements ) goto no_elements;

    // initialized data
    tuple *p_tuple = NULL;

    // construct a tuple
    if ( 0 == tuple_construct(&p_tuple, size) ) goto failed_to_construct_tuple;        

    // iterate over each key
    for (size_t i = 0; elements[i]; i++)
        p_tuple->_p_elements[i] = elements[i];

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

            no_elements:
                #ifndef NDEBUG
                    log_error("[tuple] Null pointer provided for parameter \"keys\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error 
                return 0;
        }

        // tuple errors
        {
            failed_to_construct_tuple:
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
    if ( NULL ==      pp_tuple ) goto no_tuple;
    if ( 0    == element_count ) goto no_elements;

    // uninitialized data
    va_list list;

    // initialized data
    tuple *p_tuple = NULL;

    // initialize the variadic list
    va_start(list, element_count);

    // construct a tuple
    if ( 0 == tuple_construct(&p_tuple, element_count) ) goto failed_to_construct_tuple;        

    // iterate over each key
    for (size_t i = 0; i < element_count; i++)

        // add the value to the tuple
        p_tuple->_p_elements[i] = va_arg(list, void *);
    
    // end the variadic list
    va_end(list);

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

            no_elements:
                #ifndef NDEBUG
                    log_error("[tuple] Parameter \"element_count\" must be greater than zero in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error 
                return 0;
        }

        // tuple errors
        {
            failed_to_construct_tuple:
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
    if ( NULL ==               p_tuple ) goto no_tuple;
    if ( 0    == p_tuple->element_count) goto no_elements;
    if ( NULL ==              pp_value ) goto no_value;

    // error check
    if ( p_tuple->element_count == (size_t) llabs(index) ) goto bounds_error;

    // positive index
    if ( index >= 0 )
        *pp_value = p_tuple->_p_elements[index];

    // negative numbers
    else 
        *pp_value = p_tuple->_p_elements[p_tuple->element_count - (size_t) ( index * -1 )];

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

            no_value:
                #ifndef NDEBUG
                    log_error("[tuple] Null pointer provided for parameter \"pp_valUe\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;

            no_elements:
                #ifndef NDEBUG
                    log_error("[tuple] Can not index an empty tuple in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error 
                return 0;
        }

        // tuple errors
        {
            bounds_error:
                #ifndef NDEBUG
                    log_error("[tuple] Index out of bounds in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
    }
}

int tuple_slice 
(
    const tuple *const p_tuple,
    const void **const pp_elements,
    signed long long   lower_bound,
    signed long long   upper_bound
)
{

    // argument check
    if ( NULL                   ==              p_tuple ) goto no_tuple;
    if ( 0                      ==          pp_elements ) goto no_elemenets;
    if ( 0                       >          lower_bound ) goto erroneous_lower_bound;
    if ( p_tuple->element_count  < (size_t) upper_bound ) goto erroneous_upper_bound;

    // return the elements
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
                    log_error("[tuple] Can not slice empty tuple in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error 
                return 0;
        }
    }
}

bool tuple_is_empty ( const tuple *const p_tuple )
{

    // argument check
    if ( NULL == p_tuple ) goto no_tuple;

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
    if ( NULL == p_tuple ) goto no_tuple;

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

int tuple_fori ( tuple *p_tuple, fn_fori *pfn_fori ) 
{

    // argument check
    if ( NULL ==  p_tuple ) goto no_tuple;
    if ( NULL == pfn_fori ) goto no_fn_fori;

    // iterate over each element in the tuple
    for (size_t i = 0; i < p_tuple->element_count; i++)

        // call the function
        pfn_fori(p_tuple->_p_elements[i], i);

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
            
            no_fn_fori:
                #ifndef NDEBUG
                    log_error("[tuple] Null pointer provided for parameter \"pfn_fori\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
    }
}

iterator tuple_iterator ( tuple *p_tuple )
{

    // success
    return (iterator)
    {
        .p_data = p_tuple,
        .state  = { 0 },
        .done   = tuple_iterator_done,
        .next   = tuple_iterator_next,
        .item   = tuple_iterator_item
    };
}

bool tuple_iterator_done ( iterator *p_iterator ) 
{

    // done?
    return ((size_t)p_iterator->state.p_state) >= ((tuple *) p_iterator->p_data)->element_count; 
}

void tuple_iterator_next ( iterator *p_iterator ) 
{

    // update the state
    p_iterator->state.p_state = (void *)((size_t)p_iterator->state.p_state + 1); 

    // done
    return;
}

void *tuple_iterator_item ( iterator *p_iterator ) 
{

    // done
    return ((tuple *) p_iterator->p_data)->_p_elements[(size_t)p_iterator->state.p_state]; 
}

int tuple_pack ( stream *p_stream, tuple *p_tuple, fn_pack *pfn_element )
{
    
    // argument check
    if ( NULL ==    p_stream ) goto no_stream;
    if ( NULL ==     p_tuple ) goto no_tuple;
    if ( NULL == pfn_element ) goto no_pack;

    // initialized data 
    size_t written = 0;

    // pack the length
    written += pack_pack(p_stream, "%i64", p_tuple->element_count);

    // iterate through the tuple
    for (size_t i = 0; i < p_tuple->element_count; i++)
        written += pfn_element(p_stream, p_tuple->_p_elements[i]);

    // success
    return written;

    // error handling
    {
        
        // argument errors
        {
            no_stream:
                #ifndef NDEBUG
                    log_error("[tuple] Null pointer provided for parameter \"p_stream\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;

            no_tuple:
                #ifndef NDEBUG
                    log_error("[tuple] Null pointer provided for parameter \"p_tuple\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;

            no_pack:
                #ifndef NDEBUG
                    log_error("[tuple] Null pointer provided for parameter \"pfn_element\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
    }
}

int tuple_unpack ( tuple **pp_tuple, stream *p_stream, fn_unpack *pfn_element )
{
    
    // argument check
    if ( NULL ==    pp_tuple ) goto no_tuple;
    if ( NULL ==    p_stream ) goto no_stream;
    if ( NULL == pfn_element ) goto no_unpack;

    // initialized data
    tuple  *p_tuple = NULL;
    size_t  written = 0;
    size_t  len     = 0;

    // unpack the length
    written += pack_unpack(p_stream, "%i64", &len);

    // construct a tuple
    if ( 0 == tuple_construct(&p_tuple, len) ) goto failed_to_construct_tuple;

    // iterate through each expected value
    for (size_t i = 0; i < len; i++)
    {
        
        // initialized data
        void *p_element = NULL;

        // unpack the element
        written += pfn_element(&p_element, p_stream);

        // add the element to the tuple
        p_tuple->_p_elements[i] = p_element;
    }

    // return the pointer to the caller
    *pp_tuple = p_tuple;

    // success
    return written;
    
    // error handling
    {
        
        // argument errors
        {
            no_tuple:
                #ifndef NDEBUG
                    printf("[tuple] Null pointer provided for parameter \"pp_tuple\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;

            no_stream:
                #ifndef NDEBUG
                    printf("[tuple] Null pointer provided for parameter \"p_stream\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;

            no_unpack:
                #ifndef NDEBUG
                    printf("[tuple] Null pointer provided for parameter \"pfn_element\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }

        // tuple errors
        {
            failed_to_construct_tuple:
                #ifndef NDEBUG
                    log_error("[tuple] Call to \"tuple_construct\" returned an erroneous value in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
    }
}

hash64 tuple_hash ( tuple *p_tuple, fn_hash64 *pfn_element )
{

    // argument check
    if ( NULL == p_tuple ) goto no_tuple;

    // initialized data
    hash64     result     = 0;
    fn_hash64 *pfn_hash64 = (pfn_element) ? pfn_element : hash_crc64;

    // iterate through each element in the tuple
    for (size_t i = 0; i < p_tuple->element_count; i++)
        result ^= pfn_hash64(p_tuple->_p_elements[i], 8);

    // success
    return result;

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

int tuple_destroy ( tuple **pp_tuple, fn_allocator *pfn_allocator )
{

    // argument check
    if ( NULL == pp_tuple ) goto no_tuple;

    // initialized data
    tuple *p_tuple = *pp_tuple;

    // no more pointer for end user
    *pp_tuple = NULL;

    // release the elements
    if ( pfn_allocator )
        for (size_t i = 0; i < p_tuple->element_count; i++)
            pfn_allocator(p_tuple->_p_elements[i], 0);
    
    // release the tuple
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
