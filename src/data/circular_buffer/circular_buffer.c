/** !
 * Circular buffer implementation
 * 
 * @file src/data/circular_buffer/circular_buffer.c
 * 
 * @author Jacob Smith
 */

// header
#include <data/circular_buffer.h>

// function definitions
int circular_buffer_construct ( circular_buffer **const pp_circular_buffer, size_t size )
{

	// argument check
	if ( NULL == pp_circular_buffer ) goto no_circular_buffer;
	if ( 0    ==               size ) goto no_size;

	// initialized data
	circular_buffer *p_circular_buffer = NULL;

	// allocate memory for a circular buffer
	p_circular_buffer = default_allocator(p_circular_buffer, sizeof(circular_buffer) + ( size * sizeof(void *) ));
	if ( p_circular_buffer == (void *) 0 ) goto no_mem;

	// initialize the circular buffer
	p_circular_buffer->length = size;
	p_circular_buffer->full   = false;
	p_circular_buffer->read   = 0;
	p_circular_buffer->write  = 0;

	// create a mutex
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

		// standard library errors
		{
			no_mem:
				#ifndef NDEBUG
					log_error("[standard library] Failed to allocate memory in call to function \"%s\"\n",__FUNCTION__);
				#endif

				// error
				return 0;
		}

		// sync errors
		{
			failed_to_create_mutex:
				#ifndef NDEBUG
					log_error("[sync] Failed to create mutex in call to function \"%s\"\n", __FUNCTION__);
				#endif

				// error
				return 0;
		}
	}
}

