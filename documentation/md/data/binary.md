# [gsdk](../../../README.md) > [data](../data.md) > binary

## Binary search tree

 > 1 [Example](#example)
 >
 > 2 [Definitions](#definitions)
 >
 >> 2.1 [Type definitions](#type-definitions)
 >>
 >> 2.2 [Function declarations](#function-declarations)

 ## Example
 To run the example program, execute this command
 ```
 $ ./build/examples/binary_example
 ```
 [Source](../../../src/examples/binary_example.c)

 ## Definitions

 ### Type definitions
 ```c
// type definitions
typedef struct binary_tree_s binary_tree;
typedef int (fn_binary_tree_traverse)(void *p_value);
 ```

 ### Function declarations

 ```c 
// function declarations
/// constructors
int binary_tree_construct 
(
    binary_tree **const  pp_binary_tree,
    unsigned long long   node_size,

    fn_comparator       *pfn_comparator, 
    fn_key_accessor     *pfn_key_accessor 
);

/// accessors
int binary_tree_search ( binary_tree *const p_binary_tree, const void *const p_key, void **pp_value );

/// mutators
int binary_tree_insert ( binary_tree *const p_binary_tree, const void *const p_value );
int binary_tree_remove ( binary_tree *const p_binary_tree, const void *const p_key, const void **const p_value );

/// traversal
int binary_tree_traverse_preorder  ( binary_tree *const p_binary_tree, fn_binary_tree_traverse *pfn_traverse );
int binary_tree_traverse_inorder   ( binary_tree *const p_binary_tree, fn_binary_tree_traverse *pfn_traverse );
int binary_tree_traverse_postorder ( binary_tree *const p_binary_tree, fn_binary_tree_traverse *pfn_traverse );

/// reflection
int binary_tree_pack ( void *p_buffer, binary_tree *p_binary_tree, fn_pack *pfn_element );
int binary_tree_unpack
( 
    binary_tree **pp_binary_tree, 
    void *p_buffer, 
    
    fn_unpack       *pfn_element, 
    fn_comparator   *pfn_comparator, 
    fn_key_accessor *pfn_key_accessor
);

/// hash
hash64 binary_tree_hash ( binary_tree *const p_binary_tree, fn_hash64 *pfn_hash64 );

/// destructors
int binary_tree_destroy ( binary_tree **const pp_binary_tree, fn_allocator *pfn_allocator );
 ```
