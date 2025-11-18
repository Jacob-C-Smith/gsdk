/** !
 * Include header for tuple library
 * 
 * @file tuple/tuple.h 
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
#include <stdarg.h>

// core
#include <core/interfaces.h>
#include <core/pack.h>
#include <core/hash.h>
#include <core/log.h>
#include <core/sync.h>

// Debug mode
#undef NDEBUG

// forward declarations
struct tuple_s;

// type definitions
/** !
 *  @brief The type definition of a tuple struct
 */
typedef struct tuple_s tuple;

// function declarations
/// constructors
/** !
 *  Construct a tuple with a specific size
 *
 * @param pp_tuple result
 * @param size     number of elements in a tuple
 *
 * @sa tuple_create
 * @sa tuple_destroy
 *
 * @return 1 on success, 0 on error
 */
int tuple_construct ( tuple **const pp_tuple, size_t size );

/** !
 *  Construct a tuple from a list of elements
 *
 * @param pp_tuple result
 * @param elements pointer to null terminated tuple of element pointers
 * @param size     number of elements. 
 *
 * @sa tuple_construct
 * @sa tuple_from_arguments
 * @sa tuple_destroy
 *
 * @return 1 on success, 0 on error
 */
int tuple_from_elements ( tuple **const pp_tuple, void *const *const elements, size_t size );

/** !
 *  Construct a tuple from parameters
 *
 * @param pp_tuple      result
 * @param element_count the quantity of variadic arguments 
 * @param ...           variadic elements
 *
 * @sa tuple_construct
 * @sa tuple_from_elements
 * @sa tuple_destroy
 *
 * @return 1 on success, 0 on error
 */
int tuple_from_arguments ( tuple **const pp_tuple, size_t element_count, ... );

/// accessors
/** !
 * Index a tuple with a signed number. If index is negative, index = size - |index|, such that
 * [A,B,C,D,E] index(-2) -> D
 * 
 * @param p_tuple  the tuple
 * @param index    signed index. 
 * @param pp_value return
 * 
 * @sa tuple_get
 * @sa tuple_slice
 * 
 * @return 1 on success, 0 on error 
 */
int tuple_index ( const tuple *const p_tuple, signed long long index, void **const pp_value );

/** !
 * Get a slice of the tuple specified by a lower bound and an upper bound
 * 
 * @param p_tuple     the tuple
 * @param pp_elements return
 * @param lower_bound the lower bound of the tuple
 * @param upper_bound the upper bound of the tuple
 * 
 * @sa tuple_index
 * @sa tuple_get
 * 
 * @return 1 on success, 0 on error 
*/
int tuple_slice ( const tuple *const p_tuple, const void **const pp_elements, signed long long lower_bound, signed long long upper_bound );

/** !
 *  Is a tuple empty?
 * 
 * @param p_tuple the tuple
 * 
 * @return true if tuple has no contents else false
 */
bool tuple_is_empty ( const tuple *const p_tuple );

/** !
 *  Get the size of a tuple
 * 
 * @param p_tuple the tuple
 * 
 * @return size of tuple
 */
size_t tuple_size ( const tuple *const p_tuple );

/// iterators
/** !
 * Call function on every element in a tuple
 *
 * @param p_tuple  the tuple
 * @param pfn_fori pointer to fori function
 * 
 * @return 1 on success, 0 on error
 */
int tuple_fori ( tuple *p_tuple, fn_fori *pfn_fori );

/// reflection
/** !
 * Pack a tuple into a buffer
 *
 * @param p_tuple     the tuple
 * @param p_buffer    the buffer
 * @param pfn_element the packing function for each element
 *
 * @return 1 on success, 0 on error
 */
int tuple_pack ( void *p_buffer, tuple *p_tuple, fn_pack *pfn_element );

/** !
 * Unpack a buffer into a tuple
 *
 * @param pp_tuple    the tuple
 * @param p_buffer    the buffer
 * @param pfn_element the unpacking function for each element
 *
 * @return 1 on success, 0 on error
 */
int tuple_unpack ( tuple **pp_tuple, void *p_buffer, fn_unpack *pfn_element );

/// hash
/** !
 * Compute a 64-bit hash of a tuple
 *
 * @param p_tuple     the tuple to hash
 * @param pfn_element the hashing function applied to each element
 *
 * @return hash on success, NULL on error
 */
hash64 tuple_hash ( tuple *p_tuple, fn_hash64 *pfn_element );

/// destructors
/** !
 *  Destroy and deallocate a tuple
 *
 * @param pp_tuple the tuple
 * @param pfn_allocator pointer to element deallocator IF NOT NULL ELSE, elements are not deallocated.
 *
 * @return 1 on success, 0 on error
 */
int tuple_destroy ( tuple **pp_tuple, fn_allocator *pfn_allocator );
