/** !
 * set interface
 * 
 * @file src/data/set/set.h 
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
#include <core/log.h>
#include <core/sync.h>
#include <core/hash.h>
#include <core/pack.h>
#include <core/interfaces.h>

// forward declarations
struct set_s;

// type definitions
typedef struct set_s set;

// function declarations
/// constructors
/** !
 * Construct a set with a specific number of elements
 *
 * @param pp_set         result
 * @param size           quantity of elements
 * @param pfn_comparator pointer to comparator function IF NOT NULL ELSE default
 *
 * @return 1 on success, 0 on error
 */
int set_construct ( set **const pp_set, size_t size, fn_comparator *pfn_comparator );

/** !
 * Construct a set from an array of elements
 *
 * @param pp_set         result
 * @param pp_elements    pointer to array of elements
 * @param size           quantity of elements 
 * @param pfn_comparator pointer to comparator function IF NOT NULL ELSE default
 *
 * @return 1 on success, 0 on error
 */
int set_from_elements ( set **const pp_set, void **const pp_elements, size_t size, fn_comparator *pfn_comparator );

/** !
 * Construct a set from the union of set A and set B
 *
 * @param pp_set result
 * @param p_a    set A
 * @param p_b    set B
 *
 * @return 1 on success, 0 on error
 */
int set_union ( set **const pp_set, const set *const p_a, const set *const p_b );

/** !
 * Construct a set from the difference of set A and set B
 * 
 * @param pp_set result
 * @param p_a    set A
 * @param p_b    set B
 *
 * @return 1 on success, 0 on error
*/
int set_difference ( set **const pp_set, const set *const p_a, const set *const p_b );

/** !
 * Construct a set from the intersection of set A and set B
 * 
 * @param pp_set result
 * @param p_a    set A
 * @param p_b    set B
 *
 * @return 1 on success, 0 on error
*/
int set_intersection ( set **const pp_set, const set *const p_a, const set *const p_b );

/// accessors
/** !
 * Test if set A and set B are disjoint
 * 
 * @param p_a set A
 * @param p_b set B
 * 
 * @return true IF disjoint ELSE false
 */
bool set_isdisjoint ( const set *const p_a, const set *const p_b );

/** !
 * Test if set A is a subset of set B
 * 
 * @param p_a set A
 * @param p_b set B
 * 
 * @return true IF A is a subset of B ELSE false
 */
bool set_issubset ( const set *const p_a, const set *const p_b );

/** !
 * Test if set A is a superset of set B
 * 
 * @param p_a set A
 * @param p_b set B
 * 
 * @return true IF A is a superset of B ELSE false
 */
bool set_issuperset ( const set *const p_a, const set *const p_b );

/** !
 * Return the quantity of elements in the set
 * 
 * @param p_set the set
 * 
 * @return The quantity of elements in the set
 */
size_t set_count ( const set *const p_set );

/** !
 * Get the contents of a set
 * 
 * @param p_set       the set
 * @param pp_contents the contents of the set
 * 
 * @return 1 on success, 0 on error
 */
int set_contents ( set *const p_set, void **const pp_contents );

// mutators
/** !
 * Add an element to a set. 
 *
 * @param p_set     the set
 * @param p_element the element
 *
 * @return 1 on success, 0 on error
 */
int set_add ( set *const p_set, void *const p_element );

/** !
 * Remove and return an element from a set
 *
 * @param p_set    the set
 * @param pp_value result
 *
 * @return 1 on success, 0 on error
 */
int set_pop ( set *const p_set, void **const pp_value );

/** !
 * Remove an element from a set.
 *  
 * @param p_set     the set
 * @param p_element the element
 * 
 * @return 1 on success, 0 on error
 */
int set_remove ( set *const p_set, void *const p_element );

/// iterators
/** !
 * Call function on every element in a set
 *
 * @param p_set       the set
 * @param pfn_foreach pointer to foreach function
 * 
 * @return 1 on success, 0 on error
 */
int set_foreach ( set *const p_set, fn_foreach *pfn_foreach );

/** !
 * Construct an iterator for a set
 * 
 * @param p_set the set
 * 
 * @return an iterator
 */
iterator set_iterator ( set *p_set );

/// reflection
/** !
 * Pack a set into a stream
 * 
 * @param p_stream     the stream
 * @param p_set        the set
 * @param pfn_elemenet pointer to pack function 
 * 
 * @return bytes written on success, 0 on error
 */
int set_pack ( stream *p_stream, set *p_set, fn_pack *pfn_element );

/** !
 * Unpack a stream into a set
 * 
 * @param pp_set         result
 * @param p_stream       the stream
 * @param pfn_elemenet   pointer to unpack function
 * @param pfn_comparator pointer to comparator function
 * 
 * @return bytes read on success, 0 on error
 */
int set_unpack ( set **pp_set, stream *p_stream, fn_unpack *pfn_element, fn_comparator *pfn_comparator );

/// hash
/** !
 * Compute a 64-bit hash of a set
 * 
 * @param p_set       the set
 * @param pfn_element pointer to hashing function IF NOT NULL ELSE default
 * 
 * @return hash on success, 0 on error
 */
hash64 set_hash ( set *p_set, fn_hash64 *pfn_element );

// destructors
/** !
 * Destroy and deallocate a set 
 *
 * @param pp_set        pointer to set pointer
 * @param pfn_allocator pointer to allocator function IF NOT NULL ELSE unused
 *
 * @return 1 on success, 0 on error
 */
int set_destroy ( set **const pp_set, fn_allocator *pfn_allocator );

