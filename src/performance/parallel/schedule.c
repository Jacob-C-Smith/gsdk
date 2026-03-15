/** !
 * Schedule implementation
 * 
 * @file src/performance/parallel/schedule.c
 *
 * @author Jacob Smith
 */

// header file
#include <performance/schedule.h>

// preprocessor definitions
#define PARALLEL_SCHEDULE_NAME_LENGTH        (63 + 1)
#define PARALLEL_SCHEDULE_THREAD_NAME_LENGTH (63 + 1)
#define PARALLEL_SCHEDULE_TASK_NAME_LENGTH   (63 + 1)
#define PARALLEL_SCHEDULE_MAX_THREADS        64
#define PARALLEL_SCHEDULE_MAX_TASKS          256

// external declarations
fn_key_accessor object_key_accessor;

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
    bool dependent, dependency, ready, done;
    int dependencies;
    fn_parallel_task *pfn_task;
    condition_variable _ready;
    mutex _lock;
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
    mutex _lock;
    condition_variable _ready, _go;
    volatile size_t running_threads;
    dict *p_threads;
    bool repeat, ready, go;
    void *p_parameter;
    char  _name [PARALLEL_SCHEDULE_NAME_LENGTH];
    char  _main_thread_name [PARALLEL_SCHEDULE_THREAD_NAME_LENGTH];
    parallel_schedule_work_parameter _work_parameters[PARALLEL_SCHEDULE_MAX_THREADS];
};

void *parallel_schedule_thread_key_accessor ( const void *const p_value )
{
    
    // initialized data
    parallel_schedule_thread *p_schedule_thread = (parallel_schedule_thread *) p_value;

    // done
    return p_schedule_thread->_name;
}

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

/** !
 * Return the size of a file IF buffer == 0 ELSE read a file into buffer
 * 
 * @param path path to the file
 * @param buffer buffer
 * @param binary_mode "wb" IF true ELSE "w"
 * 
 * @return 1 on success, 0 on error
 */
