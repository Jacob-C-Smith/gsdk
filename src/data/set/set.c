/** !
 * set library
 *
 * @file set.c
 *
 * @author Jacob Smith
 */

// headers
#include <set/set.h>

// Data 
static bool initialized = false;

// structure definitions
struct set_s
{
    void         **elements;
    size_t         max;
    size_t         count;
    set_equal_fn  *pfn_is_equal;
    mutex          _lock;
};

int equals_function ( const void *const a, const void *const b )
{
    
    // return
    return !( a == b );
}

void set_init ( void )
{

    // state check
    if ( initialized == true ) return;

    // Initialize log
    log_init();

    // Initialize log
    sync_init();

    // Set the initialized flag
    initialized = true;

    // done
    return;
}

int set_create ( set **const pp_set )
{

    // argument check
    if ( pp_set == (void *) 0 ) goto no_set;

    // initialized data
    set *p_set = default_allocator(0, sizeof(set));

    // error checking
    if ( p_set == (void *) 0 ) goto no_mem;

    // Zero set
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

int set_construct ( set **const pp_set, size_t size, set_equal_fn *pfn_is_equal )
{

    // argument check
    if ( pp_set == (void *) 0 ) goto no_set;

    // initialized data
    set *p_set = (void *) 0;

    // allocate the set
    if ( set_create(pp_set) == 0 ) goto failed_to_allocate_set;

    // Get a pointer to the allocated set
    p_set = *pp_set;

    // Set the maximum number of elements in the set
    p_set->max = size;

    // allocate memory for set elements
    p_set->elements = default_allocator(0, size * sizeof(void *));

    // error checking
    if ( p_set->elements == (void *) 0 ) goto no_mem;

    // Create a mutex
    mutex_create(&p_set->_lock);

    // If the caller supplied a function for testing equivalence ...
    if ( pfn_is_equal )
        
        // ... set the function
        p_set->pfn_is_equal = pfn_is_equal;
    
    // Default to '==' for comparing elements
    else
        p_set->pfn_is_equal = &equals_function;

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

int set_from_elements ( set **const pp_set, const void **const pp_elements, size_t size, set_equal_fn *pfn_is_equal )
{

    // argument check
    if ( pp_set == (void *) 0 ) goto no_set;

    // initialized data
    set *p_set = (void *) 0;

    // Construct a set
    if ( set_construct(pp_set, size, pfn_is_equal) == 0 ) goto failed_to_construct_set;

    // Get a pointer to the allocated set
    p_set = *pp_set;

    // iterate over each element
    for (size_t i = 0; i < size; i++)
    {
        
        // Add the element to the set
        set_add(p_set, pp_elements[i]);
    }
    
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

int set_contents ( const set *const p_set, void **const pp_contents )
{

    // argument check
    if ( p_set       == (void *) 0 ) goto no_set;

    // Count branch
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

            no_return:
                #ifndef NDEBUG
                    printf("[set] Null pointer provided for parameter \"pp_contents\" in call to function \"%s\"\n", __FUNCTION__);
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

        // If the element is a duplicate ...
        if ( p_set->pfn_is_equal(p_set->elements[i], p_element) == 0 )
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

        // Set errors
        {
            failed_to_construct_set:
                #ifndef NDEBUG
                    printf("[set] Call to \"set_from_elements\" returned an erroneous value in call to function \"%s\"\n", __FUNCTION__);
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

int set_union ( set **const pp_set, const set *const p_a, const set *const p_b, set_equal_fn *pfn_is_equal )
{

    // argument check
    if ( pp_set == (void *) 0 ) goto no_set;
    if ( p_a    == (void *) 0 ) goto no_a;
    if ( p_b    == (void *) 0 ) goto no_b;

    // initialized data
    set    *p_set        = 0;
    size_t  max_set_size = p_a->count + p_b->count;
    
    // Construct a set
    if ( set_construct(&p_set, max_set_size, pfn_is_equal) == 0 ) goto failed_to_construct_set;

    // iterate through set a
    for (size_t i = 0; i < p_a->count; i++)

        // Add each element to the new set
        set_add(p_set, p_a->elements[i]);
    
    // iterate through set b
    for (size_t i = 0; i < p_b->count; i++)

        // Add each element to the new set
        set_add(p_set, p_b->elements[i]);

    // return a pointer to the set to the caller
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
            failed_to_construct_set:
                #ifndef NDEBUG
                    printf("[set] Call to \"set_construct\" returned an erroneous value in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
    }
}

int set_difference ( set **const pp_set, const set *const p_a, const set *const p_b, set_equal_fn *pfn_is_equal )
{
        // argument check
    if ( pp_set == (void *) 0 ) goto no_set;
    if ( p_a    == (void *) 0 ) goto no_a;
    if ( p_b    == (void *) 0 ) goto no_b;

    // initialized data
    set    *p_set        = 0;
    size_t  max_set_size = p_a->count + p_b->count;
    
    // Construct a set
    if ( set_construct(&p_set, max_set_size, pfn_is_equal) == 0 ) goto failed_to_construct_set;

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

        // initialized data
        bool is_a_i_in_b = false;

        // iterate through set b
        for (size_t j = 0; j < p_b->count; j++)
        {
            // If a[i] is in b
            if ( pfn_is_equal(p_a->elements[i], p_b->elements[j]) == 0 )
            {

                // a[i] is in b
                set_remove(p_set, p_a->elements[i]);
            }
        }
    }

    // return a pointer to the set to the caller
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
            failed_to_construct_set:
                #ifndef NDEBUG
                    printf("[set] Call to \"set_construct\" returned an erroneous value in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
    }
}

int set_intersection ( set **const pp_set, const set *const p_a, const set *const p_b, set_equal_fn *pfn_is_equal )
{

    // argument check
    if ( pp_set == (void *) 0 ) goto no_set;
    if ( p_a    == (void *) 0 ) goto no_a;
    if ( p_b    == (void *) 0 ) goto no_b;

    // initialized data
    set    *p_set        = 0;
    size_t  max_set_size = p_a->count + p_b->count;
    
    // Construct a set
    if ( set_construct(&p_set, max_set_size, pfn_is_equal) == 0 ) goto failed_to_construct_set;

    // iterate through set a
    for (size_t i = 0; i < p_a->count; i++)
    {

        // initialized data
        bool is_a_i_in_b = false;

        // iterate through set b
        for (size_t j = 0; j < p_b->count; j++)
        {
            // If a[i] is in b
            if ( pfn_is_equal(p_a->elements[i], p_b->elements[j]) == 0 )
            {

                // a[i] is in b
                set_add(p_set, p_a->elements[i]);
            }
        }
    }

    // return a pointer to the set to the caller
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
        if ( p_set->pfn_is_equal(p_set->elements[i], p_element) == 0 )
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

int set_foreach_i ( const set *const p_set, void (*const function)(void *const value, size_t index) )
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

// TODO: Implement these functions
/*
UNION WAS HERE 
DIFFERENCE WAS HERE 
INTERSECTION WAS HERE 
ADD WAS HERE
void set_discard             ( set        *const p_set , void        *      p_element );
int  set_difference_update   ( set        *const p_a   , const set   *const p_b );
int  set_intersection_update ( set        *const p_a   , const set   *const p_b );
int  set_update              ( set        *const p_a   , const set   *const p_b );
POP WAS HERE
REMOVE WAS HERE 
int  set_clear               ( set        *const p_set );
int  set_free_clear          ( set        *const p_set , void       (*pfn_free_func) );
int  set_copy                ( const set  *const p_set , set        **const pp_set );
*/

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

void set_exit ( void )
{

    // state check
    if ( initialized == false ) return;

    // Clean up sync
    sync_exit();

    // Clean up log
    log_exit();

    // TODO: Anything else?
    //

    // Clear the initialized flag
    initialized = false;

    // done
    return;
}
