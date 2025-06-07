/** !
 * Queue implementation
 * 
 * @file queue.c
 * 
 * @author Jacob Smith
 */

// Header
#include <data/queue.h>

// Structure definitions
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
	mutex _lock;
};

// Data
static bool initialized = false;

void queue_init ( void )
{

    // State check
    if ( initialized == true ) return;

    // Initialize the log library
    log_init();

    // Initialize the sync library
    sync_init();

    // Set the initialized flag
    initialized = true;

    // Done
    return;
}

// Function definitions
int queue_create ( queue **const pp_queue )
{

	// Argument check
	if ( pp_queue == (void *) 0 ) goto no_queue;

	// Initialized data
	queue *ret = QUEUE_REALLOC(0, sizeof(queue));

	// Error check
	if ( ret == (void *)0 ) goto no_mem;

	// Zero set
	memset(ret, 0, sizeof(queue));

	// Return the queue
	*pp_queue = ret;

	// Success
	return 1;

	// Error handling
	{

		// Argument errors
		{
			no_queue:
				#ifndef NDEBUG
					log_error("[queue] Null pointer provided for \"pp_queue\" in call to function \"%s\"\n",__FUNCTION__);
				#endif

				// Error
				return 0;
		}

		// standard library errors
		{
			no_mem:
				#ifndef NDEBUG
					log_error("[Standard Library] Failed to allocate memory in call to function \"%s\"\n",__FUNCTION__);
				#endif

				// Error
				return 0;
		}
	}
}
 
