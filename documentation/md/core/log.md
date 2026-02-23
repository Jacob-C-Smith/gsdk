# [gsdk](../../../README.md) > [core](../core.md) > log

## Log errors, warnings, and information with pretty printing
  
 > 1 [Example](#example)
 >
 > 2 [Definitions](#definitions)
 >
 >> 2.1 [Enumeration definitions](#enumeration-definitions)
 >>
 >> 2.2 [Function declarations](#function-declarations)

 ## Example
 To run the example program, execute this command
 ```
 $ ./build/examples/log_example
 ```
 
 ## Definitions
 ### Enumeration definitions
 ```c
enum log_color_e
{
    black   = 30,
    red     = 31,
    green   = 32,
    yellow  = 33,
    blue    = 34,
    magenta = 35,
    cyan    = 36,
    gray    = 37,
};
 ```
 
 ### Function declarations
 ```c 
// function declarations
/// state
int log_update ( FILE *p_f, bool ansi_color );

/// logging
int log_error   ( const char *const format, ... );
int log_warning ( const char *const format, ... );
int log_info    ( const char *const format, ... );

/// tests
int log_pass     ( const char *const format, ... );
int log_fail     ( const char *const format, ... );
int log_scenario ( const char *const format, ... );

/// general
int log_colorful ( enum log_color_e color, const char *const format, ... );
 ```
