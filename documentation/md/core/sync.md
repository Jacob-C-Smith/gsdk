# [gsdk](../../../README.md) > [core](../core.md) > sync

## Cross platform synchronization primatives and high precision timestamps

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
 $ ./sync_example
 ```
 ### Example output
 [ NONE ]

 [Source](main.c) 
 ## Definitions
 ### Type definitions
 ```c
 typedef ... mutex;
 typedef ... rwlock;
 typedef ... spinlock;
 typedef ... semaphore;

 typedef ... condition_variable;
 typedef ... monitor;
 typedef ... barrier;

 typedef signed timestamp;
 ```
 *NOTE: mutex and semaphore definitions are platform dependent*
 ### Function definitions
 ```c 
 // initializer
void sync_init ( void ) __attribute__((constructor));

// timer
timestamp timer_high_precision  ( void );
signed    timer_seconds_divisor ( void );

// mutex
int mutex_create  ( mutex *p_mutex );
int mutex_lock    ( mutex *p_mutex );
int mutex_unlock  ( mutex *p_mutex );
int mutex_destroy ( mutex *p_mutex );

// semaphore
int semaphore_create  ( semaphore *p_semaphore, unsigned int count );
int semaphore_wait    ( semaphore _semaphore );
int semaphore_signal  ( semaphore _semaphore );
int semaphore_destroy ( semaphore *p_semaphore );

// condition variable
int condition_variable_create       ( condition_variable *p_condition_variable );
int condition_variable_wait         ( condition_variable *p_condition_variable, mutex *p_mutex );
int condition_variable_wait_timeout ( condition_variable *p_condition_variable, mutex *p_mutex, timestamp _time );
int condition_variable_signal       ( condition_variable *const p_condition_variable );
int condition_variable_broadcast    ( condition_variable *const p_condition_variable );
int condition_variable_destroy      ( condition_variable *p_condition_variable );

// monitor
int monitor_create     ( monitor *p_monitor );
int monitor_wait       ( monitor *p_monitor );
int monitor_notify     ( monitor *p_monitor );
int monitor_notify_all ( monitor *p_monitor );
int monitor_destroy    ( monitor *p_monitor );

// barrier
int barrier_create  ( barrier *p_barrier, int count );
int barrier_wait    ( barrier *p_barrier );
int barrier_destroy ( barrier *p_barrier );

// cleanup
void sync_exit ( void ) __attribute__((destructor));
 ```
