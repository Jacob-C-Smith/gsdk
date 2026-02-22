# [gsdk](../../../README.md) > [core](../core.md) > hash

## Hashing functions

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
$ ./build/examples/hash_example
 ```
 ## Tester
 To run the tester program, execute this command
 ```
$ ./build/tests/hash_test
 ```
 
 ## Definitions
 
 ### Function declarations
 ```c
// function declarations
// hashing
hash64 hash_fnv64 ( const void *const k, size_t l );
hash64 hash_mmh64 ( const void *const k, size_t l );
hash64 hash_crc64 ( const void *const k, size_t l );
 ```
