# [gsdk](../../../README.md) > [data](../data.md) > bitmap

## Mutable, iterable, integer indexed boolean collection
 > 1 [Example](#example)
 >
 > 2 [Tester](#tester)
 >
 > 2 [Definitions](#definitions)
 >
 >> 2.1 [Type definitions](#type-definitions)
 >>
 >> 2.2 [Function declarations](#function-declarations)

 ## Example
 To run the example program, execute this command
 ```
 $ ./build/examples/bitmap_example
 ```

 ## Tester
To run the example program, execute this command
 ```
 $ ./build/tests/bitmap_test
 ```
 

 ## Definitions
 ### Type definitions
 ```c
// type definitions
typedef struct bitmap_s bitmap;
 ```

 ### Function declarations
 ```c 
// function declarations
/// constructors
int bitmap_construct ( bitmap **pp_bitmap, size_t bits );

/// accessors
int bitmap_test ( bitmap *p_bitmap, size_t i );

/// mutators
int bitmap_set   ( bitmap *p_bitmap, size_t i );
int bitmap_clear ( bitmap *p_bitmap, size_t i );

/// print
int bitmap_print ( bitmap *p_bitmap );

/// reflection
int bitmap_pack   ( void    *p_buffer , bitmap *p_bitmap );
int bitmap_unpack ( bitmap **pp_bitmap, void   *p_buffer );

/// hash
hash64 bitmap_hash ( bitmap *p_bitmap, fn_hash64 *pfn_hash64 );

/// destructors
int bitmap_destroy ( bitmap **pp_bitmap );
 ```
