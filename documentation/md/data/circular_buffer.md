# [gsdk](../../../README.md) > [data](../data.md) > circular buffer

## Specialized FIFO; Overflows replace the least recently added element.

 > 1 [Example](#example)
 >
 >> 1.1 [Example output](#example-output)
 >
 > 2 [Definitions](#definitions)
 >
 >> 2.1 [Type definitions](#type-definitions)
 >>
 >> 2.2 [Function definitions](#function-definitions)

 ## Example
 To run the example program, execute this command
 ```
 $ ./circular_buffer_example
 ```
 ### Example output
 ```
Creating a circular buffer of size 4

Adding "First!"
Adding "Second!"
Adding "Third!"
Adding "Fourth!"
Adding "Fifth!"
Adding "Sixth!"

Dumping contents of circular buffer

"Third!"
"Fourth!"
"Fifth!"
"Sixth!"
 ```
 [Source](main.c)

 ## Definitions
 ### Type definitions
 ```c
 typedef struct circular_buffer_s circular_buffer;
 ```
 ### Function definitions
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
