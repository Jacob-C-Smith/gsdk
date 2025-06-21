/** !
 * Include header for B tree
 * 
 * @file tree/b.h 
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
#include <errno.h>

// sync submodule
#include <core/sync.h>

// tree
#include <data/tree.h>

// Forward declarations
struct b_tree_s;
struct b_tree_node_s;
struct b_tree_metadata_s;

// type definitions
/** !
 *  @brief The type definition for a b tree
 */
typedef struct b_tree_s b_tree;

/** !
 *  @brief The type definition for a b tree node
 */
typedef struct b_tree_node_s b_tree_node;

/** !
 *  @brief The type definition for a b tree metadata
 */
typedef struct b_tree_metadata_s b_tree_metadata;

/** !
 *  @brief The type definition for a function that serializes a node to a file
 * 
 *  @param p_file        the file
 *  @param p_b_tree_node the b tree node
 * 
 *  @return 1 on success, 0 on error
 */
typedef int (fn_b_tree_serialize)(FILE *p_file, b_tree_node *p_b_tree_node);

/** !
 *  @brief The type definition for a function that parses a node from a file
 * 
 *  @param p_file         the file
 *  @param p_b_tree       the b tree node
 *  @param pp_b_tree_node return
 * 
 *  @return 1 on success, 0 on error
 */
typedef int (fn_b_tree_parse)(FILE *p_file, b_tree *p_b_tree, b_tree_node **pp_b_tree_node, unsigned long long node_pointer );

/** !
 *  @brief The type definition for a function that is called on each node while traversing a tree
 * 
 *  @param p_key   the key
 *  @param p_value the value
 * 
 *  @return 1 on success, 0 on error
 */
typedef int (fn_b_tree_traverse)(void *p_key, void *p_value);

// Struct definitions
struct b_tree_node_s
{
    bool                leaf;
    int                 key_quantity;
    unsigned long long  node_pointer;
    void               **properties;
    unsigned long long _child_pointers[];
};

struct b_tree_metadata_s
{
    unsigned long long node_quantity,
                       root_address,
                       next_disk_address,
                       key_quantity;
    int node_size,
        degree,
        height;
};

struct b_tree_s
{
    b_tree_metadata   _metadata;
    b_tree_node     *p_root;
    FILE            *p_random_access;

    struct 
    {
        fn_tree_equal       *pfn_is_equal;
        fn_b_tree_serialize *pfn_serialize_node;
        fn_b_tree_parse     *pfn_parse_node;
    } functions;
};

// Allocators
/** !
 * Allocate memory for a b tree
 * 
 * @param pp_b_tree return
 * 
 * @return 1 on success, 0 on error
 */
int b_tree_create ( b_tree **const pp_b_tree );

// Constructors
/** !
 * Construct an empty b tree
 * 
 * @param pp_b_tree      return
 * @param pfn_is_equal   function for testing equality of elements in set IF parameter is not null ELSE default
 * @param degree         the degree of the b tree
 * @param node_size      the size of a serialized node in bytes
 * 
 * @return 1 on success, 0 on error
 */
int b_tree_construct ( b_tree **const pp_b_tree, const char *const path, fn_tree_equal *pfn_is_equal, int degree, unsigned long long node_size );

// Accessors
/** !
 * Search a b tree for an element
 * 
 * @param p_b_tree the b tree
 * @param p_value  the element
 * @param pp_value return
 * 
 * @return 1 on success, 0 on error
 */
int b_tree_search ( const b_tree *const p_b_tree, const void *const p_key, const void **const pp_value );

// Mutators
/** !
 * Insert a property into a b tree
 * 
 * @param p_b_tree   the b tree
 * @param p_property the property
 * 
 * @return 1 on success, 0 on error
 */
int b_tree_insert ( b_tree *const p_b_tree, const void *const p_property );

/** !
 * Remove an element from a b tree
 * 
 * @param p_b_tree the b tree
 * @param p_key    the element
 * @param pp_value return
 * 
 * @return 1 on success, 0 on error
 */
int b_tree_remove ( b_tree *const p_b_tree, const void *const p_key, const void **const p_value );

// Traversal
/** !
 * Traverse a b tree using the pre order technique
 * 
 * @param p_b_tree     pointer to b tree
 * @param pfn_traverse called for each node in the b tree
 * 
 * @return 1 on success, 0 on error
*/
int b_tree_traverse_preorder ( b_tree *const p_b_tree, fn_b_tree_traverse *pfn_traverse );

/** !
 * Traverse a b tree using the in order technique
 * 
 * @param p_b_tree     pointer to b tree
 * @param pfn_traverse called for each node in the b tree
 * 
 * @return 1 on success, 0 on error
*/
int b_tree_traverse_inorder ( b_tree *const p_b_tree, fn_b_tree_traverse *pfn_traverse );

/** !
 * Traverse a b tree using the post order technique
 * 
 * @param p_b_tree     pointer to b tree
 * @param pfn_traverse called for each node in the b tree
 * 
 * @return 1 on success, 0 on error
*/
int b_tree_traverse_postorder ( b_tree *const p_b_tree, fn_b_tree_traverse *pfn_traverse );

// Parser
/** !
 * Construct a b tree from a file
 * 
 * @param pp_b_tree      return
 * @param p_file         the file
 * @param pfn_is_equal   function for testing equality of elements in set IF parameter is not null ELSE default
 * @param pfn_parse_node a function for parsing nodes from the file
 * 
 * @return 1 on success, 0 on error
 */
int b_tree_parse ( b_tree **const pp_b_tree, FILE *p_file, fn_tree_equal *pfn_is_equal, fn_b_tree_parse *pfn_parse_node );

// Serializer
/** !
 * Write a b tree to a file
 * 
 * @param p_b_tree           the b tree 
 * @param p_path             path to the file
 * @param pfn_serialize_node a function for serializing nodes to the file
 * 
 * @return 1 on success, 0 on error
 */
int b_tree_serialize ( b_tree *const p_b_tree, const char *p_path, fn_b_tree_serialize *pfn_serialize_node );

// Destructors
/** !
 * Deallocate a b tree
 * 
 * @param pp_b_tree pointer to b tree pointer
 * 
 * @return 1 on success, 0 on error
 */
int b_tree_destroy ( b_tree **const pp_b_tree );
