# table
[![CMake](https://github.com/Jacob-C-Smith/table/actions/workflows/cmake.yml/badge.svg)](https://github.com/Jacob-C-Smith/table/actions/workflows/cmake.yml)
 
**Dependencies:**\
[![CMake](https://github.com/Jacob-C-Smith/tuple/actions/workflows/cmake.yml/badge.svg?branch=main)](https://github.com/Jacob-C-Smith/tuple/actions/workflows/cmake.yml) [![sync](https://github.com/Jacob-C-Smith/sync/actions/workflows/cmake.yml/badge.svg)](https://github.com/Jacob-C-Smith/sync/actions/workflows/cmake.yml)

 A minimal, thread-safe table implementation written in C. 
 
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
 To download table, execute the following command
 ```bash
 $ git clone https://github.com/Jacob-C-Smith/table
 ```
 ## Build
 To build on UNIX like machines, execute the following commands in the same directory
 ```bash
 $ cd table
 $ cmake .
 $ make
 ```
  This will build the example program, the tester program, and dynamic / shared libraries

  To build table for Windows machines, open the base directory in Visual Studio, and build your desired target(s)
 ## Example
 To run the example program, execute this command
 ```
 $ ./table_example
 ```
 ### Example output
 ```
 TODO
 ```
 [Source](main.c)
## Tester
```
TODO
```
 ## Definitions
 ### Type definitions
 ```c
 typedef struct table_s table;
 ```
 ### Function definitions
 ```c 
 // Allocaters
 int table_create ( table **const pp_table );
 
 // Constructors
 int table_construct ( table **const pp_table, size_t columns, size_t rows );
 
 // Accessors
 int table_get_cell ( const table *const p_table, size_t x, size_t y, void **pp_element );
 
 // Mutators
 int table_set_cell ( table *const p_table, size_t x, size_t y, void *p_element );

 // Destructors
 // TODO
 ```
