# tree
[![CMake](https://github.com/Jacob-C-Smith/tree/actions/workflows/cmake.yml/badge.svg?branch=main)](https://github.com/Jacob-C-Smith/tree/actions/workflows/cmake.yml)

**Dependencies:**\
[![sync](https://github.com/Jacob-C-Smith/sync/actions/workflows/cmake.yml/badge.svg)](https://github.com/Jacob-C-Smith/sync/actions/workflows/cmake.yml)

 Implementation of different tree structures in C
 
 > 1 [Download](#download)
 >
 > 2 [Build](#build)
 >
 > 3 [Example](#example)
 >
 >> 3.1 [Example output](#example-output)
 >
 > 4 [Tester](#tester)
 >
 > 5 [Definitions](#definitions)
 >
 >> 5.1 [Type definitions](#type-definitions)
 >>
 >> 5.2 [Function definitions](#function-definitions)

 ## Download
 To download tree, execute the following command
 ```bash
 $ git clone https://github.com/Jacob-C-Smith/tree
 ```
 ## Build
 To build on UNIX like machines, execute the following commands in the same directory
 ```bash
 $ cd tree
 $ cmake .
 $ make
 ```
  This will build the example program, the tester program, and dynamic / shared libraries

  To build tree for Windows machines, open the base directory in Visual Studio, and build your desired target(s)
 ## Example
 To run the example program, execute this command
 ```
 $ ./tree_example
 ```
 ### Example output
 ```
 TODO
 ```
 [Source](main.c)

## Tester
 TODO: 
 
 ## Definitions
 ### Binary tree 
 #### Type definitions
```c
// Type definitions
typedef struct binary_tree_s      binary_tree;
typedef struct binary_tree_node_s binary_tree_node;

typedef int (fn_binary_tree_serialize) (FILE *p_file, binary_tree_node *p_binary_tree_node);
typedef int (fn_binary_tree_parse)     (FILE *p_file, binary_tree_node *p_binary_tree_node);
typedef int (fn_binary_tree_traverse)  (void *p_key, void *p_value);
```

#### Function definitions
 ```c
// Allocators
int binary_tree_create ( binary_tree **const pp_binary_tree );

// Constructors
int binary_tree_construct ( binary_tree **const pp_binary_tree, fn_tree_equal *pfn_is_equal, unsigned long long node_size );

// Accessors
int binary_tree_search ( const binary_tree *const p_binary_tree, const void *const p_key, const void **const pp_value );

// Mutators
int binary_tree_insert ( binary_tree *const p_binary_tree, const void *const p_key, const void  *const p_value );
int binary_tree_remove ( binary_tree *const p_binary_tree, const void *const p_key, const void **const p_value );

// Traversal
int binary_tree_traverse_preorder  ( binary_tree *const p_binary_tree, fn_binary_tree_traverse *pfn_traverse );
int binary_tree_traverse_inorder   ( binary_tree *const p_binary_tree, fn_binary_tree_traverse *pfn_traverse );
int binary_tree_traverse_postorder ( binary_tree *const p_binary_tree, fn_binary_tree_traverse *pfn_traverse );

// Parser
int binary_tree_parse ( binary_tree **const pp_binary_tree, const char *p_file, fn_tree_equal *pfn_is_equal, fn_binary_tree_parse *pfn_parse_node );

// Serializer
int binary_tree_serialize ( binary_tree *const p_binary_tree, const char *p_path, fn_binary_tree_serialize *pfn_serialize_node );

// Destructors
int binary_tree_destroy ( binary_tree **const pp_binary_tree );
 ```

 ### B tree
 #### Type definitions
 ```c
typedef struct b_tree_s           b_tree;
typedef struct b_tree_node_s      b_tree_node;
typedef struct b_tree_metadata_s  b_tree_metadata;

typedef int (fn_b_tree_serialize) (FILE *p_file, b_tree_node *p_b_tree_node);
typedef int (fn_b_tree_parse)     (FILE *p_file, b_tree *p_b_tree, b_tree_node **pp_b_tree_node, unsigned long long node_pointer );
typedef int (fn_b_tree_traverse)  (void *p_key, void *p_value);
 ```
 #### Function definitions
 ```c
// Allocators
int b_tree_create ( b_tree **const pp_b_tree );

// Constructors
int b_tree_construct ( b_tree **const pp_b_tree, const char *const path, fn_tree_equal *pfn_is_equal, int degree, unsigned long long node_size );

// Accessors
int b_tree_search ( const b_tree *const p_b_tree, const void *const p_key, const void **const pp_value );

// Mutators
int b_tree_insert ( b_tree *const p_b_tree, const void *const p_key, const void *const p_value );
int b_tree_remove ( b_tree *const p_b_tree, const void *const p_key, const void **const p_value );

// Traversal
int binary_tree_traverse_preorder  ( b_tree *const p_b_tree, fn_b_tree_traverse *pfn_traverse );
int binary_tree_traverse_inorder   ( b_tree *const p_b_tree, fn_b_tree_traverse *pfn_traverse );
int binary_tree_traverse_postorder ( b_tree *const p_b_tree, fn_b_tree_traverse *pfn_traverse );

// Parser
int b_tree_parse ( b_tree **const pp_b_tree, FILE *p_file, fn_tree_equal *pfn_is_equal, fn_b_tree_parse *pfn_parse_node );

// Serializer
int b_tree_serialize ( b_tree *const p_b_tree, const char *p_path, fn_b_tree_serialize *pfn_serialize_node );

// Destructors
int b_tree_destroy ( b_tree **const pp_b_tree );
 ```
