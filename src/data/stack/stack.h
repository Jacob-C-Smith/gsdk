/** !
 * Include header for stack library
 * 
 * @file stack/stack.h 
 * 
 * @author Jacob Smith 
 */

// Include guard
#pragma once

// standard library
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

// log submodule
#include <core/log.h>

// sync submodule
#include <core/sync.h>

// Platform dependent macros

// Memory management macro
#ifndef STACK_REALLOC
#define STACK_REALLOC(p, sz) realloc(p,sz)
#endif

// Forward declarations
struct stack_s;

// type definitions
typedef struct stack_s stack;

// Initializer
/** !
 * This gets called at runtime before main. 
 * 
 * @param void
 * 
 * @return void
 */
void stack_init ( void ) __attribute__((constructor));

// Constructors 
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

// Mutators
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

// Accessors
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

// Destructors
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

// Cleanup
/** !
 * This gets called at runtime after main
 * 
 * @param void
 * 
 * @return void
 */
void stack_exit ( void ) __attribute__((destructor));
