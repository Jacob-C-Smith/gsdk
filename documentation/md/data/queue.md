# [gsdk](../../../README.md) > [data](../data.md) > queue

## FIFO data structure

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
 $ ./queue_example
 ```

 ## Definitions
 
 ### Type definitions

 ```c
 typedef struct queue_s queue;
 ```

 ### Function declarations

 ```c 
// function declarations
/// constructors
int queue_construct     ( queue **const pp_queue );
int queue_from_contents ( queue **const pp_queue, void * const* const pp_contents, size_t size );

/// accessors
int  queue_front ( queue *const p_queue, void **const pp_value );
int  queue_rear  ( queue *const p_queue, void **const pp_value );
bool queue_empty ( queue *const p_queue );
int  queue_size  ( queue *const p_queue );

/// mutators
int queue_enqueue ( queue *const p_queue, void  *const data );
int queue_dequeue ( queue *const p_queue, void **const pp_value );

/// iterators
int queue_fori ( queue *const p_queue, fn_fori *pfn_fori );

/// reflection
int queue_pack   ( void   *p_buffer, queue *p_queue , fn_pack   *pfn_element );
int queue_unpack ( queue **pp_queue, void  *p_buffer, fn_unpack *pfn_element );

/// hash
hash64 queue_hash ( queue *p_queue, fn_hash64 *pfn_element );

/// destructors
int queue_destroy ( queue **const pp_queue );
 ```
