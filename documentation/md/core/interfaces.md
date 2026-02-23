# [gsdk](../../../README.md) > [core](../core.md) > interfaces

## interfaces and fallback implementations

 > 1 [Explanation](#explanation)
 >
 >> 1.1 [allocator](#allocator)
 >>
 >> 1.2 [comparator](#comparator)
 >>
 >> 1.3 [equality](#equality)
 >>
 >> 1.4 [key_accessor](#key_accessor)
 >>
 >> 1.5 [hash64](#hash64)
 >>
 >> 1.6 [fori](#fori)
 >>
 >> 1.7 [foreach](#foreach)
 >>
 >> 1.8 [map](#map)
 >>
 >> 1.9 [pack](#pack)
 >>
 >> 1.10 [unpack](#unpack)
 >
 > 2 [Definitions](#definitions)
 >
 >> 2.1 [Type definitions](#type-definitions)
 >>
 >> 2.2 [Function declarations](#function-declarations)

 ## Explanation

The `interfaces` module provides a set of standard function pointer definitions and default implementations for common operations. These interfaces allow gsdk data structures and algorithms to be generic and customizable.

 ### allocator
 An **allocator** function is responsible for memory management. It handles allocation, reallocation, and deallocation.

 **Parameters**
 - `p_pointer` - pointer to allocated memory **OR NULL** 
 - `size` - desired allocation size

 **Returns**
 - A pointer to `size` bytes of memory **IF** `p_pointer` **IS** `NULL` **AND** `size` > **0**
 - A pointer to `size` bytes of memory **IF** `p_pointer` **IS NOT** `NULL` **AND** `size` > **0**
 - **NULL IF** `size` **IS 0**

 ### comparator
 A **comparator** function imposes a *total ordering* on some collection of objects

 **Parameters**
 - `p_a` pointer to **A**
 - `p_a` pointer to **B**

 **Returns**
 - `< 0` if **A > B**
 - `0`  if **A == B**
 - `> 0`  if **A < B**
 
 ### equality
 An **equality** function tests if two elements are equal.

 **Parameters**
 - `p_a` pointer to **A**
 - `p_a` pointer to **B**

 **Returns**
 - `1` (true) if **a == b**
 - `0` (false) otherwise

 ### key_accessor
 A **key_accessor** function extracts or returns a **key** from a **value**.

 **Parameters**
 - `k` - pointer to **key**
 - `l` - quantity of bytes to hash, starting from **k**

 **Returns**
 - A 64-bit hash of the **key**

 ### hash64
 A **hash64** function computes a 64-bit hash of a key.

 **Parameters**
 - `p_value` - pointer to **value**

 **Returns**
 - pointer to **key**

 ### fori
 A **fori** function is an iterator that provides the **element** and its **index**.

 **Parameters**
 - `p_element` - pointer to **element**
 - `i` - index of **element**

 **Returns**
 - **void**

 ### foreach
 A **foreach** function is a simple iterator that provides the element.

 **Parameters**
 - `p_element` - pointer to **element**

 **Returns**
 - **void**

 ### map
 A **map** function transforms an element.

 **Parameters**
 - `p_element` - pointer to **element**

 **Returns**
 - **NULL** or `p_element` **IF element IS** unchanged 
 - **ELSE** new pointer to **element**
 
 ### pack
 A **pack** function serializes a **value** into a **buffer**.
 
 **Parameters**
 - `p_buffer` - pointer to **buffer**
 - `p_value` - pointer to **value**

 **Returns**
 - bytes written to `p_buffer` 
 
 ### unpack
 An **unpack** function deserializes a value from a buffer.
 
 **Parameters**
 - `p_value` - pointer to **value**
 - `p_buffer` - pointer to **buffer**

 **Returns**
 - bytes read from `p_buffer` 
 
 ## Definitions
 ### Type definitions
```c
// type definitions
/// allocator
typedef void *(fn_allocator) ( void *p_pointer, unsigned long size );

/// identity
typedef int   (fn_comparator)   ( const void *p_a, const void *p_b );
typedef int   (fn_equality)     ( const void *p_a, const void *p_b );
typedef void *(fn_key_accessor) ( const void *const  p_value );

/// iterators
typedef void  (fn_fori)    ( void *p_element, int i );
typedef void  (fn_foreach) ( void *p_element );
typedef void *(fn_map)     ( void *p_element );

/// reflection
typedef int    (fn_pack)   ( void              *p_buffer, const void *const   p_value );
typedef int    (fn_unpack) ( void              *p_value , void               *p_buffer );
typedef hash64 (fn_hash64) ( const void *const  k       , unsigned long long  l );
```

### Function declarations
 ```c
// function declarations
/// allocators
void *default_allocator ( void *p_pointer, unsigned long long size );

/// identity
int   default_comparator   ( const void *p_a, const void *p_b );
int   default_equality     ( const void *const p_a, const void *const p_b );
void *default_key_accessor ( const void *const p_value );

/// reflection
hash64 default_hash ( const void *const k, unsigned long long l );
 ```
