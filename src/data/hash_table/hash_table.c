// header
#include <data/hash_table.h>

// type definitions
typedef size_t (fn_table_hash)(hash_table *p_hash_table, void *key, size_t i);

// structure definitions
struct hash_table_s
{
    enum collision_resolution_e _type;

    struct
    {
        void   **pp_data;
        size_t   logical, physical, max;
    } properties;

    mutex _lock;

    fn_equality     *pfn_equality;
    fn_key_accessor *pfn_key_get;
    fn_hash64       *pfn_hash_function;
    fn_table_hash   *pfn_table_hash;
};

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
        ._type      = _type,
        .properties = 
        {
            .pp_data  = NULL,
            .logical  = 0,
            .physical = 0,
            .max      = size,
        },

        .pfn_equality      = pfn_equality      ? pfn_equality      : default_equality,
        .pfn_hash_function = pfn_hash_function ? pfn_hash_function : default_hash,
        .pfn_key_get       = pfn_key_get       ? pfn_key_get       : default_key_accessor,
        .pfn_table_hash    = _pfn_table_hash[_type],
    };

    // allocate memory for the hash table slots
    p_hash_table->properties.pp_data = default_allocator(0, size * sizeof(void *));
    if ( NULL == p_hash_table->properties.pp_data ) goto no_mem;

    // initialize data
    memset(p_hash_table->properties.pp_data, 0, size * sizeof(void *));

    // construct a lock
    mutex_create(&p_hash_table->_lock);

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
                    log_error("[standard library] Call to function \"default_allocator\" returned an erroneous value in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
    }
}

