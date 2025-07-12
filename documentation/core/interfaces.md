# [gsdk](../../README.md) > [core](../core.md) > interfaces

## interfaces and fallback implementations

 > 1 [Explanation](#explanation)
 >
 >> 1.1 []
 > 
 > 2 [Example](#example)
 >
 >> 2.1 [Example output](#example-output)
 >
 > 3 [Tester](#tester)
 >
 > 4 [Definitions](#definitions)
 >
 >> 4.1 [Type definitions](#type-definitions)
 >>
 >> 4.2 [Function definitions](#function-definitions)

 ## Explanation

 ### allocator

 ### comparator
 A [comparator](#comparator) function. 

 - -1 if **a > b**
 - 0  if **a == b**
 - 1  if **a < b**
 ```c
int comparator ( const void *p_a, const void *p_b )
{

    // default
    return p_b - p_a;
}
 ```

 ### equality


 ### key_accessor
 ```c
void *key_accessor ( const void *const p_value )
{

    // default
    return (void *) p_value;
}
```

 ### hash64

 ### fori 
 ### fori_context

 ### foreach
 ### foreach_context

 ### map

 ### pack 
 ### unpack

 ## Example
 To run the example program, execute this command
TODO
 ### Example output
 TODO
 [Source](main.c)
## TODO: Tester

 ## Definitions
 ### Type definitions
```c

// type definitions
typedef unsigned long long hash64;
typedef struct hash_table_s hash_table;

// function declarations   
void   *default_allocator    ( void *p_pointer, unsigned long long size );
int     default_comparator   ( const void *p_a, const void *p_b );
int     default_equality     ( const void *const p_a, const void *const p_b );
hash64  default_hash         ( const void *const k, unsigned long long l );
void   *default_key_accessor ( const void *const p_value );
```

### Function definitions
 ```c
// interfacesing
interfaces64 interfaces_fnv64 ( const void *const k, size_t l );
interfaces64 interfaces_mmh64 ( const void *const k, size_t l );
interfaces64 interfaces_xxh64 ( const void *const k, size_t l );
interfaces64 interfaces_crc64 ( const void *const k, size_t l );
 ```
