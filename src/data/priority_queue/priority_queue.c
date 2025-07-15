/** !
 * priority queue library
 *
 * @file priority_queue.c
 * 
 * @author Jacob Smith
 */

// headers 
#include <data/priority_queue.h>
 
// Preprocessor macros
#define PRIORITY_QUEUE_PARENT(i) (i - 1) / 2
#define PRIORITY_QUEUE_LEFT(i)   (2 * i) + 1
#define PRIORITY_QUEUE_RIGHT(i)  (2 * i) + 2 

// structure definitions
struct priority_queue_s
{
    struct 
    {        
        void   **data;
        size_t   max,
                 count;
    } entries; 

    mutex                    _lock;
    priority_queue_equal_fn *pfn_compare_function;
};

/** !
 * Compare a to b
 * 
 * @param a pointer to a
 * @param b pointer to b
 * 
 * @return 0 if a == b else -1 if a > b else 1
 */
int priority_queue_compare_function ( const void *const a, const void *const b )
{
    
    // Return
    return ( a == b ) ? 0 : ( a < b ) ? 1 : -1;
}

/** !
 * Recursively build the max heap
 * 
 * @param p_priority_queue the priority queue
 * @param i                the index to start building from
 * 
 * @return 1 on success, 0 on error
*/
int priority_queue_heapify ( priority_queue *const p_priority_queue, size_t i );

/** !
 * Build a max heap 
 * 
 * @param p_priority_queue the priority queue
 * 
 * @return 1 on success, 0 on error
 */
int priority_queue_build_max_heap ( priority_queue *const p_priority_queue );

/** !
 * Sort a max heap 
 * 
 * @param p_priority_queue the priority queue
 * 
 * @return 1 on success, 0 on error
 */
int priority_queue_build_heap_sort ( priority_queue *const p_priority_queue );

