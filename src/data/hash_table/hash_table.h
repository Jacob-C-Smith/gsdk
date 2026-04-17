/** !
 * hash table interface
 * 
 * @file src/data/hash_table/hash_table.h 
 *
 * @author Jacob Smith
 */

// header guard
#pragma once

// standard library
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

// gsdk
/// core
#include <core/interfaces.h>
#include <core/log.h>
#include <core/sync.h>
#include <core/hash.h>
#include <core/pack.h>

// preprocessor definitions
#define TOMBSTONE ((void *)0xffffffffffffffff)

// enumeration definitions
enum collision_resolution_e 
{
    COLLISION_RESOLUTION_DEFAULT,
    LINEAR_PROBE = COLLISION_RESOLUTION_DEFAULT,
    QUADRATIC_PROBE,
    DOUBLE_HASH,
    COLLISION_RESOLUTION_QUANTITY
};

// forward declarations
struct hash_table_s;

// type definitions
typedef struct hash_table_s hash_table;

/// constructors
/** !
 * Construct a hash table
 * 
 * @param pp_hash_table    result
 * @param size             the quantity of elements that the hash table can contain
 * 
 * @param pfn_equality     pointer to equality function IF not null ELSE default
 * @param pfn_key_accessor pointer to key accessor function IF not null ELSE default
 * @param pfn_hash         pointer to hash function IF not null ELSE default
 * 
 * @return 1 on success, 0 on error
 */
int hash_table_construct 
(
    hash_table **const pp_hash_table,
    size_t size, 
    enum collision_resolution_e _type,
    
    fn_equality     *pfn_equality, 
    fn_key_accessor *pfn_key_accessor, 
    fn_hash64       *pfn_hash
);

/// accessors
/** !
 * Search a hash table for a key
 * 
 * @param p_hash_table the hash table
 * @param p_key        the key
 * @param pp_value     result
 * 
 * @return 1 on success, 0 on error 
 */
int hash_table_search ( hash_table *const p_hash_table, void *p_key, void **pp_value );

/** !
 * Get the load factor of a hash table
 * 
 * @param p_hash_table the hash table
 * 
 * @return the load factor of the hash table
 */
double hash_table_load_factor ( hash_table *p_hash_table );

/// mutators
/** !
 * Search a hash table for a key
 * 
 * @param p_hash_table the hash table 
 * @param p_property   the property
 * 
 * @return 1 on success, 0 on error 
 */
int hash_table_insert ( hash_table *const p_hash_table, void *p_property );

/** !
 * Remove a value from a hash table
 * 
 * @param p_hash_table the hash table
 * @param p_key        the key
 * @param pp_value     result IF NOT NULL ELSE unused
 * 
 * @return 1 on success, 0 on error
 */
int hash_table_remove ( hash_table *const p_hash_table, void *p_key, void **pp_value );

/// iterators
/** !
 * Call a function on every element in a hash table
 *
 * @param p_hash_table the hash table
 * @param pfn_foreach  pointer to foreach function
 * 
 * @return 1 on success, 0 on error
 */
int hash_table_foreach ( hash_table *p_hash_table, fn_foreach *pfn_foreach );

/**! 
 * Call a function on every slot in a hash table
 * 
 * @param p_hash_table the hash table
 * @param pfn_fori     pointer to fori function
 * 
 * @return 1 on success, 0 on error
 */
int hash_table_fori ( hash_table *p_hash_table, fn_fori *pfn_fori );

/// reflection
/** !
 * Pack a hash table into a buffer
 * 
 * @param p_buffer     the buffer
 * @param p_hash_table the hash table
 * @param pfn_elemenet pointer to pack function IF not null ELSE default
 * 
 * @return bytes written on success, 0 on error
 */
int hash_table_pack ( void *p_buffer, hash_table *p_hash_table, fn_pack *pfn_element );

/** !
 * Unpack a buffer into a hash table
 * 
 * @param pp_hash_table result
 * @param p_buffer      the buffer
 * @param pfn_elemenet  pointer to unpack function IF not null ELSE default
 * 
 * @param pfn_equality     pointer to equality function IF not null ELSE default
 * @param pfn_key_accessor pointer to key accessor function IF not null ELSE default
 * @param pfn_hash         pointer to hash function IF not null ELSE default
 * 
 * @return bytes read on success, 0 on error
 */
int hash_table_unpack
( 
    hash_table **pp_hash_table,
    void *p_buffer,
    fn_unpack *pfn_element,

    fn_equality     *pfn_equality,
    fn_key_accessor *pfn_key_get,
    fn_hash64       *pfn_hash_function
);

/// hash
/** !
 * Compute a 64-bit hash of a hash table
 * 
 * @param p_hash_table the hash table
 * @param pfn_element  hashing function applied to each element
 * 
 * @return hash on success, NULL on error
 */
hash64 hash_table_hash ( hash_table *p_hash_table, fn_hash64 *pfn_element );

/// destructors
/** !
 * Destroy a hash table 
 * 
 * @param pp_hash_table pointer to hash table pointer
 * @param pfn_allocator pointer to allocator function for deallocating elements
 * 
 * @return 1 on success, 0 on error
 */
int hash_table_destroy ( hash_table **const pp_hash_table, fn_allocator *pfn_allocator );
