/** !
 * double ended queue interface
 * 
 * @file src/data/double_queue/double_queue.h 
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
typedef struct double_queue_s double_queue;

// function declarations
/// constructors
/** !
 * Construct a double queue with a specific number of entries
 *
 * @param pp_double_queue result
 *
 * @return 1 on success, 0 on error
 */
int double_queue_construct ( double_queue **const pp_double_queue );

/** !
 * Construct a double ended queue from an array 
 *
 * @param pp_double_queue result
 * @param pp_contents     pointer to array
 * @param size            quantity of double queue entries 
 *
 * @return 1 on success, 0 on error
 */
int double_queue_from_contents ( double_queue **const pp_double_queue, void * const* const pp_contents, size_t size );

/// accessors
/** !
 *  Get the element at the front of a double ended queue
 *
 * @param p_double_queue the double ended queue
 *
 * @return element on success, NULL on error
 */
int double_queue_front ( double_queue *const p_double_queue, void **const pp_value );

/** !
 *  Get the element at the rear of a double ended queue
 *
 * @param p_double_queue the double ended queue
 *
 * @return element on success, NULL on error
 */
int double_queue_rear ( double_queue *const p_double_queue, void **const pp_value );

/** !
 * Get the quantity of elements a double ended queue
 * 
 * @param p_double_queue the double ended queue 
 * 
 * @return the quantity of the elements in the double ended queue
 */
size_t double_queue_size ( double_queue *const p_double_queue );

/** !
 * Test if a double ended queue is empty
 *
 * @param p_double_queue the double ended queue
 *
 * @return true IF double ended queue is empty ELSE false
 */
bool double_queue_empty ( double_queue *const p_double_queue );

/// mutators
/** !
 * Add an element to the front of a double ended queue
 *
 * @param p_double_queue the double queue
 * @param data           the element
 * 
 * @return 1 on success, 0 on error
 */
int double_queue_front_add ( double_queue *const p_double_queue, void *const p_value );

/** !
 * Remove an element from the front of a double ended queue
 *
 * @param p_double_queue the double queue
 * @param pp_data        result IF not null ELSE ignored
 *
 * @return 1 on success, 0 on error
 */
int double_queue_front_remove ( double_queue *const p_double_queue, void **const pp_value );

/** !
 * Add an element to the rear of a double ended queue
 *
 * @param p_double_queue the double queue
 * @param data           the element
 *
 * @return 1 on success, 0 on error
 */
int double_queue_rear_add ( double_queue *const p_double_queue, void *const p_value );

/** !
 * Remove an element from the rear of a double ended queue
 *
 * @param p_double_queue the double ended queue
 * @param pp_data        result IF not null ELSE ignored
 *
 * @return 1 on success, 0 on error
 */
int double_queue_rear_remove ( double_queue *const p_double_queue, void **const pp_value );

/// map
/** !
 * Apply an operation to each element in a double ended queue,
 * optionally releasing elements
 *
 * @param p_double_queue the double ended queue
 * @param pfn_map        pointer to map function
 * @param pfn_allocator  pointer to allocator function
 * 
 * @return 1 on success, 0 on error
 */
int double_queue_map ( double_queue *const p_double_queue, fn_map *pfn_map, fn_allocator *pfn_allocator );

/// iterators
/** !
 * Call function on every element in a double ended queue
 *
 * @param p_double_queue the double ended queue
 * @param pfn_foreach    pointer to foreach function
 * 
 * @return 1 on success, 0 on error
 */
int double_queue_foreach ( double_queue *const p_double_queue, fn_foreach *pfn_foreach );

/** !
 * Call function on every element in a double ended queue
 *
 * @param p_double_queue the double ended queue
 * @param pfn_fori       pointer to fori function
 * 
 * @return 1 on success, 0 on error
 */
int double_queue_fori ( double_queue *const p_double_queue, fn_fori *pfn_fori );

/// reflection
/** !
 * Pack a double ended queue into a buffer
 * 
 * @param p_buffer       result
 * @param p_double_queue the double ended queue
 * @param pfn_elemenet   pointer to pack function IF not null ELSE default
 * 
 * @return 1 on success, 0 on error
 */
int double_queue_pack ( void *p_buffer, double_queue *p_double_queue, fn_pack *pfn_element );

/** !
 * Unpack a buffer into a double ended queue
 * 
 * @param pp_double_queue result
 * @param p_buffer        the buffer
 * @param pfn_elemenet    pointer to unpack function IF not null ELSE default
 * 
 * @return 1 on success, 0 on error
 */
int double_queue_unpack ( double_queue **pp_double_queue, void *p_buffer, fn_unpack *pfn_element );

/// hash
/** !
 * Compute a 64-bit hash of a double ended queue
 * 
 * @param p_double_queue the double ended queue
 * @param pfn_element    hashing function applied to each element
 * 
 * @return hash on success, NULL on error
 */
hash64 double_queue_hash ( double_queue *p_double_queue, fn_hash64 *pfn_element );

/// destructors
/** !
 * Destroy and deallocate a double ended queue
 * 
 * @param pp_double_queue pointer to double ended queue pointer
 * @param pfn_allocator   pointer to allocator function for deallocating elements
 * 
 * @return 1 on success, 0 on error 
 */
int double_queue_destroy ( double_queue **const pp_double_queue, fn_allocator *pfn_allocator );
