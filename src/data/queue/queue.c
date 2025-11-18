/** !
 * Queue implementation
 * 
 * @file queue.c
 * 
 * @author Jacob Smith
 */

// header
#include <data/queue.h>

// structure definitions
struct queue_node_s
{
	void *content;
	struct queue_node_s *prev,
	                    *next;
};

struct queue_s
{
	struct queue_node_s *front,
                        *rear;
	size_t                size;
	mutex                _lock;
};

// function definitions
int queue_create ( queue **const pp_queue )
{

	// argument check
	if ( pp_queue == (void *) 0 ) goto no_queue;

	// initialized data
	queue *ret = default_allocator(0, sizeof(queue));

	// error check
	if ( ret == (void *)0 ) goto no_mem;

	// Zero set
	memset(ret, 0, sizeof(queue));

	// return the queue
	*pp_queue = ret;

	// success
	return 1;

	// error handling
	{

		// argument errors
		{
			no_queue:
				#ifndef NDEBUG
					log_error("[queue] Null pointer provided for \"pp_queue\" in call to function \"%s\"\n",__FUNCTION__);
				#endif

				// error
				return 0;
		}

		// standard library errors
		{
			no_mem:
				#ifndef NDEBUG
					log_error("[standard library] Failed to allocate memory in call to function \"%s\"\n",__FUNCTION__);
				#endif

				// error
				return 0;
		}
	}
}
 
