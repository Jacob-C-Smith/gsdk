// header
#include <data/hash_table.h>

// function declarations
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

size_t hash_table_linear_probe ( hash_table *p_hash_table, void *key, size_t i );

size_t hash_table_double_hash ( hash_table *p_hash_table, void *key, size_t i );

// function definitions
int hash_table_create ( hash_table **const pp_hash_table )
{

    // argument check
    if ( pp_hash_table == (void *) 0 ) goto no_hash_table;

    // initialized data
    hash_table *p_hash_table = default_allocator(0, sizeof(hash_table));

    // error check
    if (p_hash_table == (void *) 0 ) goto no_mem;

    // Initialize 
    memset(p_hash_table, 0, sizeof(hash_table));

    // return a pointer to the caller
    *pp_hash_table = p_hash_table;

    // success
    return 1;

    // error handling
    {

        // argument errors
        {
            no_hash_table:
                #ifndef NDEBUG
                    log_error("[hash table] Null pointer provided for parameter \"pp_hash_table\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }

        // standard library errors
        {
            no_mem:
                #ifndef NDEBUG
                    log_error("[hash cache] Failed to allocate memory in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
    }
}

int hash_table_construct 
(
    hash_table **const pp_hash_table,
    size_t             size,
    enum collision_resolution_e _type,
    
    fn_equality       *pfn_equality,
    fn_key_accessor   *pfn_key_get,
    fn_hash64         *pfn_hash_function
)
{

    // argument check
    if ( pp_hash_table == (void *) 0 ) goto no_hash_table;

    // initialized data
    hash_table *p_hash_table = (void *) 0;

    // Unused
    (void)size;

    // allocate memory for a hash table
    if ( hash_table_create(&p_hash_table) == 0 ) goto failed_to_allocate_hash_table;

    // Populate the hash table interfaces
    *p_hash_table = (hash_table)
    {
        .pfn_equality      = pfn_equality      ? pfn_equality      : default_equality,
        .pfn_hash_function = pfn_hash_function ? pfn_hash_function : default_hash,
        .pfn_key_get       = pfn_key_get       ? pfn_key_get       : default_key_accessor,
        .pfn_table_hash    = hash_table_double_hash
    };

    // Populate hash table data
    p_hash_table->properties.count = 0,
    p_hash_table->properties.length = 0,
    p_hash_table->properties.max = size,
    p_hash_table->properties.pp_data = default_allocator(0, size * sizeof(void *));

    // initialize data
    memset(p_hash_table->properties.pp_data, 0, size * sizeof(void *));

    // return a pointer to the caller
    *pp_hash_table = p_hash_table;

    // success
    return 1;

    // error handling
    {

        // argument errors
        {
            no_hash_table:
                #ifndef NDEBUG
                    log_error("[hash table] Null pointer provided for parameter \"pp_hash_table\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }

        // Hash table errors
        {
            failed_to_allocate_hash_table:
                #ifndef NDEBUG
                    log_error("[hash table] Failed to allocate memory for hash table in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
    }
}

int hash_table_search ( hash_table *const p_hash_table, void *p_key, void **pp_value )
{

    // initialized data
    size_t i = 0, q = 0, z = 0;

    // repeat 
    do 
    {

        // compute the hash
        q = p_hash_table->pfn_table_hash(p_hash_table, p_key, i);

        // compute the index
        z = q % p_hash_table->properties.max;

        // If there is a property stored in this slot ...
        if ( p_hash_table->properties.pp_data[z] != NULL ) 
        {

            // ... and the property is what the caller asked for ...
            if ( p_hash_table->pfn_equality(p_hash_table->properties.pp_data[z], p_key) == 0 )
            {

                // ... return a pointer to the caller 
                *pp_value = p_hash_table->properties.pp_data[z];
                // printf("p=%i ; found \"%s\"\n", i, p_key);

                // success
                return 1;
            }
        }
            
        // increment counter
        i++;
    }

    // continuation condition
    while 
    ( 
        p_hash_table->properties.pp_data[z] != (void *) 0 &&
        i < p_hash_table->properties.max 
    );

    // not found
    *pp_value = NULL;
    // printf("p=%i failed to find \"%s\"\n", i, p_key);

    // error
    return 0;
}

int hash_table_insert ( hash_table *const p_hash_table, void *p_property )
{

    // argument check
    if ( p_hash_table == (void *) 0 ) goto no_hash_table;
    
    // initialized data
    size_t i = 0;

    // Repeat this block
    do
    {
        
        // initialized data
        hash64 q = 0;
        size_t z = 0;

        // compute the hash
        q = (hash64) p_hash_table->pfn_table_hash(p_hash_table, p_property, i);

        // compute the index
        z = q % p_hash_table->properties.max;

        // if this slot is empty ...
        if ( p_hash_table->properties.pp_data[z] == (void *) 0 )
        {

            // ... store the property ...
            p_hash_table->properties.pp_data[z] = p_property;

            // ... increment the count ...
            p_hash_table->properties.count++;

            // success
            return 1;
        }

        // ... if duplicate ...
        else if 
        (
            p_hash_table->pfn_equality(
                p_hash_table->properties.pp_data[z],
                p_hash_table->pfn_key_get(p_property)
            ) == 0
        )
            // success
            return 1;        

        // increment the counter
        i++;
    }
    
    // Continuation condition
    while (i != p_hash_table->properties.max);
        
    // error
    return 0;
    
    // error handling
    {

        // argument errors
        {
            no_hash_table:
                #ifndef NDEBUG
                    log_error("[hash table] Null pointer provided for parameter \"p_hash_table\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
    }
}

int hash_table_destroy ( hash_table **const pp_hash_table )
{

    // argument check
    if ( pp_hash_table == (void *) 0 ) goto no_hash_table;

    // Stub
    return 1;

    // error handling
    {

        // argument errors
        {
            no_hash_table:
                #ifndef NDEBUG
                    log_error("[hash table] Null pointer provided for parameter \"pp_hash_table\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
    }
}

size_t hash_table_linear_probe ( hash_table *p_hash_table, void *key, size_t i )
{
    
    // initialized data
    hash64 h = (hash64) hash_table_positive_mod
    (
        p_hash_table->pfn_hash_function(key, 0),
        p_hash_table->properties.max
    );

    // done
    return ( h + i ) % p_hash_table->properties.max;
}

size_t hash_table_double_hash ( hash_table *p_hash_table, void *key, size_t i )
{
    
    // initialized data
    hash64 h = (hash64) hash_table_positive_mod
    (
        p_hash_table->pfn_hash_function(key, 0),
        p_hash_table->properties.max
    );

    // done
    return  (h % p_hash_table->properties.max) + i * (1 + (h % (p_hash_table->properties.max - 2))) % p_hash_table->properties.max;
}

bool hash_table_is_prime ( size_t n )
{

    // iterate from 2 to the upper bound
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

    // iterate through the range
    for (size_t i = start; i < end; i++)
    
        
        // Check the twin prime
        if ( hash_table_is_prime(i) && hash_table_is_prime(i+2) )

            // success
            return i + 2;
    
    // error
    return 0;
}

signed hash_table_positive_mod ( signed dividend, signed divisor )
{

    // initialized data
    signed quotient = dividend % divisor;

    // success
    return ( quotient < 0 ) ? quotient + divisor : quotient;
}
