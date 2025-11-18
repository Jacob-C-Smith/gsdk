/** !
 * High level abstraction of a schedule
 * 
 * @file schedule.c
 *
 * @author Jacob Smith
 */

// header
#include <performance/schedule.h>

// preprocessor definitions
#define PARALLEL_SCHEDULE_NAME_LENGTH        (63 + 1)
#define PARALLEL_SCHEDULE_THREAD_NAME_LENGTH (63 + 1)
#define PARALLEL_SCHEDULE_TASK_NAME_LENGTH   (63 + 1)
#define PARALLEL_SCHEDULE_MAX_THREADS        64
#define PARALLEL_SCHEDULE_MAX_TASKS          256

// forward declarations
struct parallel_schedule_thread_s;
struct parallel_schedule_task_s;
struct parallel_schedule_work_parameter_s;

// type definitions
typedef struct parallel_schedule_thread_s         parallel_schedule_thread;
typedef struct parallel_schedule_task_s           parallel_schedule_task;
typedef struct parallel_schedule_work_parameter_s parallel_schedule_work_parameter;

// structure definitions
struct parallel_schedule_task_s
{
    bool dependent, dependency, ready;
    int dependencies;
    fn_parallel_task *pfn_task;
    monitor _monitor;
    char _name [PARALLEL_SCHEDULE_TASK_NAME_LENGTH],
         _wait_thread [PARALLEL_SCHEDULE_THREAD_NAME_LENGTH],
         _wait_task [PARALLEL_SCHEDULE_TASK_NAME_LENGTH];
};

struct parallel_schedule_thread_s
{
    size_t task_quantity;
    bool running;
    parallel_thread *p_parallel_thread;
    char _name [PARALLEL_SCHEDULE_THREAD_NAME_LENGTH];
    parallel_schedule_task tasks [];
};

struct parallel_schedule_work_parameter_s
{
    schedule *p_schedule;
    parallel_schedule_thread *p_thread;
};

struct schedule_s
{
    monitor _montior;
    mutex _lock;
    size_t running_threads;
    dict *p_threads;
    bool repeat;
    void *p_parameter;
    char  _name [PARALLEL_SCHEDULE_NAME_LENGTH];
    char  _main_thread_name [PARALLEL_SCHEDULE_THREAD_NAME_LENGTH];
    parallel_schedule_work_parameter _work_parameters[PARALLEL_SCHEDULE_MAX_THREADS];
};

// function declarations
/** !
 * Allocate memory for a scheudle thread
 * 
 * @param pp_schedule_thread return
 * 
 * @sa schedule_destroy
 * 
 * @return 1 on success, 0 on error
*/
int parallel_schedule_thread_create ( parallel_schedule_thread **pp_schedule_thread, size_t task_quantity );

/** !
 * Construct a named schedule from a json value
 * 
 * @param pp_schedule return
 * @param name        the name
 * @param value       the json value
 * 
 * @return 1 on success, 0 on error
 */
int schedule_load_as_json_value ( schedule **const pp_schedule, const json_value *const p_value );

/** !
 * Construct a named thread from a json value
 * 
 * @param pp_thread return
 * @param name      the name
 * @param value     the json value
 * 
 * @return 1 on success, 0 on error
 */
int parallel_schedule_thread_load_as_json_value ( parallel_schedule_thread **const pp_thread, const char *const name, const json_value *const p_value );

/** !
 * Worker thread loop
 * 
 * @param p_parameter who am I?
 * 
 * @return ret
 */
void *parallel_schedule_work ( parallel_schedule_work_parameter *p_parameter );

/** !
 * Main thread loop
 * 
 * @param p_parameter who am I?
 * 
 * @return ret
 */
void *parallel_schedule_main_work ( parallel_schedule_work_parameter *p_parameter );

// TODO: Document
int parallel_schedule_thread_destroy ( parallel_schedule_thread **pp_thread );

/**!
 * Return the size of a file IF buffer == 0 ELSE read a file into buffer
 * 
 * @param path path to the file
 * @param buffer buffer
 * @param binary_mode "wb" IF true ELSE "w"
 * 
 * @return 1 on success, 0 on error
 */
size_t load_file ( const char *path, void *buffer, bool binary_mode );