size_t load_file ( const char *path, void *buffer, bool binary_mode );

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

    // zero set memory
    memset(p_schedule_thread, 0, sizeof(parallel_schedule_thread) + (task_quantity * sizeof(parallel_schedule_task)));

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
    
    // store the size of the file
    file_size = load_file(path, 0, true);

    // error check
    if ( file_size == 0 ) goto invalid_file;
    
    // allocate memory for the file
    p_file_contents = default_allocator(0, file_size);

    // error check
    if ( p_file_contents == (void *) 0 ) goto no_mem;

    // load the file
    load_file(path, p_file_contents, 0);

    // parse the file into a json value
    if ( json_value_parse(p_file_contents, 0, &p_value) == 0 ) goto failed_to_parse_json_value;

    // construct a schedule
    if ( schedule_load_as_json_value(pp_schedule, p_value) == 0 ) goto failed_to_construct_schedule;

    // clean up
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
        
        // file errors
        {
            invalid_file:
                #ifndef NDEBUG
                    log_error("[parallel] [schedule] Can not load test file in call to function \"%s\"\n", __FUNCTION__);
                #endif
                
                // error
                return 0;
        }
        
        // json errors
        {
            failed_to_parse_json_value:
                #ifndef NDEBUG
                    log_error("[parallel] [schedule] Failed to parse JSON text in call to function \"%s\"\n", __FUNCTION__);
                #endif
                
                // error
                return 0;
        }

        // parallel errors
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
    if ( NULL          ==       pp_schedule ) goto no_schedule;
    if ( NULL          ==           p_value ) goto no_value;
    if ( p_value->type != JSON_VALUE_OBJECT ) goto wrong_type;

    // initialized data
    dict *p_dict = p_value->object;
    json_value *p_name        = NULL,
               *p_threads     = NULL,
               *p_main_thread = NULL,
               *p_repeat      = NULL;
    schedule  _schedule  = { 0 }, 
             *p_schedule = (void *) 0;

    dict_get(p_dict, "name", (void **) &p_name),
    dict_get(p_dict, "threads", (void **) &p_threads),
    dict_get(p_dict, "main thread", (void **) &p_main_thread),
    dict_get(p_dict, "repeat", (void **) &p_repeat);

    // check for missing properties
    if ( NULL == p_name || NULL == p_threads ) goto missing_properties;

    // parse the name property
    if ( p_name->type == JSON_VALUE_STRING )
    {

        // initialized data
        char   *p_name_string = p_name->string;
        size_t  len           = strlen(p_name_string);

        // error check
        if ( len < 1 ) goto name_property_too_short;
        if ( len >= PARALLEL_SCHEDULE_NAME_LENGTH ) goto name_property_too_long;
        
        // copy the name into the schedule struct
        strncpy(_schedule._name, p_name_string, len);

        // null terminate the copied string
        _schedule._name[len] = '\0';
    } 
    
    // error check
    if ( p_main_thread == (void *) 0 ) goto no_main_thread; 

    // parse the main thread property
    if ( p_main_thread->type == JSON_VALUE_STRING )
    {

        // initialized data
        char   *p_main_thread_string = p_main_thread->string;
        size_t  len                  = strlen(p_main_thread_string);
        
        // error check
        if ( len < 1 ) goto main_thread_property_too_short;
        if ( len >= PARALLEL_SCHEDULE_THREAD_NAME_LENGTH ) goto main_thread_property_too_long;
        
        // copy the name into the schedule struct
        strncpy(_schedule._main_thread_name, p_main_thread_string, len);

        // null terminate the copied string
        _schedule._main_thread_name[len] = '\0';
    } 

    // default
    else goto wrong_main_thread_type;
    
    no_main_thread:
    // parse the threads property
    if ( p_threads->type == JSON_VALUE_OBJECT )
    {

        // initialized data
        dict       *p_dict           = p_threads->object;
        size_t      threads_quantity = 0;
        const char *_keys[PARALLEL_SCHEDULE_MAX_THREADS]     = { 0 };
        json_value *_p_values[PARALLEL_SCHEDULE_MAX_THREADS] = { 0 };
        
        // store the quantity of threads
        dict_size(p_dict, &threads_quantity);

        // error check
        if ( threads_quantity == 0 ) goto threads_property_is_empty;
        if ( threads_quantity > PARALLEL_SCHEDULE_MAX_THREADS ) goto threads_property_is_too_large;

        // store the values of the threads
        dict_values(p_dict, (void **)_p_values, threads_quantity);

        // iterate through each value
        for (size_t i = 0; i < threads_quantity; i++)
            _keys[i] = (const char *) _p_values[i]->p_key;

        // construct a dictionary for the threads
        (void) dict_construct(&_schedule.p_threads, threads_quantity * 2, NULL, parallel_schedule_thread_key_accessor, NULL);

        // iterate over each thread
        for (size_t i = 0; i < threads_quantity; i++)
        {

            // initialized data
            parallel_schedule_thread *p_thread = (void *) 0;

            // construct a schedule thread
            if ( parallel_schedule_thread_load_as_json_value(&p_thread, _keys[i], _p_values[i]) == 0 ) goto failed_to_create_thread;

            // add the schedule thread to the schedule
            dict_add(_schedule.p_threads, p_thread);
        }
    }

    // default
    else goto wrong_threads_type;

    if ( p_repeat == (void *) 0 ) goto no_repeat_property;

    // parse the repeat property
    if ( p_repeat->type == JSON_VALUE_BOOLEAN ) 

        // store the repeat property
        _schedule.repeat = p_repeat->boolean;

    // default
    else goto wrong_repeat_type;

    no_repeat_property:

    // validate the schedule
    {

        // initialized data
        size_t thread_quantity = 0;
        parallel_schedule_thread *_p_threads [PARALLEL_SCHEDULE_MAX_THREADS] = { 0 };
        parallel_schedule_thread *p_thread = (void *) 0;
        parallel_schedule_task   *p_task = (void *) 0;
        
        // store the quantity of threads
        dict_size(_schedule.p_threads, &thread_quantity);

        // store the threads from the schedule
        dict_values(_schedule.p_threads, (void **)_p_threads, thread_quantity);

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

                // construct synchronization primitives
                condition_variable_create(&p_task->_ready);
                mutex_create(&p_task->_lock);

                // will this task wait for something else?
                if ( p_task->dependent )
                {
                    
                    // initialized data
                    parallel_schedule_thread *p_dependency_thread = NULL;
                    
                    dict_get(_schedule.p_threads, p_task->_wait_thread, (void **) &p_dependency_thread);

                    // iterate through each task
                    for (size_t k = 0; k < p_dependency_thread->task_quantity; k++)
                    {
                        
                        // initialized data
                        parallel_schedule_task *i_dependency_task = &p_dependency_thread->tasks[k];

                        // will the task wait on this task?
                        if ( strcmp(i_dependency_task->_name, p_task->_wait_task) == 0 )
                        {
                            i_dependency_task->dependency = true;
                            i_dependency_task->dependencies++;
                        }
                    }
                }
            }
        }
    }

    // allocate memory for a schedule
    p_schedule = default_allocator(0, sizeof(schedule));
    if ( NULL == p_schedule ) goto no_mem;

    // copy the schedule from the stack to the heap
    memcpy(p_schedule, &_schedule, sizeof(schedule));

    // construct a condition variable for the schedule
    condition_variable_create(&p_schedule->_ready);
    condition_variable_create(&p_schedule->_go);

    // construct a mutex for the schedule
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

        // json errors
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
    
        // parallel errors
        {
            failed_to_create_thread:
                #ifndef NDEBUG
                    log_error("[parallel] [schedule] Failed to create scheduler thread in call to functon \"%s\"\n", __FUNCTION__);
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

int parallel_schedule_thread_load_as_json_value ( parallel_schedule_thread **const pp_schedule_thread, const char *const name, const json_value *const p_value )
{

    // argument check
    if ( NULL               ==  pp_schedule_thread ) goto no_thread;
    if ( NULL               ==                name ) goto no_name;
    if ( NULL               ==             p_value ) goto no_value;
    if ( p_value->type      !=    JSON_VALUE_ARRAY ) goto wrong_type;

    // initialized data
    parallel_schedule_thread *p_schedule_thread = (void *) 0;
    array *p_array = p_value->list;
    size_t task_quantity = array_size(p_array),
           thread_name_len = strlen(name);
    char *error_state = (void *) 0;

    // allocate memory for a schedule thread
    if ( parallel_schedule_thread_create(&p_schedule_thread, task_quantity) == 0 ) goto failed_to_allocate_schedule_thread;

    // error check
    if ( thread_name_len >= PARALLEL_SCHEDULE_THREAD_NAME_LENGTH ) goto thread_name_too_long;

    // copy the name of the thread
    strncpy(p_schedule_thread->_name, name, thread_name_len);
    p_schedule_thread->_name[thread_name_len] = '\0';

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
            json_value *p_task = NULL,
                       *p_wait = NULL;
            
            dict_get(p_dict, "task", (void **)&p_task),
            dict_get(p_dict, "wait", (void **)&p_wait);

            // check for missing properties
            if ( p_task == (void *) 0 ) goto missing_properties;

            // parse the task property
            if ( p_task->type == JSON_VALUE_STRING )
            {
                
                // initialized data
                size_t len = 0;
                fn_parallel_task *pfn_task = 0;
                
                // find the task from the registery
                parallel_find_task(p_task->string, &pfn_task);

                // error check
                if ( pfn_task == 0 )
                {

                    // update the error state
                    error_state = p_task->string;

                    // error
                    goto unrecognized_task;
                }

                // store the length of the task name
                len = strlen(p_task->string);

                // error check
                if ( len >= PARALLEL_SCHEDULE_TASK_NAME_LENGTH ) goto task_name_too_long;

                // copy the task name
                strncpy(p_schedule_thread->tasks[i]._name, p_task->string, len);
                p_schedule_thread->tasks[i]._name[len] = '\0';

                // store the task function pointer
                p_schedule_thread->tasks[i].pfn_task = pfn_task;
            }

            // default
            else goto wrong_task_task_type;

            // set the dependent flag
            p_schedule_thread->tasks[i].dependent = (bool) p_wait;

            // no wait property means nothing to do
            if ( p_wait == (void *) 0 ) continue;

            // parse the wait property
            if ( p_wait->type == JSON_VALUE_STRING )
            {

                // initialized data
                char *wait_thread      = p_wait->string, 
                     *wait_task        = strchr(wait_thread, ':');
                size_t wait_thread_len = 0,
                       wait_task_len   = 0;

                // error check
                if ( wait_task == (void *) 0 ) goto no_colon_delimiter;

                // compute the length of the wait thread
                wait_thread_len = (wait_task - wait_thread);

                // increment past the ':'
                wait_task++;

                // store the length of the wait task
                wait_task_len = strlen(wait_task);

                // error check
                if ( wait_task_len   >= PARALLEL_SCHEDULE_TASK_NAME_LENGTH ) goto wait_task_too_long;
                if ( wait_task_len   < 1 ) goto wait_task_too_short;
                if ( wait_thread_len >= PARALLEL_SCHEDULE_THREAD_NAME_LENGTH ) goto wait_thread_too_long;
                if ( wait_thread_len < 1 ) goto wait_thread_too_short;

                // copy the wait thread
                strncpy(p_schedule_thread->tasks[i]._wait_thread, wait_thread, wait_thread_len);
                p_schedule_thread->tasks[i]._wait_thread[wait_thread_len] = '\0';

                // copy the wait task
                strncpy(p_schedule_thread->tasks[i]._wait_task, wait_task, wait_task_len);
                p_schedule_thread->tasks[i]._wait_task[wait_task_len] = '\0';
            }

            // default 
            else goto wrong_task_wait_type;
        }

        // default
        else goto wrong_task_type;
    }
    
    // return a pointer to the caller
    *pp_schedule_thread = p_schedule_thread;

    // success
    return 1;

    // json errors
    missing_properties:
    wrong_task_type:
    wrong_task_task_type:
    wrong_task_wait_type:
    no_colon_delimiter:
    task_name_too_long:
    wait_task_too_long:
    wait_task_too_short:
    thread_name_too_long:
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

        // json errors
        {

            wrong_type:
                #ifndef NDEBUG
                    log_error("[parallel] [schedule] Parameter \"p_value\" must be of type [ array ] in call to function \"%s\"\n", __FUNCTION__);
                #endif
                
                // error
                return 0;
        }

        // parallel errors
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
    size_t thread_quantity = 0;
    parallel_schedule_thread *_p_threads [PARALLEL_SCHEDULE_MAX_THREADS] = { 0 };
    // parallel_schedule_thread *p_main_thread = NULL;
    // parallel_schedule_work_parameter *p_main_thread_work_parameter = NULL;

    // store the parameter
    p_schedule->p_parameter = p_parameter;

    // store the quantity of threads
    dict_size(p_schedule->p_threads, &thread_quantity);

    // store the threads from the schedule
    dict_values(p_schedule->p_threads, (void **)_p_threads, thread_quantity);

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

        // skip main thread
        // if ( strcmp(p_schedule->_main_thread_name, p_thread->_name) == 0 )
        // {

        //     // store the main thread
        //     p_main_thread = p_thread;

        //     // store the work parameter
        //     p_main_thread_work_parameter = &p_schedule->_work_parameters[i];

        //     // done 
        //     continue;
        // }
        
        // spawn the thread
        if ( parallel_thread_start(&p_thread->p_parallel_thread, (fn_parallel_task *) parallel_schedule_work, &p_schedule->_work_parameters[i]) == 0 ) goto failed_to_create_thread;
    }

    /*
    // start the main thread
    if ( p_main_thread )
    {

        // wait for all the threads to start        
        while ( p_schedule->running_threads != thread_quantity - 1 )
            sleep(0);

        // start the main thread
        parallel_schedule_main_work(p_main_thread_work_parameter);
    }
    */
    
    {
        mutex_lock(&p_schedule->_lock);

        // wait for worker threads
        while ( p_schedule->running_threads != thread_quantity )
            condition_variable_wait(&p_schedule->_ready, &p_schedule->_lock);
        
        p_schedule->go = true;
        // log_error("GO!!!\n"), fflush(stdout);
        condition_variable_broadcast(&p_schedule->_go);

        mutex_unlock(&p_schedule->_lock);
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

        // parallel errors
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
    
    // spin until all threads are done
    while (p_schedule->running_threads)
        sleep(0);

    // success
    return 1;
}

