/** !
 * Include header for tree library
 * 
 * @file tree/tree.h 
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



// Memory management macro
#ifndef TREE_REALLOC
    #define TREE_REALLOC(p, sz) realloc(p,sz)
#endif

// type definitions

/** !
 *  @brief The type definition for a two dimensional vector
 */ 
typedef struct { double x, y; } tree_vec2;

/** !
 *  @brief The type definition for a function that tests the equality of two nodes
 * 
 *  @param p_a pointer to a
 *  @param p_b pointer to b
 * 
 *  @return 0 if a == b else -1 if A > B else 1
 */
typedef int (fn_tree_equal) (const void *const p_a, const void *const p_b);

/** !
 *  @brief The type definition for a function that accesses the key of a given value
 * 
 *  @param p_value the value
 * 
 *  @return 1 on success, 0 on error
 */
typedef const void *(fn_tree_key_accessor)(const void *const p_value);

// Initializer
int tree_init ( void ); 

// Comparator
/** !
 * Compare a to b
 * 
 * @param p_a pointer to a
 * @param p_b pointer to b
 * 
 * @return 0 if a == b else -1 if a > b else 1
 */
int tree_compare_function ( const void *const p_a, const void *const p_b );

// Accessors
/** !
 * Default key accessor
 * 
 * @param pp_result result
 * @param p_value   the value to extract the key from
 * 
 * @return 1 on success, 0 on error
 */
const void *tree_key_is_value ( const void *const p_value );
