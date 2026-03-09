/** !
 * Dictionary interface
 * 
 * @file src/data/dict/dict.h
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

// forward declarations
struct dict_s;

// type definitions
typedef struct dict_s dict;

// function declarations
/// constructors
/** !
 * Construct a dictionary with a specific number of hash table entries
 *
 * @param pp_dict result
 * @param size    number of hash table entries. 
 * 
 * @param pfn_allocator    pointer to allocator function IF NOT NULL ELSE unused
 * @param pfn_key_accessor pointer to key accessor function IF NOT NULL ELSE default
 * @param pfn_hash64       pointer to hash function IF NOT NULL ELSE default
 *
 * @return 1 on success, 0 on error
 */
int dict_construct
(
    dict **const pp_dict,
    size_t size,

    fn_allocator    pfn_allocator,
    fn_key_accessor pfn_key_accessor,
    fn_hash64       pfn_hash64
);

// accessors
/** !
 * Get a value from a dictioanry through a key
 *
 * @param p_dict   the dictionary
 * @param p_key    the key
 * @param pp_value result IF NOT NULL ELSE unused
 *
 * @return 1 on success, 0 on error
 */
int dict_get ( dict *const p_dict, const char *const p_key, void **pp_value );

/** !
 * Get a set of values from a dictionary
 * 
 * @param p_dict    the dictionary
 * @param pp_values result
 * @param limit     the upper limit of values to store
 * 
 * @return 1 on success, 0 on error 
*/
int dict_values ( dict *p_dict, void *pp_values[], size_t limit );

/** !
 * Get the quantity of values in a dictionary
 * 
 * @param p_dict   the dictionary
 * @param p_result result
 * 
 * @return 1 on success, 0 on error
 */
int dict_size ( dict *const p_dict, size_t *p_result );

// mutators
/** !
 * Add a value to a dictionary. 
 *
 * @param p_dict  dictionary
 * @param p_value the value of the property
 *
 * @return 1 on success, 0 on error
 */
int dict_add ( dict *const p_dict, const void *const p_value );

/** !
 * Remove a value from a dictionary by key
 *
 * @param p_dict   the dictionary
 * @param p_key    the key
 * @param pp_value result IF NOT NULL ELSE unused
 *
 * @return 1 on success, 0 on error
 */
int dict_pop ( dict *const p_dict, const char *const p_key, const void **const pp_value );

/// iterators
/** !
 * Call a function on each value in a dictionary
 * 
 * @param p_dict      the dictionary
 * @param pfn_foreach pointer to foreach function
 * 
 * @return 1 on success, 0 on error
*/
int dict_foreach ( dict *const p_dict, fn_foreach *pfn_foreach );

/// reflection
/** !
 * Pack a dictionary into a buffer
 * 
 * @param p_buffer     the buffer
 * @param p_dict       the dictionary
 * @param pfn_elemenet pointer to pack function 
 * 
 * @return bytes written on success, 0 on error
 */
int dict_pack ( void *p_buffer, dict *p_dict, fn_pack *pfn_element );

/** !
 * Unpack a buffer into a dictionary
 * 
 * @param pp_dict      result
 * @param p_buffer     the buffer
 * @param pfn_elemenet pointer to unpack function
 * 
 * @param pfn_allocator    pointer to allocator function IF NOT NULL ELSE unused
 * @param pfn_key_accessor pointer to key accessor function IF NOT NULL ELSE default
 * @param pfn_hash64       pointer to hash function IF NOT NULL ELSE default
 * 
 * @return bytes read on success, 0 on error
 */
int dict_unpack
(
    dict **pp_dict,
    void *p_buffer,
    fn_unpack *pfn_element,

    fn_allocator    pfn_allocator,
    fn_key_accessor pfn_key_accessor,
    fn_hash64       pfn_hash64
);

/// hash
/** !
 * Compute a 64-bit hash of a dictionary
 * 
 * @param p_dict      the dictionary
 * @param pfn_element hashing function applied to each element
 * 
 * @return hash on success, 0 on error
 */
hash64 dict_hash ( dict *p_dict, fn_hash64 *pfn_element );

// destructors
/** !
 * Destroy and deallocate a dictionary
 *
 * @param pp_dict pointer to dictionary pointer
 *
 * @return 1 on success, 0 on error
 */
int dict_destroy ( dict **const pp_dict, fn_allocator *pfn_allocator );
