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
#include <core/sync.h>

// reflection
#include <core/pack.h>

// type definitions
/** !
 *  @brief The type definition of an array struct
 */
typedef struct array_s array;

/** !
 *  @brief A function to be called for each element in an array
 */
typedef int (fn_array_foreach_i)(const void *const value, size_t index);

// initializer
/** !
 * This gets called at runtime before main. 
 * 
 * @param void
 * 
 * @return void
 */
void array_init ( void ) __attribute__((constructor));

// Allocaters
/** !
 *  Allocate memory for an array
 *
 * @param pp_array return
 *
 * @sa array_destroy
 *
 * @return 1 on success, 0 on error
 */
int array_create ( array **pp_array );

// constructors
/** !
 *  Construct an array with a specific size
 *
 * @param pp_array return
 * @param size number of elements in an array
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
 * @param pp_array return
 * @param elements pointer to null terminated array of element pointers
 * @param size number of elements. 
 *
 * @sa array_create
 * @sa array_destroy
 *
 * @return 1 on success, 0 on error
 */
int array_from_elements ( array **pp_array, void *_p_elements[] );

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
int array_from_arguments ( array **const pp_array, size_t size, size_t element_count, ... );

// accessors
/** !
 * Index an array with a signed number. If index is negative, index = size - |index|, such that
 * [A,B,C,D,E] index(-2) -> D
 * 
 * @param p_array array
 * @param index signed index
 * @param pp_value return
 * 
 * @sa array_get
 * @sa array_slice
 * 
 * @return 1 on success, 0 on error 
 */
int array_index ( array *const p_array, signed index, void **const pp_value );

/** !
 *  Get an array of elements
 *
 * @param p_array array
 * @param pp_elements return
 *
 * @sa array_index
 * @sa array_slice
 *
 * @return  1 on success, 0 on error 
 */
int array_get ( array * const p_array, void ** const pp_elements, size_t *const p_count );

/** !
 * Get a slice of the array specified by a lower bound and an upper bound
 * 
 * @param p_array array
 * @param pp_elements return
 * @param lower_bound the lower bound of the array
 * @param upper_bound the upper bound of the array
 * 
 * @sa array_index
 * @sa array_get
 * 
 * @return 1 on success, 0 on error 
*/
int array_slice ( array *const p_array, void *pp_elements[], signed lower_bound, signed upper_bound );

/** !
 *  Is an array empty?
 * 
 * @param p_array an array
 * 
 * @return true if array has no contents else false
 */
bool array_is_empty ( array *const p_array );

/** !
 *  Get the size of an array
 * 
 * @param p_array an array
 * 
 * @return size of array
 */
size_t array_size ( array *const p_array );

// mutators
/** !
 *  Add an element to the end of an array. 
 *
 * @param p_array array
 * @param p_element the value of the element
 *
 * @return 1 on success, 0 on error
 */
int array_add ( array *const p_array, void *const p_element );

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
int array_set ( array *const p_array, signed index, void *const p_value );

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
int array_remove ( array *const p_array, signed index, void **const pp_value );

/** !
 *  Clear all elements in an array
 *
 * @param p_array array
 *
 * @return 1 on success, 0 on error
 */
int array_clear ( array *const p_array );

/** !
 *  Remove all elements from an array, and deallocate values with free_func
 *
 * @param p_array array
 * @param free_fun_ptr pointer to deallocator function 
 * 
 * @sa array_clear
 *
 * @return 1 on success, 0 on error
 */
int array_free_clear ( array *const p_array, void (*const free_fun_ptr)(void *) );

// iterators
/** !
 * Call function on every element in an array
 *
 * @param p_array  the array
 * @param function pointer to function of type void (*)(void *value, size_t index)
 * 
 * @return 1 on success, 0 on error
 */
int array_foreach_i ( array *const p_array, fn_array_foreach_i *pfn_array_foreach_i );

// info
/** !
 * Call function on every element in p_array
 *
 * @param p_array array
 * @param pfn_next pointer to log function
 * 
 * @return 1 on success, 0 on error
 */
int array_log ( array *p_array, void *pfn_next, const char *const format, ... );

// reflection
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

// destructors
/** !
 *  Destroy and deallocate an array
 *
 * @param pp_array array
 *
 * @sa array_create
 *
 * @return 1 on success, 0 on error
 */
int array_destroy ( array **const pp_array );

// cleanup
/** !
 * This gets called at runtime after main
 * 
 * @param void
 * 
 * @return void
 */
void array_exit ( void ) __attribute__((destructor));
