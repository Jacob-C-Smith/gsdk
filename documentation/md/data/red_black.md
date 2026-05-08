# [gsdk](../../../README.md) > [data](../data.md) > red black

## Specialized binary tree; Loosely balanced

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
 $ ./build/examples/red_black_example
 ```
 [Source](../../../src/examples/red_black_example.c)

 ## Definitions

 ### Type definitions
 ```c
// type definitions
typedef struct red_black_tree_s      red_black_tree;
typedef struct red_black_tree_node_s red_black_tree_node;
 ```

 ### Function declarations
 ```c 
// function declarations
/// constructors
int red_black_tree_construct 
(
    red_black_tree **const  pp_red_black_tree,
    unsigned long long      node_size,
    fn_comparator          *pfn_comparator, 
    fn_key_accessor        *pfn_key_accessor 
);

/// accessors
int red_black_tree_search ( red_black_tree *const p_red_black_tree, const void *const p_key, void **pp_value );

/// mutators
int red_black_tree_insert ( red_black_tree *const p_red_black_tree, const void *const p_value );
int red_black_tree_remove ( red_black_tree *const p_red_black_tree, const void *const p_key  , const void **const p_value );

/// iterator
int red_black_tree_traverse_preorder  ( red_black_tree *const p_red_black_tree, fn_foreach    *pfn_foreach );
int red_black_tree_traverse_inorder   ( red_black_tree *const p_red_black_tree, fn_foreach    *pfn_foreach );
int red_black_tree_traverse_postorder ( red_black_tree *const p_red_black_tree, fn_foreach    *pfn_foreach );
int red_black_tree_forcontext         ( red_black_tree *const p_red_black_tree, fn_forcontext *pfn_forcontext, void *p_context );

/// reflection
int red_black_tree_pack ( void *p_buffer, red_black_tree *p_red_black_tree, fn_pack *pfn_element );
int red_black_tree_unpack
( 
    red_black_tree **pp_red_black_tree, 
    void *p_buffer, 
    
    fn_unpack       *pfn_element, 
    fn_comparator   *pfn_comparator, 
    fn_key_accessor *pfn_key_accessor
);
hash64 red_black_tree_hash ( red_black_tree *const p_red_black_tree, fn_hash64 *pfn_hash64 );

/// destructors
int red_black_tree_destroy ( red_black_tree **const pp_red_black_tree, fn_allocator *pfn_allocator );
 ```
