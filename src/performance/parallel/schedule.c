/** !
 * Schedule implementation
 * 
 * @file src/performance/parallel/schedule.c
 *
 * @author Jacob Smith
 */

// header file
#include <performance/schedule.h>

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
    char                _name [64];
    int                 dependencies;
    fn_parallel_task   *pfn_task;
    condition_variable  _ready;
    mutex               _lock;
    bool                dependent,
                        dependency, 
                        ready, 
                        done;
    size_t              wait_quantity;
    struct 
    {
        char                             _thread[64];
        char                             _task[64];
        struct parallel_schedule_task_s *p_task;
    } waits[16];
};

struct parallel_schedule_thread_s
{
    char                    _name[64];
    size_t                  task_quantity;
    bool                    running;
    parallel_thread        *p_parallel_thread;
    parallel_schedule_task  tasks [];
};

struct parallel_schedule_work_parameter_s
{
    schedule                 *p_schedule;
    parallel_schedule_thread *p_thread;
};

struct schedule_s
{
    char                              _name[64];
    char                              _main_thread_name[64];
    mutex                             _lock;
    condition_variable                _ready,
                                      _go;
    volatile size_t                   running_threads;
    bool                              repeat, 
                                      ready, 
                                      go;
    void                             *p_parameter;
    dict                             *p_threads;
    parallel_schedule_work_parameter  _work_parameters[64];
};

// function declarations
fn_key_accessor parallel_schedule_thread_key_accessor;

/** !
 * Allocate memory for a scheudle thread
 * 
 * @param pp_schedule_thread result
 * @param task_quantity      the quantity of tasks
 * 
 * @return 1 on success, 0 on error
*/
int parallel_schedule_thread_create ( parallel_schedule_thread **pp_schedule_thread, size_t task_quantity );

/** !
 * Construct a schedule from a json value
 * 
 * @param pp_schedule result
 * @param value       the json value
 * 
 * @return 1 on success, 0 on error
 */
int schedule_load_as_json_value ( schedule **const pp_schedule, const json_value *const p_value );

/** !
 * Construct a named thread from a json value
 * 
 * @param pp_thread result
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
 * @return result
 */
void *parallel_schedule_work ( parallel_schedule_work_parameter *p_parameter );

/** !
 * Destroy a schedule thread
 * 
 * @param pp_thread pointer to schedule thread pointer
 * 
 * @return 1 on success, 0 on error
 */
int parallel_schedule_thread_destroy ( parallel_schedule_thread **pp_thread );

