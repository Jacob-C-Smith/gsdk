/** !
 * Cross platform synchronization primitives 
 * 
 * @file sync.c 
 * 
 * @author Jacob Smith
 */

// header file 
#include <core/sync.h>

// Preprocessor macros
#define SEC_2_NS 1000000000

// Data
static signed SYNC_TIMER_DIVISOR = 0;
static bool initialized = false;

void sync_init ( void ) 
{

    // State check
    if ( initialized == true ) return;

    // Initialize the log library
    log_init();

    // Platform dependent implementation
    #ifdef _WIN64
        QueryPerformanceFrequency((LARGE_INTEGER *)&SYNC_TIMER_DIVISOR);
    #else

        // Update the timer divisor
        *(signed *)(&SYNC_TIMER_DIVISOR) = SEC_2_NS;
    #endif

    // Set the initialized flag
    initialized = true;

    // Done
    return;
}

int mutex_create ( mutex *p_mutex )
{

    // Argument check
    if ( p_mutex == (void *) 0 ) goto no_mutex;

    // Platform dependent implementation
    #ifdef _WIN64

        // Create a mutex
        *p_mutex = CreateMutex(0, FALSE, 0);

        // Return
        return ( *p_mutex != 0 );
    #else

        // Return
        return ( pthread_mutex_init(p_mutex, NULL) == 0 );
    #endif

    // error handling
    {
        
        // Argument errors
        {
            no_mutex:
                #ifndef NDEBUG
                    log_error("[sync] Null pointer provided for \"p_mutex\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
    }
}

int mutex_lock ( mutex *p_mutex )
{

    // Platform dependent implementation
    #ifdef _WIN64

        // Platform dependent argument check
        if ( _mutex == INVALID_HANDLE_VALUE ) goto no_mutex;

        // Return
        return ( WaitForSingbject(_mutex, INFINITE) == WAIT_FAILED ? 0 : 1 );
    #else

        // Log
        #ifdef SYNC_DEBUG
            log_info("[sync] [mutex] Locking mutex @ 0x%x\n", (size_t)p_mutex & 0xffff);
        #endif
        
        // Return
        return ( pthread_mutex_lock(p_mutex) == 0 );
    #endif

    // error handling
    {

        // Argument error
        {
            #ifdef _WIN64
                no_mutex:
                    #ifndef NDEBUG
                        log_error("[sync] Invalid parameter provided for \"_mutex\" in call to function \"%s\"\n", __FUNCTION__);
                    #endif

                    // error
                    return 0;
            #endif
        }
    }
}

int mutex_unlock ( mutex *p_mutex )
{

    // Platform dependent implementation
    #ifdef _WIN64
        // Platform dependent argument check
        if ( p_mutex == INVALID_HANDLE_VALUE ) goto no_mutex;
        // Return
        return ReleaseMutex(_mutex);
    #else

        // Log
        #ifdef SYNC_DEBUG
            log_info("[sync] [mutex] Unlocking mutex @ 0x%x\n", (size_t)p_mutex & 0xffff);
        #endif
        
        // Return
        return ( pthread_mutex_unlock(p_mutex) == 0 );
    #endif

    // error handling
    {

        // Argument error
        {

            #ifdef _WIN64
                no_mutex:
                    #ifndef NDEBUG
                        log_error("[sync] Invalid parameter provided for \"_mutex\" in call to function \"%s\"\n", __FUNCTION__);
                    #endif

                    // error
                    return 0;
            #endif
        }
    }
}

int mutex_destroy ( mutex *p_mutex )
{

    // Argument check
    if ( p_mutex == (void *) 0 ) goto no_mutex;

    // Platform dependent implementation
    #ifdef _WIN64

        // Return
        return ( CloseHandle(*p_mutex) );
    #else

        // Return
        return ( pthread_mutex_destroy(p_mutex) == 0 );
    #endif

    // error handling
    {
        
        // Argument errors
        {
            no_mutex:
                #ifndef NDEBUG
                    log_error("[sync] Null pointer provided for \"p_mutex\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
    }
}

/*
int spinlock_create ( spinlock *p_spinlock )
{

    // Argument check
    if ( p_spinlock == (void *) 0 ) goto no_spinlock;

    // Platform dependent implementation
    #ifdef _WIN64
    #elif defined __APPLE__
        return 0;
    #else

        // Return
        return ( pthread_spin_init(p_spinlock, 0) == 0 );
    #endif

    // error handling
    {
        
        // Argument errors
        {
            no_spinlock:
                #ifndef NDEBUG
                    log_error("[sync] Null pointer provided for \"p_spinlock\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
    }
}

int spinlock_lock ( spinlock *p_spinlock )
{

    // Platform dependent implementation
    #ifdef _WIN64
        (void) p_spinlock;
        return 0;
    #elif defined __APPLE__
        (void) p_spinlock;
        return 0;
    #else
        // Return
        return ( pthread_spin_lock(p_spinlock) == 0 );
    #endif

    // error handling
    {

        // Argument error
        {
            #ifdef _WIN64
                no_spinlock:
                    #ifndef NDEBUG
                        log_error("[sync] Invalid parameter provided for \"_spinlock\" in call to function \"%s\"\n", __FUNCTION__);
                    #endif

                    // error
                    return 0;
            #endif
        }
    }
}

int spinlock_unlock ( spinlock *p_spinlock )
{

    // Platform dependent implementation
    #ifdef _WIN64
        (void) p_spinlock;
        return 0;
    #elif defined __APPLE__
        (void) p_spinlock;
        return 0;
    #else
        // Return
        return ( pthread_spin_unlock(p_spinlock) == 0 );
    #endif

    // error handling
    {

        // Argument error
        {

            #ifdef _WIN64
                no_spinlock:
                    #ifndef NDEBUG
                        log_error("[sync] Invalid parameter provided for \"_spinlock\" in call to function \"%s\"\n", __FUNCTION__);
                    #endif

                    // error
                    return 0;
            #endif
        }
    }
}

int spinlock_destroy ( spinlock *p_spinlock )
{

    // Argument check
    if ( p_spinlock == (void *) 0 ) goto no_spinlock;

    // Platform dependent implementation
    #ifdef _WIN64

    #elif defined __APPLE__
        return 0;
    #else
        // Return
        return ( pthread_spin_destroy(p_spinlock) == 0 );
    #endif

    // error handling
    {
        
        // Argument errors
        {
            no_spinlock:
                #ifndef NDEBUG
                    log_error("[sync] Null pointer provided for \"p_spinlock\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
    }
}
*/

/*
int rwlock_create ( rwlock *p_rwlock )
{
    // Argument check
    if ( p_rwlock == (void *) 0 ) goto no_rwlock;

    // Platform dependent implementation
    #ifdef _WIN64
    #else

        // Return
        return ( pthread_rwlock_init(p_rwlock, NULL) == 0 );
    #endif

    // error handling
    {
        
        // Argument errors
        {
            no_rwlock:
                #ifndef NDEBUG
                    log_error("[sync] Null pointer provided for \"p_rwlock\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
    }
}

int rwlock_lock_rd ( rwlock *p_rwlock )
{
    // Argument check
    if ( p_rwlock == (void *) 0 ) goto no_rwlock;

    // Platform dependent implementation
    #ifdef _WIN64
    #else

        // Return
        return ( pthread_rwlock_rdlock(p_rwlock) == 0 );
    #endif

    // error handling
    {
        
        // Argument errors
        {
            no_rwlock:
                #ifndef NDEBUG
                    log_error("[sync] Null pointer provided for \"p_rwlock\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
    }
}

int rwlock_lock_wr ( rwlock *p_rwlock )
{
    // Argument check
    if ( p_rwlock == (void *) 0 ) goto no_rwlock;

    // Platform dependent implementation
    #ifdef _WIN64
    #else

        // Return
        return ( pthread_rwlock_wrlock(p_rwlock) == 0 );
    #endif

    // error handling
    {
        
        // Argument errors
        {
            no_rwlock:
                #ifndef NDEBUG
                    log_error("[sync] Null pointer provided for \"p_rwlock\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
    }
}

int rwlock_lock_timeout_rd ( rwlock *p_rwlock, timestamp _time )
{
    // Argument check
    if ( p_rwlock == (void *) 0 ) goto no_rwlock;
    
    // Platform dependent initialized data
    #ifdef _WIN64
    #elif defined __APPLE__
    #else
        struct timespec abstime = { 
            .tv_sec  = _time * ( 1 / SEC_2_NS ),
            .tv_nsec = _time % SEC_2_NS
        };
    #endif

    // Platform dependent implementation
    #ifdef _WIN64
        (void) p_spinlock;
        return 0;
    #elif defined __APPLE__
        (void) p_rwlock;
        (void) _time;
        return 0;
    #else
        // Return
        return ( pthread_rwlock_timedrdlock(p_rwlock, &abstime) == 0 );
    #endif

    // error handling
    {
        
        // Argument errors
        {
            no_rwlock:
                #ifndef NDEBUG
                    log_error("[sync] Null pointer provided for \"p_rwlock\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
    }
}

int rwlock_lock_timeout_wr ( rwlock *p_rwlock, timestamp _time )
{
    // Argument check
    if ( p_rwlock == (void *) 0 ) goto no_rwlock;

    // Platform dependent initialized data
    #ifdef _WIN64
    #elif defined __APPLE__
    #else
        struct timespec abstime = { 
            .tv_sec  = _time * ( 1 / SEC_2_NS ),
            .tv_nsec = _time % SEC_2_NS
        };
    #endif

    // Platform dependent implementation
    #ifdef _WIN64

    #elif defined __APPLE__
        (void) _time;
        return 0;
    #else
        // Return
        return ( pthread_rwlock_timedwrlock(p_rwlock, &abstime) == 0 );
    #endif

    // error handling
    {
        
        // Argument errors
        {
            no_rwlock:
                #ifndef NDEBUG
                    log_error("[sync] Null pointer provided for \"p_rwlock\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
    }
}

int rwlock_unlock ( rwlock *p_rwlock )
{
    // Argument check
    if ( p_rwlock == (void *) 0 ) goto no_rwlock;

    // Platform dependent implementation
    #ifdef _WIN64
    #else

        // Return
        return ( pthread_rwlock_unlock(p_rwlock) == 0 );
    #endif

    // error handling
    {
        
        // Argument errors
        {
            no_rwlock:
                #ifndef NDEBUG
                    log_error("[sync] Null pointer provided for \"p_rwlock\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
    }
}

int rwlock_destroy ( rwlock *p_rwlock )
{

    // Argument check
    if ( p_rwlock == (void *) 0 ) goto no_rwlock;

    // Platform dependent implementation
    #ifdef _WIN64
    #else

        // Return
        return ( pthread_rwlock_destroy(p_rwlock) == 0 );
    #endif

    // error handling
    {
        
        // Argument errors
        {
            no_rwlock:
                #ifndef NDEBUG
                    log_error("[sync] Null pointer provided for \"p_rwlock\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
    }
}
*/

int semaphore_create ( semaphore *p_semaphore, unsigned int count )
{

    // Argument check
    if ( p_semaphore == (void *) 0 ) goto no_semaphore;
    
    // Platform dependent implementation
    #ifdef _WIN64

        // Create a semaphore with the specified count
        *p_semaphore = CreateSemaphore(NULL, count, count, NULL);

        // Return
        return ( p_semaphore != 0 );
    #else

        // On macOS, use named semaphores
        #ifdef __APPLE__
            static int sem_counter = 0;
            char sem_name[32];
            snprintf(sem_name, sizeof(sem_name), "/gsdk_sem_%d", sem_counter++);
            *p_semaphore = sem_open(sem_name, O_CREAT | O_EXCL, 0644, count);
            sem_unlink(sem_name); // Remove the name but keep the semaphore
            return (*p_semaphore != SEM_FAILED);
        #else
            // Return
            return ( sem_init(p_semaphore, 0, count) == 0 );
        #endif
    #endif

    // error handling
    {
        
        // Argument errors
        {
            no_semaphore:
                #ifndef NDEBUG
                    log_error("[sync] Null pointer provided for \"p_semaphore\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
    }
}

int semaphore_wait ( semaphore _semaphore )
{

    // Platform dependent implementation
    #ifdef _WIN64
        
        // Platform dependent argument check
        if ( _semaphore == INVALID_HANDLE_VALUE ) goto no_semaphore;
        
        // Return
        return ( WaitForSingbject(_semaphore, INFINITE) == WAIT_FAILED ? 0 : 1 );
    #elif defined(__APPLE__)
        // Return (using pointer type for macOS)
        return (sem_wait(_semaphore) == 0);
    #else
        // Return
        return (sem_wait(&_semaphore) == 0);
    #endif

    // error handling
    {

        // Argument error
        {
            #ifdef _WIN64
                no_semaphore:
                    #ifndef NDEBUG
                        log_error("[sync] Invalid parameter provided for \"_semaphore\" in call to function \"%s\"\n", __FUNCTION__);
                    #endif

                    // error
                    return 0;
            #endif
        }
    }
}

int semaphore_signal ( semaphore _semaphore )
{

    // Platform dependent implementation
    #ifdef _WIN64

        // Platform dependent argument check
        if ( _semaphore == INVALID_HANDLE_VALUE ) goto no_semaphore;

        // Return
        return ( ReleaseSemaphore(_semaphore, 1, 0) );
    #elif defined(__APPLE__)
        // Return (using pointer type for macOS)
        return ( sem_post(_semaphore) == 0 );
    #else
        // Return
        return ( sem_post(&_semaphore) == 0 );
    #endif



    // error handling
    {

        // Argument errors
        {
            #ifdef _WIN64
                no_semaphore:
                    #ifndef NDEBUG
                        log_error("[sync] Invalid parameter provided for \"_semaphore\" in call to function \"%s\"\n", __FUNCTION__);
                    #endif

                    // error
                    return 0;
            #endif
        }
    }
}

int semaphore_destroy ( semaphore *p_semaphore )
{

    // Argument check
    if ( p_semaphore == (void *) 0 ) goto no_semaphore;

    // Platform dependent implementation
    #ifdef _WIN64

        // Return
        return ( CloseHandle(*p_semaphore) );
    #else

        // Return
        #ifdef __APPLE__
            return (sem_close(*p_semaphore) == 0);
        #else
            return (sem_destroy(p_semaphore) == 0);
        #endif
    #endif

    // error handling
    {
        
        // Argument errors
        {
            no_semaphore:
                #ifndef NDEBUG
                    log_error("[sync] Null pointer provided for \"p_semaphore\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
    }
}

int condition_variable_create ( condition_variable *p_condition_variable )
{

    // Argument check
    if ( p_condition_variable == (void *) 0 ) goto no_condition_variable;
    
    // Platform dependent implementation
    #ifdef _WIN64
    #else

        // Return
        return ( pthread_cond_init(p_condition_variable, NULL) == 0 );
    #endif

    // error handling
    {
        
        // Argument errors
        {
            no_condition_variable:
                #ifndef NDEBUG
                    log_error("[sync] Null pointer provided for \"p_condition_variable\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
    }
}

int condition_variable_wait ( condition_variable *p_condition_variable, mutex *p_mutex )
{

    // Argument check
    if ( p_condition_variable == (void *) 0 ) goto no_condition_variable;
    if ( p_mutex              == (void *) 0 ) goto no_mutex;

    // Platform dependent implementation
    #ifdef _WIN64
    #else

        // Return
        return ( pthread_cond_wait(p_condition_variable, p_mutex) == 0 );
    #endif

    // error handling
    {
        
        // Argument errors
        {
            no_condition_variable:
                #ifndef NDEBUG
                    log_error("[sync] Null pointer provided for \"p_condition_variable\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
                
            no_mutex:
                #ifndef NDEBUG
                    log_error("[sync] Null pointer provided for \"p_mutex\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
    }
}

int condition_variable_wait_timeout ( condition_variable *p_condition_variable, mutex *p_mutex, timestamp _time )
{

    // Argument check
    if ( p_condition_variable == (void *) 0 ) goto no_condition_variable;
    if ( p_mutex              == (void *) 0 ) goto no_mutex;
    
    // Platform dependent initialized data
    #ifdef _WIN64
    #else
        struct timespec abstime = { 
            .tv_sec  = _time * ( 1 / SEC_2_NS ),
            .tv_nsec = _time % SEC_2_NS
        };
    #endif

    // Platform dependent implementation
    #ifdef _WIN64

        // TODO
    #else

        // Return
        return ( pthread_cond_timedwait(p_condition_variable, p_mutex, &abstime) == 0 );
    #endif

    // error handling
    {
        
        // Argument errors
        {
            no_condition_variable:
                #ifndef NDEBUG
                    log_error("[sync] Null pointer provided for \"p_condition_variable\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
                
            no_mutex:
                #ifndef NDEBUG
                    log_error("[sync] Null pointer provided for \"p_mutex\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
    }
}

int condition_variable_signal ( condition_variable *p_condition_variable )
{

    // Platform dependent implementation
    #ifdef _WIN64
    #else

        // Return
        return pthread_cond_signal(p_condition_variable);
    #endif
}

int condition_variable_broadcast ( condition_variable *p_condition_variable )
{

    // Platform dependent implementation
    #ifdef _WIN64
    #else

        // Return
        return pthread_cond_broadcast(p_condition_variable);
    #endif
}

int condition_variable_destroy ( condition_variable *p_condition_variable )
{

    // Argument check
    if ( p_condition_variable == (void *) 0 ) goto no_condition_variable;
    
    // Platform dependent implementation
    #ifdef _WIN64
    #else

        // Return
        return ( pthread_cond_destroy(p_condition_variable) == 0 );
    #endif

    // error handling
    {
        
        // Argument errors
        {
            no_condition_variable:
                #ifndef NDEBUG
                    log_error("[sync] Null pointer provided for \"p_condition_variable\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
    }
}

int monitor_create ( monitor *p_monitor )
{
    // Argument check
    if ( p_monitor == (void *) 0 ) goto no_monitor;
    
    // Platform dependent implementation
    #ifdef _WIN64
        // TODO
        //

    #else

        int ret = ( pthread_cond_init(&p_monitor->_cond, NULL) == 0 );
        ret &= ( pthread_mutex_init(&p_monitor->_mutex, NULL) == 0 );

        // Return
        return  ret;
    #endif

    // error handling
    {
        
        // Argument errors
        {
            no_monitor:
                #ifndef NDEBUG
                    log_error("[sync] Null pointer provided for \"p_monitor\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
    }
}

int monitor_wait ( monitor *p_monitor )
{

    // Platform dependent implementation
    #ifdef _WIN64
        
        // TODO
    #else

        // Initialized data
        int ret = 0;

        // lock
        mutex_lock(&p_monitor->_mutex);

        // Wait
        if ( pthread_cond_wait(&p_monitor->_cond, &p_monitor->_mutex) == 0 ) ret = 1;

        // unlock
        mutex_unlock(&p_monitor->_mutex);

        // Return
        return ret;
    #endif
}

int monitor_notify ( monitor *p_monitor )
{

    // Platform dependent implementation
    #ifdef _WIN64
        
        // TODO
    #else

        // Return
        return pthread_cond_signal(&p_monitor->_cond);
    #endif
}

int monitor_notify_all ( monitor *p_monitor )
{

    // Platform dependent implementation
    #ifdef _WIN64
        
        // TODO
    #else

        // Return
        return pthread_cond_broadcast(&p_monitor->_cond);
    #endif
}

int monitor_destroy ( monitor *p_monitor )
{
    
    // Argument check
    if ( p_monitor == (void *) 0 ) goto no_monitor;

    // Platform dependent implementation
    #ifdef _WIN64
        // TODO
        //

    #else

        // Return
        return ( pthread_cond_destroy(&p_monitor->_cond) == 0 ) && ( pthread_mutex_destroy(&p_monitor->_mutex) == 0 );
    #endif

    // error handling
    {
        
        // Argument errors
        {
            no_monitor:
                #ifndef NDEBUG
                    log_error("[sync] Null pointer provided for \"p_monitor\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
    }
}
/*
int barrier_create ( barrier *p_barrier, unsigned int count )
{

    // Argument check
    if ( p_barrier == (void *) 0 ) goto no_barrier;
    if ( count     ==          0 ) goto no_count;

    #ifdef _WIN64

    #elif defined __APPLE__
        return 0;
    #else
        // Construct
        pthread_barrier_init(p_barrier, NULL, count);
    #endif
    
    // success
    return 1;

    // error handling
    {
        
        // Argument errors
        {
            no_barrier:
                #ifndef NDEBUG
                    log_error("[sync] [barrier] Null pointer provided for parameter \"p_barrier\" in call to function \"%s\"\n");
                #endif

                // error
                return 0;
            
            no_count:
                #ifndef NDEBUG
                    log_error("[sync] [barrier] Parameter \"count\" must be greater than zero in call to function \"%s\"\n");
                #endif

                // error
                return 0;                
        }
    }
}

int barrier_wait ( barrier *p_barrier )
{

    // Argument check
    if ( p_barrier == (void *) 0 ) goto no_barrier;
    #ifdef _WIN64
    #elif defined __APPLE__
        return 0;
    #else
        // Wait
        pthread_barrier_wait(p_barrier);
    #endif


    // success
    return 1;

    // error handling
    {
        
        // Argument errors
        {
            no_barrier:
                #ifndef NDEBUG
                    log_error("[sync] [barrier] Null pointer provided for parameter \"p_barrier\" in call to function \"%s\"\n");
                #endif

                // error
                return 0;
        }
    }
}

int barrier_destroy ( barrier *p_barrier )
{

    // Argument check
    if ( p_barrier == (void *) 0 ) goto no_barrier;

    #ifdef _WIN64
    #elif defined __APPLE__
        return 0;
    #else
        // Destroy
        pthread_barrier_destroy(p_barrier);
    #endif


    // success
    return 1;

    // error handling
    {
        
        // Argument errors
        {
            no_barrier:
                #ifndef NDEBUG
                    log_error("[sync] [barrier] Null pointer provided for parameter \"p_barrier\" in call to function \"%s\"\n");
                #endif

                // error
                return 0;
        }
    }
}
*/

timestamp timer_high_precision ( void )
{
    
    // Initialized data
    timestamp ret = 0;

    // Platform dependent implementation
    #ifdef _WIN64

        // Query the performance counter
        QueryPerformanceCounter((LARGE_INTEGER *)&ret);
    #else

        // Initialized data
        struct timespec ts;

        // Populate the time struct using the monotonic timer
        clock_gettime(CLOCK_MONOTONIC, &ts);

        // Compute the monotonic time in nanoseconds
        ret = ( (signed)ts.tv_sec * SEC_2_NS ) + ( (signed) ts.tv_nsec );
    #endif

    // error
    return ret;
}

signed timer_seconds_divisor ( void )
{

    // Done
    return SYNC_TIMER_DIVISOR;
}

void sync_exit ( void ) 
{

    // State check
    if ( initialized == false ) return;

    // Clean up log
    log_exit();

    // TODO: Anything else?
    // 

    // Clear the initialized flag
    initialized = false;

    // Done
    return;
}
