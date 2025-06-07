/** !
 * set library
 *
 * @file set.c
 *
 * @author Jacob Smith
 */

// Headers
#include <set/set.h>

// Data 
static bool initialized = false;

// Structure definitions
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
    
    // Return
    return !( a == b );
}

void set_init ( void )
{

    // State check
    if ( initialized == true ) return;

    // Initialize log
    log_init();

    // Initialize log
    sync_init();

    // Set the initialized flag
    initialized = true;

    // Done
    return;
}

int set_create ( set **const pp_set )
{

    // Argument check
    if ( pp_set == (void *) 0 ) goto no_set;

    // Initialized data
    set *p_set = SET_REALLOC(0, sizeof(set));

    // Error checking
    if ( p_set == (void *) 0 ) goto no_mem;

    // Zero set
    memset(p_set, 0, sizeof(set));

    // Return the allocated memory
    *pp_set = p_set;

    // Success
    return 1;

    // Error handling
    {

        // Argument errors
        {
            no_set:
                #ifndef NDEBUG
                    printf("[set] Null pointer provided for parameter \"pp_set\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }

        // standard library errors
        {
            no_mem:
                #ifndef NDEBUG
                    printf("[Standard Library] Failed to allocate memory in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }
    }
}

int set_construct ( set **const pp_set, size_t size, set_equal_fn *pfn_is_equal )
{

    // Argument check
    if ( pp_set == (void *) 0 ) goto no_set;

    // Initialized data
    set *p_set = (void *) 0;

    // Allocate the set
    if ( set_create(pp_set) == 0 ) goto failed_to_allocate_set;

    // Get a pointer to the allocated set
    p_set = *pp_set;

    // Set the maximum number of elements in the set
    p_set->max = size;

    // Allocate memory for set elements
    p_set->elements = SET_REALLOC(0, size * sizeof(void *));

    // Error checking
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

    // Success
    return 1;

    // Error handling
    {

        // Argument errors
        {
            no_set:
                #ifndef NDEBUG
                    printf("[set] Null pointer provided for parameter \"pp_set\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }

        // Set errors
        {
            failed_to_allocate_set:
                #ifndef NDEBUG
                    printf("[set] Call to \"set_create\" returned an erroneous value in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }

        // standard library errors
        {
            no_mem:
                #ifndef NDEBUG
                    printf("[Standard Library] Failed to allocate memory in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }
    }
}

int set_from_elements ( set **const pp_set, const void **const pp_elements, size_t size, set_equal_fn *pfn_is_equal )
{

    // Argument check
    if ( pp_set == (void *) 0 ) goto no_set;

    // Initialized data
    set *p_set = (void *) 0;

    // Construct a set
    if ( set_construct(pp_set, size, pfn_is_equal) == 0 ) goto failed_to_construct_set;

    // Get a pointer to the allocated set
    p_set = *pp_set;

    // Iterate over each element
    for (size_t i = 0; i < size; i++)
    {
        
        // Add the element to the set
        set_add(p_set, pp_elements[i]);
    }
    
    // Success
    return 1;

    // Error handling
    {

        // Argument errors
        {
            no_set:
                #ifndef NDEBUG
                    printf("[set] Null pointer provided for parameter \"pp_set\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }

        // Set errors
        {
            failed_to_construct_set:
                #ifndef NDEBUG
                    printf("[set] Call to \"set_from_elements\" returned an erroneous value in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }

        // standard library errors
        {
            no_mem:
                #ifndef NDEBUG
                    printf("[Standard Library] Failed to allocate memory in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }
    }
}

int set_contents ( const set *const p_set, void **const pp_contents )
{

    // Argument check
    if ( p_set       == (void *) 0 ) goto no_set;

    // Count branch
    if ( pp_contents == (void *) 0 ) goto return_count;


    // Lock
    mutex_lock(&p_set->_lock);

    // Copy the elements
    memcpy(pp_contents, p_set->elements, sizeof(void *) * p_set->count);

    // Unlock
    mutex_unlock(&p_set->_lock);
    
    // Success
    return 1;

    // Count branch
    return_count:

        // Success
        return p_set->count;

    // Error handling
    {

        // Argument errors
        {
            no_set:
                #ifndef NDEBUG
                    printf("[set] Null pointer provided for parameter \"pp_set\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;

            no_return:
                #ifndef NDEBUG
                    printf("[set] Null pointer provided for parameter \"pp_contents\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }
    }
}

int set_add ( set *const p_set, void *const p_element )
{

    // Argument check
    if ( p_set == (void *) 0 ) goto no_set;

    // Lock
    mutex_lock(&p_set->_lock);

    // Iterate over each element
    for (size_t i = 0; i < p_set->count; i++)
    {

        // If the element is a duplicate ...
        if ( p_set->pfn_is_equal(p_set->elements[i], p_element) == 0 )
        {
            
            // ... unlock the mutex 
            mutex_unlock(&p_set->_lock);

            // Success
            return 1;
        }
    }
    
    // Store the element 
    p_set->elements[p_set->count] = p_element;

    // Increment the element quantity
    p_set->count++;

    // Unlock
    mutex_unlock(&p_set->_lock);
    
    // Success
    return 1;

    // Error handling
    {

        // Argument errors
        {
            no_set:
                #ifndef NDEBUG
                    printf("[set] Null pointer provided for parameter \"pp_set\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }

        // Set errors
        {
            failed_to_construct_set:
                #ifndef NDEBUG
                    printf("[set] Call to \"set_from_elements\" returned an erroneous value in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }

        // standard library errors
        {
            no_mem:
                #ifndef NDEBUG
                    printf("[Standard Library] Failed to allocate memory in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }
    }
}

int set_union ( set **const pp_set, const set *const p_a, const set *const p_b, set_equal_fn *pfn_is_equal )
{

    // Argument check
    if ( pp_set == (void *) 0 ) goto no_set;
    if ( p_a    == (void *) 0 ) goto no_a;
    if ( p_b    == (void *) 0 ) goto no_b;

    // Initialized data
    set    *p_set        = 0;
    size_t  max_set_size = p_a->count + p_b->count;
    
    // Construct a set
    if ( set_construct(&p_set, max_set_size, pfn_is_equal) == 0 ) goto failed_to_construct_set;

    // Iterate through set a
    for (size_t i = 0; i < p_a->count; i++)

        // Add each element to the new set
        set_add(p_set, p_a->elements[i]);
    
    // Iterate through set b
    for (size_t i = 0; i < p_b->count; i++)

        // Add each element to the new set
        set_add(p_set, p_b->elements[i]);

    // Return a pointer to the set to the caller
    *pp_set = p_set;

    // Success
    return 1;

    // Error handling
    {

        // Argument errors
        {
            no_set:
                #ifndef NDEBUG
                    printf("[set] Null pointer provided for parameter\"pp_set\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;

            no_a:
                #ifndef NDEBUG
                    printf("[set] Null pointer provided for parameter\"p_a\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;

            no_b:
                #ifndef NDEBUG
                    printf("[set] Null pointer provided for parameter\"p_b\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;

        }

        // Set errors
        {
            failed_to_construct_set:
                #ifndef NDEBUG
                    printf("[set] Call to \"set_construct\" returned an erroneous value in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }
    }
}

int set_difference ( set **const pp_set, const set *const p_a, const set *const p_b, set_equal_fn *pfn_is_equal )
{
        // Argument check
    if ( pp_set == (void *) 0 ) goto no_set;
    if ( p_a    == (void *) 0 ) goto no_a;
    if ( p_b    == (void *) 0 ) goto no_b;

    // Initialized data
    set    *p_set        = 0;
    size_t  max_set_size = p_a->count + p_b->count;
    
    // Construct a set
    if ( set_construct(&p_set, max_set_size, pfn_is_equal) == 0 ) goto failed_to_construct_set;

    // Iterate through set a
    for (size_t i = 0; i < p_a->count; i++)

        // Add each element to the new set
        set_add(p_set, p_a->elements[i]);
    
    // Iterate through set b
    for (size_t i = 0; i < p_b->count; i++)

        // Add each element to the new set
        set_add(p_set, p_b->elements[i]);

    // Iterate through set a
    for (size_t i = 0; i < p_a->count; i++)
    {

        // Initialized data
        bool is_a_i_in_b = false;

        // Iterate through set b
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

    // Return a pointer to the set to the caller
    *pp_set = p_set;

    // Success
    return 1;

    // Error handling
    {

        // Argument errors
        {
            no_set:
                #ifndef NDEBUG
                    printf("[set] Null pointer provided for parameter\"pp_set\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;

            no_a:
                #ifndef NDEBUG
                    printf("[set] Null pointer provided for parameter\"p_a\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;

            no_b:
                #ifndef NDEBUG
                    printf("[set] Null pointer provided for parameter\"p_b\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;

        }

        // Set errors
        {
            failed_to_construct_set:
                #ifndef NDEBUG
                    printf("[set] Call to \"set_construct\" returned an erroneous value in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }
    }
}

int set_intersection ( set **const pp_set, const set *const p_a, const set *const p_b, set_equal_fn *pfn_is_equal )
{

    // Argument check
    if ( pp_set == (void *) 0 ) goto no_set;
    if ( p_a    == (void *) 0 ) goto no_a;
    if ( p_b    == (void *) 0 ) goto no_b;

    // Initialized data
    set    *p_set        = 0;
    size_t  max_set_size = p_a->count + p_b->count;
    
    // Construct a set
    if ( set_construct(&p_set, max_set_size, pfn_is_equal) == 0 ) goto failed_to_construct_set;

    // Iterate through set a
    for (size_t i = 0; i < p_a->count; i++)
    {

        // Initialized data
        bool is_a_i_in_b = false;

        // Iterate through set b
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

    // Return a pointer to the set to the caller
    *pp_set = p_set;

    // Success
    return 1;

    // Error handling
    {

        // Argument errors
        {
            no_set:
                #ifndef NDEBUG
                    printf("[set] Null pointer provided for parameter\"pp_set\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;

            no_a:
                #ifndef NDEBUG
                    printf("[set] Null pointer provided for parameter\"p_a\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;

            no_b:
                #ifndef NDEBUG
                    printf("[set] Null pointer provided for parameter\"p_b\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;

        }

        // Set errors
        {
            failed_to_construct_set:
                #ifndef NDEBUG
                    printf("[set] Call to \"set_construct\" returned an erroneous value in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }
    }
}

size_t set_count ( const set *const p_set )
{
    
    // Argument check
    if ( p_set == (void *) 0 ) goto no_set;

    // Return
    return p_set->count;

    // Error handling
    {

        // Argument errors
        {
            no_set:
                #ifndef NDEBUG
                    printf("[set] Null pointer provided for parameter \"p_set\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }
    }
}

int set_pop ( set *const p_set, void **const pp_value )
{
    
    // Argument check
    if ( p_set == (void *) 0 ) goto no_set;

    // Lock
    mutex_lock(&p_set->_lock);

    // Decrement the quantity of elements in the set
    p_set->count--;

    // Return the value to the caller
    *pp_value = p_set->elements[p_set->count];

    // Zero set the pop()'d element
    p_set->elements[p_set->count] = (void *)0;

    // ... unlock the mutex 
    mutex_unlock(&p_set->_lock);

    // Success
    return 1;

    // Unlock
    mutex_unlock(&p_set->_lock);
    
    // Success
    return 1;

    // Error handling
    {

        // Argument errors
        {
            no_set:
                #ifndef NDEBUG
                    printf("[set] Null pointer provided for parameter \"pp_set\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }

        // standard library errors
        {
            no_mem:
                #ifndef NDEBUG
                    printf("[Standard Library] Failed to allocate memory in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }
    }
}

int set_remove ( set *const p_set , void *const p_element )
{
    
    // Argument check
    if ( p_set == (void *) 0 ) goto no_set;

    // Lock
    mutex_lock(&p_set->_lock);

    // Iterate over each element
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

            // Success
            return 1;
        }
    }
    
    // Store the element 
    p_set->elements[p_set->count] = p_element;

    // Increment the element quantity
    p_set->count++;

    // Unlock
    mutex_unlock(&p_set->_lock);
    
    // Success
    return 1;

    // Error handling
    {

        // Argument errors
        {
            no_set:
                #ifndef NDEBUG
                    printf("[set] Null pointer provided for parameter \"pp_set\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }

        // standard library errors
        {
            no_mem:
                #ifndef NDEBUG
                    printf("[Standard Library] Failed to allocate memory in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }
    }
}

int set_foreach_i ( const set *const p_set, void (*const function)(void *const value, size_t index) )
{

    // Argument check
    if ( p_set    == (void *) 0 ) goto no_set;
    if ( function == (void *) 0 ) goto no_free_func;

    // Iterate over each element in the set
    for (size_t i = 0; i < p_set->count; i++)
        
        // Call the function
        function(p_set->elements[i], i);

    // Success
    return 1;

    // Error handling
    {
        
        // Argument errors
        {
            no_set:
                #ifndef NDEBUG
                    printf("[set] Null pointer provided for \"p_set\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
            
            no_free_func:
                #ifndef NDEBUG
                    printf("[array] Null pointer provided for \"function\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
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
    
    // Argument check
    if ( pp_set == (void *) 0 ) goto no_set;

    // Initialized data
    set *p_set = *pp_set;

    // No more set for caller
    *pp_set = (void *) 0;

    // Lock the mutex
    mutex_lock(&p_set->_lock);

    // Free the set elements
    (void)SET_REALLOC(p_set->elements, 0); 

    // Destroy the lock
    mutex_destroy(&p_set->_lock);
    
    // Success
    return 1;

    // Error handling
    {

        // Argument errors
        {
            no_set: 
                #ifndef NDEBUG
                    printf("[set] Null pointer provided for parameter \"pp_set\" in call to function \"%s\"\n", __FUNCTION__);
                #endif  

                // Error
                return 0;
        }
    }
}

void set_exit ( void )
{

    // State check
    if ( initialized == false ) return;

    // Clean up sync
    sync_exit();

    // Clean up log
    log_exit();

    // TODO: Anything else?
    //

    // Clear the initialized flag
    initialized = false;

    // Done
    return;
}
