# [gsdk](../../../README.md) > [data](../data.md) > priority queue
## Specialized FIFO; Elements are rearranged according to their priority

 > 1 [Example](#example)
 >
 >> 1.1 [Example output](#example-output)
 >
 > 2 [Definitions](#definitions)
 >
 >> 2.1 [Type definitions](#type-definitions)
 >>
 >> 2.2 [Function definitions](#function-definitions)

 ## Example
 To run the example program, execute this command
 ```
 $ ./priority_queue_example
 ```
  ### Example output
  TODO: 

   [Source](main.c)

 ## Definitions
 ### Type definitions
 ```c
 typedef struct priority_queue_s priority_queue;

 typedef int (priority_queue_equal_fn)(const void *const p_a, const void *const p_b);
 ```

 ### Function definitions
 ```c 
// allocaters
int priority_queue_create ( priority_queue **const pp_priority_queue );

// constructors
int priority_queue_construct ( priority_queue **const pp_priority_queue, size_t size, priority_queue_equal_fn pfn_compare_function );

// constructors
int priority_queue_from_keys ( priority_queue **const pp_priority_queue, const char **const keys, size_t size, priority_queue_equal_fn pfn_compare_function );

// accessors
bool priority_queue_empty ( priority_queue *const p_priority_queue );

// mutators
int priority_queue_enqueue ( priority_queue *const p_priority_queue, void  *p_key );
int priority_queue_dequeue ( priority_queue *const p_priority_queue, void **pp_key );

// destructors
int priority_queue_destroy ( priority_queue **const pp_priority_queue );
 ```




