# code-gen 

[![code-gen](https://github.com/Jacob-C-Smith/code-gen/actions/workflows/cmake.yml/badge.svg)](https://github.com/Jacob-C-Smith/code-gen/actions/workflows/cmake.yml)
 
 Code generation for many architectures

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
 > 4 [Definitions](#definitions)
 >
 >> 4.1 [Type definitions](#type-definitions)
 >>
 >> 4.2 [Function definitions](#function-definitions)

## Try it
[![Open in GitHub Codespaces](https://github.com/codespaces/badge.svg)](https://codespaces.new/Jacob-C-Smith/code-gen?quickstart=1)

Wait for a few moments, then click the play button on the bottom of the window. This will run the example program.

 ## Download
 To download code-gen, execute the following command
 ```bash
 $ git clone https://github.com/Jacob-C-Smith/code-gen
 ```
 ## Build
 To build on UNIX like machines, execute the following commands in the same directory
 ```bash
 $ cd code-gen
 $ cmake .
 $ make
 ```
  This will build the example program, the tester program, and dynamic / shared libraries

  To build code-gen for Windows machines, open the base directory in Visual Studio, and build your desired target(s)
 ## Example
 To run the example program, execute this command
 ```
 $ ./code_gen_example
 ```
 ### Example output
 [ TODO ]

 [Source](main.c) 
 ## Definitions
 ### Type definitions
 [ TODO ]
 ### Function definitions
 ```c 
 // Initializer
void code_gen_init ( void ) __attribute__((constructor));

// TODO: Add x86_64 code generation functions

// Cleanup
void code_gen_exit ( void ) __attribute__((destructor));
 ```
