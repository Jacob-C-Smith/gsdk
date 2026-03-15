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
 >> 2.2 [Function declarations](#function-declarations)

 ## Example
 To run the example program, execute the following command
 ```bash
 $ ./build/examples/parallel_example [thread] [thread-pool] [schedule]
 ```
 
## Tester
 **TODO**
 
 ## Definitions
 ### Type definitions
```c
// type definitions
typedef struct parallel_task_s parallel_task;
typedef struct parallel_thread_s parallel_thread;
typedef struct thread_pool_s thread_pool;
typedef struct schedule_s schedule;

typedef void *(fn_parallel_task)(void *p_parameter);
```
 ### Function declarations
 #### Parallel function declarations
 ```c
// function declarations
/// constructors
int parallel_thread_create ( parallel_thread **pp_parallel_thread );

/// start
int parallel_thread_start ( parallel_thread **pp_parallel_thread, fn_parallel_task *pfn_task, void *p_parameter );

/// cancel
int parallel_thread_cancel ( parallel_thread *p_parallel_thread );

/// stop
int parallel_thread_join ( parallel_thread **pp_parallel_thread );

/// destructors
int parallel_thread_destory ( parallel_thread **pp_parallel_thread );
 ```

 #### Thread function declarations
 ```c
// function declarations
/// constructors
int parallel_thread_create ( parallel_thread **pp_parallel_thread );

/// start
int parallel_thread_start ( parallel_thread **pp_parallel_thread, fn_parallel_task *pfn_task, void *p_parameter );

/// cancel
int parallel_thread_cancel ( parallel_thread *p_parallel_thread );

/// stop
int parallel_thread_join ( parallel_thread **pp_parallel_thread );

/// destructors
int parallel_thread_destory ( parallel_thread **pp_parallel_thread );
 ```

 #### Thread pool function declarations
 ```c
// function declarations
/// constructors
int thread_pool_construct ( thread_pool **pp_thread_pool, size_t thread_quantity );

/// executors
int thread_pool_execute ( thread_pool *p_thread_pool, fn_parallel_task *pfn_parallel_task, void *p_parameter );

/// accessors
bool thread_pool_is_idle ( thread_pool *p_thread_pool );

/// blockers
int thread_pool_wait_idle ( thread_pool *p_thread_pool );

/// destructors
int thread_pool_destroy ( thread_pool **pp_thread_pool );
 ```

 #### Schedule function declarations
 ```c
// function declarations
/// constructors
int schedule_load               ( schedule **const pp_schedule, const char       *const path );
int schedule_load_as_json_value ( schedule **const pp_schedule, const json_value *const p_value );

/// start
int schedule_start ( schedule *const p_schedule, void *const p_parameter );

/// wait
int schedule_wait_idle ( schedule *const p_schedule );

/// destructors
int schedule_destroy ( schedule **const pp_schedule );
 ```
