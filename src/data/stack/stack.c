/** !
 * stack implementation
 * 
 * @file src/data/stack/stack.c 
 * 
 * @author Jacob Smith
 */

// header
#include <data/stack.h>

// structure declarations
struct stack_s
{
	size_t  size;      // the quantity of elements that could fit on the stack
	size_t  offset;    // the quantity of elements that are on the stack
	mutex   _lock;     // locked when reading/writing values
	void   *_p_data[]; // the stack elements
};

// function definitions
static fn_it_done stack_iterator_done;
static fn_it_next stack_iterator_next;
static fn_it_item stack_iterator_item;

// function definitions
int stack_construct ( stack **const pp_stack, size_t size )
{

	// argument check
	if ( NULL == pp_stack ) goto no_stack;
	if ( size <         1 ) goto no_size;

	// initialized data
	stack *p_stack = NULL;

	// allocate a stack
	p_stack = default_allocator(NULL, sizeof(stack) + ( size * sizeof(void *) ) );
	if ( NULL == p_stack ) goto no_mem;

	// initialize
	memset(p_stack, 0, sizeof(stack) + ( size * sizeof(void *) ));

	// set the size
	p_stack->size = size;

	// create a lock
    if ( 0 == mutex_create(&p_stack->_lock) ) goto failed_to_mutex_create;

	// return a pointer to the caller
	*pp_stack = p_stack;

	// success
	return 1;
	
	// error handling
	{

		// argument errors
		{
			no_stack:
				#ifndef NDEBUG
					log_error("[stack] Null pointer provided for \"pp_stack\" in call to function \"%s\"\n", __FUNCTION__);
				#endif

				// error
				return 0;

			no_size:
				#ifndef NDEBUG
					log_error("[stack] No size provided in call to function \"%s\"\n", __FUNCTION__);
				#endif

				// error
				return 0;
		}

		// core errors
		{
			failed_to_mutex_create:
                #ifndef NDEBUG
                    log_error("[stack] Failed to create mutex in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error 
                return 0;
		}

		// standard library errors
		{
			no_mem:
				#ifndef NDEBUG
					log_error("[stack] Failed to allocate memory in call to function \"%s\"\n", __FUNCTION__);
				#endif

				// error
				return 0;
		}
	}
}

int stack_push ( stack *const p_stack, void *const p_value )
{

	// argument check
	if ( NULL == p_stack ) goto no_stack;
	if ( NULL == p_value ) goto no_value;

	// lock
    mutex_lock(&p_stack->_lock);

	// error checking
	if ( p_stack->size == p_stack->offset ) goto stack_overflow;

	// push the data onto the stack
	p_stack->_p_data[p_stack->offset++] = p_value;

	// unlock
    mutex_unlock(&p_stack->_lock);

	// success
	return 1;

	// error handling
	{

		// argument errors
		{
			no_stack:
				#ifndef NDEBUG
					log_error("[stack] Null pointer provided for \"p_stack\" in call to function \"%s\"\n", __FUNCTION__);
				#endif

				// error
				return 0;

			no_value:
				#ifndef NDEBUG
					log_error("[stack] Null pointer provided for \"p_value\" in call to function \"%s\"\n", __FUNCTION__);
				#endif

				// error
				return 0;

		}
		
		// stack errors
		{
			stack_overflow:
				#ifndef NDEBUG
					log_error("[stack] Stack overflow in call to function \"%s\"\n", __FUNCTION__);
				#endif

				// unlock
				mutex_unlock(&p_stack->_lock);

				// error
				return 0;
		}
	}
}

bool stack_is_empty ( stack *const p_stack )
{

    // argument check
    if ( NULL == p_stack ) return true;

    // lock
    mutex_lock(&p_stack->_lock);

    // initialized data
    bool ret = ( p_stack->offset == 0 );

    // unlock
    mutex_unlock(&p_stack->_lock);

    // success
    return ret;
}

int stack_pop ( stack *const p_stack, void **const ret )
{

	// argument check
	if ( NULL == p_stack ) goto no_stack;

	// lock
	mutex_lock(&p_stack->_lock);

	// error check
	if ( p_stack->offset < 1 ) goto stack_underflow;

	// return the value to the caller
	if ( ret )
	{

		// pop the stack and write the result
		*ret = p_stack->_p_data[--p_stack->offset];
		p_stack->_p_data[p_stack->offset] = NULL;
	}
	
	// don't return a value to the caller
	else
	{
		--p_stack->offset;
		p_stack->_p_data[p_stack->offset] = NULL;
	}

	// unlock
	mutex_unlock(&p_stack->_lock);

	// success
	return 1;

	// error handling
	{

		// stack errors
		{
			stack_underflow:
				#ifndef NDEBUG
					log_error("[stack] Stack Underflow in call to function \"%s\"\n", __FUNCTION__);
				#endif

				// unlock
				mutex_unlock(&p_stack->_lock);

				// error
				return 0;
		}

		// argument errors
		{
			no_stack:
				#ifndef NDEBUG
					log_error("[stack] Null pointer provided for \"p_stack\" in call to function \"%s\"\n", __FUNCTION__);
				#endif

				// error
				return 0;
		}
	}
}

int stack_peek ( stack *p_stack, void **ret )
{

	// argument check
	if ( NULL == p_stack ) goto no_stack;
	if ( NULL ==     ret ) goto no_ret;

	// lock
	mutex_lock(&p_stack->_lock);

	// error checking
	if ( p_stack->offset < 1 ) goto stack_underflow;

	// peek the stack and write the result
	*ret = p_stack->_p_data[p_stack->offset-1];
	
	// unlock
	mutex_unlock(&p_stack->_lock);

	// success
	return 1;

	// error handling
	{

		// stack errors
		{
			stack_underflow:
				#ifndef NDEBUG
					log_error("[stack] Stack Underflow in call to function \"%s\"\n", __FUNCTION__);
				#endif

				// unlock
				mutex_unlock(&p_stack->_lock);

				// error
				return 0;
		}

		// argument errors
		{
			no_stack:
				#ifndef NDEBUG
					log_error("[stack] Null pointer provided for \"p_stack\" in call to function \"%s\"\n", __FUNCTION__);
				#endif

				// error
				return 0;

			no_ret:
				#ifndef NDEBUG
					log_error("[stack] Null pointer provided for \"ret\" in call to function \"%s\"\n", __FUNCTION__);
				#endif

				// error
				return 0;
		}
	}
}

int stack_fori ( stack *p_stack, fn_fori *pfn_fori ) 
{

    // argument check
    if ( NULL ==  p_stack ) goto no_stack;
    if ( NULL == pfn_fori ) goto no_fn_fori;

    // lock
    mutex_lock(&p_stack->_lock);

    // iterate over each element in the stack
    for (size_t i = 0; i < p_stack->offset; i++)
        
        // call the function (casting away const as the interface expects non-const)
        pfn_fori((void *)p_stack->_p_data[i], (int)i);

    // unlock
    mutex_unlock(&p_stack->_lock);

    // success
    return 1;

    // error handling
    {
        
        // argument errors
        {
            no_stack:
                #ifndef NDEBUG
                    log_error("[stack] Null pointer provided for \"p_stack\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
            
            no_fn_fori:
                #ifndef NDEBUG
                    log_error("[stack] Null pointer provided for \"pfn_fori\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
    }
}

iterator stack_iterator ( stack *p_stack )
{

    // success
    return (iterator)
    {
        .p_data = p_stack,
        .state  = { 0 },
        .done   = stack_iterator_done,
        .next   = stack_iterator_next,
        .item   = stack_iterator_item
    };
}

static bool stack_iterator_done ( iterator *p_iterator ) 
{

    // done?
    return ((size_t)p_iterator->state.p_state) >= ((stack *) p_iterator->p_data)->offset; 
}

static void stack_iterator_next ( iterator *p_iterator ) 
{

    // update the state
    p_iterator->state.p_state = (void *)((size_t)p_iterator->state.p_state + 1); 

    // done
    return;
}

static void *stack_iterator_item ( iterator *p_iterator ) 
{

    // done
    return ((stack *) p_iterator->p_data)->_p_data[(size_t)p_iterator->state.p_state]; 
}

int stack_pack ( stream *p_stream, stack *p_stack, fn_pack *pfn_element )
{
    
    // argument check
    if ( NULL ==    p_stream ) goto no_stream;
    if ( NULL ==     p_stack ) goto no_stack;
    if ( NULL == pfn_element ) goto no_pack;

    // initialized data 
    size_t written = 0;
	
    // lock
    mutex_lock(&p_stack->_lock);

    // pack the size
    written += pack_pack(p_stream, "%i64", p_stack->size);

    // pack the offset
    written += pack_pack(p_stream, "%i64", p_stack->offset);

    // iterate through the stack
    for (size_t i = 0; i < p_stack->offset; i++)

		// pack the element
        written += pfn_element(p_stream, p_stack->_p_data[i]);

    // unlock
    mutex_unlock(&p_stack->_lock);

    // success
    return (int)written;

    // error handling
    {
        
        // argument errors
        {
            no_stack:
                #ifndef NDEBUG
                    log_error("[stack] Null pointer provided for \"p_stack\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;

			no_stream:
                #ifndef NDEBUG
                    log_error("[stack] Null pointer provided for \"p_stream\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;

			no_pack:
                #ifndef NDEBUG
                    log_error("[stack] Null pointer provided for \"pfn_element\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
    }
}

int stack_unpack ( stack **pp_stack, stream *p_stream, fn_unpack *pfn_element )
{
    
    // argument check
    if ( NULL ==    pp_stack ) goto no_stack;
    if ( NULL ==    p_stream ) goto no_stream;
    if ( NULL == pfn_element ) goto no_unpack;

    // initialized data
    stack *p_stack = NULL;
    size_t read    = 0;
    size_t size    = 0;
    size_t off     = 0;

    // unpack the size
    read += pack_unpack(p_stream, "%i64", &size);

    // unpack the offset
    read += pack_unpack(p_stream, "%i64", &off);

    // construct a stack
    stack_construct(&p_stack, size);

	// iterate through the elements in the stack
    for (size_t i = 0; i < off; i++)
    {
        
        // initialized data
        void *p_element = NULL;

        // unpack the element
        read += pfn_element(&p_element, p_stream);
        
        // add the element to the stack
        stack_push(p_stack, p_element);
    }

    // return the stack to the caller
    *pp_stack = p_stack;

    // success
    return (int)read;
    
    // error handling
    {
        
        // argument errors
        {
            no_stack:
                #ifndef NDEBUG
                    log_error("[stack] Null pointer provided for \"pp_stack\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
			
			no_stream:
                #ifndef NDEBUG
                    log_error("[stack] Null pointer provided for \"p_stream\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
			
			no_unpack:
                #ifndef NDEBUG
                    log_error("[stack] Null pointer provided for \"pfn_element\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
    }
}

hash64 stack_hash ( stack *p_stack, fn_hash64 *pfn_element )
{

    // argument check
    if ( NULL ==     p_stack ) goto no_stack;
    if ( NULL == pfn_element ) goto no_pfn_element;

    // lock
    mutex_lock(&p_stack->_lock);

    // initialized data
    hash64 result = 0;

    // iterate through each element in the stack
    for (size_t i = 0; i < p_stack->offset; i++)
        result ^= pfn_element(p_stack->_p_data[i], 8);

    // unlock
    mutex_unlock(&p_stack->_lock);

    // success
    return result;

    // error handling
    {

        // argument errors
        {
            no_stack:
                #ifndef NDEBUG
                    log_error("[stack] Null pointer provided for \"p_stack\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
            
            no_pfn_element:
                #ifndef NDEBUG
                    log_error("[stack] Null pointer provided for \"pfn_element\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
    }
}

int stack_destroy ( stack **const pp_stack, fn_allocator *pfn_allocator )
{

	// argument check
	if ( NULL == pp_stack ) goto no_stack;

	// initialized data
	stack *p_stack = *pp_stack;

	// fast exit
	if ( NULL == p_stack ) return 1;

	// lock
    mutex_lock(&p_stack->_lock);

	// no more pointer for caller
	*pp_stack = NULL;

	// unlock
    mutex_unlock(&p_stack->_lock);

	// destroy the mutex
    mutex_destroy(&p_stack->_lock);

	// release stack elements
	if ( pfn_allocator )

		// iterate through each element in the stack
		for (size_t i = 0; i < p_stack->offset; i++)

			// release the element
			p_stack->_p_data[i] = pfn_allocator(p_stack->_p_data[i], 0);

	// release the stack
	p_stack = default_allocator(p_stack, 0);

	// success
	return 1;

	// error handling
	{

		// argument errors
		{
			no_stack:
				#ifndef NDEBUG
					log_error("[stack] Null pointer provided for \"pp_stack\" in call to function \"%s\"\n", __FUNCTION__);
				#endif

				// error
				return 0;
		}
	}
}
