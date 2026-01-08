# gsdk Context

## 1. Project Overview
**gsdk** is a comprehensive, cross-platform C library (targeting C23 standards) providing high-level abstractions for data structures, networking, synchronization, cryptography, reflection (serialization), and performance scheduling.

It emphasizes:
- **Object-Oriented C:** Heavy use of opaque structs, constructors/destructors, and vtables.
- **Safety:** Rigorous null-pointer checking and error handling patterns.
- **Portability:** Abstractions for threading, sockets, and time.
- **Serialization:** Built-in support for JSON, Base64, and a custom binary packing format.

## 2. Directory Structure & Key Modules

### `src/core/` (Foundational Primitives)
*   **`allocator/`**: Memory management wrappers (defaults to `malloc`/`realloc`/`free` via `default_allocator`).
*   **`log/`**: Thread-safe logging with ANSI colors and levels (Info, Warning, Error, Pass, Fail, Scenario).
*   **`pack/`**: Binary serialization using format strings (e.g., `"%i32 %s"`).
*   **`sync/`**: Cross-platform primitives: Mutexes, Semaphores, Monitors, Condition Variables, Spinlocks.
*   **`hash/`**: Hashing algorithms: FNV-1a, MurmurHash, CRC64, xxHash.
*   **`interfaces/`**: Standard function typedefs and sane default methods (`fn_comparator`, `fn_equality`, `fn_pack`, `default_comparator`, `default_equality`, `default_hash`, `default_key_accessor` etc.).

### `src/data/` (Data Structures)
*   **Collections:**
    *   `array`: Dynamic array (vector).
    *   `tuple`: Immutable, typed collection.
    *   `dict`: Hash map with chaining.
    *   `set`: Unique element collection.
    *   `cache`: LRU-like dictionary with eviction policies.
    *   `bitmap`: Bit manipulation array.
*   **Linear/Streams:**
    *   `queue`, `stack`.
    *   `double_queue`: Deque.
    *   `priority_queue`: Heap-based priority queue.
    *   `circular_buffer`: Ring buffer.
*   **Trees:**
    *   `binary`: Standard BST.
    *   `b`: B-Tree implementation with **disk storage support**.

### `src/performance/` (Concurrency)
*   **`thread`**: Wrapper for `pthread` (POSIX) or Windows threads.
*   **`thread_pool`**: Worker pool for executing tasks.
*   **`schedule`**: Directed Acyclic Graph (DAG) task scheduler (supports dependencies between tasks).
*   **`connection`**: High-level TCP connection abstraction.

### `src/reflection/` (Serialization)
*   **`json`**: Recursive descent JSON parser and serializer.
*   **`base64`**: Encoding/Decoding.
*   **`http`**: Request/Response parsing and serialization structures.

### `src/core/` (Cryptography)
*   **`aes`**: AES-128 encryption (ECB and CBC modes).
*   **`rsa`**: RSA implementation using C23 `_BitInt(2048)` for large integer arithmetic.
*   **`sha`**: SHA-256 implementation.
*   **`digital_signature`**: RSA+SHA256 signing and verification.

## 3. Coding Conventions & Idioms

### Memory Management
*   **Allocator:** Use `default_allocator(ptr, size)`.
    *   `size > 0`: Acts like `realloc` (or `malloc` if `ptr` is NULL).
    *   `size == 0`: Acts like `free`.
*   **Ownership:** Constructors allocate memory; Destructors must free it and nullify the pointer provided by the caller.

### Error Handling
*   **Return Values:** Functions typically return `int` (1 for Success, 0 for Failure). Pointers return `NULL` on failure.
*   **Pattern:** Uses `goto` for cleanup on error.
    ```c
    if (ptr == NULL) goto no_mem;
    // ...
    no_mem:
        #ifndef NDEBUG
            log_error(...);
        #endif
        return 0;
    ```
*   **Input Validation:** Every public function checks parameters for `NULL` immediately.

### Object Pattern
*   **Constructor:** `int class_construct(class **pp_obj, args...)`.
*   **Destructor:** `int class_destroy(class **pp_obj)`.
*   **Accessors/Mutators:** `class_get(...)`, `class_add(...)`.

## 4. Key API Reference

### `pack` Module (Binary Serialization)
Uses format strings to serialize data to/from buffers.
*   `%i[8|16|32|64]`: Signed integers.
*   `%f[32|64]`: Floating point.
*   `%s`: Null-terminated string.
*   Prefix with number for arrays: `%3i8` = 3 chars.

### `parallel` Module (Scheduler)
*   **Tasks:** Defined as `void *task_func(void *param)`.
*   **Registry:** Tasks are registered globally via strings using `parallel_register_task`.
*   **JSON Definition:** Schedules can be loaded from JSON files defining threads, tasks, and dependencies ("wait" conditions).

## 5. Specific Implementation Notes for the LLM

1.  **RSA implementation:** Relies on `_BitInt(2048)`. If generating code extending this, ensure the compiler environment supports C23 or distinct large-integer logic is handled carefully.
2.  **B-Tree:** This is not just in-memory. It manages a `FILE *p_random_access` pointer to read/write nodes to disk for persistence.
3.  **JSON Parser:** It is a DOM-style parser (creates a tree of `json_value` structs), not a streaming parser.
4.  **Logging:** Always prefer `log_info`, `log_error` over `printf` unless writing CLI output tools.
5.  **Thread Safety:**
    *   Data structures (Dict, Queue, etc.) have internal `mutex_lock`.
    *   Operations generally lock the mutex at the start and unlock before return.

## 6. Common Type Definitions
```c
typedef unsigned long long hash64;
typedef int (fn_comparator)(const void *p_a, const void *p_b);
typedef int (fn_equality)(const void *p_a, const void *p_b);
typedef void *(fn_key_accessor)(const void *p_value);
typedef void (fn_foreach)(void *p_element);
```

## 7. Example Usage (Pattern)

**Creating and using a Dictionary:**
```c
dict *p_dict = NULL;
// Construct with 16 buckets
if (dict_construct(&p_dict, 16, 0) == 0) {
    // Handle error
}

// Add item
dict_add(p_dict, "key", "value");

// Get item
char *val = dict_get(p_dict, "key");

// Cleanup
dict_destroy(&p_dict, NULL); // NULL = don't free content pointers, otherwise pass a free func
```
