# queue
[![CMake](https://github.com/Jacob-C-Smith/queue/actions/workflows/cmake.yml/badge.svg)](https://github.com/Jacob-C-Smith/queue/actions/workflows/cmake.yml)

**Dependencies:**\
[![sync](https://github.com/Jacob-C-Smith/sync/actions/workflows/cmake.yml/badge.svg)](https://github.com/Jacob-C-Smith/sync/actions/workflows/cmake.yml)
[![CMake](https://github.com/Jacob-C-Smith/log/actions/workflows/cmake.yml/badge.svg)](https://github.com/Jacob-C-Smith/log/actions/workflows/cmake.yml)

 A minimal queue implementation written in C. 
 
 > 0 [Try it](#try-it)
 >
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

## Try it
[![Open in GitHub Codespaces](https://github.com/codespaces/badge.svg)](https://codespaces.new/Jacob-C-Smith/queue?quickstart=1)

Wait for a few moments, then click the play button on the bottom of the window. This will run the example program.

 ## Download
 To download queue, execute the following command
 ```bash
 $ git clone https://github.com/Jacob-C-Smith/queue
 ```
 ## Build
 To build on UNIX like machines, execute the following commands in the same directory
 ```bash
 $ cd queue
 $ cmake .
 $ make
 ```
  This will build the example program, the tester program, and dynamic / shared libraries

  To build queue for Windows machines, open the base directory in Visual Studio, and build your desired target(s)
 ## Example
 To run the example program, execute this command
 ```
 $ ./queue_example
 ```
 ### Example output
 ```
First!
Second!
Third!

Fourth!
Fifth!
Sixth!
Detected queue underflow!
 ```
 [Source](main.c)
## Tester
 To run the tester program, execute this command after building
 ```
 $ ./queue_test
 ```
 [Source](queue_test.c)
 ## Definitions
 ### Type definitions
 ```c
 typedef struct queue_s queue;
 ```
 ### Function definitions
 ```c 
// Allocaters
int  queue_create        ( queue **pp_queue );

// Constructors
int  queue_construct     ( queue **pp_queue );
int  queue_from_contents ( queue **pp_queue, void **pp_contents, size_t size );

// Accessors
int  queue_front         ( queue  *p_queue , void **pp_value );
int  queue_rear          ( queue  *p_queue , void **pp_value );
bool queue_empty         ( queue  *p_queue );

// Mutators
int  queue_enqueue       ( queue  *p_queue , void  *data );
int  queue_dequeue       ( queue  *p_queue , void **pp_value );

// Destructors
int  queue_destroy       ( queue **pp_queue );
 ```
