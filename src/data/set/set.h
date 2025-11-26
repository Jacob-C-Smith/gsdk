/** !
 * Include header for set library
 * 
 * @file set/set.h 
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
/** !
 *  @brief The type definition of a set struct
 */
typedef struct set_s set;

// function declarations
/// constructors
/** !
 *  Construct a set with a specific number of elements
 *
 * @param pp_set       return
 * @param size         number of set elements. 
 * @param pfn_equality function for testing equality of elements in set IF parameter is not null ELSE default
 *
 * @return 1 on success, 0 on error
 */
int set_construct ( set **const pp_set, size_t size, fn_equality *pfn_equality );

/** !
 *  Construct a set from an array of elements
 *
 * @param pp_set       return
 * @param pp_elements  pointer to array of elements
 * @param size         quantity of elements in element parameter. 
 * @param pfn_equality function for testing equality of elements in set IF parameter is not null ELSE default
 *
 * @return 1 on success, 0 on error
 */
int set_from_elements ( set **const pp_set, const void **const pp_elements, size_t size, fn_equality *pfn_equality );

/** !
 *  Construct a set from the union of set A and set B
 *
 * @param pp_set return
 * @param p_a    pointer to set A
 * @param p_b    pointer to set B
 *
 * @return 1 on success, 0 on error
 */
int set_union ( set **const pp_set, const set *const p_a, const set *const p_b );

/** !
 *  Construct a set from the difference of set A and set B
 * 
 * @param pp_set return
 * @param p_a    set A
 * @param p_b    set B
 * @param pfn_equality function for testing equality of elements in set IF parameter is not null ELSE default
 *
 * @return 1 on success, 0 on error
*/
int set_difference ( set **const pp_set, const set *const p_a, const set *const p_b );

/** !
 *  Construct a set from the intersection of set A and set B
 * 
 * @param pp_set return
 * @param p_a    set A
 * @param p_b    set B
 *
 * @return 1 on success, 0 on error
*/
int set_intersection ( set **const pp_set, const set *const p_a, const set *const p_b );

/// accessors
/** !
 *  Return the quantity of elements in the set
 * 
 * @param p_set the set
 * 
 * @return The quantity of elements in the set
 */
size_t set_count ( const set *const p_set );

/** !
 *  Get the contents of a set
 * 
 * @param p_set       the set
 * @param pp_contents the contents of the set
 * 
 * @return 1 on success, 0 on error
 */
int set_contents ( const set *const p_set, void **const pp_contents );

// mutators
/** !
 *  Add an element to a set. 
 *
 * @param p_set   pointer to the set
 * @param p_value the element
 *
 * @return 1 on success, 0 on error
 */
int set_add ( set *const p_set, void *const p_element );

// Remove and return an element from a set
int set_pop ( set *const p_set, void **const pp_value );

// Remove an element from a set.
int set_remove ( set *const p_set, void *const p_element );

/// iterators
/** !
 * Call function on every element in p_set
 *
 * @param p_set set
 * @param function pointer to function of type void (*)(void *value, size_t index)
 * 
 * @return 1 on success, 0 on error
 */
int set_foreach_i ( const set *const p_set, void (*function)(void *const value, size_t index) );

/// reflection
/** !
 * Pack a set into a buffer
 * 
 * @param p_buffer     the buffer
 * @param p_set        the set
 * @param pfn_elemenet pointer to pack function IF not null ELSE default
 * 
 * @return 1 on success, 0 on error
 */
int set_pack ( void *p_buffer, set *p_set, fn_pack *pfn_element );

/** !
 * Unpack a buffer into a set
 * 
 * @param pp_set       result
 * @param p_buffer     the buffer
 * @param pfn_elemenet pointer to unpack function IF not null ELSE default
 * @param pfn_equality function for testing equality of elements in set IF parameter is not null ELSE default
 * 
 * @return 1 on success, 0 on error
 */
int set_unpack ( set **pp_set, void *p_buffer, fn_unpack *pfn_element, fn_equality *pfn_equality );

/// hash
/** !
 * Compute a 64-bit hash of a set
 * 
 * @param p_set       the set
 * @param pfn_element hashing function applied to each element
 * 
 * @return hash on success, NULL on error
 */
hash64 set_hash ( set *p_set, fn_hash64 *pfn_element );

// destructors
/** !
 *  Destroy and deallocate a set 
 *
 * @param pp_set pointer to a set pointer
 *
 * @return 1 on success, 0 on error
 */
int set_destroy ( set **const pp_set );

// TODO: Test if set A and set B are disjoint
// bool set_isdisjoint ( const set *const p_a, const set *const p_b );

// TODO: Test if set A is a subset of set B
// bool set_issubset ( const set *const p_a, const set *const p_b );

// TODO: Test if set A is a superset of set B
// bool set_issuperset ( const set *const p_a, const set *const p_b );

// TODO: Remove all elements from a set
// int set_clear ( set *const p_set );

// TODO: Remove all elements from a set, and deallocate values with pfn_free_func
// int set_free_clear ( set *const p_set, void (*pfn_free_func) );

// TODO: Make a shallow copy of a set
// int set_copy ( const set *const p_set, set **const pp_set );

// TODO: Remove an element form an existing set
// void set_discard ( set *const p_set, void *p_element );

// TODO: Update an existing set with the difference of itself and another set 
// int set_difference_update ( set *const p_a, const set *const p_b );

// TODO: Update an existing set with the intersection of itself and another set 
// int set_intersection_update ( set *const p_a, const set *const p_b );

// TODO: Update an existing set with the union of itself and another set 
// int set_update ( set *const p_a, const set *const p_b );
