# [gsdk](../../../README.md) > [data](../data.md) > circular buffer

## Specialized FIFO; Overflows replace the least recently added element.

 > 1 [Example](#example)
 >
 > 3 [Definitions](#definitions)
 >
 >> 3.1 [Type definitions](#type-definitions)
 >>
 >> 3.2 [Function declarations](#function-declarations)

 ## Example
 To run the example program, execute this command
 ```
 $ ./build/examples/circular_buffer_example
 ```

 ## Tester
 To run the tester program, execute this command
 ```
 $ ./build/tests/circular_buffer_test
 ```

 ## Definitions
 ### Type definitions
 ```c
 typedef struct circular_buffer_s circular_buffer;
 ```
 ### Function declarations
 ```c 
// allocaters
int circular_buffer_create ( circular_buffer **const pp_circular_buffer );

// constructors
int circular_buffer_construct     ( circular_buffer **const pp_circular_buffer, size_t size );
int circular_buffer_from_contents ( circular_buffer **const pp_circular_buffer, void * const* const pp_contents, size_t size );

// accessors
bool circular_buffer_empty ( circular_buffer *const p_circular_buffer );
bool circular_buffer_full  ( circular_buffer *const p_circular_buffer );
int  circular_buffer_peek  ( circular_buffer *const p_circular_buffer, void **pp_data );

// mutators
int circular_buffer_push ( circular_buffer *const p_circular_buffer, void  *p_data );
int circular_buffer_pop  ( circular_buffer *const p_circular_buffer, void **pp_data );

// destructors
int circular_buffer_destroy ( circular_buffer **const pp_circular_buffer );
 ```
