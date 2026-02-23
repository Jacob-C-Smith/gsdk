# [gsdk](../../../README.md) > [data](../data.md) > array

## Mutable, iterable, integer indexed collection

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
 $ ./build/examples/array_example
 ```
 [Source](../../../src/examples/array_example.c)

 ## Definitions

 ### Type definitions
 ```c
// type definitions
typedef struct array_s array;
 ```

 ### Function declarations

 ```c 
// function declarations
/// constructors
int array_construct      ( array **pp_array, size_t  size );
int array_from_elements  ( array **pp_array, void   *_p_elements[], size_t size );
int array_from_arguments ( array **pp_array, size_t  size         , size_t count, ... );

/// accessors
int    array_index    ( array *p_array, signed   index        , void   **pp_value );
int    array_get      ( array *p_array, void   **pp_elements  , size_t  *p_count );
int    array_slice    ( array *p_array, void    *pp_elements[], signed   lower_bound, signed upper_bound );
bool   array_is_empty ( array *p_array );
size_t array_size     ( array *p_array );

/// mutators
int array_add    ( array *p_array, void   *p_element );
int array_set    ( array *p_array, signed  index    , void  *p_value );
int array_remove ( array *p_array, signed  index    , void **pp_value );

/// sort
int array_sort ( array *p_array, fn_comparator *pfn_comparator );

/// map
int array_map ( array *const p_array, fn_map *pfn_map, fn_allocator *pfn_allocator );

/// iterators
int array_fori ( array *p_array, fn_fori *pfn_fori );

/// reflection
int array_pack   ( void   *p_buffer, array *p_array , fn_pack *pfn_element );
int array_unpack ( array **pp_array, void  *p_buffer, fn_unpack *pfn_element );

/// hash
hash64 array_hash ( array *p_array, fn_hash64 *pfn_element );

/// destructors
int array_destroy ( array **const pp_array, fn_allocator *pfn_allocator );
 ```
