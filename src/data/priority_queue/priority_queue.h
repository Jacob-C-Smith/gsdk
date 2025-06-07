/** !
 * Include header for priority queue library
 *
 * @file priority_queue/priority_queue.h 
 *
 * @author Jacob Smith
 */

// Include guard
#pragma once

// standard library
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

// sync submodule
#include <core/sync.h>

// Platform dependent macros

// Memory management macro
#ifndef PRIORITY_QUEUE_REALLOC
#define PRIORITY_QUEUE_REALLOC(p, sz) realloc(p,sz)
#endif

// Forward declarations
struct priority_queue_s;

// type definitions
/** !
 *  @brief The type definition of a priority queue struct
 */
typedef struct priority_queue_s priority_queue;

/** !
 *  @brief The type definition for a function that tests the equality of priorities
 * 
 *  @param p_a pointer to a
 *  @param p_b pointer to b
 * 
 *  @return 0 if a == b else -1 if A > B else 1
 */
typedef int (priority_queue_equal_fn)(const void *const p_a, const void *const p_b);

// Allocaters
/** !
 *  Allocate memory for a priority queue
 *
 * @param pp_priority_queue return
 *
 * @sa priority_queue_destroy
 *
 * @return 1 on success, 0 on error
 */
int priority_queue_create ( priority_queue **const pp_priority_queue );

// Constructors
/** !
 *  Construct a priority queue with a specific number of entries
 *
 * @param pp_priority_queue    return
 * @param size                 number of priority queue entries. 
 * @param pfn_compare_function pointer to a compare function, or 0 for default
 *
 * @sa priority_queue_create
 * @sa priority_queue_destroy
 *
 * @return 1 on success, 0 on error
 */
int priority_queue_construct ( priority_queue **const pp_priority_queue, size_t size, priority_queue_equal_fn pfn_compare_function );

// Constructors
/** !
 *  Construct a priority queue with a specific number of entries
 *
 * @param pp_priority_queue    return
 * @param keys                 pointer to null terminated array of keys
 * @param size                 number of priority queue entries. 
 * @param pfn_compare_function pointer to a compare function, or 0 for default
 *
 * @sa priority_queue_create
 * @sa priority_queue_destroy
 *
 * @return 1 on success, 0 on error
 */
int priority_queue_from_keys ( const priority_queue **const pp_priority_queue, const char **const keys, size_t size, priority_queue_equal_fn pfn_compare_function );

// Accessors
/** !
 * Is the priority queue empty?
 * 
 * @param p_priority_queue the priority queue
 * 
 * @return 1 on success, 0 on error
 */
bool priority_queue_empty ( priority_queue *const p_priority_queue );

// Mutators
/** !
 * Add a key to the back of the priority queue
 * 
 * @param p_priority_queue the priority queue
 * @param p_key            the key to insert
 * 
 * @return 1 on success, 0 on error
*/
int priority_queue_enqueue ( priority_queue *const p_priority_queue, void *p_key );

/** !
 * Remove the key in the front of the priority queue
 * 
 * @param p_priority_queue the priority queue
 * @param pp_key           return
 * 
 * @return 1 on success, 0 on error
*/
int priority_queue_dequeue ( priority_queue *const p_priority_queue, void **pp_key );

// Destructors
/** !
 *  Destroy and deallocate a priority queue
 *
 * @param pp_priority_queue pointer to priority queue pointer
 *
 * @sa priority_queue_create
 *
 * @return 1 on success, 0 on error
 */
int priority_queue_destroy ( priority_queue **const pp_priority_queue );
