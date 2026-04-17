/** !
 * double queue implementation
 * 
 * @file src/data/double_queue/double_queue.c
 * 
 * @author Jacob Smith
 */

// header file
#include <data/double_queue.h>

// structure declarations
struct double_queue_node_s;

// type definitions
typedef struct double_queue_node_s double_queue_node;

// structure definitions
struct double_queue_node_s
{
	void *content; 
	double_queue_node *prev,
	                  *next;
};

struct double_queue_s
{
	double_queue_node *front,
                      *rear;
	size_t             size;
	mutex _lock;
};

 
int double_queue_construct ( double_queue **const pp_double_queue )
{

	// argument check
	if ( NULL == pp_double_queue ) goto no_double_queue;

	// initialized data
	double_queue *p_double_queue = NULL;

	// allocate a double ended queue
	p_double_queue = default_allocator(0, sizeof(double_queue));
	if ( NULL == p_double_queue ) goto no_mem;

	// zero set
	memset(p_double_queue, 0, sizeof(double_queue));
	
	// create a lock
    if ( mutex_create(&p_double_queue->_lock) == 0 ) goto failed_to_create_lock;

	// return a pointer to the caller
	*pp_double_queue = p_double_queue;

	// success
	return 1;

	// error handling
	{

		// argument errors
		{
			no_double_queue:
				#ifndef NDEBUG
					log_error("[double queue] Null pointer provided for parameter \"pp_double_queue\" in call to function \"%s\"\n",__FUNCTION__);
				#endif
			
				// error
				return 0;
		}

		// double queue errors
		{
			failed_to_create_lock:
                #ifndef NDEBUG
                    log_error("[double queue] Failed to create mutex in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
		}
		
		// standard library errors
		{
			no_mem:
				#ifndef NDEBUG
					printf("[standard library] Failed to allocate memory in call to function \"%s\"\n",__FUNCTION__);
				#endif

				// error
				return 0;
		}
	}
}
 
int double_queue_from_contents ( double_queue **const pp_double_queue, void* const* const pp_contents, size_t size )
{

	// argument check
	if ( NULL == pp_double_queue ) goto no_double_queue;
	if ( NULL ==     pp_contents ) goto no_double_queue_contents;

	// initialized data
	double_queue *p_double_queue = NULL;

	// construct a double ended queue
	if ( 0 == double_queue_construct(pp_double_queue) ) goto failed_to_construct_queue;
	
	// iterate over each element
	for (size_t i = 0; i < size; i++)

		// add each element to the queue
		double_queue_rear_add(*pp_double_queue, pp_contents[i]);

	// return a pointer to the caller
	*pp_double_queue = p_double_queue;

	// success
	return 1;

	// error handling
	{

		// argument errors
		{
			no_double_queue:
				#ifndef NDEBUG
					log_error("[double queue] Null pointer provided for parameter \"p_double_queue\" in call to function \"%s\"\n",__FUNCTION__);
				#endif
			
				// error
				return 0;

			no_double_queue_contents:
				#ifndef NDEBUG
					log_error("[double queue] Queue has no contents in call to function \"%s\"\n", __FUNCTION__);
				#endif
			
				// error
				return 0;
		}

		// double queue errors
		{
			failed_to_construct_queue:
				#ifndef NDEBUG
					log_error("[double queue] Failed to construct queue in call to function \"%s\"\n", __FUNCTION__);
				#endif
			
				// error
				return 0;
		}
	}
}

int double_queue_front ( double_queue *const p_double_queue, void **const pp_value )
{

	// argument check
	if ( NULL == p_double_queue ) goto no_double_queue;

	// lock
	mutex_lock(&p_double_queue->_lock);

	// state check
	if ( p_double_queue->front == 0 ) goto no_double_queue_contents;

	// return a pointer to the caller
	if ( pp_value )
		*pp_value = ((double_queue_node *)(p_double_queue->front))->content;
	
	// unlock
	mutex_unlock(&p_double_queue->_lock);

	// exit
	return 1;

	// error handling
	{

		// argument errors
		{
			no_double_queue:
				#ifndef NDEBUG
					log_error("[double queue] Null pointer provided for parameter \"p_double_queue\" in call to function \"%s\"\n",__FUNCTION__);
				#endif
			
				// error
				return 0;

			no_double_queue_contents:
				#ifndef NDEBUG
					log_error("[double queue] Double queue is empty in call to function \"%s\"\n", __FUNCTION__);
				#endif
			
				// unlock
				mutex_unlock(&p_double_queue->_lock);

				// error
				return 0;
		}
	}
}
 
int double_queue_rear ( double_queue *const p_double_queue, void **const pp_value )
{

	// argument check
	if ( NULL == p_double_queue ) goto no_double_queue;
	
	// lock
	mutex_lock(&p_double_queue->_lock);

	// state check
	if ( p_double_queue->front == 0 ) goto no_double_queue_contents;

	// return a pointer to the rear element
	if ( pp_value )
		*pp_value = ((double_queue_node *)(p_double_queue->rear))->content;
	
	// unlock
	mutex_unlock(&p_double_queue->_lock);

	// exit
	return 1;

	// error handling
	{

		// argument errors
		{
			no_double_queue:
				#ifndef NDEBUG
					log_error("[double queue] Null pointer provided for parameter \"p_double_queue\" in call to function \"%s\"\n",__FUNCTION__);
				#endif
			
				// error
				return 0;

			no_double_queue_contents:
				#ifndef NDEBUG
					log_error("[double queue] Queue has no contents in call to function \"%s\"\n", __FUNCTION__);
				#endif
			
				// unlock
				mutex_unlock(&p_double_queue->_lock);

				// error
				return 0;
		}
	}
}

size_t double_queue_size ( double_queue *const p_double_queue )
{
	
	// argument check
	if ( NULL == p_double_queue ) goto no_double_queue;

	// success
	return p_double_queue->size;
	
	// error handling
	{

		// argument errors
		{
			no_double_queue:
				#ifndef NDEBUG
					log_error("[double queue] Null pointer provided for parameter \"p_double_queue\" in call to function \"%s\"\n", __FUNCTION__);
				#endif
			
				// error
				return 0;
		}
	}
}

bool double_queue_empty ( double_queue *const p_double_queue )
{
	
	// argument check
	if ( NULL == p_double_queue ) goto no_double_queue;

	// success
	return ( NULL == p_double_queue->front );
	
	// error handling
	{

		// argument errors
		{
			no_double_queue:
				#ifndef NDEBUG
					log_error("[double queue] Null pointer provided for parameter \"p_double_queue\" in call to function \"%s\"\n", __FUNCTION__);
				#endif
			
				// error
				return 0;
		}
	}
}

int double_queue_front_add ( double_queue *const p_double_queue, void *const p_value )
{

	// argument check
	if ( NULL == p_double_queue ) goto no_double_queue;

	// lock
	mutex_lock(&p_double_queue->_lock);

	// initialized data
	double_queue_node *e = p_double_queue->front, 
	                  *f = default_allocator(0, sizeof(double_queue_node));
	
	// error check
	if ( f == (void *) 0 ) goto no_mem;

	// zero set
	memset(f, 0, sizeof(double_queue_node));

	// Walk to the start from the rear
	if ( !e )
		e = p_double_queue->rear = p_double_queue->front = f;
	
	// Default
	else
	{

		// Walk the list
		while ( e->prev != 0 ) e = e->prev;
		
		e->prev = f;
		f->next = e;
		p_double_queue->front = f;
	}

	// store the content
	f->content = p_value,

	// increment the size
	p_double_queue->size++;
	
	// unlock
	mutex_unlock(&p_double_queue->_lock);

	// success
	return 1;
		
	// error handling
	{

		// argument errors
		{
			no_double_queue:
				#ifndef NDEBUG
					log_error("[double queue] Null pointer provided for parameter \"p_double_queue\" in call to function \"%s\"\n",__FUNCTION__);
				#endif

				// error
				return 0;
		}

		// standard library errors
		{
			no_mem:
				#ifndef NDEBUG
					printf("[standard library] Failed to allocate memory in call to function \"%s\"\n",__FUNCTION__);
				#endif

				// error
				return 0;
		}
	}
}

int double_queue_front_remove ( double_queue *const p_double_queue, void **const pp_value )
{
	
	// argument check
	if ( NULL == p_double_queue ) goto no_double_queue;
	
	// initialized data
	double_queue_node *ret_m = 0;

	// lock
	mutex_lock(&p_double_queue->_lock);
	
	// state check
	if ( p_double_queue->front == 0 ) goto queue_empty;

	// store the front node
	ret_m = ((double_queue_node *)(p_double_queue->front));

	// Remove the front
	if (p_double_queue->front != p_double_queue->rear)
		p_double_queue->front = ((double_queue_node *)(p_double_queue->front))->next,
		p_double_queue->front->prev = NULL;
	else
		p_double_queue->front = p_double_queue->rear = 0;
	

	// return the content to the caller
	if ( pp_value )
		*pp_value = ret_m->content;
	
	// release the node
	default_allocator(ret_m, 0);

	// decrement the size
	p_double_queue->size--;

	// unlock
	mutex_unlock(&p_double_queue->_lock);
	
	// success
	return 1;

	// error handling
	{

		// argument errors
		{
			no_double_queue:
				#ifndef NDEBUG
					log_error("[double queue] Null pointer provided for parameter \"p_double_queue\" in call to function \"%s\"\n",__FUNCTION__);
				#endif

				// error
				return 0;
		}

		// double queue errors
		{
			queue_empty:

				// No output...
				
				// unlock
				mutex_unlock(&p_double_queue->_lock);

				// error
				return 0;
		}
	}
}

int double_queue_rear_add ( double_queue *const p_double_queue, void *const p_value )
{

	// argument check
	if ( NULL == p_double_queue ) goto no_double_queue;

	// lock
	mutex_lock(&p_double_queue->_lock);

	// initialized data
	double_queue_node *q = p_double_queue->rear, // Q comes before R(ear)
	                  *r = default_allocator(0, sizeof(double_queue_node));
	
	// error check
	if ( r == (void *) 0 ) goto no_mem;

	// zero set
	memset(r, 0, sizeof(double_queue_node));

	// walk to the end from the rear
	if ( !q )
		q = p_double_queue->front = p_double_queue->rear = r;
	
	// Default
	else
	{

		// walk the list
		while ( q->next != 0 ) q = q->next;
		
		q->next = r,
		r->prev = q,
		p_double_queue->rear = r;
	}

	// store the content
	r->content = p_value,

	// increment the size
	p_double_queue->size++;
	
	// unlock
	mutex_unlock(&p_double_queue->_lock);

	// success
	return 1;
		
	// error handling
	{

		// argument errors
		{
			no_double_queue:
				#ifndef NDEBUG
					log_error("[double queue] Null pointer provided for parameter \"p_double_queue\" in call to function \"%s\"\n",__FUNCTION__);
				#endif

				// error
				return 0;
		}

		// standard library errors
		{
			no_mem:
				#ifndef NDEBUG
					printf("[standard library] Failed to allocate memory in call to function \"%s\"\n",__FUNCTION__);
				#endif

				// error
				return 0;
		}
	}
}

int double_queue_rear_remove ( double_queue *const p_double_queue, void **const pp_value )
{
	
	// argument check
	if ( NULL == p_double_queue ) goto no_double_queue;

	// initialized data
	double_queue_node *ret_m = 0;
	
	// lock
	mutex_lock(&p_double_queue->_lock);
	
	// state check
	if ( p_double_queue->rear == 0 ) goto queue_empty;
	
	// store the rearnode
	ret_m = ((double_queue_node *)(p_double_queue->rear));

	// Remove the rear
	if (p_double_queue->rear != p_double_queue->front)
		p_double_queue->rear = ((double_queue_node *)(p_double_queue->rear))->prev,
		p_double_queue->rear->next = NULL;
	else
		p_double_queue->rear = p_double_queue->front = 0;

	// return the content to the caller
	if ( pp_value )
		*pp_value = ret_m->content;

	// release the node
	default_allocator(ret_m, 0);

	// decrement the size
	p_double_queue->size--;

	// unlock
	mutex_unlock(&p_double_queue->_lock);
	
	// success
	return 1;

	// error handling
	{

		// argument errors
		{
			no_double_queue:
				#ifndef NDEBUG
					log_error("[double queue] Null pointer provided for parameter \"p_double_queue\" in call to function \"%s\"\n",__FUNCTION__);
				#endif

				// error
				return 0;
		}

		// double queue errors
		{
			queue_empty:

				// No output...
				
				// unlock
				mutex_unlock(&p_double_queue->_lock);

				// error
				return 0;
		}
	}
}

int double_queue_map ( double_queue *const p_double_queue, fn_map *pfn_map, fn_allocator *pfn_allocator )
{

	// argument check
	if ( NULL == p_double_queue ) goto no_double_queue;
	if ( NULL ==        pfn_map ) goto no_map;

	// initialized data
	double_queue_node *p_iter = NULL;

	// lock
	mutex_lock(&p_double_queue->_lock);

	// store the front of the double ended queue
	p_iter = p_double_queue->front;

	// walk the double ended queue
	while ( p_iter )
	{

		// initialized data
        void *p_old = p_iter->content,
             *p_new = NULL;

        // update
        p_new = pfn_map(p_old),
        p_iter->content = p_new;

        // release
        if ( pfn_allocator && p_old != p_new )
            pfn_allocator(p_old, 0);

		// step
		p_iter = p_iter->next;
	}

	// unlock
	mutex_unlock(&p_double_queue->_lock);

	// success
	return 1;

	// error handling
	{

		// argument errors
		{
			no_double_queue:
				#ifndef NDEBUG
					log_error("[double queue] Null pointer provided for parameter \"p_double_queue\" in call to function \"%s\"\n",__FUNCTION__);
				#endif

				// error
				return 0;

			no_map:
				#ifndef NDEBUG
					log_error("[double queue] Null pointer provided for parameter \"pfn_map\" in call to function \"%s\"\n",__FUNCTION__);
				#endif

				// error
				return 0;
		}
	}
}

int double_queue_foreach ( double_queue *const p_double_queue, fn_foreach *pfn_foreach )
{

	// argument check
	if ( NULL == p_double_queue ) goto no_double_queue;
	if ( NULL ==    pfn_foreach ) goto no_foreach;

	// initialized data
	double_queue_node *p_iter = NULL;

	// lock
	mutex_lock(&p_double_queue->_lock);

	// store the front of the double ended queue
	p_iter = p_double_queue->front;

	// walk the double ended queue
	while ( p_iter )
	{

		// call the foreach function
		pfn_foreach(p_iter->content);

		// step
		p_iter = p_iter->next;
	}

	// unlock
	mutex_unlock(&p_double_queue->_lock);

	// success
	return 1;

	// error handling
	{

		// argument errors
		{
			no_double_queue:
				#ifndef NDEBUG
					log_error("[double queue] Null pointer provided for parameter \"p_double_queue\" in call to function \"%s\"\n",__FUNCTION__);
				#endif

				// error
				return 0;

			no_foreach:
				#ifndef NDEBUG
					log_error("[double queue] Null pointer provided for parameter \"pfn_foreach\" in call to function \"%s\"\n",__FUNCTION__);
				#endif

				// error
				return 0;
		}
	}
}

int double_queue_fori ( double_queue *const p_double_queue, fn_fori *pfn_fori )
{

	// argument check
	if ( NULL == p_double_queue ) goto no_double_queue;
	if ( NULL ==       pfn_fori ) goto no_fori;

	// initialized data
	double_queue_node *p_iter = NULL;
	size_t             i      = 0;

	// lock
	mutex_lock(&p_double_queue->_lock);

	// store the front of the double ended queue
	p_iter = p_double_queue->front;

	// walk the double ended queue
	while ( p_iter )
	{

		// call the fori function
		pfn_fori(p_iter->content, i);

		// step
		i++, p_iter = p_iter->next;
	}

	// unlock
	mutex_unlock(&p_double_queue->_lock);

	// success
	return 1;

	// error handling
	{

		// argument errors
		{
			no_double_queue:
				#ifndef NDEBUG
					log_error("[double queue] Null pointer provided for parameter \"p_double_queue\" in call to function \"%s\"\n",__FUNCTION__);
				#endif

				// error
				return 0;

			no_fori:
				#ifndef NDEBUG
					log_error("[double queue] Null pointer provided for parameter \"pfn_fori\" in call to function \"%s\"\n",__FUNCTION__);
				#endif

				// error
				return 0;
		}
	}
}

int double_queue_pack ( void *p_buffer, double_queue *p_double_queue, fn_pack *pfn_element )
{

	// argument check
    if ( NULL == p_double_queue ) goto no_double_queue;
    if ( NULL ==    pfn_element ) goto no_pack;

    // initialized data 
    char *p = p_buffer;
	double_queue_node *p_iter = NULL;

    // lock
    mutex_lock(&p_double_queue->_lock);
	
	// store the front of the double ended queue
	p_iter = p_double_queue->front;

	// pack the length of the double ended queue
	p += pack_pack(p, "%i64", p_double_queue->size);

	// walk the double ended queue
	while ( p_iter )
	{

		// call the pack function
		p += pfn_element(p, p_iter->content);

		// step
		p_iter = p_iter->next;
	}

    // unlock
    mutex_unlock(&p_double_queue->_lock);

    // success
    return p - (char *)p_buffer;

    // error handling
    {
        
        // argument errors
        {
            no_double_queue:
                #ifndef NDEBUG
                    log_error("[double queue] Null pointer provided for parameter \"p_double_queue\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
			
			no_pack:
				#ifndef NDEBUG
                    log_error("[double queue] Null pointer provided for parameter \"pfn_pack\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
    }
}

int double_queue_unpack ( double_queue **pp_double_queue, void *p_buffer, fn_unpack *pfn_element )
{
	
	// argument check
    if ( NULL == pp_double_queue ) goto no_double_queue;
	if ( NULL ==        p_buffer ) goto no_buffer;
    if ( NULL ==     pfn_element ) goto no_unpack;

    // initialized data 
	double_queue *p_double_queue = NULL;
    char         *p      = p_buffer;
	size_t        size   = 0;

	// unpack the size of the double ended queue
	p += pack_unpack(p, "%i64", &size);

	// construct a double ended queue
	if ( 0 == double_queue_construct(&p_double_queue) ) goto failed_to_construct_double_queue;

	// iterate over each element
	while ( size-- )
	{

		// initialized data
		void *p_element = NULL;

		// call the foreach function
		p += pfn_element(&p_element, p);

		// add the element to the double ended queue
		double_queue_front_add(p_double_queue, p_element);
	}

	// return a pointer to the caller
	*pp_double_queue = p_double_queue;

    // success
	return p - (char *)p_buffer;

    // error handling
    {
        
        // argument errors
        {
            no_double_queue:
                #ifndef NDEBUG
                    log_error("[double queue] Null pointer provided for \"pp_double_queue\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;

			no_buffer:
                #ifndef NDEBUG
                    log_error("[double queue] Null pointer provided for \"p_buffer\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;

			no_unpack:
                #ifndef NDEBUG
                    log_error("[double queue] Null pointer provided for \"pfn_unpack\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }

		// double queue errors
		{
			failed_to_construct_double_queue:
				#ifndef NDEBUG
					log_error("[double queue] Failed to construct double queue in call to function \"%s\"\n", __FUNCTION__);
				#endif

				// error
				return 0;
		}
    }
}


hash64 double_queue_hash ( double_queue *p_double_queue, fn_hash64 *pfn_element )
{

	// argument check
	if ( NULL == p_double_queue ) goto no_double_queue;

	// initialized data
	hash64             result     = 0;
	double_queue_node *p_iter     = NULL;
    fn_hash64         *pfn_hash64 = (pfn_element) ? pfn_element : default_hash;

	// lock
	mutex_lock(&p_double_queue->_lock);

	// store the front of the double ended queue
	p_iter = p_double_queue->front;

	// walk the double ended queue
	while ( p_iter )
	{

		// call the hash function
        result ^= pfn_hash64(p_iter->content, 8);

		// step
		p_iter = p_iter->next;
	}

	// unlock
	mutex_unlock(&p_double_queue->_lock);

	// success
	return result;

	// error handling
	{

		// argument errors
		{
			no_double_queue:
				#ifndef NDEBUG
					log_error("[double queue] Null pointer provided for parameter \"p_double_queue\" in call to function \"%s\"\n",__FUNCTION__);
				#endif

				// error
				return 0;
		}
	}
}

int double_queue_destroy ( double_queue **const pp_double_queue, fn_allocator *pfn_allocator )
{

	// argument check
	if ( pp_double_queue == (void *) 0 ) goto no_double_queue;

	// initialized data
	double_queue      *p_double_queue = *pp_double_queue;
	double_queue_node *p_iter         = NULL;
	
	// lock
	mutex_lock(&p_double_queue->_lock);

	// no more queue for caller
	*pp_double_queue = 0;

	// unlock
	mutex_unlock(&p_double_queue->_lock);

	// store the front of the double ended queue
	p_iter = p_double_queue->front;

	// walk the double ended queue
	while ( p_iter )
	{

		// initialized data
		double_queue_node *p_prev = p_iter;

		// step
		p_iter = p_iter->next;

		// release the content
		if ( pfn_allocator )
			pfn_allocator(p_prev->content, 0);

		// release the node
		default_allocator(p_prev, 0);
	}

	// release the double ended queue
	p_double_queue = default_allocator(p_double_queue, 0);
		
	// success
	return 1;

	// error handling
	{

		// argument errors
		{
			no_double_queue:
				#ifndef NDEBUG
					log_error("[double queue] Null pointer provided for parameter \"pp_double_queue\" in call to function \"%s\"\n",__FUNCTION__);
				#endif

				// error
				return 0;
		}
	}
}
