/** !
 * Include header for queue library
 * 
 * @file queue/queue.h
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

// core
#include <core/log.h>
#include <core/sync.h>
#include <core/pack.h>
#include <core/hash.h>
#include <core/interfaces.h>

// forward declarations
struct queue_s;

// type definitions
/** !
 *  @brief The type definition of a queue struct
 */
typedef struct queue_s queue;

// function declarations
/// constructors
/** !
 *  Construct a queue with a specific number of entries
 *
 * @param pp_queue result
 *
 * @return 1 on success, 0 on error
 */
int queue_construct ( queue **const pp_queue );

/** !
 *  Construct a queue from a void pointer array
 *
 * @param pp_queue    result
 * @param pp_contents pointer to array of void pointers to use as queue contents.
 * @param size        number of queue entries. 
 *
 * @return 1 on success, 0 on error
 */
int queue_from_contents ( queue **const pp_queue, void * const* const pp_contents, size_t size );

/// accessors
/** !
 *  Get the element at the front of the queue
 *
 * @param p_queue the queue
 *
 * @return element value on success, null pointer on error
 */
int queue_front ( queue *const p_queue, void **const pp_value );

/** !
 *  Get the element at the rear of the queue
 *
 * @param p_queue the queue
 *
 * @sa queue_front
 *
 * @return element on success, null pointer on error
 */
int queue_rear ( queue *const p_queue, void **const pp_value );

/** !
 *  Check if a queue is empty
 *
 * @param p_queue the queue
 *
 * @return true if queue is empty else false
 */
bool queue_empty ( queue *const p_queue );

/** !
 *  Get the quantity of elements in a queue
 *
 * @param p_queue the queue
 *
 * @return the quantity of elements in the queue
 */
bool queue_size ( queue *const p_queue );

/// mutators
/** !
 *  Add an element to a queue
 *
 * @param p_queue the queue
 * @param data    element to add to the queue
 *
 * @return 1 on success, 0 on error
 */
int queue_enqueue ( queue *const p_queue, void *const data );

/** !
 *  Remove an element from a queue
 *
 * @param p_queue  the queue
 * @param pp_value 
 *
 * @return element on success, null pointer on error
 */
int queue_dequeue ( queue *const p_queue, void **const pp_value );

/// iterators
/** !
 * Call function on every element in a queue
 *
 * @param p_queue  the queue
 * @param pfn_fori pointer to fori function
 * 
 * @return 1 on success, 0 on error
 */
int queue_fori ( queue *const p_queue, fn_fori *pfn_fori );

/// reflection
/** !
 * Pack a queue into a buffer
 * 
 * @param p_buffer     the buffer
 * @param p_queue      the queue
 * @param pfn_elemenet pointer to pack function IF not null ELSE default
 * 
 * @return 1 on success, 0 on error
 */
int queue_pack ( void *p_buffer, queue *p_queue, fn_pack *pfn_element );

/** !
 * Unpack a buffer into a queue
 * 
 * @param pp_queue     result
 * @param p_buffer     the buffer
 * @param pfn_elemenet pointer to unpack function IF not null ELSE default
 * 
 * @return 1 on success, 0 on error
 */
int queue_unpack ( queue **pp_queue, void *p_buffer, fn_unpack *pfn_element );

/// hash
/** !
 * Compute a 64-bit hash of a queue
 * 
 * @param p_queue
 * 
 * @return hash on success, NULL on error
 */
hash64 queue_hash ( queue *p_queue, fn_hash64 *pfn_element );

/// destructors
/** !
 *  Destroy and deallocate a queue
 *
 * @param pp_queue the queue
 *
 * @return 1 on success, 0 on error
 */
int queue_destroy ( queue **const pp_queue );
