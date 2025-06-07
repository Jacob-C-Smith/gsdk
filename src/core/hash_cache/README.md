# hash cache

**Dependencies:**\
Abstractions for hashing and caching data.

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

 ## Download
 To download hash cache, execute the following command
 ```bash
 $ git clone https://github.com/Jacob-C-Smith/hash-cache
 ```
 ## Build
 To build on UNIX like machines, execute the following commands in the same directory
 ```bash
 $ cd hash-cache
 $ cmake .
 $ make
 ```
  This will build the example program, the tester program, and dynamic / shared libraries

  To build hash cache for Windows machines, open the base directory in Visual Studio, and build your desired target(s)
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
typedef hash64 (fn_hash64)                ( const void *const k, size_t l );
typedef int    (fn_hash_cache_equality)   ( const void *const p_a, const void *const p_b );
typedef void  *(fn_hash_cache_key_accessor) ( const void *const p_value );
typedef void   (fn_hash_cache_free)       ( void *p_property );
typedef int    (fn_hash_cache_property)   ( void *p_property );
typedef int    (fn_hash_cache_property_i) ( void *p_property, size_t i );
```
### Hash cache function definitions
 ```c
// Initializers
void hash_cache_init ( void );

// Comparator
int hash_cache_equals ( const void *const p_a, const void *p_b );

// Destructors
void hash_cache_exit ( void );
 ```

### Hash function definitions
 ```c
// Hashing
hash64 hash_fnv64 ( const void *const k, size_t l );
hash64 hash_mmh64 ( const void *const k, size_t l );
hash64 hash_xxh64 ( const void *const k, size_t l );
hash64 hash_crc64 ( const void *const k, size_t l );
 ```

### Cache function definitions
 ```c
// Allocators
int cache_create ( cache **const pp_cache );

// Constructors
int cache_construct ( cache **const pp_cache, size_t size, fn_hash_cache_equality *pfn_equality, fn_hash_cache_key_accessor *pfn_key_get );

// Accessors
int cache_get ( const cache *const p_cache, const void *const p_key, void **const pp_result );

// Mutators
int cache_insert ( cache *const p_cache, const void *const p_key, const void *const p_value );
int cache_remove ( cache *const p_cache, const void *const p_key, void **const pp_result );
int cache_clear  ( cache *const p_cache, fn_hash_cache_free *pfn_free );

// Iterators
int cache_for_i    ( const cache *const p_cache, fn_hash_cache_property_i pfn_function );
int cache_for_each ( const cache *const p_cache, fn_hash_cache_property   pfn_function );

// Destructors
int cache_destroy ( cache **const pp_cache, fn_hash_cache_free *pfn_cache_free );
 ```

### Hash table function definitions
 ```c
TODO
 ```