int priority_queue_create ( priority_queue **const pp_priority_queue )
{

    // argument check
    if ( pp_priority_queue == (void *) 0 ) goto no_priority_queue;

    // Allocate memory for a priority queue
    priority_queue *p_priority_queue = realloc(0, sizeof(priority_queue));

    // error checking
    if ( p_priority_queue == (void *) 0 ) goto no_mem;

    // Zero set
    memset(p_priority_queue, 0, sizeof(priority_queue));

    // Return the allocated memory
    *pp_priority_queue = p_priority_queue;

    // success
    return 1;

    // error handling
    {

        // argument errors
        {
            no_priority_queue:
                #ifndef NDEBUG
                    log_error("[priority queue] Null pointer provided for parameter \"pp_priority_queue\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }

        // standard library errors
        {
            no_mem:
                #ifndef NDEBUG
                    printf("[Standard Library] Failed to allocate memory in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
    }
}

int priority_queue_construct ( priority_queue **const pp_priority_queue, size_t size, priority_queue_equal_fn pfn_compare_function )
{

    // argument check
    if ( pp_priority_queue == (void *) 0 ) goto no_priority_queue;
    if ( size              ==          0 ) goto zero_size;

    // initialized data
    priority_queue *p_priority_queue = 0;

    // Allocate a priority queue
    if ( priority_queue_create(pp_priority_queue) == 0 ) goto failed_to_create_priority_queue;

    // Get a pointer to the allocated priority queue
    p_priority_queue = *pp_priority_queue;

    // Set the count and iterator max
    p_priority_queue->entries.max  = size;

    // Allocate "size" number of properties
    p_priority_queue->entries.data = realloc(0, size * sizeof(void *));

    // Zero set the allocated memory
    memset(p_priority_queue->entries.data, 0, size * sizeof(void *));

    // Create a mutex
    if ( mutex_create(&p_priority_queue->_lock) == 0 ) goto failed_to_create_mutex;

    // Set the comparator function
    // TODO: Improve documentation
    if ( pfn_compare_function == (void *) 0 )
        p_priority_queue->pfn_compare_function = priority_queue_compare_function;
    else
        p_priority_queue->pfn_compare_function = pfn_compare_function;

    // error checking
    if ( p_priority_queue->entries.data == (void *) 0 ) goto no_mem;

    // success
    return 1;

    // error handling
    {

        // argument errors
        {
            no_priority_queue:
                #ifndef NDEBUG
                    log_error("[priority queue] Null pointer provided for parameter \"pp_priority_queue\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;

            zero_size:
                #ifndef NDEBUG
                    log_error("[priority queue] Zero provided for parameter \"size\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;

        }

        // Priority queue errors
        {
            failed_to_create_priority_queue:
                #ifndef NDEBUG
                    log_error("[priority queue] Failed to create priority queue in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;

            failed_to_create_mutex:
                #ifndef NDEBUG
                    log_error("[priority queue] Failed to create mutex in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }

        // standard library errors
        {
            no_mem:
                #ifndef NDEBUG
                    printf("[Standard Library] Failed to allocate memory in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
    }
}

int priority_queue_from_keys ( priority_queue **const pp_priority_queue, const void **const keys, size_t size, priority_queue_equal_fn pfn_compare_function )
{

    // argument check
    if ( pp_priority_queue == (void *) 0 ) goto no_priority_queue;
    if ( keys              == (void *) 0 ) goto no_keys;
    if ( size              ==          0 ) goto zero_size;

    // initialized data
    priority_queue *p_priority_queue = 0;

    // Allocate a priority queue
    if ( priority_queue_construct(&p_priority_queue, size, pfn_compare_function) == 0 ) goto failed_to_construct_priority_queue;

    // Iterate over each key
    for (size_t i = 0; i < size; i++)
    {
        // Add the key to the priority queue
        p_priority_queue->entries.data[i] = (void *)keys[i];
    }

    // Set the count
    p_priority_queue->entries.count = size;

    // Build the priority queue
    priority_queue_build_max_heap(p_priority_queue);

    // Return
    *pp_priority_queue = p_priority_queue;

    // success
    return 1;

    // error handling
    {

        // argument errors
        {
            no_priority_queue:
                #ifndef NDEBUG
                    log_error("[priority queue] Null pointer provided for parameter \"pp_priority_queue\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;

            no_keys:
                #ifndef NDEBUG
                    log_error("[priority queue] Null pointer provided for parameter \"keys\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
            
            zero_size:
                #ifndef NDEBUG
                    log_error("[priority queue] Zero provided for parameter \"size\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;

        }

        // priority queue errors
        {
            failed_to_construct_priority_queue:
                #ifndef NDEBUG
                    log_error("[priority queue] Call to \"priority_queue_construct\" returned an erroneous value in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
    }
}

int priority_queue_heapify ( priority_queue *const p_priority_queue, size_t i )
{

    // argument check
    if ( p_priority_queue == (void *) 0 ) goto no_priority_queue;
    // TODO: Bounds check parameter i
    // TODO: Lock

    // initialized data
    size_t l = PRIORITY_QUEUE_LEFT(i);
    size_t r = PRIORITY_QUEUE_RIGHT(i);
    size_t largest = i;

    // If the lefthand value is greater than the root ...
    if ( ( l < p_priority_queue->entries.count ) && ( p_priority_queue->pfn_compare_function(p_priority_queue->entries.data[l], p_priority_queue->entries.data[largest]) > 0 ) )

        // ... update the index of the largest value
        largest = l;
    
    // If the righthand value is greater than the root ...
    if ( ( r < p_priority_queue->entries.count ) && ( p_priority_queue->pfn_compare_function(p_priority_queue->entries.data[r], p_priority_queue->entries.data[largest]) > 0 ) )

        // ... update the index of the largest value
        largest = r;
    
    // If the index of the largest value must be updated ...
    if ( largest != i )
    {

        // Swap the keys
        void *tmp = p_priority_queue->entries.data[i];
        p_priority_queue->entries.data[i] = p_priority_queue->entries.data[largest];
        p_priority_queue->entries.data[largest] = tmp;

        // Recursively build the heap
        // TODO: Check return code
        priority_queue_heapify(p_priority_queue, largest);
    }

    // success
    return 1;

    // error handling
    {

        // argument errors
        {
            no_priority_queue:
                #ifndef NDEBUG
                    log_error("[priority queue] Null pointer provided for parameter \"p_priority_queue\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
    }
}

int priority_queue_build_max_heap ( priority_queue *const p_priority_queue )
{

    // argument check
    if ( p_priority_queue == (void *) 0 ) goto no_priority_queue;

    // Build the heap
    for (long i = (p_priority_queue->entries.count / 2) - 1; i >= 0; i--)
    {
        // Sort the heap
        priority_queue_heapify(p_priority_queue, i);
    }

    // success
    return 1;

    // error handling
    {

        // argument errors
        {
            no_priority_queue:
                #ifndef NDEBUG
                    log_error("[priority queue] Null pointer provided for parameter \"p_priority_queue\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
    }
}

int priority_queue_build_heap_sort ( priority_queue *const p_priority_queue )
{

    // argument check
    if ( p_priority_queue == (void *) 0 ) goto no_priority_queue;

    // Build the max heap
    priority_queue_build_max_heap(p_priority_queue);

    // Iterate through the max heap
    for ( long i = p_priority_queue->entries.count - 1; i >= 1; i-- )
    {
        
        // Swap the node at the iterator with the largest
        void *tmp = p_priority_queue->entries.data[0];
        p_priority_queue->entries.data[0] = p_priority_queue->entries.data[i];
        p_priority_queue->entries.data[i] = tmp;

        // Decrement the heap size
        p_priority_queue->entries.count--;

        // Sort the heap
        priority_queue_heapify(p_priority_queue, 0);
    }
     
    // success
    return 1;

    // error handling
    {

        // argument errors
        {
            no_priority_queue:
                #ifndef NDEBUG
                    log_error("[priority queue] Null pointer provided for parameter \"p_priority_queue\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
    }
}

int priority_queue_max ( priority_queue *const p_priority_queue, void **pp_value )
{

    // argument check
    if ( p_priority_queue == (void *) 0 ) goto no_priority_queue;
    if ( pp_value         == (void *) 0 ) goto no_value;

    // Check for underflow
    if ( p_priority_queue->entries.count == 0 ) goto underflow;

    // Return the max value
    *pp_value = p_priority_queue->entries.data[0];

    // success
    return 1;

    // error handling
    {

        // argument errors
        {
            no_priority_queue:
                #ifndef NDEBUG
                    log_error("[priority queue] Null pointer provided for parameter \"p_priority_queue\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;

            no_value:
                #ifndef NDEBUG
                    log_error("[priority queue] Null pointer provided for parameter \"pp_value\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }

        // Priority queue errors
        {
            underflow:
                #ifndef NDEBUG
                    log_error("[priority queue] Priority queue underflow in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
    }
}

int priority_queue_extract_max ( priority_queue *const p_priority_queue, void **pp_value )
{

    // argument check
    if ( p_priority_queue == (void *) 0 ) goto no_priority_queue;

    // initialized data
    void *ret = (void *) 0;


    // Get the maximum element
    if ( priority_queue_max(p_priority_queue, &ret) == 0 ) goto underflow;

    // Move the last element in the heap to the first element of the heap
    p_priority_queue->entries.data[0] = p_priority_queue->entries.data[p_priority_queue->entries.count - 1];

    // Eliminate the duplicated element
    p_priority_queue->entries.data[p_priority_queue->entries.count - 1] = (void *) 0;

    // Decrement the quantity of elements in the priority queue
    p_priority_queue->entries.count--;

    // Fix the heap
    priority_queue_heapify(p_priority_queue, 0);

    // Return the maximum value to the caller
    if ( pp_value ) *pp_value = ret;
     
    // success
    return 1;

    // error handling
    {

        // argument errors
        {
            no_priority_queue:
                #ifndef NDEBUG
                    log_error("[priority queue] Null pointer provided for parameter \"p_priority_queue\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }

        // Priority queue errors
        {
            underflow:
                #ifndef NDEBUG
                    log_error("[priority queue] Priority queue underflow in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
    } 
}

int priority_queue_increase_key ( priority_queue *const p_priority_queue, size_t index, void *p_key );
int priority_queue_insert ( priority_queue *const p_priority_queue, void *p_key );

int priority_queue_increase_key ( priority_queue *const p_priority_queue, size_t index, void *p_key )
{
    // argument check
    if ( p_priority_queue == (void *) 0 ) goto no_priority_queue;

    // Ensure the key is increasing priority 
    if ( p_priority_queue->pfn_compare_function(p_key, p_priority_queue->entries.data[index]) < 0 ) goto decrease_key;

    // Store the key in the heap
    p_priority_queue->entries.data[index] = p_key;

    // Increase the key in the heap to its maximum priority
    while ( index > 0 && p_priority_queue->pfn_compare_function(p_priority_queue->entries.data[PRIORITY_QUEUE_PARENT(index)], p_priority_queue->entries.data[index]) < 0 )
    {

        // Swap the key at index with key at parent index in the heap
        size_t tmp_idx = PRIORITY_QUEUE_PARENT(index);
        void *tmp = p_priority_queue->entries.data[index];
        p_priority_queue->entries.data[index] = p_priority_queue->entries.data[tmp_idx];
        p_priority_queue->entries.data[tmp_idx] = tmp;

        // Update index to the parent index;
        index = tmp_idx;
    }

    // success
    return 1;

    // error handling
    {

        // argument errors
        {
            no_priority_queue:
                #ifndef NDEBUG
                    log_error("[priority queue] Null pointer provided for parameter \"p_priority_queue\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }

        // Priority queue errors
        {
            decrease_key:
                #ifndef NDEBUG
                    log_error("[priority queue] New key has lower priority than current key in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
    }
}

int priority_queue_insert ( priority_queue *const p_priority_queue, void *p_key )
{
    
    // argument check
    if ( p_priority_queue == (void *) 0 ) goto no_priority_queue;
    if ( p_key            == (void *) 0 ) goto no_key;

    // State check
    if ( p_priority_queue->entries.count >= p_priority_queue->entries.max ) goto heap_overflow;

    // Increment the size of the heap
    p_priority_queue->entries.count++;

    // Insert the key into the heap
    p_priority_queue->entries.data[p_priority_queue->entries.count - 1] = p_key;

    // Position the key correctly
    if ( priority_queue_increase_key(p_priority_queue, p_priority_queue->entries.count-1, p_key) == 0 ) goto failed_to_increase_key;


    // success
    return 1;

    // error handling
    {

        // argument errors
        {
            no_priority_queue:
                #ifndef NDEBUG
                    log_error("[priority queue] Null pointer provided for parameter \"p_priority_queue\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
            
            no_key:
                #ifndef NDEBUG
                    log_error("[priority queue] Null pointer provided for parameter \"p_key\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }

        // Priority queue errors
        {
            heap_overflow:
                #ifndef NDEBUG
                    log_error("[priority queue] Priority queue overflow in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;

            failed_to_increase_key:
                #ifndef NDEBUG
                    log_error("[priority queue] Call to function \"priority_queue_increase_key\" returned an erroneous value in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
    }
}

int priority_queue_enqueue ( priority_queue *const p_priority_queue, void *p_key )
{

    // argument check
    if ( p_priority_queue == (void *) 0 ) goto no_priority_queue;
    if ( p_key            == (void *) 0 ) goto no_key;

    // lock
    mutex_lock(&p_priority_queue->_lock);

    // Insert the key
    if ( priority_queue_insert(p_priority_queue, p_key) == 0 ) goto failed_to_insert_key;

    // unlock
    mutex_unlock(&p_priority_queue->_lock);

    // success
    return 1;

    // error handling
    {

        // argument errors
        {
            no_priority_queue:
                #ifndef NDEBUG
                    log_error("[priority queue] Null pointer provided for parameter \"p_priority_queue\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
            
            no_key:
                #ifndef NDEBUG
                    log_error("[priority queue] Null pointer provided for parameter \"p_key\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }

        // Priority queue errors
        {
            failed_to_insert_key:
                #ifndef NDEBUG
                    log_error("[priority queue] Call to function \"priority_queue_insert\" returned an erroneous value in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
    }
}

int priority_queue_dequeue ( priority_queue *const p_priority_queue, void **pp_key )
{
    
    // argument check
    if ( p_priority_queue == (void *) 0 ) goto no_priority_queue;
    if ( pp_key           == (void *) 0 ) goto no_key;

    // lock
    mutex_lock(&p_priority_queue->_lock);

    // Insert the key
    if ( priority_queue_extract_max(p_priority_queue, pp_key) == 0 ) goto failed_to_extract_max_key;

    // unlock
    mutex_unlock(&p_priority_queue->_lock);

    // success
    return 1;

    // error handling
    {

        // argument errors
        {
            no_priority_queue:
                #ifndef NDEBUG
                    log_error("[priority queue] Null pointer provided for parameter \"p_priority_queue\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
            
            no_key:
                #ifndef NDEBUG
                    log_error("[priority queue] Null pointer provided for parameter \"p_key\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }

        // Priority queue errors
        {
            failed_to_extract_max_key:
                #ifndef NDEBUG
                    log_error("[priority queue] Call to function \"priority_queue_extract_max\" returned an erroneous value in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
    }
}

bool priority_queue_empty ( priority_queue *const p_priority_queue )
{
    
    // argument check
    if ( p_priority_queue == (void *) 0 ) goto no_priority_queue;

    // lock
    mutex_lock(&p_priority_queue->_lock);

    // initialized data
    bool ret = ( p_priority_queue->entries.count == 0 );

    // unlock
    mutex_unlock(&p_priority_queue->_lock);

    // success
    return ret;

    // error handling
    {

        // argument errors
        {
            no_priority_queue:
                #ifndef NDEBUG
                    log_error("[priority queue] Null pointer provided for parameter \"p_priority_queue\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return true;
        }
    }
}

int priority_queue_destroy ( priority_queue **const pp_priority_queue )
{

    // argument check
    if ( pp_priority_queue  == (void *) 0 ) goto no_priority_queue;
    if ( *pp_priority_queue == (void *) 0 ) goto pp_priority_queue_null;

    // initialized data
    priority_queue *p_priority_queue = *pp_priority_queue;

    // lock
    mutex_lock(&p_priority_queue->_lock);

    // No more pointer for end user
    *pp_priority_queue = (priority_queue *) 0;

    // unlock
    mutex_unlock(&p_priority_queue->_lock);

    // TODO: Remove all the priority queue properties
    // 

    // Free the hash table
    p_priority_queue->entries.data = realloc(p_priority_queue->entries.data, 0);

    // Destroy the mutex
    mutex_destroy(&p_priority_queue->_lock);

    // Free the priority queue
    p_priority_queue = realloc(p_priority_queue, 0);

    // success
    return 1;

    // error handling
    {

        // argument errors
        {
            no_priority_queue:
                #ifndef NDEBUG
                    log_error("[priority queue] Null pointer provided for parameter \"pp_priority_queue\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;

            pp_priority_queue_null:
                #ifndef NDEBUG
                    log_error("[priority queue] Parameter \"pp_priority_queue\" points to null pointer in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
    }
}
