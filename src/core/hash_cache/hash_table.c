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
    hash_table *p_hash_table = realloc(0, sizeof(hash_table));

    // Error check
    if (p_hash_table == (void *) 0 ) goto no_mem;

    // Initialize 
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
                    log_error("[hash table] Null pointer provided for parameter \"pp_hash_table\" in call to function \"%s\"\n", __FUNCTION__);
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

int hash_table_construct 
(
    hash_table **const pp_hash_table,
    size_t             size,
    
    fn_equality       *pfn_equality,
    fn_key_accessor   *pfn_key_get,
    fn_hash64         *pfn_hash_function
)
{

    // Unused
    (void) pfn_equality;
    (void) pfn_key_get;
    (void) pfn_hash_function;

    // Argument check
    if ( pp_hash_table == (void *) 0 ) goto no_hash_table;

    // Initialized data
    hash_table *p_hash_table = (void *) 0;

    // Unused
    (void)size;

    // Allocate memory for a hash table
    if ( hash_table_create(&p_hash_table) == 0 ) goto failed_to_allocate_hash_table;

    // Populate the hash table interfaces
    *p_hash_table = (hash_table)
    {
        .pfn_equality      = default_equality,     // default
        .pfn_hash_function = hash_crc64,           // cyclic redundancy check (64-bit)
        .pfn_key_get       = default_key_accessor, // default
    };

    // Populate hash table data
    p_hash_table->properties.count = 0,
    p_hash_table->properties.length = 0,
    p_hash_table->properties.max = size,
    p_hash_table->properties.pp_data = realloc(0, size * sizeof(void *));

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
                    log_error("[hash table] Null pointer provided for parameter \"pp_hash_table\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }

        // Hash table errors
        {
            failed_to_allocate_hash_table:
                #ifndef NDEBUG
                    log_error("[hash table] Failed to allocate memory for hash table in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }
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
        q = p_hash_table->pfn_hash_function(p_key, p_hash_table->properties.length);

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

int hash_table_insert ( hash_table *const p_hash_table, void *p_property )
{

    // Argument check
    if ( p_hash_table == (void *) 0 ) goto no_hash_table;
    
    // Initialized data
    size_t i = 0;

    // Repeat this block
    do
    {
        
        // Initialized data
        size_t q = (hash64)
        p_hash_table->pfn_hash_function(     // Hash 
            p_hash_table->pfn_key_get(       //
                p_property                   // 
            ),
            p_hash_table->properties.length  //
        );

        // If this slot is empty ...
        if ( p_hash_table->properties.pp_data[q] == (void *) 0 )
        {

            // ... store the property ...
            p_hash_table->properties.pp_data[q] = p_property;

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
                    log_error("[hash table] Null pointer provided for parameter \"p_hash_table\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }
    }
}


int hash_table_destroy ( hash_table **const pp_hash_table )
{

    // Argument check
    if ( pp_hash_table == (void *) 0 ) goto no_hash_table;

    // Stub
    return 1;

    // Error handling
    {

        // Argument errors
        {
            no_hash_table:
                #ifndef NDEBUG
                    log_error("[hash table] Null pointer provided for parameter \"pp_hash_table\" in call to function \"%s\"\n", __FUNCTION__);
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


