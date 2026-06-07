/** !
 * Queue implementation
 * 
 * @file queue.c
 * 
 * @author Jacob Smith
 */

// header
#include <data/queue.h>

fn_it_done queue_iterator_done;
fn_it_next queue_iterator_next;
fn_it_item queue_iterator_item;

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
	if ( NULL == pp_queue ) goto no_queue;

	// initialized data
	queue *ret = default_allocator(0, sizeof(queue));

	// error check
	if ( NULL == ret ) goto no_mem;

	// zero set
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
	if ( NULL == pp_queue ) goto no_queue;

	// initialized data
	queue *p_queue = 0;

	// allocate a queue
	if ( 0 == queue_create(&p_queue) ) goto failed_to_create_queue;

	// construct a mutex
    if ( 0 == mutex_create(&p_queue->_lock) ) goto failed_to_create_mutex;

	// return a pointer to the caller
	*pp_queue = p_queue;

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

		// queue errors
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
	if ( NULL ==    pp_queue ) goto no_queue;
	if ( NULL == pp_contents ) goto no_queue_contents;
	if ( 0    ==        size ) goto no_queue_contents;

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

		// queue errors
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
	if ( NULL == p_queue ) goto no_queue;

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
	if ( NULL == p_queue ) goto no_queue;
	
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
	if ( NULL == p_queue ) goto no_queue;

	// lock
	mutex_lock(&p_queue->_lock);

	// initialized data
	struct queue_node_s *q = p_queue->rear, // Q comes before R(ear)
	                    *r = default_allocator(0, sizeof(struct queue_node_s));
	
	// error check
	if ( NULL == r ) goto no_mem;

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
	if ( NULL == p_queue ) goto no_queue;
	
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

		// queue errors
		{
			queue_empty:
				
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
	if ( NULL == p_queue ) goto no_queue;

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
	if ( NULL == p_queue ) goto no_queue;

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

iterator queue_iterator ( queue *p_queue )
{

    // success
    return (iterator)
    {
        .p_data = p_queue,
        .state  = { .p_state = (void *) p_queue->front },
        .done   = queue_iterator_done,
        .next   = queue_iterator_next,
        .item   = queue_iterator_item
    };
}

bool queue_iterator_done ( iterator *p_iterator ) 
{

    // done?
    return p_iterator->state.p_state == NULL; 
}

void queue_iterator_next ( iterator *p_iterator ) 
{

    // update the state
    p_iterator->state.p_state = (void *)((struct queue_node_s *)p_iterator->state.p_state)->next; 

    // done
    return;
}

void *queue_iterator_item ( iterator *p_iterator ) 
{

    // done
    return ((struct queue_node_s *)p_iterator->state.p_state)->content; 
}

int queue_pack ( stream *p_stream, queue *p_queue, fn_pack *pfn_element )
{
    
    // argument check
    if ( NULL ==      p_queue ) goto no_queue;
    if ( NULL ==     p_stream ) goto no_stream;
    if ( NULL ==  pfn_element ) goto no_element;

    // initialized data 
    size_t written = 0;
	struct queue_node_s *p_iter = 0;

    // lock
    mutex_lock(&p_queue->_lock);

    // pack the size
    written += pack_pack(p_stream, "%i64", p_queue->size);

	// store the head of the queue
	p_iter = p_queue->front;

    // iterate over each element in the queue
	while ( p_iter )

		// pack the value
		written += pfn_element(p_stream, p_iter->content),

		// iterate
		p_iter = p_iter->next;

    // unlock
    mutex_unlock(&p_queue->_lock);

    // success
    return written;

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

			no_stream:
                #ifndef NDEBUG
                    log_error("[queue] Null pointer provided for \"p_stream\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;

			no_element:
                #ifndef NDEBUG
                    log_error("[queue] Null pointer provided for \"pfn_element\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
    }
}

int queue_unpack ( queue **pp_queue, stream *p_stream, fn_unpack *pfn_element )
{
    
    // argument check
    if ( NULL ==    pp_queue ) goto no_queue;
    if ( NULL ==    p_stream ) goto no_stream;
    if ( NULL == pfn_element ) goto no_unpack;

    // initialized data
    queue *p_queue = NULL;
    size_t written = 0;
    size_t size = 0;

    // unpack the size
    written += pack_unpack(p_stream, "%i64", &size);

    // construct a queue
    queue_construct(&p_queue);

	// iterate through the elements in the queue
    for (size_t i = 0; i < size; i++)
    {
        
        // initialized data
        void *p_element = NULL;
        
        // unpack the element
        written += pfn_element(&p_element, p_stream);

        // add the element to the queue
        queue_enqueue(p_queue, p_element);
    }

    // return the queue to the caller
    *pp_queue = p_queue;

    // success
    return written;
    
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
			
			no_stream:
                #ifndef NDEBUG
                    log_error("[queue] Null pointer provided for \"p_stream\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;

			no_unpack:
                #ifndef NDEBUG
                    log_error("[queue] Null pointer provided for \"pfn_element\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
    }
}

hash64 queue_hash ( queue *p_queue, fn_hash64 *pfn_element )
{

    // argument check
    if ( NULL ==     p_queue ) goto no_queue;
    if ( NULL == pfn_element ) goto no_pfn_element;

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

int queue_destroy ( queue **const pp_queue, fn_allocator *pfn_allocator )
{

	// argument check
	if ( NULL == pp_queue ) goto no_queue;

	// initialized data
	queue *p_queue = *pp_queue;
	
	// lock
	mutex_lock(&p_queue->_lock);

	// no more queue for end user
	*pp_queue = 0;

	// release elements
	while ( p_queue->front )
	{

		// initialized data
		struct queue_node_s *p_element = p_queue->front;

		// release the element data
		if ( pfn_allocator )
			p_element->content = pfn_allocator(p_element->content, 0);

		// step
		p_queue->front = p_queue->front->next;

		// release the element
		p_element = default_allocator(p_element, 0);
	}

	// unlock
	mutex_unlock(&p_queue->_lock);

	// release the queue
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
