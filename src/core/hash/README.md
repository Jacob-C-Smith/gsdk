# hash 

Hashing functions

 > 1 [Example](#example)
 >
 >> 1.1 [Example output](#example-output)
 >
 > 2 [Tester](#tester)
 >
 > 3 [Definitions](#definitions)
 >
 >> 3.1 [Type definitions](#type-definitions)
 >>
 >> 3.2 [Function definitions](#function-definitions)

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
// Data
typedef unsigned long long hash64;
typedef struct cache_s cache;
typedef struct hash_table_s hash_table;

// Functions
typedef hash64 (fn_hash64) ( const void *const k, size_t l );
```

### Function definitions
 ```c
// Hashing
hash64 hash_fnv64 ( const void *const k, size_t l );
hash64 hash_mmh64 ( const void *const k, size_t l );
hash64 hash_xxh64 ( const void *const k, size_t l );
hash64 hash_crc64 ( const void *const k, size_t l );
 ```
