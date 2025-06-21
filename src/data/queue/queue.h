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

// sync module
#include <core/sync.h>

// forward declarations
struct queue_s;

// type definitions
/** !
 *  @brief The type definition of a queue struct
 */
typedef struct queue_s queue;

// initializer
/** !
 * This gets called at runtime before main. 
 * 
 * @param void
 * 
 * @return void
 */
void queue_init ( void ) __attribute__((constructor));

// Allocaters
/** !
 *  Allocate memory for a queue
 *
 * @param pp_queue return
 *
 * @sa destroy_queue
 *
 * @return 1 on success, 0 on error
 */
int queue_create ( queue **const pp_queue );

// constructors
/** !
 *  Construct a queue with a specific number of entries
 *
 * @param pp_queue return
 *
 * @sa queue_create
 * @sa queue_from_contents
 * @sa queue_destroy
 *
 * @return 1 on success, 0 on error
 */
int queue_construct ( queue **const pp_queue );

/** !
 * TODO:
 *  Construct a queue from a void pointer array
 *
 * @param pp_queue    return
 * @param pp_contents pointer to array of void pointers to use as queue contents.
 * @param size        number of queue entries. 
 *
 * @sa queue_construct
 * @sa queue_destroy
 *
 * @return 1 on success, 0 on error
 */
int queue_from_contents ( queue **const pp_queue, void * const* const pp_contents, size_t size );

// accessors
/** !
 *  Get the element at the front of the queue
 *
 * @param p_queue queue
 *
 * @sa queue_rear
 *
 * @return element value on success, null pointer on error
 */
int queue_front ( queue *const p_queue, void **const pp_value );

/** !
 *  Get the element at the rear of the queue
 *
 * @param p_queue queue
 *
 * @sa queue_front
 *
 * @return element on success, null pointer on error
 */
int queue_rear ( queue *const p_queue, void **const pp_value );

// mutators
/** !
 *  Add an element to a queue
 *
 * @param p_queue queue
 * @param data    element to add to the queue
 *
 * @sa queue_dequeue
 *
 * @return 1 on success, 0 on error
 */
int queue_enqueue ( queue *const p_queue, void *const data );

/** !
 *  Remove an element from a queue
 *
 * @param p_queue queue
 *
 * @sa queue_enqueue
 *
 * @return element on success, null pointer on error
 */
int queue_dequeue ( queue *const p_queue, void **const pp_value );

/** !
 *  Check if a queue is empty
 *
 * @param p_queue queue
 *
 * @sa queue_full
 *
 * @return true if queue is empty else false
 */
bool queue_empty ( queue *const p_queue );

// destructors
/** !
 *  Destroy and deallocate a queue
 *
 * @param pp_queue queue
 *
 * @sa queue_create
 *
 * @return 1 on success, 0 on error
 */
int queue_destroy ( queue **const pp_queue );

// cleanup
/** !
 * This gets called at runtime after main
 * 
 * @param void
 * 
 * @return void
 */
void queue_exit ( void ) __attribute__((destructor));
