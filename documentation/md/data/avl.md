# [gsdk](../../../README.md) > [data](../data.md) > avl

## Adelson-Velsky and Landis tree 

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
 $ ./build/examples/avl_example
 ```
 [Source](../../../src/examples/avl_example.c)

 ## Definitions

 ### Type definitions
 ```c
// type definitions
typedef struct avl_tree_s      avl_tree;
typedef struct avl_tree_node_s avl_tree_node;
 ```

 ### Function declarations

 ```c 
// function declarations
/// constructors
int avl_tree_construct 
(
    avl_tree **const     pp_avl_tree,
    unsigned long long   node_size,
    fn_comparator       *pfn_comparator, 
    fn_key_accessor     *pfn_key_accessor 
);

/// accessors
int avl_tree_search ( avl_tree *const p_avl_tree, const void *const p_key, void **pp_value );

/// mutators
int avl_tree_insert ( avl_tree *const p_avl_tree, const void *const p_value );
int avl_tree_remove ( avl_tree *const p_avl_tree, const void *const p_key  , const void **const p_value );

/// iterator
int avl_tree_traverse_preorder  ( avl_tree *const p_avl_tree, fn_foreach    *pfn_foreach );
int avl_tree_traverse_inorder   ( avl_tree *const p_avl_tree, fn_foreach    *pfn_foreach );
int avl_tree_traverse_postorder ( avl_tree *const p_avl_tree, fn_foreach    *pfn_foreach );
int avl_tree_forcontext         ( avl_tree *const p_avl_tree, fn_forcontext *pfn_forcontext, void *p_context );

/// reflection
int avl_tree_pack ( void *p_buffer, avl_tree *p_avl_tree, fn_pack *pfn_element );
int avl_tree_unpack
( 
    avl_tree **pp_avl_tree, 
    void      *p_buffer, 
    
    fn_unpack       *pfn_element, 
    fn_comparator   *pfn_comparator, 
    fn_key_accessor *pfn_key_accessor
);
hash64 avl_tree_hash ( avl_tree *const p_avl_tree, fn_hash64 *pfn_hash64 );

/// destructors
int avl_tree_destroy ( avl_tree **const pp_avl_tree, fn_allocator *pfn_allocator );
 ```
