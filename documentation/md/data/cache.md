# [gsdk](../../../README.md) > [data](../data.md) > cache

## Specialized dictionary; Overflows replace the oldest added element

 > 1 [Example](#example)
 >
 > 2 [Definitions](#definitions)
 >
 >> 2.1 [Type definitions](#type-definitions)
 >>
 >> 2.2 [Function declarations](#function-declarations)

 ## Example
 To run the example program, execute this command
 ```
 $ ./build/examples/cache_example
 ```
 [Source](../../../src/examples/cache_example.c)

 ## Definitions

 ### Type definitions
 ```c
 typedef struct cache_s cache;
 ```

 ### Function declarations

 ```c 
// function declarations 
/// constructors
int cache_construct ( cache **pp_cache, size_t size, fn_equality *pfn_equality, fn_key_accessor *pfn_key_get );

/// accessors
int cache_find ( cache *p_cache, const void *const p_key, void **const pp_result );

/// mutators
int cache_insert ( cache *p_cache, const void *const p_key, const void *const p_value );
int cache_remove ( cache *p_cache, const void *const p_key, void **const pp_result );

/// map
int cache_map ( cache *const p_cache, fn_map *pfn_map, fn_allocator *pfn_allocator );

/// iterators
int cache_fori ( cache *p_cache, fn_fori pfn_fori );
int cache_for_each ( cache *p_cache, fn_foreach pfn_foreach );

/// reflection
int cache_pack ( void *p_buffer, cache *p_cache, fn_pack *pfn_element );
int cache_unpack ( cache **pp_cache, void *p_buffer, fn_unpack *pfn_element );

/// hash
hash64 cache_hash ( cache *p_cache, fn_hash64 *pfn_element );

/// destructors
int cache_destroy ( cache **const pp_cache );
```