int queue_construct ( queue **const pp_queue )
{

	// Argument check
	if ( pp_queue == (void *) 0 ) goto no_queue;

	// Initialized data
	queue *p_queue = 0;

	// Allocate for a queue
	if ( queue_create(pp_queue) == 0 ) goto failed_to_create_queue;

	// Get a pointer to the allocated memory
	p_queue = *pp_queue;

	// Create a mutex
    if ( mutex_create(&p_queue->_lock) == 0 ) goto failed_to_create_mutex;

	// Success
	return 1;

	// Error handling
	{

		// Argument errors
		{
			no_queue:
				#ifndef NDEBUG
					log_error("[queue] Null pointer provided for \"pp_queue\" in call to function \"%s\"\n",__FUNCTION__);
				#endif
			
				// Error
				return 0;
		}

		// Queue errors
		{
			failed_to_create_queue:
				#ifndef NDEBUG
					log_error("[queue] Failed to create queue in call to function \"%s\"\n", __FUNCTION__);
				#endif

				// Error
				return 0;
			
			failed_to_create_mutex:
                #ifndef NDEBUG
                	log_error("[queue] Failed to create mutex in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
		}
	}
}
 
int queue_from_contents ( queue **const pp_queue, void* const* const pp_contents, size_t size )
{

	// Argument check
	if ( pp_queue    == (void *) 0 ) goto no_queue;
	if ( pp_contents == (void *) 0 ) goto no_queue_contents;
	if ( size        ==          0 ) goto no_queue_contents;

	// Construct a queue
	if ( queue_construct(pp_queue) == 0 ) goto failed_to_construct_queue;
	
	// Iterate over each item
	for (size_t i = 0; i < size; i++)

		// Add the item to the queue
		queue_enqueue(*pp_queue, pp_contents[i]);
	
	// Success
	return 1;

	// Error handling
	{

		// Argument errors
		{
			no_queue:
				#ifndef NDEBUG
					log_error("[queue] Null pointer provided for \"p_queue\" in call to function \"%s\"\n",__FUNCTION__);
				#endif
			
				// Error
				return 0;

			no_queue_contents:
				#ifndef NDEBUG
					log_error("[queue] Queue is empty in call to function \"%s\"\n", __FUNCTION__);
				#endif
			
				// Error
				return 0;
		}

		// Queue errors
		{
			failed_to_construct_queue:
				#ifndef NDEBUG
					log_error("[queue] Failed to construct queue in call to function \"%s\"\n", __FUNCTION__);
				#endif
			
				// Error
				return 0;
		}
	}
}

int queue_front ( queue *const p_queue, void ** const pp_value )
{

	// Argument check
	if ( p_queue == (void *) 0 ) goto no_queue;

	// Lock
	mutex_lock(&p_queue->_lock);

	// State check
	if ( p_queue->front == 0 ) goto no_queue_contents;

	// Return a pointer to the caller
	if ( pp_value )
		*pp_value = ((struct queue_node_s *)(p_queue->front))->content;
	
	// Unlock
	mutex_unlock(&p_queue->_lock);

	// Exit
	return 1;

	// Error handling
	{

		// Argument errors
		{
			no_queue:
				#ifndef NDEBUG
					log_error("[queue] Null pointer provided for \"p_queue\" in call to function \"%s\"\n",__FUNCTION__);
				#endif
			
				// Error
				return 0;

			no_queue_contents:
				#ifndef NDEBUG
					log_error("[queue] Queue is empty in call to function \"%s\"\n", __FUNCTION__);
				#endif

				// Unlock
				mutex_unlock(&p_queue->_lock);

				// Error
				return 0;		
		}
	}
}
 
int queue_rear ( queue *const p_queue, void **const pp_value )
{

	// Argument check
	if ( p_queue == (void *) 0 ) goto no_queue;
	
	// Lock
	mutex_lock(&p_queue->_lock);

	// State check
	if ( p_queue->front == 0 ) goto no_queue_contents;

	// Return a pointer to the rear element
	if ( pp_value )
		*pp_value = ((struct queue_node_s *)(p_queue->rear))->content;
	
	// Unlock
	mutex_unlock(&p_queue->_lock);

	// Exit
	return 1;

	// Error handling
	{

		// Argument errors
		{
			no_queue:
				#ifndef NDEBUG
					log_error("[queue] Null pointer provided for \"p_queue\" in call to function \"%s\"\n",__FUNCTION__);
				#endif
			
				// Error
				return 0;

			no_queue_contents:
				#ifndef NDEBUG
					log_error("[queue] Queue is empty in call to function \"%s\"\n", __FUNCTION__);
				#endif
			
				// Unlock
				mutex_unlock(&p_queue->_lock);

				// Error
				return 0;
		}
	}
}
 
int queue_enqueue ( queue *const p_queue, void *const data )
{

	// Argument check
	if ( p_queue == (void *) 0 ) goto no_queue;

	// Lock
	mutex_lock(&p_queue->_lock);

	// Initialized data
	struct queue_node_s *q = p_queue->rear, // Q comes before R(ear)
	                    *r = QUEUE_REALLOC(0, sizeof(struct queue_node_s));
	
	// Error check
	if ( r == (void *) 0 ) goto no_mem;

	// Zero set
	memset(r, 0, sizeof(struct queue_node_s));

	// Walk to the end from the rear
	if ( !q )
		q = p_queue->front = p_queue->rear = r;
	
	// Default
	else
	{

		// Walk the list
		while ( q->next != 0 ) q = q->next;
		
		q->next = r;
		r->prev = q;
		p_queue->rear = r;
	}

	r->content = data;
	
	// Unlock
	mutex_unlock(&p_queue->_lock);

	// Success
	return 1;
		
	// Error handling
	{

		// Argument errors
		{
			no_queue:
				#ifndef NDEBUG
					log_error("[queue] Null pointer provided for \"p_queue\" in call to function \"%s\"\n",__FUNCTION__);
				#endif

				// Error
				return 0;
		}

		// standard library errors
		{
			no_mem:
				#ifndef NDEBUG
					log_error("[Standard Library] Failed to allocate memory in call to function \"%s\"\n",__FUNCTION__);
				#endif

				// Unlock
				mutex_unlock(&p_queue->_lock);

				// Error
				return 0;
		}
	}
}
 
int queue_dequeue ( queue *const p_queue, void **const pp_value )
{
	
	// Argument check
	if ( p_queue == (void *) 0 ) goto no_queue;
	
	// Lock
	mutex_lock(&p_queue->_lock);
	
	// State check
	if ( p_queue->front == 0 ) goto queue_empty;

	// Initialized data
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
	ret_m = QUEUE_REALLOC(ret_m, 0);
		
	// Unlock
	mutex_unlock(&p_queue->_lock);
	
	// Success
	return 1;

	// Error handling
	{

		// Argument errors
		{
			no_queue:
				#ifndef NDEBUG
					log_error("[queue] Null pointer provided for \"p_queue\" in call to function \"%s\"\n",__FUNCTION__);
				#endif

				// Error
				return 0;
		}

		// Queue errors
		{
			queue_empty:

				// No output...
				
				// Unlock
				mutex_unlock(&p_queue->_lock);

				// Error
				return 0;
		}
	}
}

bool queue_empty ( queue *const p_queue )
{
	
	// Argument check
	if ( p_queue == (void *)0 ) goto no_queue;

	// Lock
	mutex_lock(&p_queue->_lock);

	// Initialized data
	bool ret = ( p_queue->front == 0 );

	// Unlock
	mutex_unlock(&p_queue->_lock);
	
	// Success
	return ret;
	
	// Error handling
	{

		// Argument errors
		{
			no_queue:
				#ifndef NDEBUG
					log_error("[queue] Null pointer provided for parameter \"p_queue\" in call to function \"%s\"\n", __FUNCTION__);
				#endif
			
				// Error
				return 0;
		}
	}
}

int queue_destroy ( queue **const pp_queue )
{

	// Argument check
	if ( pp_queue == (void *) 0 ) goto no_queue;

	// Initialized data
	queue *p_queue = *pp_queue;
	
	// Lock
	mutex_lock(&p_queue->_lock);

	// No more queue for end user
	*pp_queue = 0;

	// Unlock
	mutex_unlock(&p_queue->_lock);

	// Free the memory
	p_queue = QUEUE_REALLOC(p_queue, 0);
		
	// Success
	return 1;

	// Error handling
	{

		// Argument errors
		{
			no_queue:
				#ifndef NDEBUG
					log_error("[queue] Null pointer provided for \"pp_queue\" in call to function \"%s\"\n",__FUNCTION__);
				#endif

				// Error
				return 0;
		}
	}
}

void queue_exit ( void )
{
    
    // State check
    if ( initialized == false ) return;

    // Clean up the log library
    log_exit();

    // Clean up the sync library
    sync_exit();

    // Clear the initialized flag
    initialized = false;

    // Done
    return;
}