// function definitions
int schedule_create ( schedule **const pp_schedule )
{

    // argument check
    if ( pp_schedule == (void *) 0 ) goto no_schedule;

    // initialized data
    schedule *p_schedule = (void *) 0;

    // allocate memory for the schedule
    p_schedule = default_allocator(0, sizeof(schedule));

    // error check
    if ( p_schedule == (void *) 0 ) goto no_mem;

    // Zero set memory
    memset(p_schedule, 0, sizeof(schedule));

    // return a pointer to the caller
    *pp_schedule = p_schedule;

    // success
    return 1;

    // error handling
    {

        // argument errors
        {
            no_schedule: 
                #ifndef NDEBUG
                    log_error("[parallel] [schedule] Null pointer provided for parameter \"pp_schedule\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
        
        // standard library errors
        {
            no_mem:
                #ifndef NDEBUG
                    log_error("[standard library] Failed to allocate memory in call to function \"%s\"\n", __FUNCTION__);
                #endif
                
                // error
                return 0;
        }
    }
}

int parallel_schedule_thread_create ( parallel_schedule_thread **const pp_schedule_thread, size_t task_quantity )
{

    // argument check
    if ( pp_schedule_thread == (void *) 0 ) goto no_schedule_thread;

    // initialized data
    parallel_schedule_thread *p_schedule_thread = (void *) 0;

    // allocate memory for the schedule thread
    p_schedule_thread = default_allocator(0, sizeof(parallel_schedule_thread) + (task_quantity * sizeof(parallel_schedule_task)));

    // error check
    if ( p_schedule_thread == (void *) 0 ) goto no_mem;

    // Zero set memory
    memset(p_schedule_thread, 0, sizeof(parallel_schedule_thread));

    // store the task quantity
    p_schedule_thread->task_quantity = task_quantity;

    // return a pointer to the caller
    *pp_schedule_thread = p_schedule_thread;

    // success
    return 1;

    // error handling
    {

        // argument errors
        {
            no_schedule_thread: 
                #ifndef NDEBUG
                    log_error("[parallel] [schedule] [thread] Null pointer provided for parameter \"pp_schedule_thread\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
        
        // standard library errors
        {
            no_mem:
                #ifndef NDEBUG
                    log_error("[standard library] Failed to allocate memory in call to function \"%s\"\n", __FUNCTION__);
                #endif
                
                // error
                return 0;
        }
    }
}

int schedule_load ( schedule **pp_schedule, const char *const path )
{

    // argument check
    if ( pp_schedule == (void *) 0 ) goto no_schedule;
    if ( path        == (void *) 0 ) goto no_path;

    // initialized data
    size_t      file_size       = 0;
    char       *p_file_contents = (void *) 0;
    json_value *p_value         = (void *) 0;
    
    // Query the size of the file
    file_size = load_file(path, 0, true);

    // error check
    if ( file_size == 0 ) goto invalid_file;
    
    // allocate memory for the file
    p_file_contents = default_allocator(0, file_size);

    // error check
    if ( p_file_contents == (void *) 0 ) goto no_mem;

    // Load the file
    load_file(path, p_file_contents, 0);

    // Parse the file into a json value
    if ( json_value_parse(p_file_contents, 0, &p_value) == 0 ) goto failed_to_parse_json_value;

    // Construct a schedule
    if ( schedule_load_as_json_value(pp_schedule, p_value) == 0 ) goto failed_to_construct_schedule;

    // Clean up
    p_file_contents = default_allocator(p_file_contents, 0);
    
    // success
    return 1;

    // error handling
    {

        // argument errors
        {
            no_schedule: 
                #ifndef NDEBUG
                    log_error("[parallel] [schedule] Null pointer provided for parameter \"pp_schedule\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;

            no_path:
                #ifndef NDEBUG
                    log_error("[parallel] [schedule] Null pointer provided for parameter \"path\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
        
        // standard library errors
        {
            no_mem:
                #ifndef NDEBUG
                    log_error("[standard library] Failed to allocate memory in call to function \"%s\"\n", __FUNCTION__);
                #endif
                
                // error
                return 0;
        }
        
        // File errors
        {
            invalid_file:
                #ifndef NDEBUG
                    log_error("[parallel] [schedule] Can not load test file in call to function \"%s\"\n", __FUNCTION__);
                #endif
                
                // error
                return 0;
        }
        
        // JSON errors
        {
            failed_to_parse_json_value:
                #ifndef NDEBUG
                    log_error("[parallel] [schedule] Failed to parse JSON text in call to function \"%s\"\n", __FUNCTION__);
                #endif
                
                // error
                return 0;
        }

        // Parallel errors
        {
            failed_to_construct_schedule:
                #ifndef NDEBUG
                    log_error("[parallel] [schedule] Failed to construct schedule in call to function \"%s\"\n", __FUNCTION__);
                #endif
                
                // error
                return 0;
        }
    }
}

int schedule_load_as_json_value ( schedule **const pp_schedule, const json_value *const p_value )
{

    // argument check
    if ( pp_schedule   ==        (void *) 0 ) goto no_schedule;
    if ( p_value       ==        (void *) 0 ) goto no_value;
    if ( p_value->type != JSON_VALUE_OBJECT ) goto wrong_type;

    // initialized data
    dict *p_dict = p_value->object;
    const json_value *const p_name        = dict_get(p_dict, "name"),
                     *const p_threads     = dict_get(p_dict, "threads"),
                     *const p_main_thread = dict_get(p_dict, "main thread"),
                     *const p_repeat      = dict_get(p_dict, "repeat");
    schedule  _schedule  = { 0 }, 
                      *p_schedule = (void *) 0;

    // Check for missing properties
    if ( ! ( p_name && p_threads ) ) goto missing_properties;

    // Parse the name property
    if ( p_name->type == JSON_VALUE_STRING )
    {

        // initialized data
        char   *p_name_string = p_name->string;
        size_t  len           = strlen(p_name_string);

        // error check
        if ( len < 1 ) goto name_property_too_short;
        if ( len > PARALLEL_SCHEDULE_NAME_LENGTH ) goto name_property_too_long;
        
        // Copy the name into the schedule struct
        strncpy(_schedule._name, p_name_string, len);

        // Null terminate the copied string
        _schedule._name[len] = '\0';
    } 
    
    // error check
    if ( p_main_thread == (void *) 0 ) goto no_main_thread; 

    // Parse the main thread property
    if ( p_main_thread->type == JSON_VALUE_STRING )
    {

        // initialized data
        char   *p_main_thread_string = p_main_thread->string;
        size_t  len                  = strlen(p_main_thread_string);
        
        // error check
        if ( len < 1 ) goto main_thread_property_too_short;
        if ( len > PARALLEL_SCHEDULE_THREAD_NAME_LENGTH ) goto main_thread_property_too_long;
        
        // Copy the name into the schedule struct
        strncpy(_schedule._main_thread_name, p_main_thread_string, len);

        // Null terminate the copied string
        _schedule._main_thread_name[len] = '\0';
    } 

    // Default
    else goto wrong_main_thread_type;
    
    no_main_thread:
    // Parse the threads property
    if ( p_threads->type == JSON_VALUE_OBJECT )
    {

        // initialized data
        dict       *p_dict           = p_threads->object;
        size_t      threads_quantity = dict_keys(p_dict, (void *) 0 );
        const char *_keys[PARALLEL_SCHEDULE_MAX_THREADS]     = { 0 };
        json_value *_p_values[PARALLEL_SCHEDULE_MAX_THREADS] = { 0 };
        
        // error check
        if ( threads_quantity == 0 ) goto threads_property_is_empty;
        if ( threads_quantity > PARALLEL_SCHEDULE_MAX_THREADS ) goto threads_property_is_too_large;

        // store the names of the threads
        dict_keys(p_dict, _keys);

        // store the values of the threads
        dict_values(p_dict, (void **)_p_values);

        // Construct a dictionary for the threads
        (void) dict_construct(&_schedule.p_threads, threads_quantity * 2, 0);

        // iterate over each thread
        for (size_t i = 0; i < threads_quantity; i++)
        {

            // initialized data
            parallel_schedule_thread *p_thread = (void *) 0;

            // Construct a schedule thread
            if ( parallel_schedule_thread_load_as_json_value(&p_thread, _keys[i], _p_values[i]) == 0 ) goto failed_to_create_thread;

            // Add the schedule thread to the schedule
            dict_add(_schedule.p_threads, p_thread->_name, p_thread);
        }
    }

    // Default
    else goto wrong_threads_type;

    // Jump ahead
    if ( p_repeat == (void *) 0 ) goto no_repeat_property;

    // Parse the repeat property
    if ( p_repeat->type == JSON_VALUE_BOOLEAN ) 

        // store the repeat property
        _schedule.repeat = p_repeat->boolean;

    // Default
    else goto wrong_repeat_type;

    no_repeat_property:

    // Validate the schedule
    {

        // initialized data
        size_t thread_quantity = dict_values(_schedule.p_threads, 0);
        parallel_schedule_thread *_p_threads [PARALLEL_SCHEDULE_MAX_THREADS] = { 0 };
        parallel_schedule_thread *p_thread = (void *) 0;
        parallel_schedule_task   *p_task = (void *) 0;
        
        // store the threads from the schedule
        dict_values(_schedule.p_threads, (void **)_p_threads);

        // iterate over each thread
        for (size_t i = 0; i < thread_quantity; i++)
        {
            
            // store the thread
            p_thread = _p_threads[i];

            // iterate through each task
            for (size_t j = 0; j < p_thread->task_quantity; j++)
            {
                
                // store the task
                p_task = &p_thread->tasks[j];

                // Will this task wait for something else?
                if ( p_task->dependent )
                {
                    
                    // initialized data
                    parallel_schedule_thread *p_dependency_thread = (parallel_schedule_thread *) dict_get(_schedule.p_threads, p_task->_wait_thread);

                    // TODO: Error check
                    //

                    // iterate through each task
                    for (size_t k = 0; k < p_dependency_thread->task_quantity; k++)
                    {
                        
                        // initialized data
                        parallel_schedule_task *i_dependency_task = &p_dependency_thread->tasks[k];
                        struct { char *th, *ta; } thread_task[64] = { 0 };

                        // Will the task wait on this task?
                        i_dependency_task->dependency = ( strcmp(i_dependency_task->_name, p_task->_wait_task) == 0 );

                        if ( i_dependency_task->dependency )
                        {
                            thread_task[i].ta = p_thread->_name,
                            thread_task[i].th = p_task->_name;
                            i_dependency_task->dependency = true;
                            i_dependency_task->dependencies++;
                        }
                    }
                }
            }
        }
    
        // iterate over each thread
        for (size_t i = 0; i < thread_quantity; i++)
        {
            
            // store the thread
            p_thread = _p_threads[i];

            // iterate through each task
            for (size_t j = 0; j < p_thread->task_quantity; j++)
                
                // store the task
                p_task = &p_thread->tasks[j];            
        }
    }

    // allocate memory for a schedule
    if ( schedule_create(&p_schedule) == 0 ) goto failed_to_create_schedule;

    // Copy the schedule from the stack to the heap
    memcpy(p_schedule, &_schedule, sizeof(schedule));

    // Construct a monitor for the schedule
    monitor_create(&p_schedule->_montior);

    // Construct a mutex for the schedule
    mutex_create(&p_schedule->_lock);

    // return a pointer to the caller
    *pp_schedule = p_schedule;

    // success
    return 1;

    // error handling
    {

        // argument errors
        {
            no_schedule: 
                #ifndef NDEBUG
                    log_error("[parallel] [schedule] Null pointer provided for parameter \"pp_schedule\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;

            no_value: 
                #ifndef NDEBUG
                    log_error("[parallel] [schedule] Null pointer provided for parameter \"p_value\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }

        // JSON errors
        {
            wrong_type:
                #ifndef NDEBUG
                    log_error("[parallel] [schedule] Parameter \"p_value\" must be of type [ object ] in call to function \"%s\"\n\"Refer to schedule schema: [TODO: Schedule schema URL] \n", __FUNCTION__);
                #endif

                // error
                return 0;
            
            missing_properties:
                #ifndef NDEBUG
                    log_error("[parallel] [schedule] Not enough properties to construct schedule in call to function \"%s\"\n\"Refer to schedule schema: [TODO: Schedule schema URL] \n", __FUNCTION__);
                #endif

                // error
                return 0;
            
            wrong_threads_type:
                #ifndef NDEBUG
                    log_error("[parallel] [schedule] \"threads\" property of schedule object must be of type [ object ] in call to function \"%s\"\n\"Refer to schedule schema: [TODO: Schedule schema URL] \n", __FUNCTION__);
                #endif

                // error
                return 0;

            wrong_main_thread_type:
                #ifndef NDEBUG
                    log_error("[parallel] [schedule] \"main thread\" property of schedule object must be of type [ object ] in call to function \"%s\"\n\"Refer to schedule schema: [TODO: Schedule schema URL] \n", __FUNCTION__);
                #endif

                // error
                return 0;

            wrong_repeat_type:
                #ifndef NDEBUG
                    log_error("[parallel] [schedule] \"repeat\" property of schedule object must be of type [ boolean ] in call to function \"%s\"\n\"Refer to schedule schema: [TODO: Schedule schema URL] \n", __FUNCTION__);
                #endif

                // error
                return 0;

            name_property_too_long:
                #ifndef NDEBUG
                    log_error("[parallel] [schedule] \"name\" property of schedule object must be less than %d characters in call to function \"%s\"\n\"Refer to schedule schema: [TODO: Schedule schema URL] \n", PARALLEL_SCHEDULE_NAME_LENGTH, __FUNCTION__);
                #endif

                // error
                return 0;

            name_property_too_short:
                #ifndef NDEBUG
                    log_error("[parallel] [schedule] \"name\" property of schedule object must be at least 1 character long in call to function \"%s\"\n\"Refer to schedule schema: [TODO: Schedule schema URL] \n", __FUNCTION__);
                #endif

                // error
                return 0;
            
            main_thread_property_too_short:
                #ifndef NDEBUG
                    log_error("[parallel] [schedule] \"main thread\" property of schedule object must be at least 1 character long in call to function \"%s\"\n\"Refer to schedule schema: [TODO: Schedule schema URL] \n", __FUNCTION__);
                #endif

                // error
                return 0;

            main_thread_property_too_long:
                #ifndef NDEBUG
                    log_error("[parallel] [schedule] \"main thread\" property of schedule object must be less than %d characters in call to function \"%s\"\n\"Refer to schedule schema: [TODO: Schedule schema URL] \n", PARALLEL_SCHEDULE_THREAD_NAME_LENGTH, __FUNCTION__);
                #endif

                // error
                return 0;

            threads_property_is_empty:
                #ifndef NDEBUG
                    log_error("[parallel] [schedule] \"threads\" property of schedule object must contain at least 1 property in call to function \"%s\"\n\"Refer to schedule schema: [TODO: Schedule schema URL] \n", __FUNCTION__);
                #endif

                // error
                return 0;
            
            threads_property_is_too_large:
                #ifndef NDEBUG
                    log_error("[parallel] [schedule] \"threads\" property of schedule object must contain %d or fewer properties in call to function \"%s\"\n\"Refer to schedule schema: [TODO: Schedule schema URL] \n", PARALLEL_SCHEDULE_MAX_THREADS, __FUNCTION__);
                #endif

                // error
                return 0;
        }
    
        // Parallel errors
        {

            failed_to_create_schedule:
                #ifndef NDEBUG
                    log_error("[parallel] [schedule] Failed to allocate schedule in call to functon \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;

            failed_to_create_thread:
                #ifndef NDEBUG
                    log_error("[parallel] [schedule] Failed to create scheduler thread in call to functon \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
    }
}

int parallel_schedule_thread_load_as_json_value ( parallel_schedule_thread **const pp_schedule_thread, const char *const name, const json_value *const p_value )
{

    // argument check
    if ( pp_schedule_thread ==       (void *) 0 ) goto no_thread;
    if ( name               ==       (void *) 0 ) goto no_name;
    if ( p_value            ==       (void *) 0 ) goto no_value;
    if ( p_value->type      != JSON_VALUE_ARRAY ) goto wrong_type;

    // initialized data
    parallel_schedule_thread *p_schedule_thread = (void *) 0;
    array *p_array = p_value->list;
    size_t task_quantity = array_size(p_array),
           thread_name_len = strlen(name);
    char *error_state = (void *) 0;

    // allocate memory for a schedule thread
    if ( parallel_schedule_thread_create(&p_schedule_thread, task_quantity) == 0 ) goto failed_to_allocate_schedule_thread;

    // Copy the name of the thread
    strncpy(p_schedule_thread->_name, name, thread_name_len);

    // iterate through the array
    for (size_t i = 0; i < task_quantity; i++)
    {
        
        // initialized data
        const json_value *p_ith_value = (void *) 0;
        
        // store the ith json value
        (void) array_index(p_array, i, (void **)&p_ith_value);

        // error check
        if ( p_ith_value->type == JSON_VALUE_OBJECT ) 
        {

            // initialized data
            dict *p_dict = p_ith_value->object;
            const json_value *const p_task = dict_get(p_dict, "task"),
                             *const p_wait = dict_get(p_dict, "wait");
            
            // Check for missing properties
            if ( p_task == (void *) 0 ) goto missing_properties;

            // Parse the task property
            if ( p_task->type == JSON_VALUE_STRING )
            {
                
                // initialized data
                size_t len = 0;
                fn_parallel_task *pfn_task = 0;
                
                // Find the task from the registery
                parallel_find_task(p_task->string, &pfn_task);

                // error check
                if ( pfn_task == 0 )
                {

                    // Update the error state
                    error_state = p_task->string;

                    // Handle the error
                    goto unrecognized_task;
                }

                // store the length of the task name
                len = strlen(p_task->string);

                // TODO: Bounds check
                //

                // Copy the task name
                strncpy(p_schedule_thread->tasks[i]._name, p_task->string, len);

                // store the task function pointer
                p_schedule_thread->tasks[i].pfn_task = pfn_task;
            }

            // Default
            else goto wrong_task_task_type;

            // Set the dependent flag
            p_schedule_thread->tasks[i].dependent = (bool) p_wait;

            // No wait property means nothing to do
            if ( p_wait == (void *) 0 ) continue;

            // Parse the wait property
            if ( p_wait->type == JSON_VALUE_STRING )
            {

                // initialized data
                char *wait_thread      = p_wait->string, 
                     *wait_task        = strchr(wait_thread, ':');
                size_t wait_thread_len = 0,
                       wait_task_len   = 0;

                // error check
                if ( wait_task == (void *) 0 ) goto no_colon_delimiter;

                // Compute the length of the wait thread
                wait_thread_len = (wait_task - wait_thread);

                // Increment past the ':'
                wait_task++;

                // store the length of the wait task
                wait_task_len = strlen(wait_task);

                // error check
                if ( wait_task_len   > PARALLEL_SCHEDULE_TASK_NAME_LENGTH - 1 ) goto wait_task_too_long;
                if ( wait_task_len   < 1 ) goto wait_task_too_short;
                if ( wait_thread_len > PARALLEL_SCHEDULE_THREAD_NAME_LENGTH - 1 ) goto wait_thread_too_long;
                if ( wait_thread_len < 1 ) goto wait_thread_too_short;

                // Copy the wait thread
                strncpy(p_schedule_thread->tasks[i]._wait_thread, wait_thread, wait_thread_len);

                // Copy the wait task
                strncpy(p_schedule_thread->tasks[i]._wait_task, wait_task, wait_task_len);
            }

            // Default 
            else goto wrong_task_wait_type;
        }

        // Default
        else goto wrong_task_type;
    }
    
    // return a pointer to the caller
    *pp_schedule_thread = p_schedule_thread;

    // success
    return 1;

    // JSON errors
    missing_properties:
    wrong_task_type:
    wrong_task_task_type:
    wrong_task_wait_type:
    no_colon_delimiter:
    wait_task_too_long:
    wait_task_too_short:
    wait_thread_too_long:
    wait_thread_too_short:

        // error
        return 0;


    // error handling
    {

        // argument errors
        {
            no_thread:
                #ifndef NDEBUG
                    log_error("[parallel] [schedule] Null pointer provided for parameter \"pp_schedule_thread\" in call to function \"%s\"\n", __FUNCTION__);
                #endif
                
                // error
                return 0;

            no_name:
                #ifndef NDEBUG
                    log_error("[parallel] [schedule] Null pointer provided for parameter \"name\" in call to function \"%s\"\n", __FUNCTION__);
                #endif
                
                // error
                return 0;

            no_value:
                #ifndef NDEBUG
                    log_error("[parallel] [schedule] Null pointer provided for parameter \"p_value\" in call to function \"%s\"\n", __FUNCTION__);
                #endif
                
                // error
                return 0;
        }

        // JSON errors
        {

            wrong_type:
                #ifndef NDEBUG
                    log_error("[parallel] [schedule] Parameter \"p_value\" must be of type [ array ] in call to function \"%s\"\n", __FUNCTION__);
                #endif
                
                // error
                return 0;
        }

        // Parallel errors
        {
            unrecognized_task:
                #ifndef NDEBUG
                    log_error("[parallel] [schedule] Unrecognized task \"%s\" was encountered while constructing therad \"%s\" in call to function \"%s\"\n", error_state, name, __FUNCTION__);
                #endif

                // error
                return 0;
            
            failed_to_allocate_schedule_thread:
                #ifndef NDEBUG
                    log_error("[parallel] [schedule] Failed to allocate scheduler thread in call to function \"%s\"\n", error_state, name, __FUNCTION__);
                #endif

                // error
                return 0;
        }
    }   
}

int schedule_start ( schedule *const p_schedule, void *const p_parameter )
{

    // argument check
    if ( p_schedule == (void *) 0 ) goto no_schedule;

    // initialized data
    size_t thread_quantity = dict_values(p_schedule->p_threads, 0);
    parallel_schedule_thread *_p_threads [PARALLEL_SCHEDULE_MAX_THREADS] = { 0 };
    parallel_schedule_thread *p_main_thread = (void *)0;
    parallel_schedule_work_parameter *p_main_thread_work_parameter = (void *) 0;

    // store the parameter
    p_schedule->p_parameter = p_parameter;

    // store the threads from the schedule
    dict_values(p_schedule->p_threads, (void **)_p_threads);

    // iterate over each thread
    for (size_t i = 0; i < thread_quantity; i++)
    {
        
        // initialized data
        parallel_schedule_thread *p_thread = _p_threads[i];

        // store the thread parameter
        p_schedule->_work_parameters[i] = (parallel_schedule_work_parameter)
        {
            .p_schedule = p_schedule,
            .p_thread   = p_thread
        };

        // Skip main thread
        if ( strcmp(p_schedule->_main_thread_name, p_thread->_name) == 0 )
        {

            // store the main thread
            p_main_thread = p_thread;

            // store the work parameter
            p_main_thread_work_parameter = &p_schedule->_work_parameters[i];

            // done 
            continue;
        }
        
        // Spawn the thread
        if ( parallel_thread_start(&p_thread->p_parallel_thread, (fn_parallel_task *) parallel_schedule_work, &p_schedule->_work_parameters[i]) == 0 ) goto failed_to_create_thread;
    }
    
    // Start the main thread
    if ( p_main_thread )
    {

        // Wait for all the threads to start        
        while ( p_schedule->running_threads != thread_quantity - 1 )
            sleep(0);

        // Start the main thread
        parallel_schedule_main_work(p_main_thread_work_parameter);
    }
    
    // Start the worker threads
    else 
    {

        // Wait for all the threads to start        
        while ( p_schedule->running_threads != thread_quantity )
            sleep(0);

        // Start the worker threads
        monitor_notify_all(&p_schedule->_montior);
    }

    // success
    return 1;

    // error handling
    {

        // argument errors
        {
            no_schedule:
                #ifndef NDEBUG
                    log_error("[parallel] [schedule] Null pointer provided for parameter \"p_schedule\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }

        // Parallel errors
        {
            failed_to_create_thread:
                #ifndef NDEBUG
                    log_error("[parallel] [schedule] Failed to spawn thread in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
    }
}

int schedule_wait_idle ( schedule *const p_schedule )
{
    
    // Spin until all threads are done
    while (p_schedule->running_threads) sleep(0);

    // success
    return 1;
}

int schedule_pause ( schedule *const p_schedule )
{

    // Clear the repeat flag
    p_schedule->repeat = false;

    // success
    return 1;
} 

int schedule_stop ( schedule *const p_schedule )
{

    // argument check
    if ( p_schedule == (void *) 0 ) goto no_schedule;

    // initialized data
    size_t thread_quantity = dict_values(p_schedule->p_threads, 0);
    parallel_schedule_thread *_p_threads[PARALLEL_SCHEDULE_MAX_THREADS] = { 0 };

    // Clear the repeat flag
    p_schedule->repeat = false;

    // store the threads from the schedule
    dict_values(p_schedule->p_threads, (void **)_p_threads);

    // iterate over each thread
    for (size_t i = 0; i < thread_quantity; i++)
    {
        
        // initialized data
        parallel_schedule_thread *p_thread = _p_threads[i];

        // Skip the main thread
        if ( strcmp(p_schedule->_main_thread_name, p_thread->_name) == 0 ) continue;

        // Clear the running flag
        p_thread->running = false;

        // Cancel the thread
        parallel_thread_cancel(p_thread->p_parallel_thread);

        // Join the thread
        if ( parallel_thread_join(&p_thread->p_parallel_thread) == 0 ) goto failed_to_destroy_thread;
    }
    
    // success
    return 1;

    // error handling
    {

        // argument errors
        {
            no_schedule:
                #ifndef NDEBUG
                    log_error("[parallel] [schedule] Null pointer provided for parameter \"p_schedule\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }

        // Parallel errors
        {
            failed_to_destroy_thread:
                #ifndef NDEBUG
                    log_error("[parallel] [schedule] Failed to join thread in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
    }
}

int schedule_destroy ( schedule **const pp_schedule )
{

    // TODO
    (void)pp_schedule;

    // success
    return 1;
}

void *parallel_schedule_work ( parallel_schedule_work_parameter *p_parameter )
{

    // argument check
    if ( p_parameter == (void *) 0 ) goto no_work_parameter;

    // initialized data
    schedule        *p_schedule        = p_parameter->p_schedule;
    parallel_schedule_thread *p_schedule_thread = p_parameter->p_thread;
    parallel_schedule_task   *i_task            = (void *) 0;
    
    // lock
    mutex_lock(&p_schedule->_lock);

    // Set the running flag
    p_schedule_thread->running = true;

    // Increment the quantity of running threads
    p_schedule->running_threads++;

    // unlock
    mutex_unlock(&p_schedule->_lock);

    // Wait for the signal
    monitor_wait(&p_schedule->_montior);

    turnover:

    // iterate through each task
    for (size_t i = 0; i < p_schedule_thread->task_quantity; i++)
    {

        // initialized data
        i_task = &p_schedule_thread->tasks[i];

        // Wait
        if ( i_task->dependent ) goto wait_logic;

        // done waiting
        done:

        // Run the task
        i_task->pfn_task(p_parameter->p_schedule->p_parameter);
        
        // Signal
        if ( i_task->dependency ) monitor_notify_all(&i_task->_monitor);
    }
    
    // Repeat?
    if ( p_schedule->repeat ) goto turnover;

    // lock
    mutex_lock(&p_schedule->_lock);

    // Clear the running flag
    p_schedule_thread->running = false;

    // Decrement the quantity of running threads
    p_schedule->running_threads--;

    // unlock
    mutex_unlock(&p_schedule->_lock);

    // success
    return (void *) 1;

    wait_logic:
    {

        // initialized data
        char *thread_name = i_task->_wait_thread;
        parallel_schedule_thread *p_schedule_thread = (parallel_schedule_thread *) dict_get(p_schedule->p_threads, thread_name);
        
        // Find the monitor 
        for (size_t i = 0; i < p_schedule_thread->task_quantity; i++)
        {
        
            // Check the monitor
            if ( strcmp(p_schedule_thread->tasks[i]._name, i_task->_wait_task) == 0 ) 
            {

                // Wait
                monitor_wait(&p_schedule_thread->tasks[i]._monitor);

                // done
                goto done;
            }
        }

        // Default
        goto failed_to_find_monitor;
    }

    // error handling
    {

        // argument errors
        {
            no_work_parameter:
                #ifndef NDEBUG
                    log_error("[parallel] [schedule] Null pointer provided for parameter \"p_parameter\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }

        // Parallel errors
        {
            failed_to_find_monitor:
                #ifndef NDEBUG
                    log_error("[parallel] [schedule] Failed to find monitor for dependant task on thread\"%s\" in call to function \"%s\"\n", p_schedule_thread->_name, __FUNCTION__);
                #endif

                // error
                return 0;
        }
    }
}

void *parallel_schedule_main_work ( parallel_schedule_work_parameter *p_parameter )
{

    // argument check
    if ( p_parameter == (void *) 0 ) goto no_work_parameter;

    // initialized data
    schedule        *p_schedule        = p_parameter->p_schedule;
    parallel_schedule_thread *p_schedule_thread = p_parameter->p_thread;
    parallel_schedule_task   *i_task            = (void *) 0;
    
    // lock
    mutex_lock(&p_schedule->_lock);

    // Set the running flag
    p_schedule_thread->running = true;

    // Increment the quantity of running threads
    p_schedule->running_threads++;

    // unlock
    mutex_unlock(&p_schedule->_lock);

    // Start the work
    monitor_notify_all(&p_schedule->_montior);
    
    turnover:

    // iterate through each task
    for (size_t i = 0; i < p_schedule_thread->task_quantity; i++)
    {

        // initialized data
        i_task = &p_schedule_thread->tasks[i];

        // Wait
        if ( i_task->dependent ) goto wait_logic;

        // done waiting
        done:

        // Run the task
        i_task->pfn_task(p_parameter->p_schedule->p_parameter);
        
        // Signal
        if ( i_task->dependency ) monitor_notify_all(&i_task->_monitor);
    }
    
    // Repeat?
    if ( p_schedule->repeat ) goto turnover;

    // lock
    mutex_lock(&p_schedule->_lock);

    // Clear the running flag
    p_schedule_thread->running = false;

    // Decrement the quantity of running threads
    p_schedule->running_threads--;

    // unlock
    mutex_unlock(&p_schedule->_lock);

    // success
    return (void *) 1;

    wait_logic:
    {

        // initialized data
        char *thread_name = i_task->_wait_thread;
        parallel_schedule_thread *p_schedule_thread_wait = (parallel_schedule_thread *) dict_get(p_schedule->p_threads, thread_name);
        
        // Find the monitor 
        for (size_t i = 0; i < p_schedule_thread_wait->task_quantity; i++)
        {
        
            // Check the monitor
            if ( strcmp(p_schedule_thread_wait->tasks[i]._name, i_task->_wait_task) == 0 ) 
            {

                // Wait
                monitor_wait(&p_schedule_thread_wait->tasks[i]._monitor);

                // done
                goto done;
            }
        }

        // Default
        goto failed_to_find_monitor;
    }

    // error handling
    {

        // argument errors
        {
            no_work_parameter:
                #ifndef NDEBUG
                    log_error("[parallel] [schedule] Null pointer provided for parameter \"p_parameter\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }

        // Parallel errors
        {
            failed_to_find_monitor:
                #ifndef NDEBUG
                    log_error("[parallel] [schedule] Failed to find monitor for dependant task on thread\"%s\" in call to function \"%s\"\n", p_schedule_thread->_name, __FUNCTION__);
                #endif

                // error
                return 0;
        }
    }
}

int parallel_schedule_thread_destroy ( parallel_schedule_thread **pp_thread );

size_t load_file ( const char *path, void *buffer, bool binary_mode )
{

    // argument checking 
    if ( path == 0 ) goto no_path;

    // initialized data
    size_t  ret = 0;
    FILE   *f   = fopen(path, (binary_mode) ? "rb" : "r");
    
    // Check if file is valid
    if ( f == NULL ) goto invalid_file;

    // Find file size and prep for read
    fseek(f, 0, SEEK_END);
    ret = (size_t) ftell(f);
    fseek(f, 0, SEEK_SET);
    
    // Read to data
    if ( buffer ) ret = fread(buffer, 1, ret, f);

    // The file is no longer needed
    fclose(f);
    
    // success
    return ret;

    // error handling
    {

        // argument errors
        {
            no_path:
                #ifndef NDEBUG
                    printf("[parallel] Null path provided to function \"%s\n", __FUNCTION__);
                #endif

            // error
            return 0;
        }

        // File errors
        {
            invalid_file:
                #ifndef NDEBUG
                    printf("[standard library] Failed to load file \"%s\". %s\n",path, strerror(errno));
                #endif

            // error
            return 0;
        }
    }
}
