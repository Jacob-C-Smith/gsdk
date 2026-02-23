/** !
 * Cross platform synchronization primitives interface
 * 
 * @file src/core/sync/sync.h 
 * 
 * @author Jacob Smith 
 */

// header guard
#pragma once

// standard library
#include <stdio.h>
#include <time.h>

// log module
#include <core/log.h>

// Platform dependent includes
#ifdef _WIN64
    #include <windows.h>
    #include <process.h>
#else
    #include <unistd.h>
    #include <pthread.h>
    #include <semaphore.h>
#endif

// Platform dependent typedefs
#ifdef _WIN64
    typedef HANDLE mutex;
    typedef HANDLE semaphore;
    typedef HANDLE thread;
#elif defined __APPLE__
    typedef pthread_mutex_t    mutex;
    // typedef pthread_rwlock_t   rwlock;
    typedef sem_t*            semaphore;  // Use pointer type for named semaphores
    typedef pthread_cond_t     condition_variable;
    // typedef int               spinlock;    // Fallback for unsupported spinlock
    // typedef int               barrier;     // Fallback for unsupported barrier
    typedef struct
    {
        pthread_mutex_t _mutex;
        pthread_cond_t  _cond;
    } monitor;
#elif defined __linux__
    typedef pthread_mutex_t    mutex;
    // typedef pthread_rwlock_t   rwlock;
    typedef sem_t              semaphore;
    typedef pthread_cond_t     condition_variable;
    typedef struct
    {
        pthread_mutex_t _mutex;
        pthread_cond_t  _cond;
    } monitor;
    // macOS does not support pthread_spinlock_t or pthread_barrier_t by default
    // typedef int spinlock; // fallback type for unsupported spinlock
    // typedef int barrier;  // fallback type for unsupported barrier
#else
    typedef pthread_mutex_t    mutex;
    // typedef pthread_spinlock_t spinlock;
    // typedef pthread_rwlock_t   rwlock;
    typedef sem_t              semaphore;
    typedef pthread_cond_t     condition_variable;
    // typedef pthread_barrier_t  barrier;
    typedef struct
    {
        pthread_mutex_t _mutex;
        pthread_cond_t  _cond;
    } monitor;

#endif

// type definitions
typedef signed long long timestamp;

// function declarations
/// initializer
/** !
 * This gets called at runtime before main. 
 * 
 * @param void
 * 
 * @return void
 */
void sync_init ( void ) __attribute__((constructor));

/// timer
/** !
 * Get a high precision time stamp. Compute differences,
 * and use the SYNC_TIMER_DIVISOR constant to convert 
 * time to seconds
 * 
 * @param void
 * 
 * @sa timer_seconds_divisor
 * @sa sync_init
 * 
 * @return a high precision time stamp
 */
timestamp timer_high_precision ( void );

/** !
 * Get a constant for converting time to seconds. Dividing
 * the difference of two timestamp by the return yields the
 * time between timestamps in seconds.
 * 
 * @param void
 * 
 * @sa timer_high_precision
 * @sa sync_init
 * 
 * @return a constant integer for converting time to seconds
 */
signed timer_seconds_divisor ( void );

/// mutex
/** !
 * Create a mutex
 * 
 * @param p_mutex result
 * 
 * @sa mutex_destroy
 * 
 * @return 1 on success, 0 on error
*/
int mutex_create ( mutex *p_mutex );

/** !
 * Lock a mutex
 * 
 * @param p_mutex the mutex
 * 
 * @sa mutex_unlock
 * 
 * @return 1 on success, 0 on error
 */
int mutex_lock ( mutex *p_mutex );

/** !
 * Unlock a mutex
 * 
 * @param p_mutex the mutex
 * 
 * @sa mutex_lock
 * 
 * @return 1 on success, 0 on error
 */
int mutex_unlock ( mutex *p_mutex );

/** !
 * Free a mutex
 * 
 * @param p_mutex the mutex
 * 
 * @sa mutex_create
 * 
 * @return 1 on success, 0 on error
 */
int mutex_destroy ( mutex *p_mutex );

// /** !
//  * Create a spinlock
//  * 
//  * @param p_spinlock result
//  * 
//  * @sa spinlock_destroy
//  * 
//  * @return 1 on success, 0 on error
// */
// int spinlock_create ( spinlock *p_spinlock );

