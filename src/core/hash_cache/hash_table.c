// Header
#include <core/hash_table.h>

// Function declarations
/** !
 * Is an integer a prime number?
 * 
 * @param n the integer 
 * 
 * @return true if prime else false
 */
bool hash_table_is_prime ( size_t n );

/** !
 * Generate a twin prime within a range
 * 
 * @param start the lower bound of the range
 * @param end   the upper bound of the range
 * 
 * @return the prime number
 */
size_t hash_table_generate_twin_prime ( size_t start, size_t end );

/** !
 * Compute the positive modulo of some numbers
 * 
 * @param dividend the dividend
 * @param divisor  the divisor
 * 
 * @return the positive modulo of the dividend and the divisor
 */
signed hash_table_positive_mod ( signed dividend, signed divisor );

// Function definitions
int hash_table_create ( hash_table **const pp_hash_table )
{

    // Argument check
    if ( pp_hash_table == (void *) 0 ) goto no_hash_table;

    // Initialized data
    hash_table *p_hash_table = HASH_CACHE_REALLOC(0, sizeof(hash_table));

    // Error check
    if (p_hash_table == (void *) 0 ) goto no_mem;

    // Initialize memory
    memset(p_hash_table, 0, sizeof(hash_table));

    // Return a pointer to the caller
    *pp_hash_table = p_hash_table;

    // Success
    return 1;

    // Error handling
    {

        // Argument errors
        {
            no_hash_table:
                #ifndef NDEBUG
                    log_error("[hash cache] [hash table] Null pointer provided for parameter \"pp_hash_table\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }

        // standard library errors
        {
            no_mem:
                #ifndef NDEBUG
                    log_error("[hash cache] Failed to allocate memory in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }
    }
}

int hash_table_construct ( hash_table **const pp_hash_table, size_t size )
{

    // Argument check
    if ( pp_hash_table == (void *) 0 ) goto no_hash_table;

    // Initialized data
    hash_table *p_hash_table = (void *) 0;

    // Unused
    (void)size;

    // Allocate memory for a hash table
    if ( hash_table_create(&p_hash_table) == 0 ) goto failed_to_allocate_hash_table;

    //

    // Success
    return 1;

    // Error handling
    {

        // Argument errors
        {
            no_hash_table:
                #ifndef NDEBUG
                    log_error("[hash cache] [hash table] Null pointer provided for parameter \"pp_hash_table\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }

        // Hash table errors
        {
            failed_to_allocate_hash_table:
                #ifndef NDEBUG
                    log_error("[hash cache] [hash table] Failed to allocate memory for hash table in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }

        // // standard library errors
        // {
        //     no_mem:
        //         #ifndef NDEBUG
        //             log_error("[hash cache] Failed to allocate memory in call to function \"%s\"\n", __FUNCTION__);
        //         #endif

        //         // Error
        //         return 0;
        // }
    }
}

int hash_table_search ( hash_table *const p_hash_table, void *p_key, void **pp_value )
{

    // Initialized data
    size_t i = 0, q = 0;

    // Repeat this block
    do 
    {

        // Compute the hash
        q = p_hash_table->pfn_hash_function(p_key, p_hash_table->properties.length,i);

        // If there is a property stored in this slot ...
        if ( p_hash_table->properties.pp_data[q] != (void *) 0 ) 
        {

            // ... and the property is what the caller asked for ...
            if ( p_hash_table->pfn_equality(p_hash_table->properties.pp_data[q], p_key) == 0 )
            {

                // ... return a pointer to the caller 
                *pp_value = p_hash_table->properties.pp_data[q];

                // Success
                return 1;
            }
        }
    }

    // Continuation condition
    while ( p_hash_table->properties.pp_data[q] != (void *) 0 && i != p_hash_table->properties.max );

    // Error
    return 0;

}

int hash_table_insert ( hash_table *const p_hash_table, void *property )
{

    // Argument check
    if ( p_hash_table == (void *) 0 ) goto no_hash_table;
    
    // Initialized data
    size_t i = 0;

    // Repeat this block
    do
    {
        
        // Initialized data
        size_t q = p_hash_table->pfn_hash_function(p_hash_table->pfn_key_get(property), p_hash_table->properties.length, i);

        // If this slot is empty ...
        if ( p_hash_table->properties.pp_data[q] == (void *) 0 )
        {

            // ... store the property ...
            p_hash_table->properties.pp_data[q] = property;

            // Success
            return 1;
        }

        // Increment the counter
        i++;
    }
    
    // Continuation condition
    while (i != p_hash_table->properties.max);
        
    // Error
    return 0;
    
    // Error handling
    {

        // Argument errors
        {
            no_hash_table:
                #ifndef NDEBUG
                    log_error("[hash cache] [hash table] Null pointer provided for parameter \"p_hash_table\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }
    }
}

int hash_table_for_i ( const hash_table *const p_hash_table, fn_hash_cache_property_i pfn_function )
{
    
    // Argument check
    if ( p_hash_table == (void *) 0 ) goto no_hash_table;
    if ( pfn_function == (void *) 0 ) goto no_function;

    // Iterate through each cell
    for (size_t i = 0; i < p_hash_table->properties.max; i++)
    {
        
        // Initialized data
        void *p_property = p_hash_table->properties.pp_data[i];

        // Skip condition
        if ( p_property == (void *) 0 ) continue;

        // Call the function on this property
        pfn_function(p_property, i);
    }
    
    // Success
    return 1;

    // Error handling
    {

        // Argument errors
        {
            no_hash_table:
                #ifndef NDEBUG
                    log_error("[hash cache] [hash table] Null pointer provided for parameter \"p_hash_table\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;

            no_function:
                #ifndef NDEBUG
                    log_error("[hash cache] [hash table] Null pointer provided for parameter \"pfn_function\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }
    }
}

int hash_table_for_each ( const hash_table *const p_hash_table, fn_hash_cache_property pfn_function )
{
    
    // Argument check
    if ( p_hash_table == (void *) 0 ) goto no_hash_table;
    if ( pfn_function == (void *) 0 ) goto no_function;

    // Iterate through each cell
    for (size_t i = 0; i < p_hash_table->properties.max; i++)
    {
        
        // Initialized data
        void *p_property = p_hash_table->properties.pp_data[i];

        // Skip condition
        if ( p_property == (void *) 0 ) continue;

        // Call the function on this property
        pfn_function(p_property);
    }
    
    // Success
    return 1;

    // Error handling
    {

        // Argument errors
        {
            no_hash_table:
                #ifndef NDEBUG
                    log_error("[hash cache] [hash table] Null pointer provided for parameter \"p_hash_table\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;

            no_function:
                #ifndef NDEBUG
                    log_error("[hash cache] [hash table] Null pointer provided for parameter \"pfn_function\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }
    }
}

int hash_table_clear ( hash_table *p_hash_table, fn_hash_cache_free *pfn_free )
{

    // Argument check
    if ( p_hash_table == (void *) 0 ) goto no_hash_table;

    // Unused
    (void) pfn_free;

    // Success
    return 1;

    // Error handling
    {

        // Argument errors
        {
            no_hash_table:
                #ifndef NDEBUG
                    log_error("[hash cache] [hash table] Null pointer provided for parameter \"p_hash_table\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }
    }
}

int hash_table_destroy ( hash_table **const pp_hash_table, fn_hash_cache_free *pfn_free )
{

    // Argument check
    if ( pp_hash_table == (void *) 0 ) goto no_hash_table;

    // Unused
    (void) pfn_free;

    // Success
    return 1;

    // Error handling
    {

        // Argument errors
        {
            no_hash_table:
                #ifndef NDEBUG
                    log_error("[hash cache] [hash table] Null pointer provided for parameter \"pp_hash_table\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }
    }
}

bool hash_table_is_prime ( size_t n )
{

    // Iterate from 2 to the upper bound
    for (size_t i = 2; i < n; i++)

        // Check for a remainder
        if ( n % i == 0 )

            // Not a prime
            return false;
    
    // A prime
    return true;
}

size_t hash_table_generate_twin_prime ( size_t start, size_t end )
{

    // Iterate through the range
    for (size_t i = start; i < end; i++)
    
        
        // Check the twin prime
        if ( hash_table_is_prime(i) && hash_table_is_prime(i+2) )

            // Success
            return i + 2;
    
    // Error
    return 0;
}

signed hash_table_positive_mod ( signed dividend, signed divisor )
{

    // Initialized data
    signed quotient = dividend % divisor;

    // Success
    return ( quotient < 0 ) ? quotient + divisor : quotient;
}