int hash_table_search ( hash_table *const p_hash_table, void *p_key, void **pp_value )
{

    // argument check
    if ( NULL == p_hash_table ) goto no_hash_table;
    if ( NULL ==        p_key ) goto no_key;


    // initialized data
    size_t i = 0, q = 0, z = 0;

    // lock
    mutex_lock(&p_hash_table->_lock);

    // repeat 
    do 
    {

        // compute the hash
        q = p_hash_table->pfn_table_hash(p_hash_table, p_key, i);

        // compute the index
        z = q % p_hash_table->properties.max;

        // if there is a property stored in this slot ...
        if ( TOMBSTONE == p_hash_table->properties.pp_data[z] ) 
        {

            // increment the counter
            i++;

            // try again
            continue;
        }

        // if there is a property stored in this slot ...
        else if ( NULL != p_hash_table->properties.pp_data[z] ) 
        {

            // ... and the property is what the caller asked for ...
            if ( 
                p_hash_table->pfn_equality(
                    p_hash_table->pfn_key_get(p_hash_table->properties.pp_data[z]),
                    p_key
                ) == 0
            )
            {

                // ... return a pointer to the caller 
                *pp_value = p_hash_table->properties.pp_data[z];
                
                // unlock
                mutex_unlock(&p_hash_table->_lock);

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

    // unlock
    mutex_unlock(&p_hash_table->_lock);

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

            no_key:
                #ifndef NDEBUG
                    log_error("[hash table] Null pointer provided for parameter \"p_key\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
    }
}

double hash_table_load_factor ( hash_table *p_hash_table )
{

    // argument check
    if ( NULL == p_hash_table ) goto no_hash_table;

    // initialized data
    double result = 0;

    // lock
    mutex_lock(&p_hash_table->_lock);

    // compute the load factor
    result = (double) p_hash_table->properties.physical / (double) p_hash_table->properties.max;

    // unlock
    mutex_unlock(&p_hash_table->_lock);

    // done
    return result;

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
    void *p_property_key = NULL;

    // lock
    mutex_lock(&p_hash_table->_lock);

    // store the key of the property
    p_property_key = p_hash_table->pfn_key_get(p_property);

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

        // if this slot is empty or a tombstone
        if
        ( 
            NULL      == p_hash_table->properties.pp_data[z] ||
            TOMBSTONE == p_hash_table->properties.pp_data[z]
        )
        {

            // tombstone
            if ( NULL == p_hash_table->properties.pp_data[z] )
                p_hash_table->properties.physical++;

            // store the property 
            p_hash_table->properties.pp_data[z] = p_property;

            // increment the logical counter ...
            p_hash_table->properties.logical++;

            // unlock
            mutex_unlock(&p_hash_table->_lock);

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
        {

            // unlock
            mutex_unlock(&p_hash_table->_lock);
        
            // success
            return 1;
        }

        // increment the counter
        i++;
    }
    
    // continuation condition
    while (i != p_hash_table->properties.max);
        
    // unlock
    mutex_unlock(&p_hash_table->_lock);

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

int hash_table_remove ( hash_table *const p_hash_table, void *p_key, void **pp_value )
{

    // argument check
    if ( NULL == p_hash_table ) goto no_hash_table;
    if ( NULL ==        p_key ) goto no_key;
    
    // initialized data
    size_t i = 0;

    // lock
    mutex_lock(&p_hash_table->_lock);

    // repeat 
    do
    {
        
        // initialized data
        hash64 q = 0;
        size_t z = 0;

        // compute the hash
        q = (hash64) p_hash_table->pfn_table_hash(
            p_hash_table,
            p_key,
            i
        );

        // compute the index
        z = q % p_hash_table->properties.max;
        
        // tombstone?
        if ( TOMBSTONE == p_hash_table->properties.pp_data[z] )
        {
            
            // increment the counter
            i++;
            
            // try again
            continue;
        }

        // hit? 
        else if 
        (
            p_hash_table->pfn_equality(
                p_hash_table->pfn_key_get(p_hash_table->properties.pp_data[z]),
                p_key
            ) == 0
        )
        {

            // initialized data
            void *p_value = p_hash_table->properties.pp_data[z];

            // mark the slot with a tombstone
            p_hash_table->properties.pp_data[z] = TOMBSTONE;

            // return a pointer to the caller
            *pp_value = p_value;

            // decrement the quantity of logical values
            p_hash_table->properties.logical--;

            // unlock
            mutex_unlock(&p_hash_table->_lock);
        
            // success
            return 1;        
        } 
        
        // end of probe sequence
        else if ( NULL == p_hash_table->properties.pp_data[z] ) break;

        // increment the counter
        i++;
    }
    
    // continuation condition
    while (i != p_hash_table->properties.max);
        
    // unlock
    mutex_unlock(&p_hash_table->_lock);

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

            no_key:
                #ifndef NDEBUG
                    log_error("[hash table] Null pointer provided for parameter \"p_key\" in call to function \"%s\"\n", __FUNCTION__);
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

    // lock
    mutex_lock(&p_hash_table->_lock);

    // iterate through the hash table
    for (size_t i = 0; i < p_hash_table->properties.max; i++)
    {

        // skip
        if
        (
            NULL      == p_hash_table->properties.pp_data[i] ||
            TOMBSTONE == p_hash_table->properties.pp_data[i]
        )
            continue;

        // call the foreach function
        pfn_foreach(p_hash_table->properties.pp_data[i]);
    }

    // unlock
    mutex_unlock(&p_hash_table->_lock);

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

    // lock
    mutex_lock(&p_hash_table->_lock);

    // iterate through the hash table
    for (size_t i = 0; i < p_hash_table->properties.max; i++)
        pfn_fori(p_hash_table->properties.pp_data[i], i);

    // unlock
    mutex_unlock(&p_hash_table->_lock);

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

int hash_table_pack ( void *p_buffer, hash_table *p_hash_table, fn_pack *pfn_element )
{
    // argument check
    if ( NULL == p_hash_table ) goto no_hash_table;
    if ( NULL ==  pfn_element ) return 0;

    // initialized data 
    char *p = p_buffer;

    // lock
    mutex_lock(&p_hash_table->_lock);

    // pack the type
    // NOTE: Yes, using 4 bytes for the type is wasteful. But it keeps reads aligned
    p += pack_pack(p, "%i32", p_hash_table->_type);

    // pack the max, physical, and logical sizes
    p += pack_pack(p, "%3i64", 
        p_hash_table->properties.max,
        p_hash_table->properties.physical,
        p_hash_table->properties.logical
    );

    // pack the elements
    for (size_t i = 0; i < p_hash_table->properties.max; i++)
    {

        // skip
        if ( NULL == p_hash_table->properties.pp_data[i] ) continue;

        // pack the index
        p += pack_pack(p, "%i64", i);

        // pack the tombstone
        if ( TOMBSTONE == p_hash_table->properties.pp_data[i] )
            p += pack_pack(p, "%i64", TOMBSTONE);
        
        // pack the element
        else
            p += pfn_element(p, p_hash_table->properties.pp_data[i]);
    }

    // unlock
    mutex_unlock(&p_hash_table->_lock);

    // success
    return p - (char *)p_buffer;

    // error handling
    {
        
        // argument errors
        {
            no_hash_table:
                #ifndef NDEBUG
                    log_error("[array] Null pointer provided for \"p_hash_table\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
    }
}

int hash_table_unpack
( 
    hash_table **pp_hash_table,
    void *p_buffer,
    fn_unpack *pfn_element,

    fn_equality     *pfn_equality,
    fn_key_accessor *pfn_key_get,
    fn_hash64       *pfn_hash_function
)
{
	
	// argument check
    if ( NULL == pp_hash_table ) goto no_hash_table;
	if ( NULL ==      p_buffer ) goto no_buffer;
    if ( NULL ==   pfn_element ) goto no_unpack;

    // initialized data 
	hash_table *p_hash_table = NULL;
    char       *p            = p_buffer;
	size_t      max          = 0;
    size_t      physical     = 0;
	size_t      logical      = 0;
    int         result       = 0;

    enum collision_resolution_e _type = 0;

    // unpack the type
    p += pack_unpack(p, "%i32", &_type);

    // unpack the size of the hash table
	p += pack_unpack(p, "%3i64", 
        &max,
        &physical,
        &logical
    );

	// construct a hash table
    result = hash_table_construct(
        &p_hash_table,
        max,
        _type,
        
        pfn_equality,
        pfn_key_get,
        pfn_hash_function
    );

    // error check
	if ( 0 == result ) goto failed_to_construct_hash_table;

    // unpack the elements
    for (size_t i = 0; i < physical; i++)
    {

        // initialized data
		void *p_element = NULL;
        size_t index = 0;

        // unpack the index
        p += pack_unpack(p, "%i64", &index);

        // tombstone 
        if ( TOMBSTONE == *(void **)p )
            p_element = TOMBSTONE,
            p += sizeof(TOMBSTONE);

        // call the unpack function
        else 
            p += pfn_element(&p_element, p);

		// add the element to the hash table
		p_hash_table->properties.pp_data[index] = p_element;
    }

    // store the quantity of physical and logical elements
    p_hash_table->properties.physical = physical,
    p_hash_table->properties.logical  = logical;

	// return a pointer to the caller
	*pp_hash_table = p_hash_table;

    // success
	return p - (char *)p_buffer;

    // error handling
    {
        
        // argument errors
        {
            no_hash_table:
                #ifndef NDEBUG
                    log_error("[hash table] Null pointer provided for \"p_hash_table\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;

            no_buffer:
                #ifndef NDEBUG
                    log_error("[hash table] Null pointer provided for \"p_buffer\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;

            no_unpack:
                #ifndef NDEBUG
                    log_error("[hash table] Null pointer provided for \"pfn_element\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }

		// hash table errors
		{
			failed_to_construct_hash_table:
				#ifndef NDEBUG
					log_error("[hash table] Failed to construct hash table in call to function \"%s\"\n", __FUNCTION__);
				#endif

				// error
				return 0;
		}
    }
}

hash64 hash_table_hash ( hash_table *p_hash_table, fn_hash64 *pfn_element )
{

    // argument check
    if ( NULL == p_hash_table ) goto no_hash_table;

    // initialized data
    hash64     result     = 0;
    fn_hash64 *pfn_hash64 = (pfn_element) ? pfn_element : hash_crc64;

    // lock
    mutex_lock(&p_hash_table->_lock);

    // hash the elements
    for (size_t i = 0; i < p_hash_table->properties.max; i++)
    {

        // skip
        if
        (
            NULL      == p_hash_table->properties.pp_data[i] ||
            TOMBSTONE == p_hash_table->properties.pp_data[i]
        ) 
            continue;

        // hash the element 
        result ^= pfn_hash64(p_hash_table->properties.pp_data[i], 8);
    }

    // unlock
    mutex_unlock(&p_hash_table->_lock);

    // success
    return result;

    // error handling
    {

        // argument errors
        {
            no_hash_table:
                #ifndef NDEBUG
                    log_error("[hash table] Null pointer provided for \"p_hash_table\" in call to function \"%s\"\n", __FUNCTION__);
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

    // lock 
    mutex_lock(&p_hash_table->_lock);
    
    // no more pointer for caller
    *pp_hash_table = NULL;

    // unlock
    mutex_unlock(&p_hash_table->_lock);

    // destroy the lock
    mutex_destroy(&p_hash_table->_lock);

    // iterate through each slot in the hash table
    for (size_t i = 0; i < p_hash_table->properties.max; i++)
    {

        // skip
        if ( NULL      == p_hash_table->properties.pp_data[i] ) continue;
        if ( TOMBSTONE == p_hash_table->properties.pp_data[i] ) continue;

        // release the element
        if ( NULL != pfn_allocator )
            pfn_allocator(p_hash_table->properties.pp_data[i], 0);
    }

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
        p_hash_table->pfn_hash_function(key, 8),
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
        p_hash_table->pfn_hash_function(key, 8),
        p_hash_table->properties.max
    );

    // done
    return ( h + i * i ) % p_hash_table->properties.max;
}

size_t hash_table_double_hash ( hash_table *p_hash_table, void *key, size_t i )
{
    
    // initialized data
    hash64 h = p_hash_table->pfn_hash_function(key, 8);
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
