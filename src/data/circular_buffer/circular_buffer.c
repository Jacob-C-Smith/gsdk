/** !
 * Circular buffer implementation
 * 
 * @file circular_buffer.c
 * 
 * @author Jacob Smith
 */

// header
#include <data/circular_buffer.h>

// function definitions
int circular_buffer_create ( circular_buffer **const pp_circular_buffer )
{

	// argument check
	if ( pp_circular_buffer == (void *) 0 ) goto no_circular_buffer;

	// initialized data
	circular_buffer *ret = default_allocator(0, sizeof(circular_buffer));

	// error check
	if ( ret == (void *)0 ) goto no_mem;

	// Zero set
	memset(ret, 0, sizeof(circular_buffer));

	// return a pointer to the caller
	*pp_circular_buffer = ret;

	// success
	return 1;

	// error handling
	{

		// argument errors
		{
			no_circular_buffer:
				#ifndef NDEBUG
					log_error("[circular buffer] Null pointer provided for \"pp_circular_buffer\" in call to function \"%s\"\n",__FUNCTION__);
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
 
int circular_buffer_construct ( circular_buffer **const pp_circular_buffer, size_t size )
{

	// argument check
	if ( pp_circular_buffer == (void *) 0 ) goto no_circular_buffer;
	if ( size               ==          0 ) goto no_size;

	// initialized data
	circular_buffer *p_circular_buffer = 0;

	// allocate memory for a circular buffer
	if ( circular_buffer_create(&p_circular_buffer) == 0 ) goto failed_to_create_circular_buffer;

	// Grow the circular buffer
	p_circular_buffer = default_allocator(p_circular_buffer, sizeof(circular_buffer) + ( size * sizeof(void *) ));

	// error check
	if ( p_circular_buffer == (void *) 0 ) goto no_mem;

	// store the size of the circular buffer
	p_circular_buffer->length = size;

	// Create a mutex
    if ( mutex_create(&p_circular_buffer->_lock) == 0 ) goto failed_to_create_mutex;

	// return a pointer to the caller
	*pp_circular_buffer = p_circular_buffer;

	// success
	return 1;

	// error handling
	{

		// argument errors
		{
			no_circular_buffer:
				#ifndef NDEBUG
					log_error("[circular buffer] Null pointer provided for parameter \"pp_circular_buffer\" in call to function \"%s\"\n", __FUNCTION__);
				#endif
			
				// error
				return 0;

			no_size:
				#ifndef NDEBUG
					log_error("[circular buffer] Parameter \"size\" must be greater than zero in call to function \"%s\"\n", __FUNCTION__);
				#endif
			
				// error
				return 0;
		}

		// Circular buffer errors
		{
			failed_to_create_circular_buffer:
				#ifndef NDEBUG
					log_error("[circular buffer] Failed to allocate circular buffer in call to function \"%s\"\n", __FUNCTION__);
				#endif

				// error
				return 0;
			
			failed_to_create_mutex:
                #ifndef NDEBUG
                    log_error("[circular buffer] Failed to create mutex in call to function \"%s\"\n", __FUNCTION__);
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

int circular_buffer_from_contents ( circular_buffer **const pp_circular_buffer, const void *const *pp_contents, size_t size )
{

	// argument check
	if ( pp_circular_buffer == (void *) 0 ) goto no_circular_buffer;
	if ( pp_contents        == (void *) 0 ) goto no_circular_buffer_contents;
	if ( size               ==          0 ) goto no_circular_buffer_contents;

	// initialized data
	circular_buffer *p_circular_buffer = (void *) 0;

	// Construct a circular buffer
	if ( circular_buffer_construct(&p_circular_buffer, size) == 0 ) goto failed_to_construct_circular_buffer;
	
	// iterate over each item
	for (size_t i = 0; i < size; i++)

		// Add the item to the circular buffer
		circular_buffer_push(p_circular_buffer, (void *)pp_contents[i]);
	
	// return a pointer to the caller
	*pp_circular_buffer = p_circular_buffer;
	
	// success
	return 1;

	// error handling
	{

		// argument errors
		{
			no_circular_buffer:
				#ifndef NDEBUG
					log_error("[circular buffer] Null pointer provided for parameter \"pp_circular_buffer\" in call to function \"%s\"\n",__FUNCTION__);
				#endif
			
				// error
				return 0;

			no_circular_buffer_contents:
				#ifndef NDEBUG
					log_error("[circular buffer] Circular buffer has no contents in call to function \"%s\"\n", __FUNCTION__);
				#endif
			
				// error
				return 0;
		}

		// Circular buffer errors
		{
			failed_to_construct_circular_buffer:
				#ifndef NDEBUG
					log_error("[circular buffer] Failed to construct circular buffer in call to function \"%s\"\n", __FUNCTION__);
				#endif
			
				// error
				return 0;
		}
	}
}

bool circular_buffer_empty ( circular_buffer *const p_circular_buffer )
{
	
	// argument check
	if ( p_circular_buffer == (void *)0 ) goto no_circular_buffer;

	// lock
	mutex_lock(&p_circular_buffer->_lock);

	// initialized data
	bool ret = ( p_circular_buffer->full == false && p_circular_buffer->read == p_circular_buffer->write );

	// unlock
	mutex_unlock(&p_circular_buffer->_lock);
	
	// success
	return ret;
	
	// error handling
	{

		// argument errors
		{
			no_circular_buffer:
				#ifndef NDEBUG
					log_error("[circular buffer] Null pointer provided for parameter \"p_circular_buffer\" in call to function \"%s\"\n", __FUNCTION__);
				#endif
			
				// error
				return 0;
		}
	}
}

bool circular_buffer_full ( circular_buffer *const p_circular_buffer )
{
	
	// argument check
	if ( p_circular_buffer == (void *)0 ) goto no_circular_buffer;

	// lock
	mutex_lock(&p_circular_buffer->_lock);

	// initialized data
	bool ret = p_circular_buffer->full;

	// unlock
	mutex_unlock(&p_circular_buffer->_lock);
	
	// success
	return ret;
	
	// error handling
	{

		// argument errors
		{
			no_circular_buffer:
				#ifndef NDEBUG
					log_error("[circular buffer] Null pointer provided for parameter \"p_circular_buffer\" in call to function \"%s\"\n", __FUNCTION__);
				#endif
			
				// error
				return 0;
		}
	}
}

int circular_buffer_push ( circular_buffer *const p_circular_buffer, void *p_data )
{

	// argument check
	if ( p_circular_buffer == (void *) 0 ) goto no_circular_buffer;
	if ( p_data            == (void *) 0 ) goto no_data;
		
	// lock
	mutex_lock(&p_circular_buffer->_lock);

	// store the element
	p_circular_buffer->_p_data[p_circular_buffer->write] = p_data;

	// Update the write index
	p_circular_buffer->write = ( p_circular_buffer->write + 1 ) % p_circular_buffer->length;

	// Handle overflows
	if ( p_circular_buffer->full ) goto overflow;

	// Update the full flag
	p_circular_buffer->full = ( p_circular_buffer->read == p_circular_buffer->write );

	// unlock
	mutex_unlock(&p_circular_buffer->_lock);

	// success
	return 1;

	overflow:
	{

		// Update the read index
		p_circular_buffer->read = ( p_circular_buffer->read + 1 ) % p_circular_buffer->length;

		// unlock
		mutex_unlock(&p_circular_buffer->_lock);

		// success
		return 1;
	}

	// error handling
	{

		// argument errors
		{
			no_circular_buffer:
				#ifndef NDEBUG
					log_error("[circular buffer] Null pointer provided for parameter \"p_circular_buffer\" in call to function \"%s\"\n", __FUNCTION__);
				#endif

				// error
				return 0;

			no_data:
				#ifndef NDEBUG
					log_error("[circular buffer] Null pointer provided for parameter \"p_data\" in call to function \"%s\"\n", __FUNCTION__);
				#endif

				// error
				return 0;
		}
	}
}

int circular_buffer_peek ( circular_buffer *const p_circular_buffer, void **pp_data )
{

	// argument check
	if ( p_circular_buffer == (void *) 0 ) goto no_circular_buffer;
	if ( pp_data           == (void *) 0 ) goto no_data;

	// lock
	mutex_lock(&p_circular_buffer->_lock);

	// state check
	if ( p_circular_buffer->full == false && p_circular_buffer->read == p_circular_buffer->write ) goto circular_buffer_empty;

	// return data to the caller
	*pp_data = p_circular_buffer->_p_data[p_circular_buffer->read];

	// unlock
	mutex_unlock(&p_circular_buffer->_lock);

	// success
	return 1;

	// Empty
	circular_buffer_empty:
	{

		// unlock
		mutex_unlock(&p_circular_buffer->_lock);

		// error
		return 0;
	}

	// error handling
	{

		// argument errors
		{
			no_circular_buffer:
				#ifndef NDEBUG
					log_error("[circular buffer] Null pointer provided for parameter \"p_circular_buffer\" in call to function \"%s\"\n", __FUNCTION__);
				#endif

				// error
				return 0;

			no_data:
				#ifndef NDEBUG
					log_error("[circular buffer] Null pointer provided for parameter \"pp_data\" in call to function \"%s\"\n", __FUNCTION__);
				#endif

				// error
				return 0;
		}
	}
}

int circular_buffer_pop ( circular_buffer *const p_circular_buffer, void **pp_data )
{

	// argument check
	if ( p_circular_buffer == (void *) 0 ) goto no_circular_buffer;
	if ( pp_data           == (void *) 0 ) goto no_data;

	// lock
	mutex_lock(&p_circular_buffer->_lock);
	
	// Check if buffer is empty
	if ( p_circular_buffer->full == false && p_circular_buffer->read == p_circular_buffer->write ) goto circular_buffer_empty;

	// initialized data
	void *p_data = p_circular_buffer->_p_data[p_circular_buffer->read];

	// Update the read index
	p_circular_buffer->read = ( p_circular_buffer->read + 1 ) % p_circular_buffer->length;

	// Clear the full flag
	p_circular_buffer->full = false;

	// return a pointer to the caller
	*pp_data = p_data;

	// unlock
	mutex_unlock(&p_circular_buffer->_lock);

	// success
	return 1;

	// Empty buffer
	circular_buffer_empty:
	{

		// unlock
		mutex_unlock(&p_circular_buffer->_lock);

		// error
		return 0;
	}

	// error handling
	{

		// argument errors
		{
			no_circular_buffer:
				#ifndef NDEBUG
					log_error("[circular buffer] Null pointer provided for parameter \"p_circular_buffer\" in call to function \"%s\"\n", __FUNCTION__);
				#endif

				// error
				return 0;

			no_data:
				#ifndef NDEBUG
					log_error("[circular buffer] Null pointer provided for parameter \"pp_data\" in call to function \"%s\"\n", __FUNCTION__);
				#endif

				// error
				return 0;
		}
	}
}

int circular_buffer_destroy ( circular_buffer **const pp_circular_buffer )
{

	// argument check
	if ( pp_circular_buffer == (void *) 0 ) goto no_circular_buffer;

	// initialized data
	circular_buffer *p_circular_buffer = *pp_circular_buffer;
	
	// lock
	mutex_lock(&p_circular_buffer->_lock);

	// No more circular buffer for end user
	*pp_circular_buffer = 0;

	// unlock
	mutex_unlock(&p_circular_buffer->_lock);

	// Empty the circular buffer
	while (!circular_buffer_empty(p_circular_buffer))
	{
		void *p_data = 0;
		circular_buffer_pop(p_circular_buffer, &p_data);
		// Note: If the stored items need custom cleanup,
		// it should be done here or documented that the user
		// must empty the buffer before destroying it
	}

	// Destroy the mutex
	mutex_destroy(&p_circular_buffer->_lock);

	// Free the memory
	p_circular_buffer = default_allocator(p_circular_buffer, 0);
		
	// success
	return 1;

	// error handling
	{

		// argument errors
		{
			no_circular_buffer:
				#ifndef NDEBUG
					log_error("[circular buffer] Null pointer provided for \"pp_circular_buffer\" in call to function \"%s\"\n", __FUNCTION__);
				#endif

				// error
				return 0;
		}
	}
}
