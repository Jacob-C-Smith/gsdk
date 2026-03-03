/** !
 * Binary search tree interface
 *
 * @file src/data/binary/binary.h 
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
#include <core/sync.h>
#include <core/interfaces.h>

// forward declarations
struct binary_tree_s;
struct binary_tree_node_s;

// type definitions
typedef struct binary_tree_s binary_tree;
typedef struct binary_tree_node_s binary_tree_node;
typedef int (fn_binary_tree_traverse)(void *p_value);

// structure definitions
struct binary_tree_node_s
{ 
    void *p_value;
    binary_tree_node *p_left, *p_right;
    unsigned long long  node_pointer;
};

struct binary_tree_s
{
    mutex             _lock;
    binary_tree_node *p_root;
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
 * Construct an empty binary tree
 * 
 * @param pp_binary_tree   return
 * @param node_size        the size of a serialized value in bytes
 * @param pfn_comparator   function for testing equality of elements in set IF parameter is not null ELSE default
 * @param pfn_key_accessor function for accessing the key of a value IF parameter is not null ELSE default
 * 
 * @return 1 on success, 0 on error
 */
int binary_tree_construct 
(
    binary_tree **const  pp_binary_tree,
    unsigned long long   node_size,
    fn_comparator       *pfn_comparator, 
    fn_key_accessor     *pfn_key_accessor 
);

/** !
 * Construct a balanced binary tree from a sorted list of keys and values. 
 * 
 * @param pp_binary_tree    return
 * @param pp_values         the list of values
 * @param property_quantity the size of the list
 * @param pfn_comparator      function for testing equality of elements in set IF parameter is not null ELSE default
 * @param node_size         the size of a serialized node in bytes
 * 
*/
int binary_tree_construct_balanced
(
    binary_tree **const pp_binary_tree, 
    void **pp_values, 
    size_t property_quantity, 
    fn_comparator *pfn_comparator, 
    fn_key_accessor *pfn_key_accessor, 
    unsigned long long node_size
);

/// accessors
/** !
 * Search a binary tree for an element
 * 
 * @param p_binary_tree the binary tree
 * @param p_value       the element
 * @param pp_value      return
 * 
 * @return 1 on success, 0 on error
 */
int binary_tree_search ( binary_tree *const p_binary_tree, const void *const p_key, void **pp_value );

/// mutators
/** !
 * Insert a property into a binary tree
 * 
 * @param p_binary_tree the binary tree
 * @param p_value       the property value
 * 
 * @return 1 on success, 0 on error
 */
int binary_tree_insert ( binary_tree *const p_binary_tree, const void *const p_value );

/** !
 * Remove an element from a binary tree
 * 
 * @param p_binary_tree the binary tree
 * @param p_key         the element
 * @param pp_value      return
 * 
 * @return 1 on success, 0 on error
 */
int binary_tree_remove ( binary_tree *const p_binary_tree, const void *const p_key, const void **const p_value );

/// traversal
/** !
 * Traverse a binary tree using the pre order technique
 * 
 * @param p_binary_tree pointer to binary tree
 * @param pfn_traverse  called for each node in the binary tree
 * 
 * @return 1 on success, 0 on error
*/
int binary_tree_traverse_preorder ( binary_tree *const p_binary_tree, fn_binary_tree_traverse *pfn_traverse );

/** !
 * Traverse a binary tree using the in order technique
 * 
 * @param p_binary_tree pointer to binary tree
 * @param pfn_traverse  called for each node in the binary tree
 * 
 * @return 1 on success, 0 on error
*/
int binary_tree_traverse_inorder ( binary_tree *const p_binary_tree, fn_binary_tree_traverse *pfn_traverse );

/** !
 * Traverse a binary tree using the post order technique
 * 
 * @param p_binary_tree pointer to binary tree
 * @param pfn_traverse  called for each node in the binary tree
 * 
 * @return 1 on success, 0 on error
*/
int binary_tree_traverse_postorder ( binary_tree *const p_binary_tree, fn_binary_tree_traverse *pfn_traverse );

/// reflection
/** !
 * Construct a binary tree from a file
 * 
 * @param pp_binary_tree return
 * @param p_file         path to the file
 * @param pfn_comparator   function for testing equality of elements in set IF parameter is not null ELSE default
 * @param pfn_parse_node a function for parsing nodes from the file
 * 
 * @return 1 on success, 0 on error
 */
int binary_tree_parse 
(
    binary_tree **const  pp_binary_tree, 
    const char          *p_file, 
    
    fn_comparator   *pfn_comparator, 
    fn_key_accessor *pfn_key_accessor, 
    fn_unpack       *pfn_unpack
);

/** !
 * Write a binary tree to a file
 * 
 * @param p_binary_tree the binary tree 
 * @param p_path        path to the file
 * @param pfn_pack      pack function
 * 
 * @return 1 on success, 0 on error
 */
int binary_tree_serialize ( binary_tree *const p_binary_tree, const char *p_path, fn_pack *pfn_pack );

/** !
 * Compute the hash of a binary tree
 * 
 * @param p_binary_tree the binary tree
 * @param pfn_hash64    pointer to hashing function
 * 
 * @return the hash of a binary tree
 */
hash64 binary_tree_hash ( binary_tree *const p_binary_tree, fn_hash64 *pfn_hash64 );

/// destructors
/** !
 * Deallocate a binary tree
 * 
 * @param pp_binary_tree pointer to binary tree pointer
 * 
 * @return 1 on success, 0 on error
 */
int binary_tree_destroy ( binary_tree **const pp_binary_tree );
