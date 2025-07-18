/** !
 * Include header for binary search tree
 *
 * @file tree/binary.h 
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

// core
#include <core/log.h>
#include <core/sync.h>

// data
#include <data/tree.h>

// forward declarations
struct binary_tree_s;
struct binary_tree_node_s;

// type definitions
/** !
 *  @brief The type definition for a binary tree
 */
typedef struct binary_tree_s binary_tree;

/** !
 *  @brief The type definition for a binary tree node
 */
typedef struct binary_tree_node_s binary_tree_node;

/** !
 *  @brief The type definition for a function that serializes a node to a file
 * 
 *  @param p_file             the file
 *  @param p_binary_tree_node the binary tree node
 * 
 *  @return 1 on success, 0 on error
 */
typedef int (fn_binary_tree_serialize)(FILE *p_file, binary_tree_node *p_binary_tree_node);

/** !
 *  @brief The type definition for a function that parses a node from a file
 * 
 *  @param p_file              the file
 *  @param p_binary_tree       the binary tree node
 *  @param pp_binary_tree_node return
 * 
 *  @return 1 on success, 0 on error
 */
typedef int (fn_binary_tree_parse)(FILE *p_file, binary_tree_node *p_binary_tree_node );

/** !
 *  @brief The type definition for a function that is called on each node while traversing a tree
 * 
 *  @param p_value the value
 * 
 *  @return 1 on success, 0 on error
 */
typedef int (fn_binary_tree_traverse)(void *p_value);

// Struct definitions
struct binary_tree_node_s
{ 
    void *p_value;
    binary_tree_node *p_left,
                     *p_right;
    unsigned long long  node_pointer;
};

struct binary_tree_s
{
    mutex _lock;
    binary_tree_node *p_root;
    FILE             *p_random_access;
    
    struct 
    {
        fn_tree_equal            *pfn_is_equal;
        fn_tree_key_accessor     *pfn_key_accessor;
        fn_binary_tree_serialize *pfn_serialize_node;
        fn_binary_tree_parse     *pfn_parse_node;
    } functions;

    struct 
    {
        unsigned long long node_quantity;
        unsigned long long node_size;
    } metadata;
};

// constructors
/** !
 * Construct an empty binary tree
 * 
 * @param pp_binary_tree   return
 * @param pfn_is_equal     function for testing equality of elements in set IF parameter is not null ELSE default
 * @param pfn_key_accessor function for accessing the key of a value IF parameter is not null ELSE default
 * @param node_size        the size of a serialized node in bytes
 * 
 * @return 1 on success, 0 on error
 */
int binary_tree_construct ( binary_tree **const pp_binary_tree, fn_tree_equal *pfn_is_equal, fn_tree_key_accessor *pfn_key_accessor, unsigned long long node_size );

/** !
 * Construct a balanced binary tree from a sorted list of keys and values. 
 * 
 * @param pp_binary_tree    return
 * @param pp_values         the list of values
 * @param property_quantity the size of the list
 * @param pfn_is_equal      function for testing equality of elements in set IF parameter is not null ELSE default
 * @param node_size         the size of a serialized node in bytes
 * 
*/
int binary_tree_construct_balanced ( binary_tree **const pp_binary_tree, void **pp_values, size_t property_quantity, fn_tree_equal *pfn_is_equal, fn_tree_key_accessor *pfn_key_accessor, unsigned long long node_size );

// accessors
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

// mutators
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

// Traversal
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

// Parser
/** !
 * Construct a binary tree from a file
 * 
 * @param pp_binary_tree return
 * @param p_file         path to the file
 * @param pfn_is_equal   function for testing equality of elements in set IF parameter is not null ELSE default
 * @param pfn_parse_node a function for parsing nodes from the file
 * 
 * @return 1 on success, 0 on error
 */
int binary_tree_parse ( binary_tree **const pp_binary_tree, const char *p_file, fn_tree_equal *pfn_is_equal, fn_tree_key_accessor *pfn_tree_key_accessor, fn_binary_tree_parse *pfn_parse_node );

// Serializer
/** !
 * Write a binary tree to a file
 * 
 * @param p_binary_tree      the binary tree 
 * @param p_path             path to the file
 * @param pfn_serialize_node a function for serializing nodes to the file
 * 
 * @return 1 on success, 0 on error
 */
int binary_tree_serialize ( binary_tree *const p_binary_tree, const char *p_path, fn_binary_tree_serialize *pfn_serialize_node );

// destructors
/** !
 * Deallocate a binary tree
 * 
 * @param pp_binary_tree pointer to binary tree pointer
 * 
 * @return 1 on success, 0 on error
 */
int binary_tree_destroy ( binary_tree **const pp_binary_tree );
