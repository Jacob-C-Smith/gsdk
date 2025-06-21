# double-queue
[![CMake](https://github.com/Jacob-C-Smith/double-queue/actions/workflows/cmake.yml/badge.svg)](https://github.com/Jacob-C-Smith/double-queue/actions/workflows/cmake.yml)

**Dependencies:**\
[![log](https://github.com/Jacob-C-Smith/log/actions/workflows/cmake.yml/badge.svg)](https://github.com/Jacob-C-Smith/log/actions/workflows/cmake.yml) [![sync](https://github.com/Jacob-C-Smith/sync/actions/workflows/cmake.yml/badge.svg)](https://github.com/Jacob-C-Smith/sync/actions/workflows/cmake.yml)

 A minimal, thread-safe double queue written in C. 

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
 To download double-queue, execute the following command
 ```bash
 $ git clone https://github.com/Jacob-C-Smith/double-queue
 ```
 ## Build
 To build on UNIX like machines, execute the following commands in the same directory
 ```bash
 $ cd double-queue
 $ cmake .
 $ make
 ```
  This will build the example program, the tester program, and dynamic / shared libraries

  To build double-queue for Windows machines, open the base directory in Visual Studio, and build your desired target(s)
 ## Example
 To run the example program, execute this command
 ```
 $ ./double_queue_example
 ```
 ### Example output
TODO: 

 [Source](main.c)
## Tester
 To run the tester program, execute this command after building
 ```
 $ ./double_queue_test
 ```
 [Source](double_queue_test.c)
 
 TODO: [Tester output](test_output.txt)
 ## Definitions
 ### Type definitions
 ```c
 typedef struct double_queue_s double_queue;
 ```
 ### Function definitions
 ```c 
// Allocaters
int double_queue_create ( double_queue **const pp_double_queue );

// Constructors
int double_queue_construct     ( double_queue **const pp_double_queue );
int double_queue_from_contents ( double_queue **const pp_double_queue, void * const* const pp_contents, size_t size );

// Accessors
int  double_queue_front ( const double_queue *const p_double_queue, const void **const pp_value );
int  double_queue_rear  ( const double_queue *const p_double_queue, const void **const pp_value );
bool double_queue_empty ( const double_queue *const p_double_queue );

// Mutators
int double_queue_front_add ( double_queue *const p_double_queue,       void  *const data );
int double_queue_front_remove ( double_queue *const p_double_queue, const void **const pp_value );
int double_queue_rear_add  ( double_queue *const p_double_queue,       void  *const data );
int double_queue_rear_remove  ( double_queue *const p_double_queue, const void **const pp_value );

// Destructors
int double_queue_destroy ( double_queue **const pp_double_queue );
```
