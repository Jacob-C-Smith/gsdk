# [gsdk](../../../README.md) > [data](../data.md) > tree
## Abstract binary search tree

 > 1 [Example](#example)
 >
 > 2 [Tester](#tester)
 >
 > 3 [Definitions](#definitions)
 >
 >> 3.1 [Enumeration definitions](#enumeration-definitions)
 >> 
 >> 3.2 [Type definitions](#type-definitions)
 >>
 >> 3.3 [Function declarations](#function-declarations)

 ## Example
 To run the example program, execute this command
 ```
 $ ./build/examples/tree_example
 ```
 [Source](../../../src/examples/tree_example.c)
 
 ## Tester
 To run the tester program, execute this command
 ```
 $ ./build/tests/tree_test 
 ```

 ## Definitions
 ### Enumeration definitions
 ```c
// enumeration definitions
enum tree_type_e
{
    TREE_BINARY     = 0,
    TREE_AVL        = 1,
    TREE_RED_BLACK  = 2,
    TREE_QUANTITY   = 3,
};
 ```

 ### Type definitions
 ```c
// type definitions
typedef struct tree_s tree;

typedef int    (fn_tree_search)   ( void *const p_tree, const void *const p_key, void **pp_value);
typedef bool   (fn_tree_is_empty) ( void *const p_tree );
typedef size_t (fn_tree_size)     ( void *const p_tree );

typedef int (fn_tree_insert) ( void *const p_tree, const void *const p_value);
typedef int (fn_tree_remove) ( void *const p_tree, const void *const p_key, const void **const p_value);

typedef int (fn_tree_traverse_inorder) ( void *const p_tree, fn_foreach *pfn_foreach );

typedef int    (fn_tree_pack)   ( void *p_buffer, void *p_tree, fn_pack *pfn_element );
typedef int    (fn_tree_unpack) ( void **pp_tree, void  *p_buffer, fn_unpack *pfn_element, fn_comparator *pfn_comparator, fn_key_accessor *pfn_key_accessor);
typedef hash64 (fn_tree_hash)   ( void *const p_tree, fn_hash64 *pfn_hash64);

typedef int (fn_tree_destroy) ( void **const pp_tree, fn_allocator *pfn_allocator);
 ```

 ### Function declarations
 ```c 
// function declarations
/// constructors
int tree_construct 
(
    tree **const         pp_tree,
    enum tree_type_e     _type,
    unsigned long long   node_size,
    fn_comparator       *pfn_comparator, 
    fn_key_accessor     *pfn_key_accessor 
);

/// accessors
int    tree_search   ( tree *const p_tree, const void *const p_key, void **pp_value );
bool   tree_is_empty ( tree *      p_tree );
size_t tree_size     ( tree *      p_tree );

/// mutators
int tree_insert ( tree *const p_tree, const void *const p_value );
int tree_remove ( tree *const p_tree, const void *const p_key  , const void **const p_value );

/// iterator
int tree_traverse_inorder ( tree *const p_tree, fn_foreach *pfn_foreach );

/// reflection
int tree_pack ( void *p_buffer, tree *p_tree, fn_pack *pfn_element );
int tree_unpack
( 
    tree **pp_tree, 
    void  *p_buffer, 
    
    fn_unpack       *pfn_element, 
    fn_comparator   *pfn_comparator, 
    fn_key_accessor *pfn_key_accessor
);
hash64 tree_hash ( tree *const p_tree, fn_hash64 *pfn_hash64 );

/// destructors
int tree_destroy ( tree **const pp_tree, fn_allocator *pfn_allocator );
 ```
