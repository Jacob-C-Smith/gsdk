/** !
 * Include header for stack library
 * 
 * @file core/stack.h 
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
#include <core/hash.h>
#include <core/log.h>
#include <core/pack.h>
#include <core/sync.h>

// forward declarations
struct stack_s;

// type definitions
typedef struct stack_s stack;

// function declarations
/// constructors 
/** !
 * Construct a stack of a specified size
 * 
 * @param pp_stack result
 * 
 * @sa stack_destroy
 * 
 * @return 1 on success, 0 on error
*/
int stack_construct ( stack **const pp_stack, size_t size );

/// mutators
/** !
 * Push a value onto a stack
 * 
 * @param p_stack the stack
 * @param p_value the value
 * 
 * @sa stack_pop
 * 
 * @return 1 on success, 0 on error
*/
int stack_push ( stack *const p_stack, const void *const p_value );

/** !
 * Pop a value off a stack
 * 
 * @param p_stack the stack
 * @param ret result
 * 
 * @sa stack_push
 * 
 * @return 1 on success, 0 on error
*/
int stack_pop ( stack *const p_stack, const void **const ret );

/// accessors
/** !
 * Peek the top of the stack
 * 
 * @param p_stack the stack
 * @param ret result
 * 
 * @sa stack_pop
 * 
 * @return 1 on success, 0 on error
*/
int stack_peek ( stack *const p_stack, const void **const ret );

/// iterators
/** !
 * Call function on every element in a stack
 *
 * @param p_stack  the stack
 * @param pfn_fori pointer to fori function
 * 
 * @return 1 on success, 0 on error
 */
int stack_fori ( stack *const p_stack, fn_fori *pfn_fori );

/// reflection
/** !
 * Pack a stack into a buffer
 * 
 * @param p_buffer     the buffer
 * @param p_stack      the stack
 * @param pfn_elemenet pointer to pack function IF not null ELSE default
 * 
 * @return 1 on success, 0 on error
 */
int stack_pack ( void *p_buffer, stack *p_stack, fn_pack *pfn_element );

/** !
 * Unpack a buffer into a stack
 * 
 * @param pp_stack     result
 * @param p_buffer     the buffer
 * @param pfn_elemenet pointer to unpack function IF not null ELSE default
 * 
 * @return 1 on success, 0 on error
 */
int stack_unpack ( stack **pp_stack, void *p_buffer, fn_unpack *pfn_element );

/// hash
/** !
 * Compute a 64-bit hash of a stack
 * 
 * @param p_stack
 * 
 * @return hash on success, NULL on error
 */
hash64 stack_hash ( stack *p_stack, fn_hash64 *pfn_element );

/// destructors
/** !
 * Deallocate a stack
 * 
 * @param pp_stack pointer to stack pointer
 * 
 * @sa stack_construct
 * 
 * @return 1 on success, 0 on error
*/
int stack_destroy ( stack **const pp_stack );
