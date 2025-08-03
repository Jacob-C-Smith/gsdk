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
 $ ./tuple_example
 ```
 ## Definitions
 ### Type definitions
 ```c
 typedef struct tuple_s tuple;
 ```
 ### Function definitions
 ```c 
// Allocaters
int tuple_create ( tuple **pp_tuple );

// constructors
int tuple_construct      ( tuple **pp_tuple, size_t size );
int tuple_from_elements  ( tuple **pp_tuple, void **elements, size_t size );
int tuple_from_arguments ( tuple **pp_tuple, int element_count, ... );

// accessors
int    tuple_index    ( const tuple *p_tuple, signed index, void **pp_value );
int    tuple_get      ( const tuple *p_tuple, void **pp_elements, size_t *p_count );
int    tuple_slice    ( const tuple *p_tuple, void **pp_elements, signed lower_bound, signed upper_bound );
bool   tuple_is_empty ( const tuple *p_tuple );
size_t tuple_size     ( const tuple *p_tuple );

// iterators
int tuple_foreach_i ( const tuple *p_tuple, void (*function)(void *value, size_t index) );

// destructors
int tuple_destroy ( tuple **pp_tuple );
```