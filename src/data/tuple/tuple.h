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

// Debug mode
#undef NDEBUG

// forward declarations
struct tuple_s;

// type definitions
/** !
 *  @brief The type definition of a tuple struct
 */
typedef struct tuple_s tuple;

// initializers
/** !
 * This gets called once before main
 * 
 * @param void
 * 
 * @return void
*/
void tuple_init ( void ) __attribute__((constructor));

// Allocaters
/** !
 *  Allocate memory for a tuple
 *
 * @param pp_tuple return
 *
 * @sa tuple_destroy
 *
 * @return 1 on success, 0 on error
 */
int tuple_create ( tuple **const pp_tuple );

// constructors
/** !
 *  Construct a tuple with a specific size
 *
 * @param pp_tuple return
 * @param size number of elements in a tuple
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
 * @param pp_tuple return
 * @param elements pointer to null terminated tuple of element pointers
 * @param size number of elements. 
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
 * @param pp_tuple      return
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

// accessors
/** !
 * Index a tuple with a signed number. If index is negative, index = size - |index|, such that
 * [A,B,C,D,E] index(-2) -> D
 * 
 * @param p_tuple tuple
 * @param index signed index. 
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
 * @param p_tuple tuple
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
 * @param p_tuple a tuple
 * 
 * @return true if tuple has no contents else false
 */
bool tuple_is_empty ( const tuple *const p_tuple );

/** !
 *  Get the size of a tuple
 * 
 * @param p_tuple a tuple
 * 
 * @return size of tuple
 */
size_t tuple_size ( const tuple *const p_tuple );

// iterators
/** !
 * Call function on every element in p_tuple
 *
 * @param p_tuple tuple
 * @param function pointer to function of type void (*)(void *value, size_t index)
 * 
 * @return 1 on success, 0 on error
 */
int tuple_foreach_i ( const tuple *const p_tuple, void (*const function)(void *const value, size_t index) );

int tuple_pack ( tuple *p_tuple, void *p_buffer, fn_pack *pfn_element );

int tuple_unpack ( tuple **pp_tuple, void *p_buffer, fn_unpack *pfn_element );

// destructors
/** !
 *  Destroy and deallocate a tuple
 *
 * @param pp_tuple tuple
 *
 * @sa tuple_create
 *
 * @return 1 on success, 0 on error
 */
int tuple_destroy ( tuple **const pp_tuple );

// cleanup
/** !
 * This gets called once after main
 * 
 * @param void
 * 
 * @return void
*/
void tuple_exit ( void ) __attribute__((destructor));
