# [gsdk](../../../README.md) > [data](../data.md) > dict

 ## Mutable, string indexed collection

 > 1 [Commentary](#commentary)
 > 
 > 2 [Example](#example)
 >
 > 3 [Tester](#tester)
 >
 > 4 [Definitions](#definitions)
 >
 >> 4.1 [Type definitions](#type-definitions)
 >>
 >> 4.2 [Function definitions](#function-declarations)

 ## Commentary
 - I implemented the dictionary using a hash table.
 - I opted to use chaining over open addressing.
 - I opted to use **cyclic redundancy check** after evaluating a few hashing functions.
   - While xxHash is marginally better, I had issues with alignment on my machine
 - I evaluated the hashing functions by running the tester 1024 times, and averaging the run times.
 
 | Hash function           | Time (μs) |
 |-------------------------|-----------|
 | xxHash                  | 193.247   |
 | Cyclic redundancy check | 197.451   |
 | Fowler–Noll–Vo Hash     | 216.446   |
 | MurMur Hash             | 241.187   |

 ## Example
 To run the example program, execute this command
 ```
 $ ./build/examples/dict_example
 ```

 ## Tester
 To run the tester program, execute this command
 ```
 $ ./build/tests/dict_test
 ```

 ## Definitions
 ### Type definitions
 ```c
// type definitions
typedef struct dict_s dict;
 ```
 ### Function declarations
 ```c 
// function declarations
/// constructors
int dict_construct
(
    dict **const pp_dict,
    size_t size,

    fn_allocator    *pfn_allocator,
    fn_key_accessor *pfn_key_accessor,
    fn_hash64       *pfn_hash64
);

/// accessors
int dict_get    ( dict *const p_dict, const char *const p_key, void **pp_value );
int dict_values ( dict *const p_dict, void *pp_values[], size_t limit );
int dict_size   ( dict *const p_dict, size_t *p_result );

/// mutators
int dict_add ( dict *const p_dict, const void *const p_value );
int dict_pop ( dict *const p_dict, const char *const p_key, const void **const pp_value );

/// iterators
int dict_foreach ( dict *const p_dict, fn_foreach *pfn_foreach );

/// reflection
int dict_pack ( void *p_buffer, dict *const p_dict, fn_pack *pfn_element );
int dict_unpack
(
    dict **const pp_dict,
    void *const p_buffer,
    fn_unpack *pfn_element,

    fn_allocator    *pfn_allocator,
    fn_key_accessor *pfn_key_accessor,
    fn_hash64       *pfn_hash64
);

/// hash
hash64 dict_hash ( dict *const p_dict, fn_hash64 *pfn_element );

/// destructors
int dict_destroy ( dict **const pp_dict, fn_allocator *pfn_allocator );
 ```

