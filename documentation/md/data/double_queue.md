# [gsdk](../../../README.md) > [data](../data.md) > double queue

## Specialized FIFO; Enqueue and dequeue from both ends

 > 1 [Example](#example)
 >
 > 2 [Tester](#tester)
 >
 > 3 [Definitions](#definitions)
 >
 >> 3.1 [Type definitions](#type-definitions)
 >>
 >> 3.2 [Function declarations](#function-declarations)

 ## Example
 To run the example program, execute this command
 ```
 $ ./build/examples/double_queue_example
 ```
 
## Tester
 To run the tester program, execute this command 
 ```
 $ ./build/tests/double_queue_test
 ```

 ## Definitions
 ### Type definitions
 ```c
// type definitions
typedef struct double_queue_s double_queue;
 ```

 ### Function declarations
 ```c 
// function declarations
/// constructors
int double_queue_construct     ( double_queue **const pp_double_queue );
int double_queue_from_contents ( double_queue **const pp_double_queue, void *const* const pp_contents, size_t size );

/// accessors
int double_queue_front ( double_queue *const p_double_queue, void **const pp_value );
int double_queue_rear  ( double_queue *const p_double_queue, void **const pp_value );

size_t double_queue_size  ( double_queue *const p_double_queue );
bool   double_queue_empty ( double_queue *const p_double_queue );

/// mutators
int double_queue_front_add    ( double_queue *const p_double_queue, void  *const p_value );
int double_queue_front_remove ( double_queue *const p_double_queue, void **const pp_value );
int double_queue_rear_add     ( double_queue *const p_double_queue, void  *const p_value );
int double_queue_rear_remove  ( double_queue *const p_double_queue, void **const pp_value );

/// map
int double_queue_map ( double_queue *const p_double_queue, fn_map *pfn_map, fn_allocator *pfn_allocator );

/// iterators
int double_queue_foreach ( double_queue *const p_double_queue, fn_foreach *pfn_foreach );
int double_queue_fori    ( double_queue *const p_double_queue, fn_fori    *pfn_fori );

/// reflection
int double_queue_pack   ( void          *p_buffer       , double_queue *p_double_queue, fn_pack   *pfn_element );
int double_queue_unpack ( double_queue **pp_double_queue, void         *p_buffer      , fn_unpack *pfn_element );

/// hash
hash64 double_queue_hash ( double_queue *p_double_queue, fn_hash64 *pfn_element );

/// destructors
int double_queue_destroy ( double_queue **const pp_double_queue, fn_allocator *pfn_allocator );
```
