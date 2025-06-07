# sync 

[![sync](https://github.com/Jacob-C-Smith/sync/actions/workflows/cmake.yml/badge.svg)](https://github.com/Jacob-C-Smith/sync/actions/workflows/cmake.yml)
 
 Cross platform synchronization primatives and high precision timestamps

 > 0 [Try it](#try-it)
 >
 > 1 [Download](#download)
 >
 > 2 [Build](#build)
 >
 > 3 [Example](#example)
 >
 >> 3.1 [Example output](#example-output)
 >
 > 4 [Definitions](#definitions)
 >
 >> 4.1 [Type definitions](#type-definitions)
 >>
 >> 4.2 [Function definitions](#function-definitions)

## Try it
[![Open in GitHub Codespaces](https://github.com/codespaces/badge.svg)](https://codespaces.new/Jacob-C-Smith/sync?quickstart=1)

Wait for a few moments, then click the play button on the bottom of the window. This will run the example program.

 ## Download
 To download sync, execute the following command
 ```bash
 $ git clone https://github.com/Jacob-C-Smith/sync
 ```
 ## Build
 To build on UNIX like machines, execute the following commands in the same directory
 ```bash
 $ cd sync
 $ cmake .
 $ make
 ```
  This will build the example program, the tester program, and dynamic / shared libraries

  To build sync for Windows machines, open the base directory in Visual Studio, and build your desired target(s)
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
 // Initializer
void sync_init ( void ) __attribute__((constructor));

// Timer
timestamp timer_high_precision  ( void );
signed    timer_seconds_divisor ( void );

// Mutex
int mutex_create  ( mutex *p_mutex );
int mutex_lock    ( mutex *p_mutex );
int mutex_unlock  ( mutex *p_mutex );
int mutex_destroy ( mutex *p_mutex );

// Spinlock
int spinlock_create  ( spinlock *p_spinlock );
int spinlock_lock    ( spinlock *p_spinlock );
int spinlock_unlock  ( spinlock *p_spinlock );
int spinlock_destroy ( spinlock *p_spinlock );

// Read Write Lock
int rwlock_create          ( rwlock *p_rwlock );
int rwlock_lock_rd         ( rwlock *p_rwlock );
int rwlock_lock_wr         ( rwlock *p_rwlock );
int rwlock_lock_timeout_rd ( rwlock *p_rwlock, timestamp _time );
int rwlock_lock_timeout_wr ( rwlock *p_rwlock, timestamp _time );
int rwlock_unlock          ( rwlock *p_rwlock );
int rwlock_destroy         ( rwlock *p_rwlock );

// Semaphore
int semaphore_create  ( semaphore *p_semaphore, unsigned int count );
int semaphore_wait    ( semaphore _semaphore );
int semaphore_signal  ( semaphore _semaphore );
int semaphore_destroy ( semaphore *p_semaphore );

// Condition variable
int condition_variable_create       ( condition_variable *p_condition_variable );
int condition_variable_wait         ( condition_variable *p_condition_variable, mutex *p_mutex );
int condition_variable_wait_timeout ( condition_variable *p_condition_variable, mutex *p_mutex, timestamp _time );
int condition_variable_signal       ( condition_variable *const p_condition_variable );
int condition_variable_broadcast    ( condition_variable *const p_condition_variable );
int condition_variable_destroy      ( condition_variable *p_condition_variable );

// Monitor
int monitor_create     ( monitor *p_monitor );
int monitor_wait       ( monitor *p_monitor );
int monitor_notify     ( monitor *p_monitor );
int monitor_notify_all ( monitor *p_monitor );
int monitor_destroy    ( monitor *p_monitor );

// Barrier
int barrier_create  ( barrier *p_barrier, int count );
int barrier_wait    ( barrier *p_barrier );
int barrier_destroy ( barrier *p_barrier );

// Cleanup
void sync_exit ( void ) __attribute__((destructor));
 ```
