/** !
 * Adelson-Velsky and Landis tree interface
 *
 * @file src/data/avl/avl.h 
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
struct avl_tree_s;
struct avl_tree_node_s;

// type definitions
typedef struct avl_tree_s      avl_tree;
typedef struct avl_tree_node_s avl_tree_node;

// structure definitions
struct avl_tree_node_s
{ 
    void               *p_value;
    int                 height;
    unsigned long long  node_pointer;
    avl_tree_node      *p_left, *p_right;
};

struct avl_tree_s
{
    mutex          _lock;
    avl_tree_node *p_root;
    FILE          *p_random_access;
    
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
 * Construct an empty avl tree
 * 
 * @param pp_avl_tree      result
 * @param node_size        the size of a serialized value in bytes
 * @param pfn_comparator   function for testing equality of elements in set IF parameter is not null ELSE default
 * @param pfn_key_accessor function for accessing the key of a value IF parameter is not null ELSE default
 * 
 * @return 1 on success, 0 on error
 */
int avl_tree_construct 
(
    avl_tree **const     pp_avl_tree,
    unsigned long long   node_size,
    fn_comparator       *pfn_comparator, 
    fn_key_accessor     *pfn_key_accessor 
);

/// accessors
/** !
 * Search an avl tree for an element
 * 
 * @param p_avl_tree the avl tree
 * @param p_value    the element
 * @param pp_value   return
 * 
 * @return 1 on success, 0 on error
 */
int avl_tree_search ( avl_tree *const p_avl_tree, const void *const p_key, void **pp_value );

/// mutators
/** !
 * Insert a property into an avl tree
 * 
 * @param p_avl_tree the avl tree
 * @param p_value    the property value
 * 
 * @return 1 on success, 0 on error
 */
int avl_tree_insert ( avl_tree *const p_avl_tree, const void *const p_value );

/** !
 * Remove an element from an avl tree
 * 
 * @param p_avl_tree the avl tree
 * @param p_key      the element
 * @param pp_value   return
 * 
 * @return 1 on success, 0 on error
 */
int avl_tree_remove ( avl_tree *const p_avl_tree, const void *const p_key, const void **const p_value );

/// iterator
/** !
 * Traverse an avl tree using the pre order technique
 * 
 * @param p_avl_tree  pointer to avl tree
 * @param pfn_foreach called for each node in the avl tree
 * 
 * @return 1 on success, 0 on error
*/
int avl_tree_traverse_preorder ( avl_tree *const p_avl_tree, fn_foreach *pfn_foreach );

/** !
 * Traverse an avl tree using the in order technique
 * 
 * @param p_avl_tree  pointer to avl tree
 * @param pfn_foreach called for each node in the avl tree
 * 
 * @return 1 on success, 0 on error
*/
int avl_tree_traverse_inorder ( avl_tree *const p_avl_tree, fn_foreach *pfn_foreach );

/** !
 * Traverse an avl tree using the post order technique
 * 
 * @param p_avl_tree  pointer to avl tree
 * @param pfn_foreach called for each node in the avl tree
 * 
 * @return 1 on success, 0 on error
*/
int avl_tree_traverse_postorder ( avl_tree *const p_avl_tree, fn_foreach *pfn_foreach );

/** !
 * Call function on every node in an avl tree
 *
 * @param p_avl_tree     the avl tree
 * @param pfn_forcontext pointer to forcontext function
 * @param p_context      the context
 * 
 * @return 1 on success, 0 on error
 */
int avl_tree_forcontext ( avl_tree *const p_avl_tree, fn_forcontext *pfn_forcontext, void *p_context );

/// reflection
/** !
 * Pack an avl tree into a buffer
 * 
 * @param p_buffer    the buffer
 * @param p_avl_tree  the avl tree
 * @param pfn_element pointer to pack function IF not null ELSE default
 * 
 * @return 1 on success, 0 on error
 */
int avl_tree_pack ( void *p_buffer, avl_tree *p_avl_tree, fn_pack *pfn_element );

/** !
 * Unpack an avl tree into a buffer
 * 
 * @param pp_avl_tree      result
 * @param p_buffer         the buffer
 * @param pfn_element      pointer to unpack function IF not null ELSE default
 * @param pfn_comparator   function for testing equality of elements in set IF parameter is not null ELSE default
 * @param pfn_key_accessor function for accessing the key of a value IF parameter is not null ELSE default
 * 
 * @return 1 on success, 0 on error
 */
int avl_tree_unpack
( 
    avl_tree **pp_avl_tree, 
    void      *p_buffer, 
    
    fn_unpack       *pfn_element, 
    fn_comparator   *pfn_comparator, 
    fn_key_accessor *pfn_key_accessor
);

/** !
 * Compute the hash of an avl tree
 * 
 * @param p_avl_tree the avl tree
 * @param pfn_hash64 pointer to hashing function
 * 
 * @return the hash of an avl tree
 */
hash64 avl_tree_hash ( avl_tree *const p_avl_tree, fn_hash64 *pfn_hash64 );

/// destructors
/** !
 * Destroy and deallocate an avl tree
 * 
 * @param pp_avl_tree   pointer to avl tree pointer
 * @param pfn_allocator pointer to allocator function for deallocating elements
 * 
 * @return 1 on success, 0 on error
 */
int avl_tree_destroy ( avl_tree **const pp_avl_tree, fn_allocator *pfn_allocator );
