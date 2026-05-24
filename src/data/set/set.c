/** !
 * set library
 *
 * @file src/data/set/set.c
 *
 * @author Jacob Smith
 */

// headers
#include <data/set.h>

fn_it_done set_iterator_done;
fn_it_next set_iterator_next;
fn_it_item set_iterator_item;

// structure definitions
struct set_s
{
    void         **elements;
    size_t         max;
    size_t         count;
    fn_equality  *pfn_equality;
    mutex          _lock;
};


// allocaters
/** !
 *  Allocate memory for a set
 *
 * @param pp_set return
 *
 * @sa set_destroy
 *
 * @return 1 on success, 0 on error
 */
int set_create ( set **const pp_set );

// function definitions
int set_create ( set **const pp_set )
{

    // argument check
    if ( pp_set == (void *) 0 ) goto no_set;

    // initialized data
    set *p_set = default_allocator(0, sizeof(set));

    // error checking
    if ( p_set == (void *) 0 ) goto no_mem;

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
                    printf("[set] Null pointer provided for parameter \"pp_set\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }

        // standard library errors
        {
            no_mem:
                #ifndef NDEBUG
                    printf("[standard library] Failed to allocate memory in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
    }
}

int set_construct ( set **const pp_set, size_t size, fn_equality *pfn_equality )
{

    // argument check
    if ( pp_set == (void *) 0 ) goto no_set;

    // initialized data
    set *p_set = (void *) 0;

    // allocate the set
    if ( set_create(&p_set) == 0 ) goto failed_to_allocate_set;

    // set the maximum number of elements in the set
    p_set->max = size;

    // allocate memory for set elements
    p_set->elements = default_allocator(0, size * sizeof(void *));

    // error checking
    if ( p_set->elements == (void *) 0 ) goto no_mem;

    // create a mutex
    mutex_create(&p_set->_lock);

    // if the caller supplied a function for testing equivalence ...
    if ( pfn_equality )
        
        // ... set the function
        p_set->pfn_equality = pfn_equality;
    
    // default to '==' for comparing elements
    else
        p_set->pfn_equality = default_equality;

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
                    printf("[set] Null pointer provided for parameter \"pp_set\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }

        // Set errors
        {
            failed_to_allocate_set:
                #ifndef NDEBUG
                    printf("[set] Call to \"set_create\" returned an erroneous value in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }

        // standard library errors
        {
            no_mem:
                #ifndef NDEBUG
                    printf("[standard library] Failed to allocate memory in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
    }
}

int set_from_elements ( set **const pp_set, void **const pp_elements, size_t size, fn_equality *pfn_equality )
{

    // argument check
    if ( pp_set == (void *) 0 ) goto no_set;

    // initialized data
    set *p_set = (void *) 0;

    // construct a set
    if ( set_construct(&p_set, size, pfn_equality) == 0 ) goto failed_to_construct_set;

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
                    printf("[set] Null pointer provided for parameter \"pp_set\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }

        // Set errors
        {
            failed_to_construct_set:
                #ifndef NDEBUG
                    printf("[set] Call to \"set_from_elements\" returned an erroneous value in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
    }
}

int set_contents ( set *const p_set, void **const pp_contents )
{

    // argument check
    if ( p_set == (void *) 0 ) goto no_set;

    // count 
    if ( pp_contents == (void *) 0 ) goto return_count;

    // lock
    mutex_lock(&p_set->_lock);

    // Copy the elements
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
                    printf("[set] Null pointer provided for parameter \"pp_set\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
    }
}

int set_add ( set *const p_set, void *const p_element )
{

    // argument check
    if ( p_set == (void *) 0 ) goto no_set;

    // lock
    mutex_lock(&p_set->_lock);

    // iterate over each element
    for (size_t i = 0; i < p_set->count; i++)
    {

        // if the element is a duplicate ...
        if ( p_set->pfn_equality(p_set->elements[i], p_element) == 0 )
        {
            
            // ... unlock the mutex 
            mutex_unlock(&p_set->_lock);

            // success
            return 1;
        }
    }
    
    // store the element 
    p_set->elements[p_set->count] = p_element;

    // Increment the element quantity
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
                    printf("[set] Null pointer provided for parameter \"pp_set\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
    }
}

int set_union ( set **const pp_set, const set *const p_a, const set *const p_b)
{

    // argument check
    if ( pp_set == (void *) 0 ) goto no_set;
    if ( p_a    == (void *) 0 ) goto no_a;
    if ( p_b    == (void *) 0 ) goto no_b;

    // state check
    if ( p_a->pfn_equality != p_b->pfn_equality ) goto incomparable_sets;

    // initialized data
    set    *p_set        = 0;
    size_t  max_set_size = p_a->count + p_b->count;
    
    // construct a set
    if ( set_construct(&p_set, max_set_size, p_a->pfn_equality) == 0 ) goto failed_to_construct_set;

    // iterate through set a
    for (size_t i = 0; i < p_a->count; i++)

        // Add each element to the new set
        set_add(p_set, p_a->elements[i]);
    
    // iterate through set b
    for (size_t i = 0; i < p_b->count; i++)

        // Add each element to the new set
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
                    printf("[set] Null pointer provided for parameter\"pp_set\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;

            no_a:
                #ifndef NDEBUG
                    printf("[set] Null pointer provided for parameter\"p_a\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;

            no_b:
                #ifndef NDEBUG
                    printf("[set] Null pointer provided for parameter\"p_b\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;

        }

        // set errors
        {
            incomparable_sets:
                #ifndef NDEBUG
                    printf("[set] Sets \"p_a\" and \"p_b\" are of different types in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
                
            failed_to_construct_set:
                #ifndef NDEBUG
                    printf("[set] Call to \"set_construct\" returned an erroneous value in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
    }
}

int set_difference ( set **const pp_set, const set *const p_a, const set *const p_b )
{
    
    // argument check
    if ( pp_set == (void *) 0 ) goto no_set;
    if ( p_a    == (void *) 0 ) goto no_a;
    if ( p_b    == (void *) 0 ) goto no_b;

    // state check
    if ( p_a->pfn_equality != p_b->pfn_equality ) goto incomparable_sets;

    // initialized data
    set    *p_set        = 0;
    size_t  max_set_size = p_a->count + p_b->count;
    
    // Construct a set
    if ( set_construct(&p_set, max_set_size, p_a->pfn_equality) == 0 ) goto failed_to_construct_set;

    // iterate through set a
    for (size_t i = 0; i < p_a->count; i++)

        // Add each element to the new set
        set_add(p_set, p_a->elements[i]);
    
    // iterate through set b
    for (size_t i = 0; i < p_b->count; i++)

        // Add each element to the new set
        set_add(p_set, p_b->elements[i]);

    // iterate through set a
    for (size_t i = 0; i < p_a->count; i++)
    {

        // iterate through set b
        for (size_t j = 0; j < p_b->count; j++)
        {
            // If a[i] is in b
            if ( p_set->pfn_equality(p_a->elements[i], p_b->elements[j]) == 0 )
            {

                // a[i] is in b
                set_remove(p_set, p_a->elements[i]);
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
                    printf("[set] Null pointer provided for parameter\"pp_set\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;

            no_a:
                #ifndef NDEBUG
                    printf("[set] Null pointer provided for parameter\"p_a\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;

            no_b:
                #ifndef NDEBUG
                    printf("[set] Null pointer provided for parameter\"p_b\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;

        }

        // Set errors
        {
            incomparable_sets:
                #ifndef NDEBUG
                    printf("[set] Sets \"p_a\" and \"p_b\" are of different types in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
                
            failed_to_construct_set:
                #ifndef NDEBUG
                    printf("[set] Call to \"set_construct\" returned an erroneous value in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
    }
}

int set_intersection ( set **const pp_set, const set *const p_a, const set *const p_b )
{

    // argument check
    if ( pp_set == (void *) 0 ) goto no_set;
    if ( p_a    == (void *) 0 ) goto no_a;
    if ( p_b    == (void *) 0 ) goto no_b;

    // state check
    if ( p_a->pfn_equality != p_b->pfn_equality ) goto incomparable_sets;

    // initialized data
    set    *p_set        = 0;
    size_t  max_set_size = p_a->count + p_b->count;
    
    // Construct a set
    if ( set_construct(&p_set, max_set_size, p_a->pfn_equality) == 0 ) goto failed_to_construct_set;

    // iterate through set a
    for (size_t i = 0; i < p_a->count; i++)
    {

        // iterate through set b
        for (size_t j = 0; j < p_b->count; j++)
        {
            // If a[i] is in b
            if ( p_set->pfn_equality(p_a->elements[i], p_b->elements[j]) == 0 )
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
                    printf("[set] Null pointer provided for parameter\"pp_set\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;

            no_a:
                #ifndef NDEBUG
                    printf("[set] Null pointer provided for parameter\"p_a\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;

            no_b:
                #ifndef NDEBUG
                    printf("[set] Null pointer provided for parameter\"p_b\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;

        }

        // Set errors
        {
            incomparable_sets:
                #ifndef NDEBUG
                    printf("[set] Sets \"p_a\" and \"p_b\" are of different types in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
                
            failed_to_construct_set:
                #ifndef NDEBUG
                    printf("[set] Call to \"set_construct\" returned an erroneous value in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
    }
}

size_t set_count ( const set *const p_set )
{
    
    // argument check
    if ( p_set == (void *) 0 ) goto no_set;

    // return
    return p_set->count;

    // error handling
    {

        // argument errors
        {
            no_set:
                #ifndef NDEBUG
                    printf("[set] Null pointer provided for parameter \"p_set\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
    }
}

int set_pop ( set *const p_set, void **const pp_value )
{
    
    // argument check
    if ( p_set == (void *) 0 ) goto no_set;

    // lock
    mutex_lock(&p_set->_lock);

    // Decrement the quantity of elements in the set
    p_set->count--;

    // return the value to the caller
    *pp_value = p_set->elements[p_set->count];

    // Zero set the pop()'d element
    p_set->elements[p_set->count] = (void *)0;

    // ... unlock the mutex 
    mutex_unlock(&p_set->_lock);

    // success
    return 1;

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
                    printf("[set] Null pointer provided for parameter \"pp_set\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
    }
}

int set_remove ( set *const p_set , void *const p_element )
{
    
    // argument check
    if ( p_set == (void *) 0 ) goto no_set;

    // lock
    mutex_lock(&p_set->_lock);

    // iterate over each element
    for (size_t i = 0; i < p_set->count; i++)
    {

        // If the element is a duplicate ...
        if ( p_set->pfn_equality(p_set->elements[i], p_element) == 0 )
        {
            
            p_set->count--;

            p_set->elements[i] = p_set->elements[p_set->count];

            p_set->elements[p_set->count] = (void *) 0;

            // ... unlock the mutex 
            mutex_unlock(&p_set->_lock);

            // success
            return 1;
        }
    }
    
    // store the element 
    p_set->elements[p_set->count] = p_element;

    // Increment the element quantity
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
                    printf("[set] Null pointer provided for parameter \"pp_set\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
    }
}

int set_foreach_i ( set *const p_set, void (*const function)(void *const value, size_t index) )
{

    // argument check
    if ( p_set    == (void *) 0 ) goto no_set;
    if ( function == (void *) 0 ) goto no_free_func;

    // iterate over each element in the set
    for (size_t i = 0; i < p_set->count; i++)
        
        // Call the function
        function(p_set->elements[i], i);

    // success
    return 1;

    // error handling
    {
        
        // argument errors
        {
            no_set:
                #ifndef NDEBUG
                    printf("[set] Null pointer provided for \"p_set\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
            
            no_free_func:
                #ifndef NDEBUG
                    printf("[array] Null pointer provided for \"function\" in call to function \"%s\"\n", __FUNCTION__);
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

bool set_iterator_done ( iterator *p_iterator ) 
{

    // done?
    return ((size_t)p_iterator->state.p_state) >= ((set *) p_iterator->p_data)->count; 
}

void set_iterator_next ( iterator *p_iterator ) 
{

    // update the state
    p_iterator->state.p_state = (void *)((size_t)p_iterator->state.p_state + 1); 

    // done
    return;
}

void *set_iterator_item ( iterator *p_iterator ) 
{

    // done
    return ((set *) p_iterator->p_data)->elements[(size_t)p_iterator->state.p_state]; 
}

int set_pack ( stream *p_stream, set *p_set, fn_pack *pfn_element )
{
    
    // argument check
    if ( p_stream      == (void *) 0 ) return 0;
    if ( p_set         == (void *) 0 ) goto no_set;
    if ( pfn_element   == (void *) 0 ) return 0;

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
        }
    }
}

int set_unpack ( set **pp_set, stream *p_stream, fn_unpack *pfn_element, fn_equality *pfn_equality )
{
    
    // argument check
    if ( pp_set      == (void *) 0 ) goto no_set;
    if ( p_stream    == (void *) 0 ) return 0;
    if ( pfn_element == (void *) 0 ) return 0;

    // initialized data
    set *p_set = NULL;
    size_t written = 0;
    size_t len = 0;

    // unpack the length
    written += pack_unpack(p_stream, "%i64", &len);

    // construct a set
    set_construct(&p_set, len, pfn_equality);

    for (size_t i = 0; i < len; i++)
    {
        
        // initialized data
        void *p_element = NULL;
        
        // unpack the element
        written += pfn_element(&p_element, p_stream);

        // add the element to the set
        set_add(p_set, p_element);
    }

    // return the set to the caller
    *pp_set = p_set;

    // success
    return written;
    
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

hash64 set_hash ( set *p_set, fn_hash64 *pfn_element )
{

    // argument check
    if ( p_set == (void *) 0 ) goto no_set;

    // initialized data
    hash64     result     = 0;
    fn_hash64 *pfn_hash64 = (pfn_element) ? pfn_element : hash_crc64;

    // iterate through each element in the set
    for (size_t i = 0; i < p_set->count; i++)
        result ^= pfn_hash64(p_set->elements[i], 8);

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

int set_destroy ( set **const pp_set )
{
    
    // argument check
    if ( pp_set == (void *) 0 ) goto no_set;

    // initialized data
    set *p_set = *pp_set;

    // No more set for caller
    *pp_set = (void *) 0;

    // lock the mutex
    mutex_lock(&p_set->_lock);

    // Free the set elements
    (void)default_allocator(p_set->elements, 0); 

    // Destroy the lock
    mutex_destroy(&p_set->_lock);
    
    // success
    return 1;

    // error handling
    {

        // argument errors
        {
            no_set: 
                #ifndef NDEBUG
                    printf("[set] Null pointer provided for parameter \"pp_set\" in call to function \"%s\"\n", __FUNCTION__);
                #endif  

                // error
                return 0;
        }
    }
}
