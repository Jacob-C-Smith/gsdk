# [gsdk](../../README.md) > [data](../data.md) > queue
## FIFO data structure

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
 $ ./queue_example
 ```
 ### Example output
 ```
First!
Second!
Third!

Fourth!
Fifth!
Sixth!
Detected queue underflow!
 ```
 [Source](main.c)

 ## Definitions
 ### Type definitions
 ```c
 typedef struct queue_s queue;
 ```
 ### Function definitions
 ```c 
// Allocaters
int  queue_create        ( queue **pp_queue );

// constructors
int  queue_construct     ( queue **pp_queue );
int  queue_from_contents ( queue **pp_queue, void **pp_contents, size_t size );

// accessors
int  queue_front         ( queue  *p_queue , void **pp_value );
int  queue_rear          ( queue  *p_queue , void **pp_value );
bool queue_empty         ( queue  *p_queue );

// mutators
int  queue_enqueue       ( queue  *p_queue , void  *data );
int  queue_dequeue       ( queue  *p_queue , void **pp_value );

// destructors
int  queue_destroy       ( queue **pp_queue );
 ```
