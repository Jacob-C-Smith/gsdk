# priority-queue
[![CMake](https://github.com/Jacob-C-Smith/priority-queue/actions/workflows/cmake.yml/badge.svg)](https://github.com/Jacob-C-Smith/priority-queue/actions/workflows/cmake.yml)

**Dependencies:**\
[![log](https://github.com/Jacob-C-Smith/log/actions/workflows/cmake.yml/badge.svg)](https://github.com/Jacob-C-Smith/log/actions/workflows/cmake.yml) [![sync](https://github.com/Jacob-C-Smith/sync/actions/workflows/cmake.yml/badge.svg)](https://github.com/Jacob-C-Smith/sync/actions/workflows/cmake.yml)

 A minimal, thread-safe priority queue written in C. 

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
 To download priority-queue, execute the following command
 ```bash
 $ git clone https://github.com/Jacob-C-Smith/priority-queue
 ```
 ## Build
 To build on UNIX like machines, execute the following commands in the same directory
 ```bash
 $ cd priority-queue
 $ cmake .
 $ make
 ```
  This will build the example program, the tester program, and dynamic / shared libraries

  To build priority-queue for Windows machines, open the base directory in Visual Studio, and build your desired target(s)
 ## Example
 To run the example program, execute this command
 ```
 $ ./priority_queue_example
 ```
 ### Example output
TODO: 

 [Source](main.c)
## Tester
 To run the tester program, execute this command after building
 ```
 $ ./priority_queue_test
 ```
 [Source](priority_queue_test.c)
 
 TODO: [Tester output](test_output.txt)
 ## Definitions
 ### Type definitions
 ```c
 typedef struct priority_queue_s priority_queue;

 typedef int (priority_queue_equal_fn)(const void *const p_a, const void *const p_b);
 ```

 ### Function definitions
 ```c 
// Allocaters
int priority_queue_create ( priority_queue **const pp_priority_queue );

// Constructors
int priority_queue_construct ( priority_queue **const pp_priority_queue, size_t size, priority_queue_equal_fn pfn_compare_function );

// Constructors
int priority_queue_from_keys ( priority_queue **const pp_priority_queue, const char **const keys, size_t size, priority_queue_equal_fn pfn_compare_function );

// Accessors
bool priority_queue_empty ( priority_queue *const p_priority_queue );

// Mutators
int priority_queue_enqueue ( priority_queue *const p_priority_queue, void  *p_key );
int priority_queue_dequeue ( priority_queue *const p_priority_queue, void **pp_key );

// Destructors
int priority_queue_destroy ( priority_queue **const pp_priority_queue );
 ```




