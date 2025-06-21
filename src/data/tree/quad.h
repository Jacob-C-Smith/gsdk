/** !
 * Include header for quadtree
 * 
 * @file tree/quad.h 
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

// sync submodule
#include <core/sync.h>

// tree
#include <data/tree.h>

// forward declarations
struct quad_tree_s;
struct quad_tree_node_s;

// type definitions
/** !
 *  @brief The type definition for a quad tree
 */
typedef struct quad_tree_s quad_tree;

/** !
 *  @brief The type definition for a quad tree node
 */
typedef struct quad_tree_node_s quad_tree_node;

/** !
 *  @brief The type definition for a function that serializes a node to a file
 * 
 *  @param p_file           the file
 *  @param p_quad_tree_node the quad tree node
 * 
 *  @return 1 on success, 0 on error
 */
typedef int (fn_quad_tree_serialize)(FILE *p_file, quad_tree_node *p_quad_tree_node);

/** !
 *  @brief The type definition for a function that parses a node from a file
 * 
 *  @param p_file            the file
 *  @param p_quad_tree       the quad tree node
 *  @param pp_quad_tree_node return
 * 
 *  @return 1 on success, 0 on error
 */
typedef int (fn_quad_tree_parse)(FILE *p_file, quad_tree *p_quad_tree, quad_tree_node **pp_quad_tree_node, unsigned long long node_pointer );

// Struct definitions
struct quad_tree_node_s
{ 
    void *p_key,
         *p_value;
    struct
    {
        quad_tree_node *p_top_left,
                       *p_top_right,
                       *p_bottom_left,
                       *p_bottom_right;
    } quadrants;
    unsigned long long node_pointer;
};

struct quad_tree_s
{
    quad_tree_node *p_root;

    struct 
    {
        fn_tree_equal          *pfn_is_equal;
        fn_quad_tree_serialize *pfn_serialize_node;
        fn_quad_tree_parse     *pfn_parse_node;
    } functions;

    struct 
    {
        unsigned long long node_quantity;
        unsigned long long node_size;
    } metadata;
};

// allocators
/** !
 * Allocate memory for a quad tree
 * 
 * @param pp_quad_tree return
 * 
 * @return 1 on success, 0 on error
 */
int quad_tree_create ( quad_tree **const pp_quad_tree );

// constructors
/** !
 * Construct an empty quad tree
 * 
 * @param pp_quad_tree return
 * @param pfn_is_equal function for testing equality of elements in set IF parameter is not null ELSE default
 * @param node_size    the size of a serialized node in bytes
 * 
 * @return 1 on success, 0 on error
 */
int quad_tree_construct ( quad_tree **const pp_quad_tree, fn_tree_equal *pfn_is_equal, unsigned long long node_size );

// accessors
/** !
 * Search a quad tree for an element
 * 
 * @param p_quad_tree the quad tree
 * @param p_value     the element
 * @param pp_value    return
 * 
 * @return 1 on success, 0 on error
 */
int quad_tree_search ( const quad_tree *const p_quad_tree, const void *const p_key, const void **const pp_value );

// mutators
/** !
 * Insert a property into a quad tree
 * 
 * @param p_quad_tree the quad tree
 * @param p_key       the property key
 * @param p_value     the property value
 * 
 * @return 1 on success, 0 on error
 */
int quad_tree_insert ( quad_tree *const p_quad_tree, const void *const p_key, const void *const p_value );

/** !
 * Remove an element from a quad tree
 * 
 * @param p_quad_tree the quad tree
 * @param p_key       the element
 * @param pp_value    return
 * 
 * @return 1 on success, 0 on error
 */
int quad_tree_remove ( quad_tree *const p_quad_tree, const void *const p_key, const void **const p_value );

// Parser
/** !
 * Construct a quad tree from a file
 * 
 * @param pp_quad_tree   return
 * @param p_file         the file
 * @param pfn_is_equal   function for testing equality of elements in set IF parameter is not null ELSE default
 * @param pfn_parse_node a function for parsing nodes from the file
 * 
 * @return 1 on success, 0 on error
 */
int quad_tree_parse ( quad_tree **const pp_quad_tree, FILE *p_file, fn_tree_equal *pfn_is_equal, fn_quad_tree_parse *pfn_parse_node );

// Serializer
/** !
 * Write a quad tree to a file
 * 
 * @param p_quad_tree        the quad tree 
 * @param p_file             the file
 * @param pfn_serialize_node a function for serializing nodes to the file
 * 
 * @return 1 on success, 0 on error
 */
int quad_tree_serialize ( quad_tree *const p_quad_tree, FILE *p_file, fn_quad_tree_serialize *pfn_serialize_node );

// destructors
/** !
 * Deallocate a quad tree
 * 
 * @param pp_quad_tree pointer to quad tree pointer
 * 
 * @return 1 on success, 0 on error
 */
int quad_tree_destroy ( quad_tree **const pp_quad_tree );
