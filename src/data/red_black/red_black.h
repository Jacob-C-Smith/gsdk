/** !
 * Red Black tree interface
 *
 * @file src/data/red_black/red_black.h 
 * 
 * @author Jacob Smith
 */

// header guard
#pragma once

// standard library
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

// gsdk
/// core
#include <core/log.h>
#include <core/hash.h>
#include <core/pack.h>
#include <core/sync.h>
#include <core/interfaces.h>

// forward declarations
struct red_black_tree_s;
struct red_black_tree_node_s;

// type definitions
typedef struct red_black_tree_s      red_black_tree;
typedef struct red_black_tree_node_s red_black_tree_node;

enum red_black_tree_node_color_e
{
    RED_BLACK_TREE_NODE_COLOR_BLACK = 0,
    RED_BLACK_TREE_NODE_COLOR_RED   = 1
} red_black_tree_node_color;

// structure definitions
struct red_black_tree_node_s
{ 
    void *p_value;
    red_black_tree_node *p_left, *p_right, *p_parent;
    enum red_black_tree_node_color_e color;
    unsigned long long  node_pointer;
};

struct red_black_tree_s
{
    mutex             _lock;
    red_black_tree_node *p_root;
    FILE             *p_random_access;
    
    struct 
    {
        unsigned long long quantity;
        unsigned long long size;
    } metadata;

    fn_comparator   *pfn_comparator;
    fn_key_accessor *pfn_key_accessor;
    fn_pack         *pfn_pack;
    fn_unpack       *pfn_unpack;
};

// function declarations
/// constructors
/** !
 * Construct an empty red black tree
 * 
 * @param pp_red_black_tree result
 * @param node_size         the size of a serialized value in bytes
 * @param pfn_comparator    function for testing equality of elements in set IF parameter is not null ELSE default
 * @param pfn_key_accessor  function for accessing the key of a value IF parameter is not null ELSE default
 * 
 * @return 1 on success, 0 on error
 */
int red_black_tree_construct 
(
    red_black_tree **const  pp_red_black_tree,
    unsigned long long      node_size,
    fn_comparator          *pfn_comparator, 
    fn_key_accessor        *pfn_key_accessor 
);

/// accessors
/** !
 * Search a red black tree for an element
 * 
 * @param p_red_black_tree the red black tree
 * @param p_value          the element
 * @param pp_value         result
 * 
 * @return 1 on success, 0 on error
 */
int red_black_tree_search ( red_black_tree *const p_red_black_tree, const void *const p_key, void **pp_value );

/** !
 * Is a red black tree empty?
 * 
 * @param p_red_black the red black tree
 * 
 * @return true if red black tree has no values else false
 */
bool red_black_tree_is_empty ( red_black_tree *p_red_black );

/** !
 * Get the size of a red black tree
 * 
 * @param p_red_black the red black tree
 * 
 * @return size of red black tree
 */
size_t red_black_tree_size ( red_black_tree *p_red_black );

/// mutators
/** !
 * Insert a value into a red black tree
 * 
 * @param p_red_black_tree the red black tree
 * @param p_value          the value
 * 
 * @return 1 on success, 0 on error
 */
int red_black_tree_insert ( red_black_tree *const p_red_black_tree, const void *const p_value );

/** !
 * Remove an element from a red black tree
 * 
 * @param p_red_black_tree the red black tree
 * @param p_key            the element
 * @param pp_value         return
 * 
 * @return 1 on success, 0 on error
 */
int red_black_tree_remove ( red_black_tree *const p_red_black_tree, const void *const p_key, const void **const p_value );

/// iterator
/** !
 * Traverse a red black tree using the pre order technique
 * 
 * @param p_red_black_tree pointer to red black tree
 * @param pfn_foreach      called for each node in the red black tree
 * 
 * @return 1 on success, 0 on error
*/
int red_black_tree_traverse_preorder ( red_black_tree *const p_red_black_tree, fn_foreach *pfn_foreach );

/** !
 * Traverse a red black tree using the in order technique
 * 
 * @param p_red_black_tree pointer to red black tree
 * @param pfn_foreach      called for each node in the red black tree
 * 
 * @return 1 on success, 0 on error
*/
int red_black_tree_traverse_inorder ( red_black_tree *const p_red_black_tree, fn_foreach *pfn_foreach );

/** !
 * Traverse a red black tree using the post order technique
 * 
 * @param p_red_black_tree pointer to red black tree
 * @param pfn_foreach      called for each node in the red black tree
 * 
 * @return 1 on success, 0 on error
*/
int red_black_tree_traverse_postorder ( red_black_tree *const p_red_black_tree, fn_foreach *pfn_foreach );

/** !
 * Call function on every node in a red black tree
 *
 * @param p_red_black_tree the red black tree
 * @param pfn_forcontext   pointer to forcontext function
 * @param p_context        the context
 * 
 * @return 1 on success, 0 on error
 */
int red_black_tree_forcontext ( red_black_tree *const p_red_black_tree, fn_forcontext *pfn_forcontext, void *p_context );

/// reflection
/** !
 * Pack a red black tree into a buffer
 * 
 * @param p_buffer         the buffer
 * @param p_red_black_tree the red black tree
 * @param pfn_element      pointer to pack function IF not null ELSE default
 * 
 * @return 1 on success, 0 on error
 */
int red_black_tree_pack ( void *p_buffer, red_black_tree *p_red_black_tree, fn_pack *pfn_element );

/** !
 * Unpack a red black tree into a buffer
 * 
 * @param pp_red_black_tree result
 * @param p_buffer          the buffer
 * @param pfn_element       pointer to unpack function IF not null ELSE default
 * @param pfn_comparator    function for testing equality of elements in set IF parameter is not null ELSE default
 * @param pfn_key_accessor  function for accessing the key of a value IF parameter is not null ELSE default
 * 
 * @return 1 on success, 0 on error
 */
int red_black_tree_unpack
( 
    red_black_tree **pp_red_black_tree, 
    void *p_buffer, 
    
    fn_unpack       *pfn_element, 
    fn_comparator   *pfn_comparator, 
    fn_key_accessor *pfn_key_accessor
);

/** !
 * Compute the hash of a red black tree
 * 
 * @param p_red_black_tree the red black tree
 * @param pfn_hash64       pointer to hashing function
 * 
 * @return the hash of a red black tree
 */
hash64 red_black_tree_hash ( red_black_tree *const p_red_black_tree, fn_hash64 *pfn_hash64 );

/// destructors
/** !
 * Destroy and deallocate a red black tree
 * 
 * @param pp_red_black_tree pointer to red black tree pointer
 * @param pfn_allocator     pointer to allocator function for deallocating elements
 * 
 * @return 1 on success, 0 on error
 */
int red_black_tree_destroy ( red_black_tree **const pp_red_black_tree, fn_allocator *pfn_allocator );