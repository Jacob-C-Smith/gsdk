/** !
 * set implementation
 *
 * @file src/data/set/set.c
 *
 * @author Jacob Smith
 */

// headers
#include <data/set.h>

// forward declarations
static fn_it_done set_iterator_done;
static fn_it_next set_iterator_next;
static fn_it_item set_iterator_item;

// structure definitions
struct set_s
{
    void          **elements;
    size_t          max;
    size_t          count;
    fn_comparator  *pfn_comparator;
    mutex           _lock;
};

// function definitions
int set_create ( set **const pp_set )
{

    // argument check
    if ( NULL == pp_set ) goto no_set;

    // initialized data
    set *p_set = default_allocator(0, sizeof(set));

    // error checking
    if ( NULL == p_set ) goto no_mem;

    // initialize data
    memset(p_set, 0, sizeof(set));

    // return the allocated memory
    *pp_set = p_set;

    // success
    return 1;

    // error handling
    {

        // argument errors
        {
            no_set:
                #ifndef NDEBUG
                    log_error("[set] Null pointer provided for parameter \"pp_set\" in call to function \"%s\"\n", __FUNCTION__);
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

int set_construct ( set **const pp_set, size_t size, fn_comparator *pfn_comparator )
{

    // argument check
    if ( NULL == pp_set ) goto no_set;
    if ( 0    ==   size ) goto no_size;
    
    // initialized data
    set *p_set = NULL;

    // allocate the set
    if ( 0 == set_create(&p_set) ) goto failed_to_allocate_set;

    // set the maximum number of elements in the set
    p_set->max = size;

    // allocate memory for set elements
    p_set->elements = default_allocator(0, size * sizeof(void *));

    // error checking
    if ( NULL == p_set->elements ) goto no_mem;

    // create a mutex
    mutex_create(&p_set->_lock);

    // if the caller supplied a function for testing equivalence ...
    if ( pfn_comparator )
        
        // ... set the function
        p_set->pfn_comparator = pfn_comparator;
    
    // default to comparator
    else
        p_set->pfn_comparator = default_comparator;

    // return a pointer to the caller
    *pp_set = p_set;

    // success
    return 1;

    // error handling
    {

        // argument errors
        {
            no_set:
                #ifndef NDEBUG
                    log_error("[set] Null pointer provided for parameter \"pp_set\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;

            no_size:
                #ifndef NDEBUG
                    log_error("[set] Parameter \"size\" must be greater than zero in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }

        // set errors
        {
            failed_to_allocate_set:
                #ifndef NDEBUG
                    log_error("[set] Call to \"set_create\" returned an erroneous value in call to function \"%s\"\n", __FUNCTION__);
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

int set_from_elements ( set **const pp_set, void **const pp_elements, size_t size, fn_comparator *pfn_comparator )
{

    // argument check
    if ( NULL == pp_set ) goto no_set;

    // initialized data
    set *p_set = NULL;

    // construct a set
    if ( 0 == set_construct(&p_set, size, pfn_comparator) ) goto failed_to_construct_set;

    // iterate over each element
    for (size_t i = 0; i < size; i++)
        
        // add the element to the set
        set_add(p_set, pp_elements[i]);

    // return a pointer to the caller
    *pp_set = p_set;
    
    // success
    return 1;

    // error handling
    {

        // argument errors
        {
            no_set:
                #ifndef NDEBUG
                    log_error("[set] Null pointer provided for parameter \"pp_set\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }

        // set errors
        {
            failed_to_construct_set:
                #ifndef NDEBUG
                    log_error("[set] Call to \"set_from_elements\" returned an erroneous value in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
    }
}

int set_contents ( set *const p_set, void **const pp_contents )
{

    // argument check
    if ( NULL ==       p_set ) goto no_set;
    if ( NULL == pp_contents ) goto return_count;

    // lock
    mutex_lock(&p_set->_lock);

    // copy the elements
    memcpy(pp_contents, p_set->elements, sizeof(void *) * p_set->count);

    // unlock
    mutex_unlock(&p_set->_lock);
    
    // success
    return 1;

    // Count branch
    return_count:

        // success
        return p_set->count;

    // error handling
    {

        // argument errors
        {
            no_set:
                #ifndef NDEBUG
                    log_error("[set] Null pointer provided for parameter \"pp_set\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
    }
}

int set_add ( set *const p_set, void *const p_element )
{

    // argument check
    if ( NULL == p_set ) goto no_set;

    // lock
    mutex_lock(&p_set->_lock);

    // iterate over each element
    for (size_t i = 0; i < p_set->count; i++)
    {

        // if the element is a duplicate ...
        if ( 0 == p_set->pfn_comparator(p_set->elements[i], p_element) )
        {
            
            // ... unlock the mutex 
            mutex_unlock(&p_set->_lock);

            // success
            return 1;
        }
    }
    
    // capacity?
    if ( p_set->count >= p_set->max ) goto set_full;

    // store the element 
    p_set->elements[p_set->count] = p_element;

    // increment the element quantity
    p_set->count++;

    // unlock
    mutex_unlock(&p_set->_lock);
    
    // success
    return 1;

    // error handling
    {

        // argument errors
        {
            no_set:
                #ifndef NDEBUG
                    log_error("[set] Null pointer provided for parameter \"pp_set\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }

        // set errors
        {
            set_full:
                #ifndef NDEBUG
                    log_error("[set] Set is full in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // unlock
                mutex_unlock(&p_set->_lock);

                // error
                return 0;
        }
    }
}

int set_union ( set **const pp_set, const set *const p_a, const set *const p_b)
{

    // argument check
    if ( NULL == pp_set ) goto no_set;
    if ( NULL ==    p_a ) goto no_a;
    if ( NULL ==    p_b ) goto no_b;

    // state check
    if ( p_a->pfn_comparator != p_b->pfn_comparator ) goto incomparable_sets;

    // initialized data
    set    *p_set = 0;
    size_t  size  = p_a->count + p_b->count;
    
    // construct a set
    if ( 0 == set_construct(&p_set, size, p_a->pfn_comparator) ) goto failed_to_construct_set;

    // iterate through set a
    for (size_t i = 0; i < p_a->count; i++)

        // add each element to the new set
        set_add(p_set, p_a->elements[i]);
    
    // iterate through set b
    for (size_t i = 0; i < p_b->count; i++)

        // add each element to the new set
        set_add(p_set, p_b->elements[i]);

    // return a pointer to the caller
    *pp_set = p_set;

    // success
    return 1;

    // error handling
    {

        // argument errors
        {
            no_set:
                #ifndef NDEBUG
                    log_error("[set] Null pointer provided for parameter\"pp_set\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;

            no_a:
                #ifndef NDEBUG
                    log_error("[set] Null pointer provided for parameter\"p_a\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;

            no_b:
                #ifndef NDEBUG
                    log_error("[set] Null pointer provided for parameter\"p_b\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;

        }

        // set errors
        {
            incomparable_sets:
                #ifndef NDEBUG
                    log_error("[set] Sets \"p_a\" and \"p_b\" are of different types in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
                
            failed_to_construct_set:
                #ifndef NDEBUG
                    log_error("[set] Call to \"set_construct\" returned an erroneous value in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
    }
}

int set_difference ( set **const pp_set, const set *const p_a, const set *const p_b )
{
    
    // argument check
    if ( NULL == pp_set ) goto no_set;
    if ( NULL ==    p_a ) goto no_a;
    if ( NULL ==    p_b ) goto no_b;

    // state check
    if ( p_a->pfn_comparator != p_b->pfn_comparator ) goto incomparable_sets;

    // initialized data
    set    *p_set = 0;
    size_t  size  = p_a->count;
    
    // edge case
    if ( 0 == size ) size++;

    // construct a set
    if ( 0 == set_construct(&p_set, size, p_a->pfn_comparator) ) goto failed_to_construct_set;

    // iterate through set a
    for (size_t i = 0; i < p_a->count; i++)
    {
        bool found = false;

        // iterate through set b
        for (size_t j = 0; j < p_b->count; j++)
        {
            // If a[i] is in b
            if ( 0 == p_a->pfn_comparator(p_a->elements[i], p_b->elements[j]) )
            {
                found = true;
                break;
            }
        }

        // if the element was not found in set b ...
        if ( !found )
            
            // ... add it to the result set
            set_add(p_set, p_a->elements[i]);
    }

    // return a pointer to the caller
    *pp_set = p_set;

    // success
    return 1;

    // error handling
    {

        // argument errors
        {
            no_set:
                #ifndef NDEBUG
                    log_error("[set] Null pointer provided for parameter\"pp_set\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;

            no_a:
                #ifndef NDEBUG
                    log_error("[set] Null pointer provided for parameter\"p_a\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;

            no_b:
                #ifndef NDEBUG
                    log_error("[set] Null pointer provided for parameter\"p_b\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;

        }

        // set errors
        {
            incomparable_sets:
                #ifndef NDEBUG
                    log_error("[set] Sets \"p_a\" and \"p_b\" are of different types in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
                
            failed_to_construct_set:
                #ifndef NDEBUG
                    log_error("[set] Call to \"set_construct\" returned an erroneous value in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
    }
}

int set_intersection ( set **const pp_set, const set *const p_a, const set *const p_b )
{

    // argument check
    if ( NULL == pp_set ) goto no_set;
    if ( NULL ==    p_a ) goto no_a;
    if ( NULL ==    p_b ) goto no_b;

    // state check
    if ( p_a->pfn_comparator != p_b->pfn_comparator ) goto incomparable_sets;

    // initialized data
    set    *p_set = NULL;
    size_t  size  = ( p_a->count < p_b->count ) ? p_a->count : p_b->count;
    
    // edge case
    if ( 0 == size ) size++;

    // construct a set
    if ( 0 == set_construct(&p_set, size, p_a->pfn_comparator) ) goto failed_to_construct_set;

    // iterate through set a
    for (size_t i = 0; i < p_a->count; i++)
    {

        // iterate through set b
        for (size_t j = 0; j < p_b->count; j++)
        {
            // a[i] is in b?
            if ( 0 == p_a->pfn_comparator(p_a->elements[i], p_b->elements[j]) )
            {

                // a[i] is in b
                set_add(p_set, p_a->elements[i]);
            }
        }
    }

    // return a pointer to the caller
    *pp_set = p_set;

    // success
    return 1;

    // error handling
    {

        // argument errors
        {
            no_set:
                #ifndef NDEBUG
                    log_error("[set] Null pointer provided for parameter\"pp_set\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;

            no_a:
                #ifndef NDEBUG
                    log_error("[set] Null pointer provided for parameter\"p_a\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;

            no_b:
                #ifndef NDEBUG
                    log_error("[set] Null pointer provided for parameter\"p_b\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;

        }

        // set errors
        {
            incomparable_sets:
                #ifndef NDEBUG
                    log_error("[set] Sets \"p_a\" and \"p_b\" are of different types in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
                
            failed_to_construct_set:
                #ifndef NDEBUG
                    log_error("[set] Call to \"set_construct\" returned an erroneous value in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
    }
}

bool set_isdisjoint ( const set *const p_a, const set *const p_b )
{

    // argument check
    if ( NULL == p_a ) return false;
    if ( NULL == p_b ) return false;

    // state check
    if ( p_a->pfn_comparator != p_b->pfn_comparator ) return false;

    // iterate through set a
    for (size_t i = 0; i < p_a->count; i++)
    {

        // iterate through set b
        for (size_t j = 0; j < p_b->count; j++)
        {

            // a[i] is in b?
            if ( 0 == p_a->pfn_comparator(p_a->elements[i], p_b->elements[j]) )
            {
                // error
                return false;
            }
        }
    }

    // success
    return true;
}

bool set_issubset ( const set *const p_a, const set *const p_b )
{

    // argument check
    if ( NULL == p_a ) return false;
    if ( NULL == p_b ) return false;

    // state check
    if ( p_a->pfn_comparator != p_b->pfn_comparator ) return false;

    // fast exit
    if ( p_a->count > p_b->count ) return false;

    // iterate through set a
    for (size_t i = 0; i < p_a->count; i++)
    {

        // initialized data
        bool found = false;

        // iterate through set b
        for (size_t j = 0; j < p_b->count; j++)
        {
            
            // a[i] is in b?
            if ( 0 == p_a->pfn_comparator(p_a->elements[i], p_b->elements[j]) )
            {

                // set the found flag
                found = true;

                // done
                break;
            }
        }

        // a not in b?
        if ( !found )

            // error
            return false;
    }

    // success
    return true;
}

bool set_issuperset ( const set *const p_a, const set *const p_b )
{
    
    // done
    return set_issubset(p_b, p_a);
}

size_t set_count ( const set *const p_set )
{
    
    // argument check
    if ( NULL == p_set ) goto no_set;

    // return
    return p_set->count;

    // error handling
    {

        // argument errors
        {
            no_set:
                #ifndef NDEBUG
                    log_error("[set] Null pointer provided for parameter \"p_set\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
    }
}

int set_pop ( set *const p_set, void **const pp_value )
{
    
    // argument check
    if ( NULL == p_set ) goto no_set;

    // lock
    mutex_lock(&p_set->_lock);

    // state check
    if ( 0 == p_set->count ) goto set_empty;

    // decrement the quantity of elements in the set
    p_set->count--;

    // return the value to the caller
    if ( pp_value ) 
        *pp_value = p_set->elements[p_set->count];

    // zero set the pop()'d element
    p_set->elements[p_set->count] = (void *)0;

    // unlock 
    mutex_unlock(&p_set->_lock);

    // success
    return 1;

    // error handling
    {

        // argument errors
        {
            no_set:
                #ifndef NDEBUG
                    log_error("[set] Null pointer provided for parameter \"pp_set\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }

        // set errors
        {
            set_empty:
                #ifndef NDEBUG
                    log_error("[set] Set is empty in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // unlock
                mutex_unlock(&p_set->_lock);

                // error
                return 0;
        }
    }
}

int set_remove ( set *const p_set , void *const p_element )
{
    
    // argument check
    if ( NULL == p_set ) goto no_set;

    // lock
    mutex_lock(&p_set->_lock);

    // iterate over each element
    for (size_t i = 0; i < p_set->count; i++)
    {

        // duplicate?
        if ( 0 == p_set->pfn_comparator(p_set->elements[i], p_element) )
        {
            
            // decrement the count
            p_set->count--;

            // shift the element
            p_set->elements[i] = p_set->elements[p_set->count];

            // zero the element
            p_set->elements[p_set->count] = NULL;

            // unlock 
            mutex_unlock(&p_set->_lock);

            // success
            return 1;
        }
    }
    
    // unlock
    mutex_unlock(&p_set->_lock);
    
    // error
    return 0;

    // error handling
    {

        // argument errors
        {
            no_set:
                #ifndef NDEBUG
                    log_error("[set] Null pointer provided for parameter \"pp_set\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
    }
}

int set_foreach_i ( set *const p_set, void (*const function)(void *const value, size_t index) )
{

    // argument check
    if ( NULL ==    p_set ) goto no_set;
    if ( NULL == function ) goto no_free_func;

    // lock
    mutex_lock(&p_set->_lock);

    // iterate over each element in the set
    for (size_t i = 0; i < p_set->count; i++)
        
        // call the function
        function(p_set->elements[i], i);

    // unlock
    mutex_unlock(&p_set->_lock);

    // success
    return 1;

    // error handling
    {
        
        // argument errors
        {
            no_set:
                #ifndef NDEBUG
                    log_error("[set] Null pointer provided for \"p_set\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
            
            no_free_func:
                #ifndef NDEBUG
                    log_error("[set] Null pointer provided for \"function\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
    }
}

iterator set_iterator ( set *p_set )
{

    // success
    return (iterator)
    {
        .p_data = p_set,
        .state  = { 0 },
        .done   = set_iterator_done,
        .next   = set_iterator_next,
        .item   = set_iterator_item
    };
}

static bool set_iterator_done ( iterator *p_iterator ) 
{

    // done?
    return ((size_t)p_iterator->state.p_state) >= ((set *) p_iterator->p_data)->count; 
}

static void set_iterator_next ( iterator *p_iterator ) 
{

    // update the state
    p_iterator->state.p_state = (void *)((size_t)p_iterator->state.p_state + 1); 

    // done
    return;
}

static void *set_iterator_item ( iterator *p_iterator ) 
{

    // done
    return ((set *) p_iterator->p_data)->elements[(size_t)p_iterator->state.p_state]; 
}

int set_pack ( stream *p_stream, set *p_set, fn_pack *pfn_element )
{
    
    // argument check
    if ( NULL ==    p_stream ) goto no_stream;
    if ( NULL ==       p_set ) goto no_set;
    if ( NULL == pfn_element ) goto no_pack;

    // initialized data 
    size_t written = 0;

    // lock
    mutex_lock(&p_set->_lock);

    // pack the length
    written += pack_pack(p_stream, "%i64", p_set->count);

    // iterate through the set
    for (size_t i = 0; i < p_set->count; i++)
        written += pfn_element(p_stream, p_set->elements[i]);

    // unlock
    mutex_unlock(&p_set->_lock);

    // success
    return written;

    // error handling
    {
        
        // argument errors
        {
            no_set:
                #ifndef NDEBUG
                    log_error("[set] Null pointer provided for \"p_set\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;

            no_stream:
                #ifndef NDEBUG
                    log_error("[set] Null pointer provided for \"p_stream\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;

            no_pack:
                #ifndef NDEBUG
                    log_error("[set] Null pointer provided for \"pfn_element\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
    }
}

int set_unpack ( set **pp_set, stream *p_stream, fn_unpack *pfn_element, fn_comparator *pfn_comparator )
{
    
    // argument check
    if ( NULL ==       pp_set ) goto no_set;
    if ( NULL ==     p_stream ) goto no_stream;
    if ( NULL ==  pfn_element ) goto no_unpack;

    // initialized data
    set    *p_set = NULL;
    size_t  read  = 0;
    size_t  len   = 0;

    // unpack the length
    read += pack_unpack(p_stream, "%i64", &len);

    // construct a set
    set_construct(&p_set, len, pfn_comparator);

    for (size_t i = 0; i < len; i++)
    {
        
        // initialized data
        void *p_element = NULL;
        
        // unpack the element
        read += pfn_element(&p_element, p_stream);

        // add the element to the set
        set_add(p_set, p_element);
    }

    // return the set to the caller
    *pp_set = p_set;

    // success
    return read;
    
    // error handling
    {
        
        // argument errors
        {
            no_set:
                #ifndef NDEBUG
                    log_error("[set] Null pointer provided for \"pp_set\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
            
            no_stream:
                #ifndef NDEBUG
                    log_error("[set] Null pointer provided for \"p_stream\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
            
            no_unpack:
                #ifndef NDEBUG
                    log_error("[set] Null pointer provided for \"pfn_element\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
    }
}

hash64 set_hash ( set *p_set, fn_hash64 *pfn_element )
{

    // argument check
    if ( NULL == p_set ) goto no_set;

    // initialized data
    hash64     result     = 0;
    fn_hash64 *pfn_hash64 = (pfn_element) ? pfn_element : hash_crc64;

    // lock
    mutex_lock(&p_set->_lock);

    // iterate through each element in the set
    for (size_t i = 0; i < p_set->count; i++)
        result ^= pfn_hash64(p_set->elements[i], 8);

    // unlock
    mutex_unlock(&p_set->_lock);

    // success
    return result;

    // error handling
    {

        // argument errors
        {
            no_set:
                #ifndef NDEBUG
                    log_error("[set] Null pointer provided for \"pp_set\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
    }
}

int set_destroy ( set **const pp_set, fn_allocator *pfn_allocator )
{
    
    // argument check
    if ( NULL == pp_set ) goto no_set;

    // initialized data
    set *p_set = *pp_set;

    // fast exit
    if ( NULL == p_set ) return 1;

    // no more set for caller
    *pp_set = (void *) 0;

    // lock
    mutex_lock(&p_set->_lock);

    // release the elements?
    if ( pfn_allocator )

        // release the elements
        for (size_t i = 0; i < p_set->count; i++)

            // release the element
            p_set->elements[i] = pfn_allocator(p_set->elements[i], 0);
    
    // release the elements
    p_set->elements = default_allocator(p_set->elements, 0); 

    // unlock
    mutex_unlock(&p_set->_lock);

    // release the lock
    mutex_destroy(&p_set->_lock);

    // release the set
    p_set = default_allocator(p_set, 0);
    
    // success
    return 1;

    // error handling
    {

        // argument errors
        {
            no_set: 
                #ifndef NDEBUG
                    log_error("[set] Null pointer provided for parameter \"pp_set\" in call to function \"%s\"\n", __FUNCTION__);
                #endif  

                // error
                return 0;
        }
    }
}
