# circular buffer
[![CMake](https://github.com/Jacob-C-Smith/circular-buffer/actions/workflows/cmake.yml/badge.svg)](https://github.com/Jacob-C-Smith/circular-buffer/actions/workflows/cmake.yml)

**Dependencies:**\
[![sync](https://github.com/Jacob-C-Smith/sync/actions/workflows/cmake.yml/badge.svg)](https://github.com/Jacob-C-Smith/sync/actions/workflows/cmake.yml)
[![log](https://github.com/Jacob-C-Smith/sync/actions/workflows/cmake.yml/badge.svg)](https://github.com/Jacob-C-Smith/sync/actions/workflows/cmake.yml)

 Specialized FIFO; Overflows replace the least recently added element.
 
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
 To download circular buffer, execute the following command
 ```bash
 $ git clone https://github.com/Jacob-C-Smith/circular-buffer
 ```
 ## Build
 To build on UNIX like machines, execute the following commands in the same directory
 ```bash
 $ cd circular-buffer
 $ cmake .
 $ make
 ```
  This will build the example program, the tester program, and dynamic / shared libraries

  To build circular buffer for Windows machines, open the base directory in Visual Studio, and build your desired target(s)
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
## Tester
 To run the tester program, execute this command after building
 ```
 $ ./circular_buffer_test
 ```
 [Source](circular_buffer_test.c)
 ## Definitions
 ### Type definitions
 ```c
 typedef struct circular_buffer_s circular_buffer;
 ```
 ### Function definitions
 ```c 
// Allocaters
int circular_buffer_create ( circular_buffer **const pp_circular_buffer );

// Constructors
int circular_buffer_construct     ( circular_buffer **const pp_circular_buffer, size_t size );
int circular_buffer_from_contents ( circular_buffer **const pp_circular_buffer, void * const* const pp_contents, size_t size );

// Accessors
bool circular_buffer_empty ( circular_buffer *const p_circular_buffer );
bool circular_buffer_full  ( circular_buffer *const p_circular_buffer );
int  circular_buffer_peek  ( circular_buffer *const p_circular_buffer, void **pp_data );

// Mutators
int circular_buffer_push ( circular_buffer *const p_circular_buffer, void  *p_data );
int circular_buffer_pop  ( circular_buffer *const p_circular_buffer, void **pp_data );

// Destructors
int circular_buffer_destroy ( circular_buffer **const pp_circular_buffer );
 ```
