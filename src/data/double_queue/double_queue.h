/** !
 * Include header for double queue library
 * 
 * @file double_queue/double_queue.h 
 * 
 * @author Jacob Smith
 */

// header guard
#pragma once

// standard library
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

// gsdk
/// core
#include <core/log.h>
#include <core/pack.h>
#include <core/hash.h>
#include <core/sync.h>
#include <core/interfaces.h>

// forward declarations
struct double_queue_s;

// type definitions
/** !
 *  @brief The type definition of a double queue struct
 */
typedef struct double_queue_s double_queue;

// allocaters
/** !
 *  Allocate memory for a double queue
 *
 * @param pp_double_queue return
 *
 * @return 1 on success, 0 on error
 */
int double_queue_create ( double_queue **const pp_double_queue );

// constructors
/** !
 *  Construct a double queue with a specific number of entries
 *
 * @param pp_double_queue return
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
 * @return 1 on success, 0 on error
 */
int double_queue_from_contents ( double_queue **const pp_double_queue, void * const* const pp_contents, size_t size );

// accessors
/** !
 *  Get the element at the front of the double queue
 *
 * @param p_double_queue double queue
 *
 * @return element value on success, null pointer on error
 */
int double_queue_front ( const double_queue *const p_double_queue, void **const pp_value );

/** !
 *  Get the element at the rear of the double queue
 *
 * @param p_double_queue double queue
 *
 * @return element on success, null pointer on error
 */
int double_queue_rear ( const double_queue *const p_double_queue, void **const pp_value );

/** !
 *  Check if a double queue is empty
 *
 * @param p_double_queue double queue
 *
 * @return true if double queue is empty else false
 */
bool double_queue_empty ( const double_queue *const p_double_queue );

// mutators
/** !
 *  Add an element to the front of a double queue
 *
 * @param p_double_queue the double queue
 * @param data           element to add to the double queue
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
 * @return 1 on success, 0 on error
 */
int double_queue_front_remove ( double_queue *const p_double_queue, void **const pp_value );

/** !
 *  Add an element to the rear of a double queue
 *
 * @param p_double_queue the double queue
 * @param data           element to add to the double queue
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
 * @return 1 on success, 0 on error
 */
int double_queue_rear_remove ( double_queue *const p_double_queue, void **const pp_value );

/** !
 * Destroy a double queue
 * 
 * @param pp_double_queue pointer to double queue pointer
 * 
 * @return 1 on success, 0 on error 
 */
int double_queue_destroy ( double_queue **const pp_double_queue );
