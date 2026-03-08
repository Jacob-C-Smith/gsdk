// header
#include <data/hash_table.h>

// forward declarations
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

/** !
 * Collision resolution with linear probing
 * 
 * @param p_hash_table the hash table
 * @param key          the key
 * @param i            the probe number
 * 
 * @return the index in the hash table
 */
size_t hash_table_linear_probe ( hash_table *p_hash_table, void *key, size_t i );

/** !
 * Collision resolution with quadratic probing
 * 
 * @param p_hash_table the hash table
 * @param key          the key
 * @param i            the probe number
 * 
 * @return the index in the hash table
 */
size_t hash_table_quadratic_probe ( hash_table *p_hash_table, void *key, size_t i );

/** !
 * Collision resolution with double hashing
 * 
 * @param p_hash_table the hash table
 * @param key          the key
 * @param i            the probe number
 * 
 * @return the index in the hash table
 */
size_t hash_table_double_hash ( hash_table *p_hash_table, void *key, size_t i );

// data
fn_table_hash *_pfn_table_hash[] = 
{
    [LINEAR_PROBE]    = hash_table_linear_probe,
    [QUADRATIC_PROBE] = hash_table_quadratic_probe,
    [DOUBLE_HASH]     = hash_table_double_hash,
};

// function definitions
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
    if ( NULL == pp_hash_table ) goto no_hash_table;

    // initialized data
    hash_table *p_hash_table = NULL;

    // allocate a hash table
    p_hash_table = default_allocator(NULL, sizeof(hash_table));
    if ( NULL == p_hash_table ) goto no_mem;

    // populate the hash table fields
    *p_hash_table = (hash_table)
    {
        .pfn_equality      = pfn_equality      ? pfn_equality      : default_equality,
        .pfn_hash_function = pfn_hash_function ? pfn_hash_function : default_hash,
        .pfn_key_get       = pfn_key_get       ? pfn_key_get       : default_key_accessor,
        .pfn_table_hash    = _pfn_table_hash[_type],
    };

    // populate hash table data
    p_hash_table->properties.count = 0,
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

        // standard library
        {
            no_mem:
                #ifndef NDEBUG
                    printf("[standard library] Call to function \"default_allocator\" returned an erroneous value in call to function \"%s\"\n", __FUNCTION__);
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

double hash_table_load_factor ( hash_table *p_hash_table )
{

    // argument check
    if ( NULL == p_hash_table ) goto no_hash_table;

    // done
    return (double) p_hash_table->properties.count / (double) p_hash_table->properties.max;

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

int hash_table_insert ( hash_table *const p_hash_table, void *p_property )
{

    // argument check
    if ( p_hash_table == (void *) 0 ) goto no_hash_table;
    
    // initialized data
    size_t i = 0;
    void *p_property_key = p_hash_table->pfn_key_get(p_property);

    // repeat 
    do
    {
        
        // initialized data
        hash64 q = 0;
        size_t z = 0;

        // compute the hash
        q = (hash64) p_hash_table->pfn_table_hash(
            p_hash_table,
            p_property_key,
            i
        );

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
                p_hash_table->pfn_key_get(p_hash_table->properties.pp_data[z]),
                p_property_key
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

int hash_table_foreach ( hash_table *p_hash_table, fn_foreach *pfn_foreach )
{

    // argument check
    if ( NULL == p_hash_table ) goto no_hash_table;
    if ( NULL ==  pfn_foreach ) goto no_foreach;

    // iterate through the hash table
    for (size_t i = 0; i < p_hash_table->properties.max; i++)
    {

        // skip
        if ( NULL == p_hash_table->properties.pp_data[i] ) continue;

        // call the foreach function
        pfn_foreach(p_hash_table->properties.pp_data[i]);
    }

    // success
    return 1;

    // erorr handling
    {

        // argument errors
        {
            no_hash_table:
                #ifndef NDEBUG
                    log_error("[hash table] Null pointer provided for parameter \"p_hash_table\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;

            no_foreach:
                #ifndef NDEBUG
                    log_error("[hash table] Null pointer provided for parameter \"pfn_foreach\" in call to function \"%s\"\n", __FUNCTION__);
                #endif
    
                // error
                return 0;
        }
    }
}

int hash_table_fori ( hash_table *p_hash_table, fn_fori *pfn_fori )
{
    
    // argument check
    if ( NULL == p_hash_table ) goto no_hash_table;
    if ( NULL ==     pfn_fori ) goto no_fori;

    // iterate through the hash table
    for (size_t i = 0; i < p_hash_table->properties.max; i++)
        pfn_fori(p_hash_table->properties.pp_data[i], i);

    // success
    return 1;

    // erorr handling
    {

        // argument errors
        {
            no_hash_table:
                #ifndef NDEBUG
                    log_error("[hash table] Null pointer provided for parameter \"p_hash_table\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;

            no_fori:
                #ifndef NDEBUG
                    log_error("[hash table] Null pointer provided for parameter \"pfn_fori\" in call to function \"%s\"\n", __FUNCTION__);
                #endif
    
                // error
                return 0;
        }
    }
}

int hash_table_destroy ( hash_table **const pp_hash_table, fn_allocator *pfn_allocator )
{

    // argument check
    if ( pp_hash_table == (void *) 0 ) goto no_hash_table;

    // initialized data
    hash_table *p_hash_table = *pp_hash_table;

    // no more pointer for caller
    *pp_hash_table = NULL;

    // TODO: iterate through each slot in the hash table
    for (size_t i = 0; i < p_hash_table->properties.max; i++);

    // release the hash table array
    p_hash_table->properties.pp_data = default_allocator(p_hash_table->properties.pp_data, 0);

    // release the hash table
    p_hash_table = default_allocator(p_hash_table, 0);

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

size_t hash_table_quadratic_probe ( hash_table *p_hash_table, void *key, size_t i )
{
    
    // initialized data
    hash64 h = (hash64) hash_table_positive_mod
    (
        p_hash_table->pfn_hash_function(key, 0),
        p_hash_table->properties.max
    );

    // done
    return ( h + i * i ) % p_hash_table->properties.max;
}

size_t hash_table_double_hash ( hash_table *p_hash_table, void *key, size_t i )
{
    
    // initialized data
    hash64 h = p_hash_table->pfn_hash_function(key, 0);
    size_t m = p_hash_table->properties.max;
    size_t h1 = hash_table_positive_mod(h, m);
    size_t h2 = 1 + hash_table_positive_mod(h, m > 1 ? m - 1 : 1);

    // done
    return ( h1 + i * h2 ) % m;
}

bool hash_table_is_prime ( size_t n )
{

    // iterate from 2 to the upper bound
    for (size_t i = 2; i < n; i++)

        // check for a remainder
        if ( n % i == 0 )

            // not a prime
            return false;
    
    // a prime
    return true;
}

size_t hash_table_generate_twin_prime ( size_t start, size_t end )
{

    // iterate through the range
    for (size_t i = start; i < end; i++)
        
        // check the twin prime
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