int circular_buffer_from_contents ( circular_buffer **const pp_circular_buffer, const void *const *pp_contents, size_t size )
{

	// argument check
	if ( NULL == pp_circular_buffer ) goto no_circular_buffer;
	if ( NULL ==        pp_contents ) goto no_circular_buffer_contents;
	if ( 0    ==               size ) goto no_circular_buffer_contents;

	// initialized data
	circular_buffer *p_circular_buffer = (void *) 0;

	// construct a circular buffer
	if ( circular_buffer_construct(&p_circular_buffer, size) == 0 ) goto failed_to_construct_circular_buffer;
	
	// iterate over each item
	for (size_t i = 0; i < size; i++)

		// add the item to the circular buffer
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

		// circular buffer errors
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
	if ( NULL == p_circular_buffer ) goto no_circular_buffer;

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
	if ( NULL == p_circular_buffer ) goto no_circular_buffer;

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

size_t circular_buffer_size ( circular_buffer *p_circular_buffer )
{

	// argument check
	if ( NULL == p_circular_buffer ) goto no_circular_buffer;

	// initialized data
	size_t size = 0;

	// lock
	mutex_lock(&p_circular_buffer->_lock);

	// calculate the size
	size = ( p_circular_buffer->full ) ?
	         p_circular_buffer->length : 
		   ( p_circular_buffer->write + p_circular_buffer->length - p_circular_buffer->read ) % p_circular_buffer->length;

	// unlock
	mutex_unlock(&p_circular_buffer->_lock);
	
	// success
	return size;
	
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

int circular_buffer_peek ( circular_buffer *const p_circular_buffer, void **pp_data )
{

	// argument check
	if ( NULL == p_circular_buffer ) goto no_circular_buffer;
	if ( NULL ==           pp_data ) goto no_data;

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

	// empty
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

int circular_buffer_push ( circular_buffer *const p_circular_buffer, void *p_data )
{

	// argument check
	if ( NULL == p_circular_buffer ) goto no_circular_buffer;
	if ( NULL ==            p_data ) goto no_data;
		
	// lock
	mutex_lock(&p_circular_buffer->_lock);

	// store the element
	p_circular_buffer->_p_data[p_circular_buffer->write] = p_data;

	// update the write index
	p_circular_buffer->write = ( p_circular_buffer->write + 1 ) % p_circular_buffer->length;

	// handle overflows
	if ( p_circular_buffer->full ) goto overflow;

	// update the full flag
	p_circular_buffer->full = ( p_circular_buffer->read == p_circular_buffer->write );

	// unlock
	mutex_unlock(&p_circular_buffer->_lock);

	// success
	return 1;

	overflow:
	{

		// update the read index
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

int circular_buffer_pop ( circular_buffer *const p_circular_buffer, void **pp_data )
{

	// argument check
	if ( NULL == p_circular_buffer ) goto no_circular_buffer;

	// lock
	mutex_lock(&p_circular_buffer->_lock);
	
	// Check if buffer is empty
	if ( p_circular_buffer->full == false && p_circular_buffer->read == p_circular_buffer->write ) goto circular_buffer_empty;

	// initialized data
	void *p_data = p_circular_buffer->_p_data[p_circular_buffer->read];

	// update the read index
	p_circular_buffer->read = ( p_circular_buffer->read + 1 ) % p_circular_buffer->length;

	// clear the full flag
	p_circular_buffer->full = false;

	// return a pointer to the caller
	if ( pp_data)
		*pp_data = p_data;

	// unlock
	mutex_unlock(&p_circular_buffer->_lock);

	// success
	return 1;

	// empty buffer
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
		}
	}
}

int circular_buffer_map ( circular_buffer *const p_circular_buffer, fn_map *pfn_map, fn_allocator *pfn_allocator )
{

    // argument check
    if ( NULL == p_circular_buffer ) goto no_circular_buffer;
    if ( NULL ==           pfn_map ) goto no_fn_map;

    // initialized data
    size_t i = 0;
    bool   f = false;

    // lock
    mutex_lock(&p_circular_buffer->_lock);

    // check if the buffer is empty
    if ( 
        p_circular_buffer->full == false                    &&
        p_circular_buffer->read == p_circular_buffer->write 
    ) goto done;

    // store the index and full flag 
    i = p_circular_buffer->read,
    f = p_circular_buffer->full;

    // iterate over each element
    while ( f || i != p_circular_buffer->write )
    {

        // initialized data
        void *p_old = p_circular_buffer->_p_data[i],
             *p_new = NULL;

        // update
        p_new = pfn_map(p_old),
        p_circular_buffer->_p_data[i] = p_new;

        // release
        if ( pfn_allocator && p_old != p_new )
            pfn_allocator(p_old, 0);

        // update the index
        i = ( i + 1 ) % p_circular_buffer->length;

        // clear the full flag
        f = false;
    }

    done:

    // unlock
    mutex_unlock(&p_circular_buffer->_lock);

    // success
    return 1;

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
            
            no_fn_map:
                #ifndef NDEBUG
                    log_error("[circular buffer] Null pointer provided for parameter \"pfn_map\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
    }
}

int circular_buffer_foreach ( circular_buffer *p_circular_buffer, fn_foreach *pfn_foreach )
{

	// argument check
	if ( NULL == p_circular_buffer ) goto no_circular_buffer;
	if ( NULL ==       pfn_foreach ) goto no_fn_foreach;

	// initialized data
	size_t i = 0;
	bool   f = 0;

	// lock
	mutex_lock(&p_circular_buffer->_lock);

	// check if the buffer is empty
	if ( 
		p_circular_buffer->full == false                    &&
		p_circular_buffer->read == p_circular_buffer->write 
	) goto circular_buffer_empty;

	// store the index and full flag 
	i = p_circular_buffer->read,
	f = p_circular_buffer->full;
	
	// iterate over each element
	while ( f || i != p_circular_buffer->write )
	{

		// call the foreach function
		pfn_foreach(p_circular_buffer->_p_data[i]);

		// update the index
		i = ( i + 1 ) % p_circular_buffer->length;

		// clear the full flag
		f = false;
	}

	circular_buffer_empty:

	// unlock
	mutex_unlock(&p_circular_buffer->_lock);

	// success
	return 1;

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

			no_fn_foreach:
				#ifndef NDEBUG
					log_error("[circular buffer] Null pointer provided for parameter \"pfn_foreach\" in call to function \"%s\"\n", __FUNCTION__);
				#endif

				// error
				return 0;
		}
	}
}

int circular_buffer_pack ( void *p_buffer, circular_buffer *p_circular_buffer, fn_pack *pfn_element )
{
	
	// argument check
    if ( NULL == p_circular_buffer ) goto no_circular_buffer;
    if ( NULL ==       pfn_element ) return 0;

    // initialized data 
    char   *p    = p_buffer;
	size_t  size = 0;
	size_t  i    = 0;
	bool    f    = 0;

    // lock
    mutex_lock(&p_circular_buffer->_lock);

	// calculate the size
	i    = p_circular_buffer->read,
	f    = p_circular_buffer->full,
	size = ( p_circular_buffer->full ) ?
	         p_circular_buffer->length : 
		   ( p_circular_buffer->write + p_circular_buffer->length - p_circular_buffer->read ) % p_circular_buffer->length;

	// pack the length of the circular buffer
	p += pack_pack(p, "%i64", p_circular_buffer->length),

	// pack the length of the circular buffer
	p += pack_pack(p, "%i64", size);

	// iterate over each element
	while ( f || i != p_circular_buffer->write )
	{

		// call the foreach function
		p += pfn_element(p, p_circular_buffer->_p_data[i]);

		// update the index
		i = ( i + 1 ) % p_circular_buffer->length;

		// clear the full flag
		f = false;
	}

    // unlock
    mutex_unlock(&p_circular_buffer->_lock);

    // success
    return p - (char *)p_buffer;

    // error handling
    {
        
        // argument errors
        {
            no_circular_buffer:
                #ifndef NDEBUG
                    log_error("[circular buffer] Null pointer provided for \"p_circular_buffer\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
    }
}

int circular_buffer_unpack ( circular_buffer **pp_circular_buffer, void *p_buffer, fn_unpack *pfn_element )
{
	
	// argument check
    if ( NULL == pp_circular_buffer ) goto no_circular_buffer;
	if ( NULL ==           p_buffer ) return 0;
    if ( NULL ==        pfn_element ) return 0;

    // initialized data 
	circular_buffer *p_circular_buffer = NULL;
    char   *p      = p_buffer;
	size_t  length = 0;
	size_t  size   = 0;
	size_t  i      = 0;
	bool    f      = 0;

	// unpack the length of the circular buffer
	p += pack_unpack(p, "%i64", &length);

	// unpack the size of the circular buffer
	p += pack_unpack(p, "%i64", &size);

	// construct a circular buffer
	if ( circular_buffer_construct(&p_circular_buffer, length) == 0 ) goto failed_to_construct_circular_buffer;

	// iterate over each element
	while ( size-- )
	{

		// initialized data
		void *p_element = NULL;

		// call the foreach function
		p += pfn_element(&p_element, p);

		// add the element to the circular buffer
		circular_buffer_push(p_circular_buffer, p_element);
	}

	// return a pointer to the caller
	*pp_circular_buffer = p_circular_buffer;

    // success
	return p - (char *)p_buffer;

    // error handling
    {
        
        // argument errors
        {
            no_circular_buffer:
                #ifndef NDEBUG
                    log_error("[circular buffer] Null pointer provided for \"p_circular_buffer\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }

		// circular buffer errors
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

hash64 circular_buffer_hash ( circular_buffer *p_circular_buffer, fn_hash64 *pfn_element )
{
	// argument check
    if ( NULL == p_circular_buffer ) goto no_circular_buffer;

    // initialized data 
    hash64     result     = 0;
    fn_hash64 *pfn_hash64 = (pfn_element) ? pfn_element : hash_crc64;
	size_t     size       = 0;
	size_t     i          = 0;
	bool       f          = 0;

    // lock
    mutex_lock(&p_circular_buffer->_lock);

	// calculate the size
	i    = p_circular_buffer->read,
	f    = p_circular_buffer->full,
	size = ( p_circular_buffer->full ) ?
	         p_circular_buffer->length : 
		   ( p_circular_buffer->write + p_circular_buffer->length - p_circular_buffer->read ) % p_circular_buffer->length;

	// iterate over each element
	while ( f || i != p_circular_buffer->write )
	{

		// call the hash function
        result ^= pfn_hash64(p_circular_buffer->_p_data[i], 8);

		// update the index
		i = ( i + 1 ) % p_circular_buffer->length;

		// clear the full flag
		f = false;
	}

    // unlock
    mutex_unlock(&p_circular_buffer->_lock);

    // success
    return result;

    // error handling
    {
        
        // argument errors
        {
            no_circular_buffer:
                #ifndef NDEBUG
                    log_error("[circular buffer] Null pointer provided for \"p_circular_buffer\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
    }
}

int circular_buffer_destroy ( circular_buffer **const pp_circular_buffer, fn_allocator *pfn_allocator )
{

	// argument check
	if ( NULL == pp_circular_buffer ) goto no_circular_buffer;

	// initialized data
	circular_buffer *p_circular_buffer = *pp_circular_buffer;
	
	// lock
	mutex_lock(&p_circular_buffer->_lock);

	// No more circular buffer for end user
	*pp_circular_buffer = 0;

	// unlock
	mutex_unlock(&p_circular_buffer->_lock);

	// empty the circular buffer
	while ( !circular_buffer_empty(p_circular_buffer) )
	{

		// initialized data
		void *p_data = 0;

		// remove an element
		circular_buffer_pop(p_circular_buffer, &p_data);

		// release the element
		if ( pfn_allocator )
			pfn_allocator(p_data, 0);
	}

	// destroy the mutex
	mutex_destroy(&p_circular_buffer->_lock);

	// free the memory
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
