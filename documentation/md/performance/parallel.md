# [gsdk](../../../README.md) > [performance](../performance.md)

## High level abstractions for parallel computing
 
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
 To run the example program, execute the following commands
 ```bash
 $ ./parallel_example
 ```
 ### Example output
 
 **TODO**
 
 [Source](main.c)

## Tester
 **TODO**
 
 ## Definitions
 ### Type definitions
```c
// type definitions
typedef struct parallel_thread_s parallel_thread;
typedef struct thread_pool_s     thread_pool;
typedef struct schedule_s        schedule;

typedef void *(fn_parallel_task)(void *p_parameter);
```
### Parallel function definitions
 ```c
// initializers
int parallel_init ( void );

// Task
int parallel_register_task  ( const char *const name, fn_parallel_task  *pfn_parallel_task );
int parallel_unrgister_task ( const char *const name );
int parallel_find_task      ( const char *const name, fn_parallel_task **p_pfn_parallel_task );

// destructors
void parallel_quit ( void );
 ```

### Thread function definitions
 ```c
// constructors
int parallel_thread_create ( parallel_thread **pp_parallel_thread );

// Start
int parallel_thread_start ( parallel_thread **pp_parallel_thread, fn_parallel_task *pfn_task, void *p_parameter );

// Stop
int parallel_thread_join ( parallel_thread **pp_parallel_thread );

// destructors
int parallel_thread_destory ( parallel_thread **pp_parallel_thread );
 ```

### Thread pool function definitions
 ```c
 // TODO
 ```

### Schedule function definitions
 ```c
// allocators
int schedule_create ( schedule **const pp_schedule );

// constructors
int schedule_load ( schedule **const pp_schedule, const char *const path );

// Start
int schedule_start ( schedule *const p_schedule );

// Stop
int schedule_stop ( schedule *const p_schedule );

// destructors
int schedule_destroy ( schedule **const pp_schedule );
 ```
