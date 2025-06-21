/** !
 * Include header for double queue library
 * 
 * @file double_queue/double_queue.h 
 * 
 * @author Jacob Smith
 */

// Include guard
#pragma once

// sync submodule
#include <core/sync.h>

// log submodule
#include <core/log.h>

// standard library
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

// Platform dependent macros

// Memory management macro
#ifndef DOUBLE_QUEUE_REALLOC
#define DOUBLE_QUEUE_REALLOC(p, sz) realloc(p,sz)
#endif

// Forward declarations
struct double_queue_s;

// type definitions
/** !
 *  @brief The type definition of a double queue struct
 */
typedef struct double_queue_s double_queue;

// Initializers
/** !
 * This is called before main
 * 
 * @parma void
 * 
 * @return void
 */
void double_queue_init ( void ) __attribute__((constructor));

// Allocaters
/** !
 *  Allocate memory for a double queue
 *
 * @param pp_double_queue return
 *
 * @sa double_queue_destroy
 *
 * @return 1 on success, 0 on error
 */
int double_queue_create ( double_queue **const pp_double_queue );

// Constructors
/** !
 *  Construct a double queue with a specific number of entries
 *
 * @param pp_double_queue return
 *
 * @sa double_queue_create
 * @sa double_queue_from_contents
 * @sa double_queue_destroy
 *
 * @return 1 on success, 0 on error
 */
int double_queue_construct ( double_queue **const pp_double_queue );

/** !
 *  Construct a double queue from a void pointer array
 *
 * @param pp_double_queue return
 * @param pp_contents     pointer to array of void pointers to use as double queue contents.
 * @param size            number of double queue entries. 
 *
 * @sa double_queue_construct
 * @sa double_queue_destroy
 *
 * @return 1 on success, 0 on error
 */
int double_queue_from_contents ( double_queue **const pp_double_queue, void * const* const pp_contents, size_t size );

// Accessors
/** !
 *  Get the element at the front of the double queue
 *
 * @param p_double_queue double queue
 *
 * @sa double_queue_rear
 *
 * @return element value on success, null pointer on error
 */
int double_queue_front ( const double_queue *const p_double_queue, void **const pp_value );

/** !
 *  Get the element at the rear of the double queue
 *
 * @param p_double_queue double queue
 *
 * @sa double_queue_front
 *
 * @return element on success, null pointer on error
 */
int double_queue_rear ( const double_queue *const p_double_queue, void **const pp_value );

/** !
 *  Check if a double queue is empty
 *
 * @param p_double_queue double queue
 *
 * @sa double_queue_full
 *
 * @return true if double queue is empty else false
 */
bool double_queue_empty ( const double_queue *const p_double_queue );

// Mutators
/** !
 *  Add an element to the front of a double queue
 *
 * @param p_double_queue the double queue
 * @param data           element to add to the double queue
 *
 * @sa double_queue_front_remove
 *
 * @return 1 on success, 0 on error
 */
int double_queue_front_add ( double_queue *const p_double_queue, void *const data );

/** !
 *  Remove an element from the front of a double queue
 *
 * @param p_double_queue the double queue
 * @param pp_value       return
 *
 * @sa double_queue_front_add
 *
 * @return 1 on success, 0 on error
 */
int double_queue_front_remove ( double_queue *const p_double_queue, void **const pp_value );

/** !
 *  Add an element to the rear of a double queue
 *
 * @param p_double_queue the double queue
 * @param data           element to add to the double queue
 *
 * @sa double_queue_rear_remove
 *
 * @return 1 on success, 0 on error
 */
int double_queue_rear_add ( double_queue *const p_double_queue, void *const data );

/** !
 *  Remove an element from the rear of a double queue
 *
 * @param p_double_queue the double queue
 * @param pp_value       return
 *
 * @sa double_queue_rear_add
 *
 * @return 1 on success, 0 on error
 */
int double_queue_rear_remove ( double_queue *const p_double_queue, void **const pp_value );

// Destructors
/** !
 *  Destroy and deallocate a double queue
 *
 * @param pp_double_queue pointer to double queue
 *
 * @sa double_queue_create
 *
 * @return 1 on success, 0 on error
 */
int double_queue_destroy ( double_queue **const pp_double_queue );
