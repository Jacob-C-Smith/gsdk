# [gsdk](../../../README.md) > [data](../data.md) > hash table
## Mutable, key indexed collection; Open addressing collision resolution

 > 1 [Example](#example)
 >
 > 2 [Definitions](#definitions)
 >
 >> 2.1 [Preprocessor definitions](#preprocessor-definitions)
 >>
 >> 2.2 [Enumeration definitions](#enumeration-definitions)
 >>
 >> 2.3 [Type definitions](#type-definitions)
 >>
 >> 2.4 [Function declarations](#function-declarations)

 ## Example
 To run the example program, execute this command
 ```
 $ ./build/examples/hash_table_example
 ```

 ## Definitions
 ### Preprocessor definitions
 ```c
// preprocessor definitions
#define TOMBSTONE 0xffffffffffffffff
 ```

 ### Enumeration definitions
 ```c
// enumeration definitions
enum collision_resolution_e 
{
    COLLISION_RESOLUTION_DEFAULT,
    LINEAR_PROBE = COLLISION_RESOLUTION_DEFAULT,
    QUADRATIC_PROBE,
    DOUBLE_HASH,
    COLLISION_RESOLUTION_QUANTITY
};
 ```
 ### Type definitions
 ```c
// type definitions
typedef struct hash_table_s hash_table;
 ```

 ### Function declarations
 ```c 
// function declarations
/// constructors
int hash_table_construct 
(
    hash_table **const pp_hash_table,
    size_t size, 
    enum collision_resolution_e _type,
    
    fn_equality     *pfn_equality, 
    fn_key_accessor *pfn_key_accessor, 
    fn_hash64       *pfn_hash
);

/// accessors
int    hash_table_search      ( hash_table *const p_hash_table, void *p_key, void **pp_value );
double hash_table_load_factor ( hash_table *const p_hash_table );

/// mutators
int hash_table_insert ( hash_table *const p_hash_table, void *p_property );
int hash_table_remove ( hash_table *const p_hash_table, void *p_key, void **pp_value );

/// iterators
int hash_table_foreach ( hash_table *p_hash_table, fn_foreach *pfn_foreach );
int hash_table_fori    ( hash_table *p_hash_table, fn_fori    *pfn_fori );

/// reflection
int hash_table_pack   ( void *p_buffer, hash_table *p_hash_table, fn_pack *pfn_element );
int hash_table_unpack
( 
    hash_table **pp_hash_table,
    void *p_buffer,
    fn_unpack *pfn_element,

    fn_equality     *pfn_equality,
    fn_key_accessor *pfn_key_get,
    fn_hash64       *pfn_hash_function
);

/// hash
hash64 hash_table_hash ( hash_table *p_hash_table, fn_hash64 *pfn_element );

/// destructors
int hash_table_destroy ( hash_table **const pp_hash_table, fn_allocator *pfn_allocator );
 ```