int queue_construct ( queue **const pp_queue )
{

	// argument check
	if ( pp_queue == (void *) 0 ) goto no_queue;

	// initialized data
	queue *p_queue = 0;

	// allocate for a queue
	if ( queue_create(pp_queue) == 0 ) goto failed_to_create_queue;

	// Get a pointer to the allocated memory
	p_queue = *pp_queue;

	// Create a mutex
    if ( mutex_create(&p_queue->_lock) == 0 ) goto failed_to_create_mutex;

	// success
	return 1;

	// error handling
	{

		// argument errors
		{
			no_queue:
				#ifndef NDEBUG
					log_error("[queue] Null pointer provided for \"pp_queue\" in call to function \"%s\"\n",__FUNCTION__);
				#endif
			
				// error
				return 0;
		}

		// Queue errors
		{
			failed_to_create_queue:
				#ifndef NDEBUG
					log_error("[queue] Failed to create queue in call to function \"%s\"\n", __FUNCTION__);
				#endif

				// error
				return 0;
			
			failed_to_create_mutex:
                #ifndef NDEBUG
                	log_error("[queue] Failed to create mutex in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
		}
	}
}
 
int queue_from_contents ( queue **const pp_queue, void* const* const pp_contents, size_t size )
{

	// argument check
	if ( pp_queue    == (void *) 0 ) goto no_queue;
	if ( pp_contents == (void *) 0 ) goto no_queue_contents;
	if ( size        ==          0 ) goto no_queue_contents;

	// Construct a queue
	if ( queue_construct(pp_queue) == 0 ) goto failed_to_construct_queue;
	
	// iterate over each item
	for (size_t i = 0; i < size; i++)

		// add the item to the queue
		queue_enqueue(*pp_queue, pp_contents[i]);
	
	// store the size of the queue

	// success
	return 1;

	// error handling
	{

		// argument errors
		{
			no_queue:
				#ifndef NDEBUG
					log_error("[queue] Null pointer provided for \"p_queue\" in call to function \"%s\"\n",__FUNCTION__);
				#endif
			
				// error
				return 0;

			no_queue_contents:
				#ifndef NDEBUG
					log_error("[queue] Queue is empty in call to function \"%s\"\n", __FUNCTION__);
				#endif
			
				// error
				return 0;
		}

		// Queue errors
		{
			failed_to_construct_queue:
				#ifndef NDEBUG
					log_error("[queue] Failed to construct queue in call to function \"%s\"\n", __FUNCTION__);
				#endif
			
				// error
				return 0;
		}
	}
}

int queue_front ( queue *const p_queue, void ** const pp_value )
{

	// argument check
	if ( p_queue == (void *) 0 ) goto no_queue;

	// lock
	mutex_lock(&p_queue->_lock);

	// state check
	if ( p_queue->front == 0 ) goto no_queue_contents;

	// return a pointer to the caller
	if ( pp_value )
		*pp_value = ((struct queue_node_s *)(p_queue->front))->content;
	
	// unlock
	mutex_unlock(&p_queue->_lock);

	// exit
	return 1;

	// error handling
	{

		// argument errors
		{
			no_queue:
				#ifndef NDEBUG
					log_error("[queue] Null pointer provided for \"p_queue\" in call to function \"%s\"\n",__FUNCTION__);
				#endif
			
				// error
				return 0;

			no_queue_contents:
				#ifndef NDEBUG
					log_error("[queue] Queue is empty in call to function \"%s\"\n", __FUNCTION__);
				#endif

				// unlock
				mutex_unlock(&p_queue->_lock);

				// error
				return 0;		
		}
	}
}
 
int queue_rear ( queue *const p_queue, void **const pp_value )
{

	// argument check
	if ( p_queue == (void *) 0 ) goto no_queue;
	
	// lock
	mutex_lock(&p_queue->_lock);

	// state check
	if ( p_queue->front == 0 ) goto no_queue_contents;

	// return a pointer to the rear element
	if ( pp_value )
		*pp_value = ((struct queue_node_s *)(p_queue->rear))->content;
	
	// unlock
	mutex_unlock(&p_queue->_lock);

	// exit
	return 1;

	// error handling
	{

		// argument errors
		{
			no_queue:
				#ifndef NDEBUG
					log_error("[queue] Null pointer provided for \"p_queue\" in call to function \"%s\"\n",__FUNCTION__);
				#endif
			
				// error
				return 0;

			no_queue_contents:
				#ifndef NDEBUG
					log_error("[queue] Queue is empty in call to function \"%s\"\n", __FUNCTION__);
				#endif
			
				// unlock
				mutex_unlock(&p_queue->_lock);

				// error
				return 0;
		}
	}
}
 
int queue_enqueue ( queue *const p_queue, void *const data )
{

	// argument check
	if ( p_queue == (void *) 0 ) goto no_queue;

	// lock
	mutex_lock(&p_queue->_lock);

	// initialized data
	struct queue_node_s *q = p_queue->rear, // Q comes before R(ear)
	                    *r = default_allocator(0, sizeof(struct queue_node_s));
	
	// error check
	if ( r == (void *) 0 ) goto no_mem;

	// zero set
	memset(r, 0, sizeof(struct queue_node_s));

	// walk to the end from the rear
	if ( !q )
		q = p_queue->front = p_queue->rear = r;
	
	// default
	else
	{

		// walk the list
		while ( q->next != 0 ) q = q->next;
		
		q->next = r;
		r->prev = q;
		p_queue->rear = r;
	}

	// store the value
	r->content = data;
	
	// increment the size
	p_queue->size++;

	// unlock
	mutex_unlock(&p_queue->_lock);

	// success
	return 1;
		
	// error handling
	{

		// argument errors
		{
			no_queue:
				#ifndef NDEBUG
					log_error("[queue] Null pointer provided for \"p_queue\" in call to function \"%s\"\n",__FUNCTION__);
				#endif

				// error
				return 0;
		}

		// standard library errors
		{
			no_mem:
				#ifndef NDEBUG
					log_error("[standard library] Failed to allocate memory in call to function \"%s\"\n",__FUNCTION__);
				#endif

				// unlock
				mutex_unlock(&p_queue->_lock);

				// error
				return 0;
		}
	}
}
 
int queue_dequeue ( queue *const p_queue, void **const pp_value )
{
	
	// argument check
	if ( p_queue == (void *) 0 ) goto no_queue;
	
	// lock
	mutex_lock(&p_queue->_lock);
	
	// state check
	if ( p_queue->front == 0 ) goto queue_empty;

	// initialized data
	struct queue_node_s *ret_m = 0;

	ret_m = ((struct queue_node_s *)(p_queue->front));

	// Remove the front
	if (p_queue->front != p_queue->rear)
		p_queue->front = ((struct queue_node_s *)(p_queue->front))->next;
	else
	{
		p_queue->front = 0;
		p_queue->rear  = 0;
	}

	// If the caller specified a return...
	if ( pp_value )
		
		// ... return the content to the caller
		*pp_value = ret_m->content;

	// Free the memory
	ret_m = default_allocator(ret_m, 0);
		
	// unlock
	mutex_unlock(&p_queue->_lock);
	
	// success
	return 1;

	// error handling
	{

		// argument errors
		{
			no_queue:
				#ifndef NDEBUG
					log_error("[queue] Null pointer provided for \"p_queue\" in call to function \"%s\"\n",__FUNCTION__);
				#endif

				// error
				return 0;
		}

		// Queue errors
		{
			queue_empty:

				// No output...
				
				// unlock
				mutex_unlock(&p_queue->_lock);

				// error
				return 0;
		}
	}
}

bool queue_empty ( queue *const p_queue )
{
	
	// argument check
	if ( p_queue == (void *)0 ) goto no_queue;

	// lock
	mutex_lock(&p_queue->_lock);

	// initialized data
	bool ret = ( p_queue->front == 0 );

	// unlock
	mutex_unlock(&p_queue->_lock);
	
	// success
	return ret;
	
	// error handling
	{

		// argument errors
		{
			no_queue:
				#ifndef NDEBUG
					log_error("[queue] Null pointer provided for parameter \"p_queue\" in call to function \"%s\"\n", __FUNCTION__);
				#endif
			
				// error
				return 0;
		}
	}
}

bool queue_size ( queue *const p_queue )
{
	
	// argument check
	if ( p_queue == (void *)0 ) goto no_queue;

	// lock
	mutex_lock(&p_queue->_lock);

	// initialized data
	int ret = p_queue->size;

	// unlock
	mutex_unlock(&p_queue->_lock);
	
	// success
	return ret;
	
	// error handling
	{

		// argument errors
		{
			no_queue:
				#ifndef NDEBUG
					log_error("[queue] Null pointer provided for parameter \"p_queue\" in call to function \"%s\"\n", __FUNCTION__);
				#endif
			
				// error
				return 0;
		}
	}
}

int queue_fori ( queue *p_queue, fn_fori *pfn_fori ) 
{

    // argument check
    if ( NULL == p_queue  ) goto no_queue;
    if ( NULL == pfn_fori ) goto no_fn_fori;

	// initialized data
	struct queue_node_s *p_iter = 0;
	size_t i = 0;

    // lock
    mutex_lock(&p_queue->_lock);

	// store the head of the queue
	p_iter = p_queue->front;

    // iterate over each element in the queue
	while ( p_iter )

		// call the function
		pfn_fori(p_iter->content, i),

		// iterate
		i++, 
		p_iter = p_iter->next;

    // unlock
    mutex_unlock(&p_queue->_lock);

    // success
    return 1;

    // error handling
    {
        
        // argument errors
        {
            no_queue:
                #ifndef NDEBUG
                    log_error("[queue] Null pointer provided for \"p_queue\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
            
            no_fn_fori:
                #ifndef NDEBUG
                    log_error("[queue] Null pointer provided for \"pfn_fori\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
    }
}

int queue_pack ( void *p_buffer, queue *p_queue, fn_pack *pfn_element )
{
    
    // argument check
    if ( p_queue     == (void *) 0 ) goto no_queue;
    if ( pfn_element == (void *) 0 ) return 0;

    // initialized data 
    char *p = p_buffer;
	struct queue_node_s *p_iter = 0;

    // lock
    mutex_lock(&p_queue->_lock);

    // pack the size
    p += pack_pack(p, "%i64", p_queue->size);

	// store the head of the queue
	p_iter = p_queue->front;

    // iterate over each element in the queue
	while ( p_iter )

		// pack the value
		p += pfn_element(p, p_iter->content),

		// iterate
		p_iter = p_iter->next;

    // unlock
    mutex_unlock(&p_queue->_lock);

    // success
    return p - (char *)p_buffer;

    // error handling
    {
        
        // argument errors
        {
            no_queue:
                #ifndef NDEBUG
                    log_error("[queue] Null pointer provided for \"p_queue\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
    }
}

int queue_unpack ( queue **pp_queue, void *p_buffer, fn_unpack *pfn_element )
{
    
    // argument check
    if ( pp_queue    == (void *) 0 ) goto no_queue;
    if ( pfn_element == (void *) 0 ) return 0;

    // initialized data
    queue *p_queue = NULL;
    char *p = p_buffer;
    size_t size = 0;

    // unpack the size
    p += pack_unpack(p, "%i64", &size);

    // construct a queue
    queue_construct(&p_queue);

	// iterate through the elements in the queue
    for (size_t i = 0; i < size; i++)
    {
        
        // initialized data
        char _result[1024] = { 0 };
        void *p_element = NULL;
        size_t len_result = pfn_element(_result, p);

        // advance the buffer
        p += len_result;

        // allocate memory for the element
        p_element = default_allocator(0, len_result),

        // copy the memory
        memcpy(p_element, _result, len_result),
        
        // add the element to the queue
        queue_enqueue(p_queue, p_element);
    }

    // return the queue to the caller
    *pp_queue = p_queue;

    // success
    return 1;
    
    // error handling
    {
        
        // argument errors
        {
            no_queue:
                #ifndef NDEBUG
                    log_error("[queue] Null pointer provided for \"pp_queue\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
    }
}

hash64 queue_hash ( queue *p_queue, fn_hash64 *pfn_element )
{

    // argument check
    if ( p_queue == (void *) 0 ) goto no_queue;
    if ( pfn_element == (void *) 0 ) goto no_pfn_element;

    // initialized data
    hash64 result = 0;
	struct queue_node_s *p_iter = NULL;

	// lock
	mutex_lock(&p_queue->_lock);

	// store
	p_iter = p_queue->front;

    // iterate over each element in the queue
	while ( p_iter )

		// hash
		result ^= pfn_element(p_iter->content, 8),

		// iterate
		p_iter = p_iter->next;

	// unlock
	mutex_unlock(&p_queue->_lock);

    // success
    return result;

    // error handling
    {

        // argument errors
        {
            no_queue:
                #ifndef NDEBUG
                    log_error("[queue] Null pointer provided for \"p_queue\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
            
            no_pfn_element:
                #ifndef NDEBUG
                    log_error("[queue] Null pointer provided for \"pfn_element\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
    }
}

int queue_destroy ( queue **const pp_queue )
{

	// argument check
	if ( pp_queue == (void *) 0 ) goto no_queue;

	// initialized data
	queue *p_queue = *pp_queue;
	
	// lock
	mutex_lock(&p_queue->_lock);

	// No more queue for end user
	*pp_queue = 0;

	// unlock
	mutex_unlock(&p_queue->_lock);

	// Free the memory
	p_queue = default_allocator(p_queue, 0);
		
	// success
	return 1;

	// error handling
	{

		// argument errors
		{
			no_queue:
				#ifndef NDEBUG
					log_error("[queue] Null pointer provided for \"pp_queue\" in call to function \"%s\"\n",__FUNCTION__);
				#endif

				// error
				return 0;
		}
	}
}
