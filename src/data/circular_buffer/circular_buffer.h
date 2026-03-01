/** !
 * Circular buffer interface
 * 
 * @file src/data/circular_buffer/circular_buffer.h
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
#include <core/sync.h>
#include <core/pack.h>
#include <core/hash.h>
#include <core/interfaces.h>

// structure definitions
struct circular_buffer_s
{
	bool full;
	size_t read, write, length;
	mutex _lock;
	void *_p_data[];
};

// type definitions
typedef struct circular_buffer_s circular_buffer;

// function definitions
/// constructors
/** !
 *  Construct a circular buffer with a specific number of entries
 *
 * @param pp_circular_buffer return
 * @param size               the maximum quantity of elements 
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
 * @return 1 on success, 0 on error
 */
int circular_buffer_from_contents ( circular_buffer **const pp_circular_buffer, const void *const *pp_contents, size_t size );

/// accessors
/** !
 *  Check if a circular buffer is empty
 *
 * @param p_circular_buffer the circular buffer
 *
 * @return true IF circular buffer is empty ELSE false
 */
bool circular_buffer_empty ( circular_buffer *const p_circular_buffer );

/** !
 *  Check if a circular buffer is full
 *
 * @param p_circular_buffer the circular buffer
 *
 * @return true IF circular buffer is empty ELSE false
 */
bool circular_buffer_full ( circular_buffer *const p_circular_buffer );

/** !
 *  Get the size of a circular buffer
 * 
 * @param p_circular_buffer the circular buffer
 * 
 * @return size of circular buffer
 */
size_t circular_buffer_size ( circular_buffer *p_circular_buffer );

/** !
 * Get the last value in the circular buffer
 * 
 * @param p_circular_buffer the circular buffer
 * @param pp_data           result
 * 
 * @return 1 on success, 0 on error
 */
int circular_buffer_peek ( circular_buffer *const p_circular_buffer, void **pp_data );

/// mutators
/** !
 * Add a value to a circular buffer
 * 
 * @param p_circular_buffer the circular buffer
 * @param p_data            the value
 * 
 * @return 1 on success, 0 on error
 */
int circular_buffer_push ( circular_buffer *const p_circular_buffer, void  *p_data );

/** !
 * Remove a value from a circular buffer
 * 
 * @param p_circular_buffer the circular buffer
 * @param pp_data           result IF not null ELSE ignored
 * 
 * @return 1 on success, 0 on error
 */
int circular_buffer_pop  ( circular_buffer *const p_circular_buffer, void **pp_data );

/// map
/** !
 * Apply an operation to each element in a circular buffer,
 * optionally releasing elements
 *
 * @param p_circular_buffer the circular buffer
 * @param pfn_map           pointer to map function
 * @param pfn_allocator     pointer to allocator function
 * 
 * @return 1 on success, 0 on error
 */
int circular_buffer_map ( circular_buffer *const p_circular_buffer, fn_map *pfn_map, fn_allocator *pfn_allocator );

/// iterators
/** !
 * Call function on every element in a circular buffer
 *
 * @param p_circular_buffer the circular buffer
 * @param pfn_foreach       pointer to foreach function
 * 
 * @return 1 on success, 0 on error
 */
int circular_buffer_foreach ( circular_buffer *p_circular_buffer, fn_foreach *pfn_foreach );

/// reflection
/** !
 * Pack a circular buffer into a buffer
 * 
 * @param p_buffer          result
 * @param p_circular_buffer the circular buffer
 * @param pfn_elemenet      pointer to pack function IF not null ELSE default
 * 
 * @return 1 on success, 0 on error
 */
int circular_buffer_pack ( void *p_buffer, circular_buffer *p_circular_buffer, fn_pack *pfn_element );

/** !
 * Unpack a buffer into a circular buffer
 * 
 * @param pp_circular_buffer result
 * @param p_buffer           the buffer
 * @param pfn_elemenet       pointer to unpack function IF not null ELSE default
 * 
 * @return 1 on success, 0 on error
 */
int circular_buffer_unpack ( circular_buffer **pp_circular_buffer, void *p_buffer, fn_unpack *pfn_element );

/// hash
/** !
 * Compute a 64-bit hash of a circular buffer
 * 
 * @param p_circular_buffer the circular buffer
 * @param pfn_element       hashing function applied to each element
 * 
 * @return hash on success, NULL on error
 */
hash64 circular_buffer_hash ( circular_buffer *p_circular_buffer, fn_hash64 *pfn_element );

/// destructors
/** !
 *  Destroy and deallocate a circular buffer
 *
 * @param pp_circular_buffer pointer to the circular buffer
 * @param pfn_allocator      pointer to allocator function for deallocating elements
 *
 * @return 1 on success, 0 on error
 */
int circular_buffer_destroy ( circular_buffer **const pp_circular_buffer, fn_allocator *pfn_allocator );
