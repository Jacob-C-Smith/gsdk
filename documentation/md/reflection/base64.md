# base64

## base64 encoding and decoding
 > 1 [Example](#example)
 >
 > 2 [Tester](#tester)
 >
 > 3 [Definitions](#definitions)
 >
 >> 3.1 [Function declarations](#function-declarations)

 ## Example
 To run the example program, execute this command
 ```
 $ ./build/examples/base64_example
 ```

 ## Tester
 To run the tester program, execute this command
 ```
 $ ./build/tests/base64_test
 ```

 ## Definitions
 ### Function declarations
 ```c 
// function declarations
/// encode
int base64_encode ( char *const p_output, const void *const p_data, size_t len );

/// decode
int base64_decode ( void *const p_output, const char *const p_data, size_t len );
 ```
