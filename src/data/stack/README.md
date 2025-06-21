# stack
[![CMake](https://github.com/Jacob-C-Smith/stack/actions/workflows/cmake.yml/badge.svg)](https://github.com/Jacob-C-Smith/stack/actions/workflows/cmake.yml)

**Dependencies:**\
[![sync](https://github.com/Jacob-C-Smith/sync/actions/workflows/cmake.yml/badge.svg)](https://github.com/Jacob-C-Smith/sync/actions/workflows/cmake.yml)
[![CMake](https://github.com/Jacob-C-Smith/log/actions/workflows/cmake.yml/badge.svg)](https://github.com/Jacob-C-Smith/log/actions/workflows/cmake.yml)


 A minimal, thread-safe stack implementation written in C. 
 
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
[![Open in GitHub Codespaces](https://github.com/codespaces/badge.svg)](https://codespaces.new/Jacob-C-Smith/log?quickstart=1)

Wait for a few moments, then click the play button on the bottom of the window. This will run the example program.

 ## Download
 To download stack, execute the following command
 ```bash
 $ git clone https://github.com/Jacob-C-Smith/stack
 ```
 ## Build
 To build on UNIX like machines, execute the following commands in the same directory
 ```bash
 $ cd stack
 $ cmake .
 $ make
 ```
  This will build the example program, the tester program, and dynamic / shared libraries

  To build stack for Windows machines, open the base directory in Visual Studio, and build your desired target(s)
 ## Example
 To run the example program, execute this command
 ```
 $ ./stack_example
 ```
 ### Example output

 [Source](main.c)
## Tester
 To run the tester program, execute this command after building
 ```
 $ ./stack_test
 ```
 [Source](stack_test.c)
 
 [Tester output](test_output.txt)

 ## Definitions
 ### Type definitions
 ```c
 typedef struct stack_s stack;
 ```
 ### Function definitions
 ```c 
// Constructors 
int stack_construct ( const stack **const pp_stack, size_t size );

// Mutators
int stack_push ( stack *const p_stack, const void *const        p_value );
int stack_pop  ( stack *const p_stack, const void *      *const ret );

// Accessors
int stack_peek ( const stack *const p_stack, const void **const ret );

// Destructors
int stack_destroy ( stack **const pp_stack );
```