// /** !
//  * Lock a spinlock
//  * 
//  * @param p_spinlock the spinlock
//  * 
//  * @sa spinlock_unlock
//  * 
//  * @return 1 on success, 0 on error
//  */
// int spinlock_lock ( spinlock *p_spinlock );

// /** !
//  * Unlock a spinlock
//  * 
//  * @param p_spinlock the spinlock
//  * 
//  * @sa spinlock_lock
//  * 
//  * @return 1 on success, 0 on error
//  */
// int spinlock_unlock ( spinlock *p_spinlock );

// /** !
//  * Free a spinlock
//  * 
//  * @param p_spinlock the spinlock
//  * 
//  * @sa spinlock_create
//  * 
//  * @return 1 on success, 0 on error
//  */
// int spinlock_destroy ( spinlock *p_spinlock );

// /** !
//  * Create a read-write lock
//  * 
//  * @param p_rwlock result
//  * 
//  * @sa rwlock_destroy
//  * 
//  * @return 1 on success, 0 on error
//  */
// int rwlock_create ( rwlock *p_rwlock );

// /** !
//  * Lock a reader
//  * 
//  * @param p_rwlock the read-write lock
//  * 
//  * @sa rwlock_lock_wr
//  * @sa rwlock_lock_timeout_rd
//  * @sa rwlock_lock_timeout_wr
//  * @sa rwlock_unlock
//  * 
//  * @return 1 on success, 0 on error
//  */
// int rwlock_lock_rd ( rwlock *p_rwlock );

// /** !
//  * Lock a writer
//  * 
//  * @param p_rwlock the read-write lock
//  * 
//  * @sa rwlock_lock_rd
//  * @sa rwlock_lock_timeout_rd
//  * @sa rwlock_lock_timeout_wr
//  * @sa rwlock_unlock
//  * 
//  * @return 1 on success, 0 on error
//  */
// int rwlock_lock_wr ( rwlock *p_rwlock );

// /** !
//  * Lock a reader
//  * 
//  * @param p_rwlock the read-write lock
//  * @param _time    the quantity of time to wait, in nanoseconds 
//  * 
//  * @sa rwlock_lock_wr
//  * @sa rwlock_lock_timeout_wr
//  * @sa rwlock_unlock
//  * 
//  * @return 1 on success, 0 on error
//  */
// int rwlock_lock_timeout_rd ( rwlock *p_rwlock, timestamp _time );

// /** !
//  * Lock a writer
//  * 
//  * @param p_rwlock the read-write lock
//  * @param _time    the quantity of time to wait, in nanoseconds 
//  * 
//  * @sa rwlock_lock_rd
//  * @sa rwlock_lock_timeout_rd
//  * @sa rwlock_unlock
//  * 
//  * @return 1 on success, 0 on error
//  */
// int rwlock_lock_timeout_wr ( rwlock *p_rwlock, timestamp _time );

// /** !
//  * Unlock a read-write lock
//  * 
//  * @param p_rwlock the read-write lock
//  * 
//  * @sa rwlock_lock_rd
//  * @sa rwlock_lock_wr
//  * 
//  * @return 1 on success, 0 on error
//  */
// int rwlock_unlock ( rwlock *p_rwlock );

// /** !
//  * Destroy a read-write lock
//  * 
//  * @param p_rwlock the read-write lock
//  * 
//  * @sa rwlock_create
//  * 
//  * @return 1 on success, 0 on error
//  */
// int rwlock_destroy ( rwlock *p_rwlock );

/// semaphore
/** !
 * Create a semaphore
 * 
 * @param p_semaphore result
 * @param count       the initial count
 * 
 * @sa semaphore_destroy
 * 
 * @return 1 on success, 0 on error
 */
int semaphore_create ( semaphore *p_semaphore, unsigned int count );

/** !
 * Wait on a semaphore
 * 
 * @param _semaphore the semaphore
 * 
 * @sa semaphore_signal
 * 
 * @return 1 on success, 0 on error
 */
int semaphore_wait ( semaphore _semaphore );

/** !
 * Try to wait on a semaphore (non-blocking)
 * 
 * @param _semaphore the semaphore
 * 
 * @sa semaphore_wait
 * 
 * @return 1 on success, 0 on error/busy
 */
int semaphore_try_wait ( semaphore _semaphore );

/** !
 * Signal a semaphore
 * 
 * @param _semaphore the semaphore
 * 
 * @sa semaphore_wait
 * 
 * @return 1 on success, 0 on error
 */
