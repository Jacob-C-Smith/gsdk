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
 typedef struct dict_s dict;
 ```
 ### Function declarations
 ```c 
// function declarations
/// constructors
int dict_construct ( dict **pp_dict, size_t   size, fn_hash64 *pfn_hash_function );
int dict_from_keys ( dict **pp_dict, char   **keys, size_t     keys_length );

/// accessors
void   *dict_get    ( dict *p_dict, char  *key );
size_t  dict_values ( dict *p_dict, char **values );
size_t  dict_keys   ( dict *p_dict, char **keys );

/// mutators
int dict_add ( dict *p_dict, const char *key, void  *p_value );
int dict_pop ( dict *p_dict, char       *key, void **pp_value );

/// shallow copy
int dict_copy ( dict *p_dict, dict **pp_dict );

/// clear all items
int dict_clear      ( dict *p_dict );
int dict_free_clear ( dict *p_dict, void (*free_func) (void *) );

/// destructors
int dict_destroy ( dict **pp_dict );
 ```

