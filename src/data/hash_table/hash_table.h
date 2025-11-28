/** !
 * Header for hash table library
 * 
 * @file include/data/hash_table.h 
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

// enumeration definitions
enum collision_resolution_e 
{
    LINEAR_PROBE,
    QUADRATIC_PROBE,
    DOUBLE_HASH,
    COLLISION_RESOLUTION_QUANTITY
};

// forward declarations
struct hash_table_s;

// type definitions
typedef struct hash_table_s hash_table;
typedef size_t (fn_table_hash)(hash_table *p_hash_table, void *key, size_t i);

// structure definitions
struct hash_table_s
{
    fn_equality     *pfn_equality;
    fn_key_accessor *pfn_key_get;
    fn_hash64       *pfn_hash_function;
    fn_table_hash   *pfn_table_hash;

    struct
    {
        void   **pp_data;
        size_t   count, max, length;
    } properties;
};


// constructors
/** !
 * Construct a hash table
 * 
 * @param pp_hash_table result
 * @param size          the quantity of elements that the has table can contain
 * 
 * @return 1 on success, 0 on error
 */
int hash_table_construct 
(
    hash_table **const pp_hash_table,
    size_t size, 
    enum collision_resolution_e _type,
    
    fn_equality *pfn_equality, 
    fn_key_accessor *pfn_key_get, 
    fn_hash64 *pfn_hash_function
);

// accessors
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

// TODO: Mutators
/** !
 * Search a hash table for a key
 * 
 * @param p_hash_table the hash table 
 * @param p_property   the property
 * 
 * @return 1 on success, 0 on error 
 */
int hash_table_insert ( hash_table *const p_hash_table, void *p_property );

// destructors
/** !
 * Destroy a hash table 
 * 
 * @param pp_hash_table pointer to hash table pointer
 * 
 * @return 1 on success, 0 on error
 */
int hash_table_destroy ( hash_table **const pp_hash_table );
