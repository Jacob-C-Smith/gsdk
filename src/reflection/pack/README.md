# pack
[![CMake](https://github.com/Jacob-C-Smith/pack/actions/workflows/cmake.yml/badge.svg?branch=main)](https://github.com/Jacob-C-Smith/pack/actions/workflows/cmake.yml)

 Pack and unpack binary data

 > 1 [Example](#example)
 >
 >> 1.1 [Example output](#example-output)
 >
 > 2 [Tester](#tester)
 >
 > 3 [Definitions](#definitions)
 >
 >> 3.1 [Type definitions](#type-definitions)
 >>
 >> 3.2 [Function definitions](#function-definitions)

## Try it
[![Open in GitHub Codespaces](https://github.com/codespaces/badge.svg)](https://codespaces.new/Jacob-C-Smith/pack?quickstart=1)

Wait for a few moments, then click the play button on the bottom of the window. This will run the example program.

 ## Download
 To download pack, execute the following command
 ```bash
 $ git clone https://github.com/Jacob-C-Smith/pack
 ```
 ## Build
 To build on UNIX like machines, execute the following commands in the same directory
 ```bash
 $ cd pack
 $ cmake .
 $ make
 ```
  This will build the example program, the tester program, and dynamic / shared libraries

  To build pack for Windows machines, open the base directory in Visual Studio, and build your desired target(s)

## Example
 To run the example program, execute this command
 ```
 $ ./pack_example
 ```
### Example output

 ![](pack_output.png)

 [Source](main.c)
## Tester
 TODO

## Definitions
### Type definitions
```c
// type definitions
typedef int (fn_pack)   ( void *p_buffer, const void *const p_value );
typedef int (fn_unpack) ( void *p_value, void *p_buffer );
```

### Function definitions
```c 
// Reflectin
size_t pack   ( void *p_buffer, const char *restrict format, ... );
size_t unpack ( void *p_buffer, const char *restrict format, ... );
```