// function definitions
int parallel_schedule_thread_create ( parallel_schedule_thread **const pp_schedule_thread, size_t task_quantity )
{

    // argument check
    if ( NULL == pp_schedule_thread ) goto no_schedule_thread;

    // initialized data
    parallel_schedule_thread *p_schedule_thread = NULL;

    // allocate memory for the schedule thread
    p_schedule_thread = default_allocator(0, sizeof(parallel_schedule_thread) + (task_quantity * sizeof(parallel_schedule_task)));
    if ( NULL == p_schedule_thread ) goto no_mem;

    // zero memory
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
                    log_error("[schedule] Null pointer provided for parameter \"pp_schedule_thread\" in call to function \"%s\"\n", __FUNCTION__);
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
    if ( NULL == pp_schedule ) goto no_schedule;
    if ( NULL ==        path ) goto no_path;

    // initialized data
    stream     *p_stream = NULL;
    json_value *p_value  = NULL;
    
    // construct a stream
    stream_from_path(&p_stream, path);

    // parse the file into a json value
    if ( 0 == json_parse(&p_value, p_stream, NULL) ) goto failed_to_parse_json_value;

    // construct a schedule
    if ( 0 == schedule_load_as_json_value(pp_schedule, p_value) ) goto failed_to_construct_schedule;

    // release the json value
    p_value = json_allocator(p_value, 0);

    // release the stream
    stream_destroy(&p_stream);

    // success
    return 1;

    // error handling
    {

        // argument errors
        {
            no_schedule: 
                #ifndef NDEBUG
                    log_error("[schedule] Null pointer provided for parameter \"pp_schedule\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;

            no_path:
                #ifndef NDEBUG
                    log_error("[schedule] Null pointer provided for parameter \"path\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
        
        // json errors
        {
            failed_to_parse_json_value:
                #ifndef NDEBUG
                    log_error("[schedule] Failed to parse JSON text in call to function \"%s\"\n", __FUNCTION__);
                #endif
                
                // error
                return 0;
        }

        // parallel errors
        {
            failed_to_construct_schedule:
                #ifndef NDEBUG
                    log_error("[schedule] Failed to construct schedule in call to function \"%s\"\n", __FUNCTION__);
                #endif
                
                // error
                return 0;
        }
    }
}

int schedule_load_as_json_value ( schedule **const pp_schedule, const json_value *const p_value )
{

    // argument check
    if ( NULL              ==   pp_schedule ) goto no_schedule;
    if ( NULL              ==       p_value ) goto no_value;
    if ( JSON_VALUE_OBJECT != p_value->type ) goto wrong_type;

    // initialized data
    dict                     *p_dict           = p_value->object;
    json_value               *p_name           = NULL,
                             *p_threads        = NULL,
                             *p_main_thread    = NULL,
                             *p_repeat         = NULL;
    schedule                  _schedule        = { 0 }, 
                             *p_schedule       = NULL;
    size_t                    threads_quantity = 0;
    parallel_schedule_thread *p_thread         = NULL;
    parallel_schedule_task   *p_task           = NULL;

    // store the properties
    dict_get(p_dict, "name"       , (void **) &p_name),
    dict_get(p_dict, "threads"    , (void **) &p_threads),
    dict_get(p_dict, "main thread", (void **) &p_main_thread),
    dict_get(p_dict, "repeat"     , (void **) &p_repeat);

    // check for missing properties
    if ( NULL ==    p_name ) goto missing_name;
    if ( NULL == p_threads ) goto missing_threads;

    // parse the name property
    if ( JSON_VALUE_STRING == p_name->type )
    {

        // initialized data
        char   *p_name_string = p_name->string;
        size_t  len           = strlen(p_name_string);

        // error check
        if ( 1  >  len ) goto name_property_too_short;
        if ( 64 <= len ) goto name_property_too_long;
        
        // store the name
        strncpy(_schedule._name, p_name_string, len);
        _schedule._name[len] = '\0';
    } 
    
    // state check
    if ( NULL == p_main_thread ) goto no_main_thread; 

    // parse the main thread property
    if ( JSON_VALUE_STRING == p_main_thread->type )
    {

        // initialized data
        char   *p_main_thread_string = p_main_thread->string;
        size_t  len                  = strlen(p_main_thread_string);
        
        // error check
        if ( 1  >  len ) goto main_thread_property_too_short;
        if ( 64 <= len ) goto main_thread_property_too_long;
        
        // store the name
        strncpy(_schedule._main_thread_name, p_main_thread_string, len);
        _schedule._main_thread_name[len] = '\0';
    } 

    // default
    else goto wrong_main_thread_type;
    
    no_main_thread:

    // error check
    if ( JSON_VALUE_OBJECT != p_threads->type ) goto wrong_threads_type;
    
    // store the quantity of threads
    dict_size(p_threads->object, &threads_quantity);

    // error check
    if ( 0  == threads_quantity ) goto threads_property_is_empty;
    if ( 64 <  threads_quantity ) goto threads_property_is_too_large;

    // construct a dictionary for the threads
    dict_construct(&_schedule.p_threads, threads_quantity * 2, NULL, parallel_schedule_thread_key_accessor, NULL);

    // iterate over each thread
    for (iterator it = dict_iterator(p_threads->object); !it.done(&it); it.next(&it))
    {

        // initialized data
        parallel_schedule_thread *p_thread = NULL;
        json_value               *p_val    = (json_value *) it.item(&it);

        // construct a schedule thread
        if ( 0 == parallel_schedule_thread_load_as_json_value(&p_thread, p_val->p_key, p_val) ) goto failed_to_create_thread;

        // add the schedule thread to the schedule
        dict_add(_schedule.p_threads, p_thread);
    }

    // state check
    if ( NULL == p_repeat ) goto no_repeat_property;

    // parse the repeat property
    if ( JSON_VALUE_BOOLEAN != p_repeat->type ) goto wrong_repeat_type;

    // store the repeat property
    _schedule.repeat = p_repeat->boolean;

    no_repeat_property:

    // validate the schedule
    for (iterator it = dict_iterator(_schedule.p_threads); !it.done(&it); it.next(&it))
    {
        
        // store the thread
        p_thread = (parallel_schedule_thread *) it.item(&it);

        // iterate through each task
        for (size_t j = 0; j < p_thread->task_quantity; j++)
        {
            
            // store the task
            p_task = &p_thread->tasks[j];

            // construct a condition variable
            condition_variable_create(&p_task->_ready);

            // construct a lock
            mutex_create(&p_task->_lock);

            // will this task wait for something else?
            if ( false == p_task->dependent ) continue;
                
            // iterate through each wait
            for (size_t l = 0; l < p_task->wait_quantity; l++)
            {

                // initialized data
                parallel_schedule_thread *p_dependency_thread = NULL;
                
                // store the l'th thread
                dict_get(_schedule.p_threads, p_task->waits[l]._thread, (void **) &p_dependency_thread);

                // error check
                if ( NULL == p_dependency_thread ) goto failed_to_resolve_dependency;

                // iterate through each task
                for (size_t k = 0; k < p_dependency_thread->task_quantity; k++)
                {
                    
                    // initialized data
                    parallel_schedule_task *i_dependency_task = &p_dependency_thread->tasks[k];

                    // will the task wait on this task?
                    if ( 0 == strcmp(i_dependency_task->_name, p_task->waits[l]._task) )
                    {

                        // set the dependency flag
                        i_dependency_task->dependency = true;

                        // increment the quantity of dependencies
                        i_dependency_task->dependencies++;

                        // store the dependent task
                        p_task->waits[l].p_task = i_dependency_task;

                        // done
                        break;
                    }
                }

                // error check
                if ( NULL == p_task->waits[l].p_task ) goto failed_to_resolve_dependency;
            }
        }
    }

    // allocate memory for a schedule
    p_schedule = default_allocator(0, sizeof(schedule));
    if ( NULL == p_schedule ) goto no_mem;

    // copy the schedule from the stack to the heap
    memcpy(p_schedule, &_schedule, sizeof(schedule));

    // construct condition variables
    condition_variable_create(&p_schedule->_ready),
    condition_variable_create(&p_schedule->_go);

    // construct a lock 
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
                    log_error("[schedule] Null pointer provided for parameter \"pp_schedule\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;

            no_value: 
                #ifndef NDEBUG
                    log_error("[schedule] Null pointer provided for parameter \"p_value\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }

        // json errors
        {
            wrong_type:
                #ifndef NDEBUG
                    log_error("[schedule] Parameter \"p_value\" must be of type [ object ] in call to function \"%s\"\n\"Refer to schedule schema: [TODO: Schedule schema URL] \n", __FUNCTION__);
                #endif

                // error
                return 0;
            
            missing_name:
                #ifndef NDEBUG
                    log_error("[schedule] Parameter \"p_value\" missing required property \"name\" in call to function \"%s\"\n\"Refer to schedule schema: [TODO: Schedule schema URL] \n", __FUNCTION__);
                #endif

                // error
                return 0;
            
            missing_threads:
                #ifndef NDEBUG
                    log_error("[schedule] Parameter \"p_value\" missing required property \"threads\" in call to function \"%s\"\n\"Refer to schedule schema: [TODO: Schedule schema URL] \n", __FUNCTION__);
                #endif

                // error
                return 0;
            
            wrong_threads_type:
                #ifndef NDEBUG
                    log_error("[schedule] \"threads\" property of schedule object must be of type [ object ] in call to function \"%s\"\n\"Refer to schedule schema: [TODO: Schedule schema URL] \n", __FUNCTION__);
                #endif

                // error
                return 0;

            wrong_main_thread_type:
                #ifndef NDEBUG
                    log_error("[schedule] \"main thread\" property of schedule object must be of type [ object ] in call to function \"%s\"\n\"Refer to schedule schema: [TODO: Schedule schema URL] \n", __FUNCTION__);
                #endif

                // error
                return 0;

            wrong_repeat_type:
                #ifndef NDEBUG
                    log_error("[schedule] \"repeat\" property of schedule object must be of type [ boolean ] in call to function \"%s\"\n\"Refer to schedule schema: [TODO: Schedule schema URL] \n", __FUNCTION__);
                #endif

                // error
                return 0;

            name_property_too_long:
                #ifndef NDEBUG
                    log_error("[schedule] \"name\" property of schedule object must be less than %d characters in call to function \"%s\"\n\"Refer to schedule schema: [TODO: Schedule schema URL] \n", 64, __FUNCTION__);
                #endif

                // error
                return 0;

            name_property_too_short:
                #ifndef NDEBUG
                    log_error("[schedule] \"name\" property of schedule object must be at least 1 character long in call to function \"%s\"\n\"Refer to schedule schema: [TODO: Schedule schema URL] \n", __FUNCTION__);
                #endif

                // error
                return 0;
            
            main_thread_property_too_short:
                #ifndef NDEBUG
                    log_error("[schedule] \"main thread\" property of schedule object must be at least 1 character long in call to function \"%s\"\n\"Refer to schedule schema: [TODO: Schedule schema URL] \n", __FUNCTION__);
                #endif

                // error
                return 0;

            main_thread_property_too_long:
                #ifndef NDEBUG
                    log_error("[schedule] \"main thread\" property of schedule object must be less than %d characters in call to function \"%s\"\n\"Refer to schedule schema: [TODO: Schedule schema URL] \n", 64, __FUNCTION__);
                #endif

                // error
                return 0;

            threads_property_is_empty:
                #ifndef NDEBUG
                    log_error("[schedule] \"threads\" property of schedule object must contain at least 1 property in call to function \"%s\"\n\"Refer to schedule schema: [TODO: Schedule schema URL] \n", __FUNCTION__);
                #endif

                // error
                return 0;
            
            threads_property_is_too_large:
                #ifndef NDEBUG
                    log_error("[schedule] \"threads\" property of schedule object must contain %d or fewer properties in call to function \"%s\"\n\"Refer to schedule schema: [TODO: Schedule schema URL] \n", 64, __FUNCTION__);
                #endif

                // error
                return 0;
        }
    
        // parallel errors
        {
            failed_to_create_thread:
                #ifndef NDEBUG
                    log_error("[schedule] Failed to create scheduler thread in call to functon \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
            
            failed_to_resolve_dependency:
                #ifndef NDEBUG
                    log_error("[schedule] Failed to resolve dependency in call to function \"%s\"\n", __FUNCTION__);
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
    if ( NULL          ==  pp_schedule_thread ) goto no_thread;
    if ( NULL          ==                name ) goto no_name;
    if ( NULL          ==             p_value ) goto no_value;
    if ( p_value->type !=    JSON_VALUE_ARRAY ) goto wrong_type;

    // initialized data
    parallel_schedule_thread *p_schedule_thread = NULL;
    array                    *p_array           = p_value->list;
    size_t                    task_quantity     = array_size(p_array),
                              thread_name_len   = strlen(name);
    char                     *error_state       = NULL;
    size_t                    i                 = 0;

    // allocate memory for a schedule thread
    if ( 0 == parallel_schedule_thread_create(&p_schedule_thread, task_quantity) ) goto failed_to_allocate_schedule_thread;

    // error check
    if ( 64 <= thread_name_len ) goto thread_name_too_long;

    // store the name
    strncpy(p_schedule_thread->_name, name, thread_name_len);
    p_schedule_thread->_name[thread_name_len] = '\0';

    // iterate through the array
    for (iterator it = array_iterator(p_array); !it.done(&it); it.next(&it), i++)
    {
        
        // initialized data
        const json_value *p_ith_value = (const json_value *) it.item(&it);

        // error check
        if ( p_ith_value->type == JSON_VALUE_OBJECT ) 
        {

            // initialized data
            dict       *p_dict = p_ith_value->object;
            json_value *p_task = NULL,
                       *p_wait = NULL;
            
            // store the properties
            dict_get(p_dict, "task", (void **)&p_task),
            dict_get(p_dict, "wait", (void **)&p_wait);

            // check for missing properties
            if ( NULL == p_task ) goto missing_task;

            // parse the task property
            if ( JSON_VALUE_STRING == p_task->type )
            {
                
                // initialized data
                fn_parallel_task *pfn_task = 0;
                size_t            len      = 0;
                
                // find the task from the registery
                parallel_find_task(p_task->string, &pfn_task);

                // error check
                if ( 0 == pfn_task )
                {

                    // update the error state
                    error_state = p_task->string;

                    // error
                    goto unrecognized_task;
                }

                // store the length of the task name
                len = strlen(p_task->string);

                // error check
                if ( 64 <= len ) goto task_name_too_long;

                // store the task name
                strncpy(p_schedule_thread->tasks[i]._name, p_task->string, len);
                p_schedule_thread->tasks[i]._name[len] = '\0';

                // store the task function pointer
                p_schedule_thread->tasks[i].pfn_task = pfn_task;
            }

            // default
            else goto wrong_task_task_type;

            // store the dependent flag
            p_schedule_thread->tasks[i].dependent = (bool) p_wait;

            // wait?
            if ( NULL == p_wait ) continue;

            // parse the wait property
            if ( JSON_VALUE_STRING == p_wait->type )
            {

                // initialized data
                char   *wait_thread     = p_wait->string, 
                       *wait_task       = strchr(wait_thread, ':');
                size_t  wait_thread_len = 0,
                        wait_task_len   = 0;

                // error check
                if ( NULL == wait_task ) goto no_colon_delimiter;

                // compute the length of the wait thread
                wait_thread_len = (wait_task - wait_thread);

                // skip the ':'
                wait_task++;

                // store the length of the wait task
                wait_task_len = strlen(wait_task);

                // error check
                if ( 64 <=   wait_task_len ) goto wait_task_too_long;
                if ( 1  >    wait_task_len ) goto wait_task_too_short;
                if ( 64 <= wait_thread_len ) goto wait_thread_too_long;
                if ( 1  >  wait_thread_len ) goto wait_thread_too_short;

                // store the wait thread
                strncpy(p_schedule_thread->tasks[i].waits[0]._thread, wait_thread, wait_thread_len);
                p_schedule_thread->tasks[i].waits[0]._thread[wait_thread_len] = '\0';

                // store the wait task
                strncpy(p_schedule_thread->tasks[i].waits[0]._task, wait_task, wait_task_len);
                p_schedule_thread->tasks[i].waits[0]._task[wait_task_len] = '\0';

                // increment the wait quantity
                p_schedule_thread->tasks[i].wait_quantity = 1;
            }
            
            // wait can be an array of strings
            else if ( JSON_VALUE_ARRAY == p_wait->type )
            {

                // initialized data
                array  *p_waits       = p_wait->list;
                size_t  wait_quantity = array_size(p_waits);

                // error check
                if ( 16 < wait_quantity ) goto wait_quantity_too_large;

                // store the wait quantity
                p_schedule_thread->tasks[i].wait_quantity = wait_quantity;

                // iterate through each wait item
                for (size_t j = 0; j < wait_quantity; j++)
                {

                    // initialized data
                    json_value *p_wait_item     = NULL;
                    char       *wait_thread     = NULL,
                               *wait_task       = NULL;
                    size_t      wait_thread_len = 0,
                                wait_task_len   = 0;

                    // store the jth wait item
                    array_index(p_waits, j, (void **)&p_wait_item);

                    // error check
                    if ( JSON_VALUE_STRING != p_wait_item->type ) goto wrong_task_wait_type;

                    // store the name of the wait item
                    wait_thread = p_wait_item->string, 
                    wait_task = strchr(wait_thread, ':');
                    
                    // error check
                    if ( NULL == wait_task ) goto no_colon_delimiter;

                    // compute the length of the wait thread
                    wait_thread_len = (wait_task - wait_thread);

                    // skip the ':'
                    wait_task++;

                    // store the length of the wait task
                    wait_task_len = strlen(wait_task);

                    // error check
                    if ( 64 <=   wait_task_len ) goto wait_task_too_long;
                    if ( 1  >    wait_task_len ) goto wait_task_too_short;
                    if ( 64 <= wait_thread_len ) goto wait_thread_too_long;
                    if ( 1  >  wait_thread_len ) goto wait_thread_too_short;

                    // store the wait thread
                    strncpy(p_schedule_thread->tasks[i].waits[j]._thread, wait_thread, wait_thread_len);
                    p_schedule_thread->tasks[i].waits[j]._thread[wait_thread_len] = '\0';

                    // store the wait task
                    strncpy(p_schedule_thread->tasks[i].waits[j]._task, wait_task, wait_task_len);
                    p_schedule_thread->tasks[i].waits[j]._task[wait_task_len] = '\0';
                }
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

    // error handling
    {

        // argument errors
        {
            no_thread:
                #ifndef NDEBUG
                    log_error("[schedule] Null pointer provided for parameter \"pp_schedule_thread\" in call to function \"%s\"\n", __FUNCTION__);
                #endif
                
                // error
                return 0;

            no_name:
                #ifndef NDEBUG
                    log_error("[schedule] Null pointer provided for parameter \"name\" in call to function \"%s\"\n", __FUNCTION__);
                #endif
                
                // error
                return 0;

            no_value:
                #ifndef NDEBUG
                    log_error("[schedule] Null pointer provided for parameter \"p_value\" in call to function \"%s\"\n", __FUNCTION__);
                #endif
                
                // error
                return 0;
        }

        // json errors
        {
            wrong_type:
                #ifndef NDEBUG
                    log_error("[schedule] Parameter \"p_value\" must be of type [ array ] in call to function \"%s\"\n", __FUNCTION__);
                #endif
                
                // error
                return 0;

            missing_task:
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
            wait_quantity_too_large:
                return 0;
        }

        // parallel errors
        {
            unrecognized_task:
                #ifndef NDEBUG
                    log_error("[schedule] Unrecognized task \"%s\" was encountered while constructing therad \"%s\" in call to function \"%s\"\n", error_state, name, __FUNCTION__);
                #endif

                // error
                return 0;
            
            failed_to_allocate_schedule_thread:
                #ifndef NDEBUG
                    log_error("[schedule] Failed to allocate scheduler thread in call to function \"%s\"\n", error_state, name, __FUNCTION__);
                #endif

                // error
                return 0;
        }
    }   
}

int schedule_start ( schedule *const p_schedule, void *const p_parameter )
{

    // argument check
    if ( NULL == p_schedule ) goto no_schedule;

    // initialized data
    size_t thread_quantity = 0;
    size_t i               = 0;

    // store the parameter
    p_schedule->p_parameter = p_parameter;

    // store the quantity of threads
    dict_size(p_schedule->p_threads, &thread_quantity);

    // iterate over each thread
    for (iterator it = dict_iterator(p_schedule->p_threads); !it.done(&it); it.next(&it), i++)
    {
        
        // initialized data
        parallel_schedule_thread *p_thread = (parallel_schedule_thread *) it.item(&it);

        // store the thread parameter
        p_schedule->_work_parameters[i] = (parallel_schedule_work_parameter)
        {
            .p_schedule = p_schedule,
            .p_thread   = p_thread
        };

        // spawn the thread
        if ( 0 == parallel_thread_start(&p_thread->p_parallel_thread, (fn_parallel_task *) parallel_schedule_work, &p_schedule->_work_parameters[i]) ) goto failed_to_create_thread;
    }

    // lock
    mutex_lock(&p_schedule->_lock);

    // wait for workers
    while ( p_schedule->running_threads != thread_quantity ) condition_variable_wait(&p_schedule->_ready, &p_schedule->_lock);
    
    // set the go flag
    p_schedule->go = true;
    
    // broadcast
    condition_variable_broadcast(&p_schedule->_go);

    // unlock
    mutex_unlock(&p_schedule->_lock);

    // success
    return 1;

    // error handling
    {

        // argument errors
        {
            no_schedule:
                #ifndef NDEBUG
                    log_error("[schedule] Null pointer provided for parameter \"p_schedule\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }

        // parallel errors
        {
            failed_to_create_thread:
                #ifndef NDEBUG
                    log_error("[schedule] Failed to spawn thread in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
    }
}

int schedule_wait_idle ( schedule *const p_schedule )
{

    // argument check
    if ( NULL == p_schedule ) goto no_schedule;

    // lock
    mutex_lock(&p_schedule->_lock);

    // wait for all threads
    while ( p_schedule->running_threads > 0 ) condition_variable_wait(&p_schedule->_ready, &p_schedule->_lock);

    // unlock
    mutex_unlock(&p_schedule->_lock);

    // success
    return 1;

    // error handling
    {

        // argument errors
        {
            no_schedule:
                #ifndef NDEBUG
                    log_error("[schedule] Null pointer provided for parameter \"p_schedule\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
    }
}

int schedule_destroy ( schedule **const pp_schedule )
{

    // argument check
    if ( NULL == pp_schedule ) goto no_schedule;

    // initialized data
    schedule *p_schedule = *pp_schedule;

    // fast exit
    if ( NULL == p_schedule ) return 1;

    // no more pointer for caller
    *pp_schedule = NULL;

    // join all threads
    for (iterator it = dict_iterator(p_schedule->p_threads); !it.done(&it); it.next(&it))
    {

        // initialized data
        parallel_schedule_thread *p_thread = it.item(&it);

        // release the thread
        parallel_schedule_thread_destroy(&p_thread);
    }

    // release the dictionary
    dict_destroy(&p_schedule->p_threads, (void *) 0);

    // release the condition variables
    condition_variable_destroy(&p_schedule->_ready),
    condition_variable_destroy(&p_schedule->_go),

    // release the lock
    mutex_destroy(&p_schedule->_lock);

    // release the schedule 
    p_schedule = default_allocator(p_schedule, 0);

    // success
    return 1;

    // error handling
    {

        // argument errors
        {
            no_schedule:
                #ifndef NDEBUG
                    log_error("[schedule] Null pointer provided for parameter \"pp_schedule\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
    }
}

void *parallel_schedule_work ( parallel_schedule_work_parameter *p_parameter )
{

    // argument check
    if ( NULL == p_parameter ) goto no_work_parameter;

    // initialized data
    schedule                 *p_schedule        = p_parameter->p_schedule;
    parallel_schedule_thread *p_schedule_thread = p_parameter->p_thread;
    parallel_schedule_task   *i_task            = NULL;

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

        // set the done flag
        i_task->done = true;

        // lock
        mutex_lock(&p_schedule->_lock);
        
        // broadcast
        if ( i_task->dependency ) condition_variable_broadcast(&i_task->_ready);

        // unlock
        mutex_unlock(&p_schedule->_lock);
    }

    // lock
    mutex_lock(&p_schedule->_lock);
    
    // clear the running flag
    p_schedule_thread->running = false;
    
    // decrement the quantity of running threads
    p_schedule->running_threads--;
    
    // signal
    condition_variable_signal(&p_schedule->_ready);

    // unlock
    mutex_unlock(&p_schedule->_lock);

    // success
    return (void *) 1;

    wait_logic:
    {
        
        // iterate through each wait
        for (size_t j = 0; j < i_task->wait_quantity; j++)
        {
            
            // initialized data
            parallel_schedule_task *p_dependency_task = i_task->waits[j].p_task;

            // lock
            mutex_lock(&p_schedule->_lock);

            // monitor
            while ( !p_dependency_task->done )
                condition_variable_wait(&p_dependency_task->_ready, &p_schedule->_lock);

            // unlock
            mutex_unlock(&p_schedule->_lock);
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
                    log_error("[schedule] Null pointer provided for parameter \"p_parameter\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
    }
}

int parallel_schedule_thread_destroy ( parallel_schedule_thread **pp_thread )
{

    // argument check
    if ( NULL == pp_thread ) goto no_thread;

    // initialized data
    parallel_schedule_thread *p_thread = *pp_thread;

    // fast exit
    if ( NULL == p_thread ) return 1;

    // no more pointer for caller
    *pp_thread = NULL;

    // join the thread
    parallel_thread_join(&p_thread->p_parallel_thread);

    // iterate through each task
    for (size_t i = 0; i < p_thread->task_quantity; i++)

        // release the condition variable
        condition_variable_destroy(&p_thread->tasks[i]._ready),

        // release the lock
        mutex_destroy(&p_thread->tasks[i]._lock);

    // release the thread
    p_thread = default_allocator(p_thread, 0);

    // success
    return 1;

    // error handling
    {

        // argument errors
        {
            no_thread:
                #ifndef NDEBUG
                    log_error("[schedule] Null pointer provided for parameter \"pp_thread\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
    }
}

void *parallel_schedule_thread_key_accessor ( const void *const p_value )
{
    
    // initialized data
    parallel_schedule_thread *p_schedule_thread = (parallel_schedule_thread *) p_value;

    // done
    return p_schedule_thread->_name;
}