int semaphore_signal ( semaphore _semaphore );

/** !
 * Free a semaphore
 * 
 * @param p_semaphore the semaphore
 * 
 * @sa semaphore_create
 * 
 * @return 1 on success, 0 on error
 */
int semaphore_destroy ( semaphore *p_semaphore );

/// condition variable
/** !
 * Create a condition variable
 * 
 * @param p_condition_variable result
 * 
 * @sa condition_variable_destroy
 * 
 * @return 1 on success, 0 on error
 */
int condition_variable_create ( condition_variable *p_condition_variable );

/** !
 * Wait on a condition variable
 * 
 * @param p_condition_variable the condition variable
 * @param p_mutex              the mutex
 * 
 * @sa condition_variable_wait_timeout
 * 
 * @return 1 on success, 0 on error
 */
int condition_variable_wait ( condition_variable *p_condition_variable, mutex *p_mutex );

/** !
 * Wait on a condition variable with some timeout
 * 
 * @param p_condition_variable the condition variable
 * @param p_mutex              the mutex
 * @param _time                the quantity of time to wait, in nanoseconds
 * 
 * @sa condition_variable_wait
 * 
 * @return 1 on success, 0 on error
 */
int condition_variable_wait_timeout ( condition_variable *p_condition_variable, mutex *p_mutex, timestamp _time );

/** !
 * Signal once thread
 * 
 * @param p_condition_variable the condition variable
 * 
 * @sa condition_variable_broadcast
 * 
 * @return 1 on success, 0 on error
 */
int condition_variable_signal ( condition_variable *const p_condition_variable );

/** !
 * Signal all threads
 * 
 * @param p_condition_variable the condition variable
 * 
 * @sa condition_variable_signal
 * 
 * @return 1 on success, 0 on error
 */
int condition_variable_broadcast ( condition_variable *const p_condition_variable );

/** !
 * Destroy a condition variable
 * 
 * @param p_condition_variable the condition variable
 * 
 * @sa condition_variable_create
 * 
 * @return 1 on success, 0 on error
 */
int condition_variable_destroy ( condition_variable *p_condition_variable );

/// monitor
/** !
 * Create a monitor
 * 
 * @param p_monitor result
 * 
 * @sa monitor_destroy
 * 
 * @return 1 on success, 0 on error
 */
int monitor_create ( monitor *p_monitor );

/** !
 * Wait on a monitor
 * 
 * @param p_monitor the monitor
 * 
 * @sa monitor_notify
 * @sa monitor_notify_all
 * 
 * @return 1 on success, 0 on error
 */
int monitor_wait ( monitor *p_monitor );

/** !
 * Signal one thread
 * 
 * @param p_monitor the monitor
 * 
 * @sa monitor_wait
 * @sa monitor_notify_all
 * 
 * @return 1 on success, 0 on error
 */
int monitor_notify ( monitor *p_monitor );

/** !
 * Signal all threads
 * 
 * @param p_monitor the monitor
 * 
 * @sa monitor_wait
 * @sa monitor_notify
 * 
 * @return 1 on success, 0 on error
 */
int monitor_notify_all ( monitor *p_monitor );

/** !
 * Free a monitor
 * 
 * @param p_monitor the monitor
 * 
 * @sa monitor_create
 * 
 * @return 1 on success, 0 on error
 */
int monitor_destroy ( monitor *p_monitor );

// /** !
//  * Create a barrier
//  * 
//  * @param p_barrier result
//  * @param count     the quantity of threads that must wait at the barrier
//  * 
//  * @sa barrier_destroy
//  * 
//  * @return 1 on success, 0 on error
//  */
// int barrier_create ( barrier *p_barrier, unsigned int count );

// /** !
//  * Wait at a barrier
//  * 
//  * @param p_barrier the barrier
//  * 
//  * @return 1 on success, 0 on error
//  */
// int barrier_wait ( barrier *p_barrier );

// /** !
//  * Destroy a barrier
//  * 
//  * @param p_barrier the barrier
//  * 
//  * @sa barrier_create
//  * 
//  * @return 1 on success, 0 on error
//  */
// int barrier_destroy ( barrier *p_barrier );

/// cleanup
/** !
 * This gets called at runtime after main
 * 
 * @param void
 * 
 * @return void
 */
void sync_exit ( void ) __attribute__((destructor));
