# [gsdk](../../README.md) > [core](../core.md) > log

## Log errors, warnings, and information with pretty printing
  
 > 1 [Example](#example)
 >
 > 2 [Definitions](#definitions)
 >
 >> 2.1 [Function definitions](#function-definitions)

 ## Example
 To run the example program, execute this command
 ```
 $ ./build/examples/log_example
 ```
 [Source](../../src/examples/log_example.c)
 ## Definitions
 ### Function definitions
 ```c 
 // initializer
 int log_init ( const char *const path );

 // Debug logging
 int log_error   ( const char *const format, ... );
 int log_warning ( const char *const format, ... );
 int log_info    ( const char *const format, ... );
 ```

