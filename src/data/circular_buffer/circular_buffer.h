/** !
 * Include header for circular buffer library
 * 
 * @file circular_buffer/circular_buffer.h
 * 
 * @author Jacob Smith
 */

// Include guard
#pragma once

// sync submodule
#include <core/sync.h>

// standard library
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

// Platform dependent macros

// Memory management macro
#ifndef CIRCULAR_BUFFER_REALLOC
#define CIRCULAR_BUFFER_REALLOC(p, sz) realloc(p,sz)
#endif

// Forward declarations
// Structure definitions
struct circular_buffer_s
{
	bool full;
	size_t read, write, length;
	mutex _lock;
	void *_p_data[];
};

// type definitions
/** !
 *  @brief The type definition of a circular buffer struct
 */
typedef struct circular_buffer_s circular_buffer;

// Constructors
/** !
 *  Construct a circular buffer with a specific number of entries
 *
 * @param pp_circular_buffer return
 * @param size               the maximum quantity of elements 
 *
 * @sa circular_buffer_from_contents
 * @sa circular_buffer_destroy
 *
 * @return 1 on success, 0 on error
 */
int circular_buffer_construct ( circular_buffer **const pp_circular_buffer, size_t size );

/** !
 * TODO:
 *  Construct a circular buffer from a void pointer array
 *
 * @param pp_circular_buffer return
 * @param pp_contents        pointer to array of void pointers to use as circular buffer contents.
 * @param size               number of circular buffer entries. 
 *
 * @sa circular_buffer_construct
 * @sa circular_buffer_destroy
 *
 * @return 1 on success, 0 on error
 */
int circular_buffer_from_contents ( circular_buffer **const pp_circular_buffer, const void *const *pp_contents, size_t size );

// Accessors
/** !
 *  Check if a circular buffer is empty
 *
 * @param p_circular_buffer the circular buffer
 *
 * @sa circular_buffer_full
 *
 * @return true if circular buffer is empty else false
 */
bool circular_buffer_empty ( circular_buffer *const p_circular_buffer );

/** !
 *  Check if a circular buffer is full
 *
 * @param p_circular_buffer the circular buffer
 *
 * @sa circular_buffer_empty
 *
 * @return true if circular buffer is empty else false
 */
bool circular_buffer_full ( circular_buffer *const p_circular_buffer );

// Mutators
/** !
 * Add a value to a circular buffer
 * 
 * @param p_circular_buffer the circular buffer
 * @param p_data            the value
 * 
 * @sa circular_buffer_peek
 * @sa circular_buffer_pop
 * 
 * @return 1 on success, 0 on error
 */
int circular_buffer_push ( circular_buffer *const p_circular_buffer, void  *p_data );

/** !
 * Get the last value in the circular buffer
 * 
 * @param p_circular_buffer the circular buffer
 * @param pp_data           result
 * 
 * @sa circular_buffer_push
 * @sa circular_buffer_pop
 * 
 * @return 1 on success, 0 on error
 */
int circular_buffer_peek ( circular_buffer *const p_circular_buffer, void **pp_data );

/** !
 * Remove a value from a circular buffer
 * 
 * @param p_circular_buffer the circular buffer
 * @param pp_data           result
 * 
 * @sa circular_buffer_push
 * @sa circular_buffer_peek
 * 
 * @return 1 on success, 0 on error
 */
int circular_buffer_pop  ( circular_buffer *const p_circular_buffer, void **pp_data );

// Destructors
/** !
 *  Destroy and deallocate a circular buffer
 *
 * @param pp_circular_buffer pointer to the circular buffer
 *
 * @sa circular_buffer_construct
 *
 * @return 1 on success, 0 on error
 */
int circular_buffer_destroy ( circular_buffer **const pp_circular_buffer );
