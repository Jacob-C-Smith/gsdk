# set
[![CMake](https://github.com/Jacob-C-Smith/set/actions/workflows/cmake.yml/badge.svg)](https://github.com/Jacob-C-Smith/set/actions/workflows/cmake.yml)
 
**Dependencies:**\
[![sync](https://github.com/Jacob-C-Smith/sync/actions/workflows/cmake.yml/badge.svg)](https://github.com/Jacob-C-Smith/sync/actions/workflows/cmake.yml)
[![log](https://github.com/Jacob-C-Smith/log/actions/workflows/cmake.yml/badge.svg)](https://github.com/Jacob-C-Smith/log/actions/workflows/cmake.yml)

 A minimal, thread-safe set implementation written in C. 
 
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
[![Open in GitHub Codespaces](https://github.com/codespaces/badge.svg)](https://codespaces.new/Jacob-C-Smith/set?quickstart=1)

Wait for a few moments, then click the play button on the bottom of the window. This will run the example program.

 ## Download
 To download set, execute the following command
 ```bash
 $ git clone https://github.com/Jacob-C-Smith/set
 ```
 ## Build
 To build on UNIX like machines, execute the following commands in the same directory
 ```bash
 $ cd set
 $ cmake .
 $ make
 ```
  This will build the example program, the tester program, and dynamic / shared libraries

  To build set for Windows machines, open the base directory in Visual Studio, and build your desired target(s)
 ## Example
 To run the example program, execute this command
 ```
 $ ./set_example
 ```
 ### TODO: Example output
 ## TODO: Tester

 ## Definitions
 ### Type definitions
 ```c
 typedef struct set_s set;
 ```
 ### Function definitions
 ```c
// Allocaters
int set_create ( set **const pp_set );

// Constructors
int  set_construct     ( set **const pp_set, size_t             size );
int  set_from_elements ( set **const pp_set, const void **const pp_elements, size_t size );
int  set_union         ( set **const pp_set, const set   *const p_a        , const  set *const p_b );
int  set_difference    ( set **const pp_set, const set   *const p_a        , const  set *const p_b );
int  set_intersection  ( set **const pp_set, const set   *const p_a        , const  set *const p_b );

// Accessors
bool set_isdisjoint ( const set *const p_a, const set *const p_b );
bool set_issubset   ( const set *const p_a, const set *const p_b );
bool set_issuperset ( const set *const p_a, const set *const p_b );

// Mutators
int  set_add                 ( set *const p_set , const void  *      p_element );
void set_discard             ( set *const p_set , void        *      p_element );
int  set_difference_update   ( set *const p_a   , const set   *const p_b );
int  set_intersection_update ( set *const p_a   , const set   *const p_b );
int  set_update              ( set *const p_a   , const set   *const p_b );
int  set_pop                 ( set *const p_set , void       **const pp_value );
int  set_remove              ( set *const p_set , void        *const p_value );

// Clear elements
int  set_clear      ( set *const p_set );
int  set_free_clear ( set *const p_set , void (*pfn_free_func) );

// Shallow copy
int  set_copy ( const set *const p_set , set **const pp_set );

// Destructors
int  set_destroy ( set **const pp_set );
```