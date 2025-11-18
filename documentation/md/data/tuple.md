# [gsdk](../../../README.md) > [data](../data.md) > tuple

## Immutable, iterable, integer indexed collection
 > 1 [Example](#example)
 >
 > 2 [Definitions](#definitions)
 >
 >> 2.1 [Type definitions](#type-definitions)
 >>
 >> 2.2 [Function definitions](#function-definitions)

 ## Example
 To run the example program, execute this command
 ```
 $ ./build/examples/tuple_example
 ```
 ## Definitions
 ### Type definitions
 ```c
 typedef struct tuple_s tuple;
 ```
 ### Function definitions
 ```c 
/// constructors
int tuple_from_elements  ( tuple **const pp_tuple, void *const *const elements     , size_t size );
int tuple_from_arguments ( tuple **const pp_tuple, size_t             element_count, ... );

/// accessors
int   tuple_index    ( const tuple *const p_tuple, signed long long   index, void **const pp_value );
int   tuple_slice    ( const tuple *const p_tuple, const void **const pp_elements, signed long long lower_bound, signed long long upper_bound );
bool  tuple_is_empty ( const tuple *const p_tuple );
size_t tuple_size    ( const tuple *const p_tuple );

/// iterators
int tuple_fori ( tuple *p_tuple, fn_fori *pfn_fori );

/// reflection
int tuple_pack ( void *p_buffer, tuple *p_tuple, fn_pack *pfn_element );
int tuple_unpack ( tuple **pp_tuple, void *p_buffer, fn_unpack *pfn_element );

/// hash
hash64 tuple_hash ( tuple *p_tuple, fn_hash64 *pfn_element );

/// destructors
int tuple_destroy ( tuple **pp_tuple, fn_allocator *pfn_allocator );
```