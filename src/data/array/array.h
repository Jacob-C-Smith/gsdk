/** !
 * Include header for array library
 * 
 * @file array/array.h 
 * 
 * @author Jacob Smith
 */

// header guard
#pragma once

// standard library
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stdbool.h>
#include <string.h>

// core
#include <core/interfaces.h>

// type definitions
/// @brief The type definition of an array struct 
typedef struct array_s array;

// function declarations
/// constructors
/** !
 *  Construct an array with a specific size
 *
 * @param pp_array result
 * @param size     number of elements in an array
 *
 * @sa array_create
 * @sa array_destroy
 *
 * @return 1 on success, 0 on error
 */
int array_construct ( array **pp_array, size_t size );

/** !
 *  Construct an array from an array of elements
 *
 * @param pp_array result
 * @param elements pointer to null terminated array of element pointers
 * @param size     number of elements. 
 *
 * @sa array_create
 * @sa array_destroy
 *
 * @return 1 on success, 0 on error
 */
int array_from_elements ( array **pp_array, void *_p_elements[], size_t size );

/** !
 *  Construct an array from parameters
 *
 * @param pp_array      return
 * @param size          the size of the array
 * @param element_count the quantity of variadic arguments 
 * @param ...           variadic elements
 *
 * @sa array_construct
 * @sa array_from_elements
 * @sa array_destroy
 *
 * @return 1 on success, 0 on error
 */
int array_from_arguments ( array **pp_array, size_t size, size_t count, ... );

/// accessors
/** !
 * Index an array with a signed number. If index is negative, index = size - |index|, such that
 * [A,B,C,D,E] index(-2) -> D
 * 
 * @param p_array  the array
 * @param index    signed index
 * @param pp_value result
 * 
 * @sa array_get
 * @sa array_slice
 * 
 * @return 1 on success, 0 on error 
 */
int array_index ( array *p_array, signed index, void **pp_value );

/** !
 *  Get an array of elements
 *
 * @param p_array     the array
 * @param pp_elements result
 *
 * @sa array_index
 * @sa array_slice
 *
 * @return  1 on success, 0 on error 
 */
int array_get ( array *p_array, void **pp_elements, size_t *p_count );

/** !
 * Get a slice of the array specified by a lower bound and an upper bound
 * 
 * @param p_array     the array
 * @param pp_elements return
 * @param lower_bound the lower bound of the array
 * @param upper_bound the upper bound of the array
 * 
 * @sa array_index
 * @sa array_get
 * 
 * @return 1 on success, 0 on error 
*/
int array_slice ( array *p_array, void *pp_elements[], signed lower_bound, signed upper_bound );

/** !
 *  Is an array empty?
 * 
 * @param p_array the array
 * 
 * @return true if array has no contents else false
 */
bool array_is_empty ( array *p_array );

/** !
 *  Get the size of an array
 * 
 * @param p_array the array
 * 
 * @return size of array
 */
size_t array_size ( array *p_array );

/// mutators
/** !
 *  Add an element to the end of an array. 
 *
 * @param p_array the array
 * @param p_element the value of the element
 *
 * @return 1 on success, 0 on error
 */
int array_add ( array *p_array, void *p_element );

/** !
 *  Update an array element at an index
 *
 * @param p_array the array
 * @param index the index
 * @param p_value the new element
 *
 * @sa array_index
 * @sa array_get
 * @sa array_remove
 *
 * @return  1 on success, 0 on error 
 */
int array_set ( array *p_array, signed index, void *p_value );

/** !
 *  Remove an element from an array. If index is negative,
 *  index = size - |index|, such that [A,B,C,D,E] remove(-2) -> D
 *
 * @param p_array  the array
 * @param index    signed index
 * @param pp_value return
 *
 * @return 1 on success, 0 on error
 */
int array_remove ( array *p_array, signed index, void **pp_value );

/// sort
/** !
 * Sort an array
 * 
 * @param p_array        the array
 * @param pfn_comparator pointer to comparator function
 * 
 * @return 1 on success, 0 on error
 */
int array_sort ( array *p_array, fn_comparator *pfn_comparator );

/// map
/** !
 * Apply an operation to each element in an array,
 * optionally releasing elements
 *
 * @param p_array       the array
 * @param pfn_map       pointer to map function
 * @param pfn_allocator pointer to allocator function
 * 
 * @return 1 on success, 0 on error
 */
int array_map ( array *const p_array, fn_map *pfn_map, fn_allocator *pfn_allocator );

/// iterators
/** !
 * Call function on every element in an array
 *
 * @param p_array  the array
 * @param pfn_fori pointer to fori function
 * 
 * @return 1 on success, 0 on error
 */
int array_fori ( array *p_array, fn_fori *pfn_fori );

/** !
 * Call function on every element in an array
 *
 * @param p_array     the array
 * @param pfn_foreach pointer to foreach function
 * 
 * @return 1 on success, 0 on error
 */
int array_foreach ( array *p_array, fn_foreach *pfn_foreach );

/// reflection
/** !
 * Pack an array into a buffer
 * 
 * @param p_buffer     the buffer
 * @param p_array      the array
 * @param pfn_elemenet pointer to pack function IF not null ELSE default
 * 
 * @return 1 on success, 0 on error
 */
int array_pack ( void *p_buffer, array *p_array, fn_pack *pfn_element );

/** !
 * Unpack a buffer into an array
 * 
 * @param pp_array     result
 * @param p_buffer     the buffer
 * @param pfn_elemenet pointer to unpack function IF not null ELSE default
 * 
 * @return 1 on success, 0 on error
 */
int array_unpack ( array **pp_array, void *p_buffer, fn_unpack *pfn_element );

/// hash
/** !
 * Compute a 64-bit hash of an array
 * 
 * @param p_array     the array
 * @param pfn_element hashing function applied to each element
 * 
 * @return hash on success, NULL on error
 */
hash64 array_hash ( array *p_array, fn_hash64 *pfn_element );

/// destructors
/** !
 *  Destroy and deallocate an array
 *
 * @param pp_array      pointer to array pointer
 * @param pfn_allocator pointer to allocator function for deallocating elements
 *
 * @sa array_create
 *
 * @return 1 on success, 0 on error
 */
int array_destroy ( array **const pp_array, fn_allocator *pfn_allocator );
