/** !
 * Tree interface
 *
 * @file src/data/tree/tree.h 
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

// enumeration definitions
enum tree_type_e
{
    TREE_BINARY     = 0,
    TREE_AVL        = 1,
    TREE_RED_BLACK  = 2,
    TREE_QUANTITY   = 3,
};

// forward declarations
struct tree_s;

// type definitions
typedef struct tree_s tree;
typedef int(fn_tree_insert)(void *const p_tree, const void *const p_value);
typedef int(fn_tree_remove)(void *const p_tree, const void *const p_key, const void **const p_value);
typedef int(fn_tree_search)(void *const p_tree, const void *const p_key, void **pp_value);
typedef bool(fn_tree_is_empty)(void *const p_tree);
typedef size_t(fn_tree_size)(void *const p_tree);
typedef int(fn_tree_traverse_inorder)(void *const p_tree, fn_foreach *pfn_foreach);
typedef int(fn_tree_pack)(void *p_buffer, void *p_tree, fn_pack *pfn_element);
typedef int(fn_tree_unpack)(void **pp_tree, void  *p_buffer, fn_unpack *pfn_element, fn_comparator *pfn_comparator, fn_key_accessor *pfn_key_accessor);
typedef int(fn_tree_destroy)(void **const pp_tree, fn_allocator *pfn_allocator);
typedef hash64(fn_tree_hash)(void *const p_tree, fn_hash64 *pfn_hash64);

// structure definitions
struct tree_s
{
    void *p_tree;
    enum tree_type_e _type;

    fn_tree_insert           *pfn_insert;
    fn_tree_remove           *pfn_remove;
    fn_tree_search           *pfn_search;
    fn_tree_is_empty         *pfn_is_empty;
    fn_tree_size             *pfn_size;
    fn_tree_traverse_inorder *pfn_traverse_inorder;
    fn_tree_pack             *pfn_pack;
    fn_tree_unpack           *pfn_unpack;
    fn_tree_destroy          *pfn_destroy;
    fn_tree_hash             *pfn_hash;
};

// function declarations
/// constructors
/** !
 * Construct an empty tree
 * 
 * @param pp_tree          result
 * @param node_size        the size of a serialized value in bytes
 * @param pfn_comparator   function for testing equality of elements in set IF parameter is not null ELSE default
 * @param pfn_key_accessor function for accessing the key of a value IF parameter is not null ELSE default
 * 
 * @return 1 on success, 0 on error
 */
int tree_construct 
(
    tree **const         pp_tree,
    enum tree_type_e     _type,
    unsigned long long   node_size,
    fn_comparator       *pfn_comparator, 
    fn_key_accessor     *pfn_key_accessor 
);

/// accessors
/** !
 * Search a tree for a value
 * 
 * @param p_tree   the tree
 * @param p_value  the element
 * @param pp_value result
 * 
 * @return 1 on success, 0 on error
 */
int tree_search ( tree *const p_tree, const void *const p_key, void **pp_value );

/** !
 * Is a tree empty?
 * 
 * @param p_tree the tree
 * 
 * @return true if tree has no values else false
 */
bool tree_is_empty ( tree *p_tree );

/** !
 * Get the size of an tree
 * 
 * @param p_tree the tree
 * 
 * @return size of tree
 */
size_t tree_size ( tree *p_tree );

/// mutators
/** !
 * Insert a value into a tree
 * 
 * @param p_tree  the tree
 * @param p_value the value
 * 
 * @return 1 on success, 0 on error
 */
int tree_insert ( tree *const p_tree, const void *const p_value );

/** !
 * Remove a value from a tree
 * 
 * @param p_tree   the tree
 * @param p_key    the key
 * @param pp_value result
 * 
 * @return 1 on success, 0 on error
 */
int tree_remove ( tree *const p_tree, const void *const p_key, const void **const p_value );

/// iterator
/** !
 * Traverse a tree using the in order technique
 * 
 * @param p_tree      pointer to tree
 * @param pfn_foreach called for each node in the tree
 * 
 * @return 1 on success, 0 on error
*/
int tree_traverse_inorder ( tree *const p_tree, fn_foreach *pfn_foreach );

/// reflection
/** !
 * Pack a tree into a buffer
 * 
 * @param p_buffer    the buffer
 * @param p_tree      the tree
 * @param pfn_element pointer to pack function IF not null ELSE default
 * 
 * @return 1 on success, 0 on error
 */
int tree_pack ( void *p_buffer, tree *p_tree, fn_pack *pfn_element );

/** !
 * Unpack a tree into a buffer
 * 
 * @param pp_tree          result
 * @param p_buffer         the buffer
 * @param pfn_element      pointer to unpack function IF not null ELSE default
 * @param pfn_comparator   function for testing equality of elements in set IF parameter is not null ELSE default
 * @param pfn_key_accessor function for accessing the key of a value IF parameter is not null ELSE default
 * 
 * @return 1 on success, 0 on error
 */
int tree_unpack
( 
    tree **pp_tree, 
    void  *p_buffer, 
    
    fn_unpack       *pfn_element, 
    fn_comparator   *pfn_comparator, 
    fn_key_accessor *pfn_key_accessor
);

/** !
 * Compute the hash of a tree
 * 
 * @param p_tree     the tree
 * @param pfn_hash64 pointer to hashing function
 * 
 * @return the hash of a tree
 */
hash64 tree_hash ( tree *const p_tree, fn_hash64 *pfn_hash64 );

/// destructors
/** !
 * Destroy and deallocate a tree
 * 
 * @param pp_tree       pointer to tree pointer
 * @param pfn_allocator pointer to allocator function for deallocating elements
 * 
 * @return 1 on success, 0 on error
 */
int tree_destroy ( tree **const pp_tree, fn_allocator *pfn_allocator );
