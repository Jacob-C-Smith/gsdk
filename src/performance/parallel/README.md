# parallel
[![CMake](https://github.com/Jacob-C-Smith/parallel/actions/workflows/cmake.yml/badge.svg?branch=main)](https://github.com/Jacob-C-Smith/parallel/actions/workflows/cmake.yml)

**Dependencies:**\
[![sync](https://github.com/Jacob-C-Smith/sync/actions/workflows/cmake.yml/badge.svg)](https://github.com/Jacob-C-Smith/sync/actions/workflows/cmake.yml)
[![crypto](https://github.com/Jacob-C-Smith/crypto/actions/workflows/cmake.yml/badge.svg)](https://github.com/Jacob-C-Smith/crypto/actions/workflows/cmake.yml)
[![dict](https://github.com/Jacob-C-Smith/dict/actions/workflows/cmake.yml/badge.svg)](https://github.com/Jacob-C-Smith/dict/workflows/cmake.yml)
[![array](https://github.com/Jacob-C-Smith/array/actions/workflows/cmake.yml/badge.svg)](https://github.com/Jacob-C-Smith/arrayon/actions/workflows/cmake.yml)
[![json](https://github.com/Jacob-C-Smith/json/actions/workflows/cmake.yml/badge.svg)](https://github.com/Jacob-C-Smith/json/actions/workflows/cmake.yml)
[![log](https://github.com/Jacob-C-Smith/log/actions/workflows/cmake.yml/badge.svg)](https://github.com/Jacob-C-Smith/log/actions/workflows/cmake.yml)
[![sync](https://github.com/Jacob-C-Smith/sync/actions/workflows/cmake.yml/badge.svg)](https://github.com/Jacob-C-Smith/sync/actions/workflows/cmake.yml)

 High level abstractions for parallel computing
 
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
[![Open in GitHub Codespaces](https://github.com/codespaces/badge.svg)](https://codespaces.new/Jacob-C-Smith/parallel?quickstart=1)

Wait for a few moments, then click the play button on the bottom of the window. This will run the example program.

 ## Download
 To download parallel, execute the following command
 ```bash
 $ git clone https://github.com/Jacob-C-Smith/parallel
 ```
 ## Build
 To build on UNIX like machines, execute the following commands in the same directory
 ```bash
 $ cd parallel
 $ git submodule update --init
 $ cmake .
 $ make
 ```
  This will build the example program, the tester program, and dynamic / shared libraries

  To build parallel for Windows machines, open the base directory in Visual Studio, and build your desired target(s)
 ## Example
 **NOTE: You may have to copy the ```schedule.json``` file to the ```build``` directory.**
 
 To run the example program, execute the following commands
 ```bash
 $ ./parallel_example
 ```
 ### Example output
 
 **TODO**
 
 [Source](main.c)

## Tester
 **TODO**
 
 ## Definitions
 ### Type definitions
```c
// type definitions
typedef struct parallel_thread_s parallel_thread;
typedef struct thread_pool_s     thread_pool;
typedef struct schedule_s        schedule;

typedef void *(fn_parallel_task)(void *p_parameter);
```
### Parallel function definitions
 ```c
// initializers
int parallel_init ( void );

// Task
int parallel_register_task  ( const char *const name, fn_parallel_task  *pfn_parallel_task );
int parallel_unrgister_task ( const char *const name );
int parallel_find_task      ( const char *const name, fn_parallel_task **p_pfn_parallel_task );

// destructors
void parallel_quit ( void );
 ```

### Thread function definitions
 ```c
// constructors
int parallel_thread_create ( parallel_thread **pp_parallel_thread );

// Start
int parallel_thread_start ( parallel_thread **pp_parallel_thread, fn_parallel_task *pfn_task, void *p_parameter );

// Stop
int parallel_thread_join ( parallel_thread **pp_parallel_thread );

// destructors
int parallel_thread_destory ( parallel_thread **pp_parallel_thread );
 ```

### Thread pool function definitions
 ```c
 // TODO
 ```

### Schedule function definitions
 ```c
// allocators
int schedule_create ( schedule **const pp_schedule );

// constructors
int schedule_load ( schedule **const pp_schedule, const char *const path );

// Start
int schedule_start ( schedule *const p_schedule );

// Stop
int schedule_stop ( schedule *const p_schedule );

// destructors
int schedule_destroy ( schedule **const pp_schedule );
 ```
