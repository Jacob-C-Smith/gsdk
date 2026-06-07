# GSDK Project Instructions

This document outlines the architecture, coding conventions, and mandatory patterns for the `gsdk` project. Adherence to these guidelines is required for all codebase modifications.

## Architecture Overview
The SDK is organized into modular subsystems:
- **`core/`**: Foundational types, custom logging, hashing functions, binary serialization (`pack`), socket wrappers, synchronization primitives, and a custom testing framework.
- **`crypto/`**: Cryptographic primitives (ChaCha20, Poly1305, Ed25519, X25519, SHA256/512), certificates, and secure socket layers.
- **`data/`**: Comprehensive custom data structures (Graphs, Trees, Lists, Queues, Hash Tables, Sets, Tuples, Bitmaps).
- **`performance/`**: Threading utilities, thread pools, parallel task scheduling.
- **`reflection/`**: Data serialization and parsing (JSON, Base64).

## Memory Management
The project strictly abstracts memory management through a custom universal allocator. 
- **Rule**: **NEVER** use `malloc`, `calloc`, `realloc`, or `free` directly.
- **Rule**: ALWAYS use `default_allocator(void *p_pointer, unsigned long long size)`.
  - **Allocation**: `type *p = default_allocator(NULL, sizeof(type));`
  - **Reallocation**: `p = default_allocator(p, new_size);`
  - **Deallocation**: `p = default_allocator(p, 0);`

## Object-Oriented C Patterns
The codebase emulates object-oriented programming via opaque structs and strict constructor/destructor patterns.

### Struct Definitions
Define structs with a `_s` suffix and typedef them:
```c
struct my_type_s {
    // fields
};
typedef struct my_type_s my_type;
```

### Constructors & Destructors
Object lifecycles are managed by functions that take double pointers. They return `1` on success and `0` on error.
- **Constructor**: `int my_type_construct(my_type **pp_obj, ...)` allocates memory internally and initializes the object.
- **Destructor**: `int my_type_destroy(my_type **pp_obj)` cleans up internal allocations, releases the object, and explicitly sets the user's pointer to `NULL`.

## Error Handling Pattern (Critical)
Functions use a strict `goto` pattern for error handling. Every function that can fail must include a single `error handling` block at the bottom.

```c
int my_function ( my_type *p_obj )
{
    // argument check
    if ( NULL == p_obj ) goto no_obj;

    // ... functional logic ...

    // success
    return 1;

    // error handling
    {
        // argument errors
        {
            no_obj:
                #ifndef NDEBUG
                    log_error("[module] Null pointer provided for parameter \"p_obj\" in call to function \"%s\"\n", __FUNCTION__);
                #endif
                return 0;
        }
        // other error categories...
    }
}
```
- **Logging**: Error logs must use `log_error` and **must** be wrapped in `#ifndef NDEBUG`.
- **Return Values**: Standard successful operations return `1`, errors return `0`.

## Naming & Style Conventions
- **General**: `snake_case` is used for all functions, structs, and variables.
- **Pointers**: Single pointers are prefixed with `p_` (e.g., `p_value`).
- **Double Pointers**: Double pointers are prefixed with `pp_` (e.g., `pp_value`).
- **Function Pointers**: Prefixed with `pfn_` (e.g., `pfn_comparator`).
- **Private Fields**: Internal struct members and locks are prefixed with an underscore (e.g., `_lock`, `_type`).
- **Inline Comments**: Use descriptive block phases like `// argument check`, `// initialized data`, `// success`, `// error`, `// lock`, `// unlock`, `// error handling`.
- **Documentation**: Use `/** ! ... */` Javadoc/Doxygen-style blocks for function declarations in headers.

## Concurrency and Thread Safety
When creating concurrent data structures, embed a `mutex _lock` from `core/sync.h`.
- Initialize it in the constructor: `if ( 0 == mutex_create(&p_obj->_lock) ) goto failed_to_create_mutex;`
- Destroy it in the destructor: `mutex_destroy(&p_obj->_lock);`
- Wrap critical reads and mutations with `mutex_lock(&p_obj->_lock);` and `mutex_unlock(&p_obj->_lock);`. Remember to unlock the mutex before returning or jumping to an error state if the lock is held.

## Iterators
Collections utilize a generic iterator interface defined in `core/interfaces.h`. It consists of an opaque state and function pointers `done`, `next`, and `item`. When implementing iterable collections, strictly follow the established iterator pattern found across `data/`.