int schedule_pause ( schedule *const p_schedule )
{

    // clear the repeat flag
    p_schedule->repeat = false;

    // success
    return 1;
} 

int schedule_stop ( schedule *const p_schedule )
{

    // argument check
    if ( p_schedule == (void *) 0 ) goto no_schedule;

    // initialized data
    size_t thread_quantity = 0;
    parallel_schedule_thread *_p_threads[PARALLEL_SCHEDULE_MAX_THREADS] = { 0 };

    // clear the repeat flag
    p_schedule->repeat = false;
 
    // store the quantity of threads
    dict_size(p_schedule->p_threads, &thread_quantity);

    // store the threads from the schedule
    dict_values(p_schedule->p_threads, (void **)_p_threads, thread_quantity);

    // iterate over each thread
    for (size_t i = 0; i < thread_quantity; i++)
    {
        
        // initialized data
        parallel_schedule_thread *p_thread = _p_threads[i];

        // skip the main thread
        if ( strcmp(p_schedule->_main_thread_name, p_thread->_name) == 0 ) continue;

        // clear the running flag
        p_thread->running = false;

        // cancel the thread
        parallel_thread_cancel(p_thread->p_parallel_thread);

        // jsonoin the thread
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

        // parallel errors
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

    // set this thread's ready flag
    p_schedule_thread->running = true;

    // increment running threads
    p_schedule->running_threads++;

    // signal
    condition_variable_signal(&p_schedule->_ready);

    // monitor
    while ( !p_schedule->go )
        condition_variable_wait(&p_schedule->_go, &p_schedule->_lock);

    // unlock
    mutex_unlock(&p_schedule->_lock);
    
    // iterate through each task
    for (size_t i = 0; i < p_schedule_thread->task_quantity; i++)
    {

        // initialized data
        i_task = &p_schedule_thread->tasks[i];
        
        // wait
        if ( i_task->dependent ) goto wait_logic;

        // done waiting
        done:

        // run the task
        i_task->pfn_task(p_parameter->p_schedule->p_parameter);

        // set the done task
        i_task->done = true;

        // lock
        mutex_lock(&p_schedule->_lock);
        
        // signal
        if ( i_task->dependency ) 
            // log_error("[%s] Broadcast\n", p_parameter->p_thread->_name), fflush(stdout),
            condition_variable_broadcast(&i_task->_ready);

        // unlock
        mutex_unlock(&p_schedule->_lock);
    }

    // lock
    mutex_lock(&p_schedule->_lock);
    
    // clear the running flag
    p_schedule_thread->running = false;
    
    // decrement the quantity of running threads
    p_schedule->running_threads--;
    
    // unlock
    mutex_unlock(&p_schedule->_lock);

    // success
    return (void *) 1;

    wait_logic:
    {

        // initialized data
        char *thread_name = i_task->_wait_thread;
        parallel_schedule_thread *p_dependency_thread = NULL;
        
        // store the dependency
        dict_get(p_schedule->p_threads, thread_name, (void **)&p_dependency_thread);
        
        // find the monitor
        for (size_t k = 0; k < p_dependency_thread->task_quantity; k++)
        {
        
            // check the monitor
            if ( strcmp(p_dependency_thread->tasks[k]._name, i_task->_wait_task) == 0 ) 
            {

                // log_info("[%s] waiting for %s:%s\n", p_parameter->p_thread->_name, p_dependency_thread->_name, i_task->_wait_task), fflush(stdout);

                // lock
                mutex_lock(&p_schedule->_lock);

                // monitor
                while ( !p_dependency_thread->tasks[k].done )
                    condition_variable_wait(&p_dependency_thread->tasks[k]._ready, &p_schedule->_lock);

                // unlock
                mutex_unlock(&p_schedule->_lock);

                // done
                break;
            }
        }

        // done
        goto done;
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
    }
}

void *parallel_schedule_main_work ( parallel_schedule_work_parameter *p_parameter )
{

    // argument check
    if ( NULL == p_parameter ) goto no_parameter;

    // stub
    return (void *) 1;
    /*
    // initialized data
    schedule        *p_schedule        = p_parameter->p_schedule;
    parallel_schedule_thread *p_schedule_thread = p_parameter->p_thread;
    parallel_schedule_task   *i_task            = (void *) 0;
    
    // lock
    mutex_lock(&p_schedule->_lock);

    // set the running flag
    p_schedule_thread->running = true;

    // increment the quantity of running threads
    p_schedule->running_threads++;

    // unlock
    mutex_unlock(&p_schedule->_lock);

    // start the work
    monitor_notify_all(&p_schedule->_montior);
    
    turnover:

    // iterate through each task
    for (size_t i = 0; i < p_schedule_thread->task_quantity; i++)
    {

        // initialized data
        i_task = &p_schedule_thread->tasks[i];

        // wait
        if ( i_task->dependent ) goto wait_logic;

        // done waiting
        done:

        // run the task
        i_task->pfn_task(p_parameter->p_schedule->p_parameter);
        
        // signal
        if ( i_task->dependency ) monitor_notify_all(&i_task->_monitor);
    }
    
    // repeat?
    if ( p_schedule->repeat ) goto turnover;

    // lock
    mutex_lock(&p_schedule->_lock);

    // clear the running flag
    p_schedule_thread->running = false;

    // decrement the quantity of running threads
    p_schedule->running_threads--;

    // unlock
    mutex_unlock(&p_schedule->_lock);

    // success
    return (void *) 1;

    wait_logic:
    {

        // initialized data
        char *thread_name = i_task->_wait_thread;
        parallel_schedule_thread *p_schedule_thread_wait = NULL;

        dict_get(p_schedule->p_threads, thread_name, &p_schedule_thread_wait);
        
        // find the monitor 
        for (size_t i = 0; i < p_schedule_thread_wait->task_quantity; i++)
        {
        
            // check the monitor
            if ( strcmp(p_schedule_thread_wait->tasks[i]._name, i_task->_wait_task) == 0 ) 
            {

                // wait
                monitor_wait(&p_schedule_thread_wait->tasks[i]._monitor);

                // done
                goto done;
            }
        }

        // default
        goto failed_to_find_monitor;
    }
    */
    // error handling
    {

        // argument errors
        {
            no_parameter:
                #ifndef NDEBUG
                    log_error("[parallel] [schedule] Null pointer provided for parameter \"p_parameter\" in call to function \"%s\"\n", __FUNCTION__);
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
    
    // check if file is valid
    if ( f == NULL ) goto invalid_file;

    // find file size and prep for read
    fseek(f, 0, SEEK_END);
    ret = (size_t) ftell(f);
    fseek(f, 0, SEEK_SET);
    
    // read to data
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

        // file errors
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
