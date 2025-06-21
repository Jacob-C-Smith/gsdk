# hash cache

Abstractions for hashing and caching data.

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
 >> 3.2 [Hash function definitions](#hash-function-definitions)
 >>
 >> 3.3 [Cache function definitions](#cache-function-definitions)
 >>
 >> 3.4 [Hash table function definitions](#hash-table-function-definitions)

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
// Constructor
int hash_table_construct ( hash_table **const pp_hash_table, size_t size );

// Mutators
int hash_table_search ( hash_table *const p_hash_table, void *p_key, void **pp_value );
int hash_table_insert ( hash_table *const p_hash_table, void *p_property );

// Iterators
int hash_table_for_i ( const hash_table *const p_hash_table, fn_fori pfn_fori );
int hash_table_for_each ( const hash_table *const p_hash_table, fn_foreach pfn_foreach );

// Destructors
int hash_table_destroy ( hash_table **const pp_hash_table );
```
