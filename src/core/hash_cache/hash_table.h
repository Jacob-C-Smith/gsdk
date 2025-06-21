/** !
 * Header for hash table library
 * 
 * @file hash_table/hash_table.h 
 *
 * @author Jacob Smith
 */

// Include guard
#pragma once

// standard library
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

// core
#include <core/interfaces.h>
#include <core/log.h>
#include <core/sync.h>
#include <core/hash.h>

// Forward declarations
struct hash_table_s;

// Structure definitions
struct hash_table_s
{
    fn_equality     *pfn_equality;
    fn_key_accessor *pfn_key_get;
    fn_hash64       *pfn_hash_function;

    struct
    {
        void   **pp_data;
        size_t   count, max, length;
    } properties;
};

// Constructors
/** !
 * Construct a hash table
 * 
 * @param pp_hash_table result
 * @param size          the quantity of elements that the has table can contain
 * 
 * @return 1 on success, 0 on error
 */
int hash_table_construct ( hash_table **const pp_hash_table, size_t size, fn_equality *pfn_equality, fn_key_accessor *pfn_key_get, fn_hash64 *pfn_hash_function );

// Accessors
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

// Destructors
/** !
 * Destroy a hash table 
 * 
 * @param pp_hash_table pointer to hash table pointer
 * 
 * @return 1 on success, 0 on error
 */
int hash_table_destroy ( hash_table **const pp_hash_table );
