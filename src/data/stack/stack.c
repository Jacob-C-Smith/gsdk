/** !
 * stack library
 * 
 * @file src/core/stack/stack.c 
 * 
 * @author Jacob Smith
 */

// header
#include <data/stack.h>

// structure declarations
struct stack_s
{
	size_t      size;      // the quantity of elements that could fit on the stack
	size_t      offset;    // the quantity of elements in the stack
	mutex       _lock;     // locked when reading/writing values
	const void *_p_data[]; // the stack elements
};

// function definitions
int stack_construct ( stack **const pp_stack, size_t size )
{

	// argument check
	if ( pp_stack == (void *) 0 ) goto no_stack;
	if ( size < 1 ) goto no_size;

	// initialized data
	stack *p_stack = realloc(NULL, sizeof(stack) + ( size * sizeof(void *) ) );

	// allocate a stack
	if ( NULL == p_stack ) goto no_mem;

	// initialize
	memset(p_stack, 0, sizeof(stack) + ( size * sizeof(void *) ));

	// set the size
	p_stack->size = size;

	// create a lock
    if ( mutex_create(&p_stack->_lock) == 0 ) goto failed_to_mutex_create;

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
					log_error("[Standard Library] Failed to allocate memory in call to function \"%s\"\n", __FUNCTION__);
				#endif

				// error
				return 0;
		}
	}
}

int stack_push ( stack *const p_stack, const void *const p_value )
{

	// argument check
	if ( p_stack == (void *) 0 ) goto no_stack;
	if ( p_value == (void *) 0 ) goto no_value;

	// error checking
	if ( p_stack->size == p_stack->offset ) goto stack_overflow;

	// lock
    mutex_lock(&p_stack->_lock);

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
					log_error("[stack] Stack overflow!\n");
				#endif

				// error
				return 0;
		}
	}
}

int stack_pop ( stack *const p_stack, const void **const ret )
{

	// argument check
	if ( p_stack == (void *) 0 ) goto no_stack;

	// error checking
	if ( p_stack->offset < 1 ) goto stack_underflow;

	// lock
	mutex_lock(&p_stack->_lock);

	// return the value to the caller
	if ( ret )

		// pop the stack and write the result
		*ret = p_stack->_p_data[--p_stack->offset],
		p_stack->_p_data[p_stack->offset] = NULL;
	
	// don't return a value to the caller
	else
		--p_stack->offset,
		p_stack->_p_data[p_stack->offset] = NULL;

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
					log_error("[stack] Stack Underflow!\n");
				#endif

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
	if ( p_stack == (void *) 0 ) goto no_stack;
	if ( ret     == (void *) 0 ) goto no_ret;

	// error checking
	if ( p_stack->offset < 1 ) goto stack_underflow;

	// lock
	mutex_lock(&p_stack->_lock);

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
					log_error("[stack] Stack Underflow!\n");
				#endif

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
    if ( NULL == p_stack  ) goto no_stack;
    if ( NULL == pfn_fori ) goto no_fn_fori;

    // lock
    mutex_lock(&p_stack->_lock);

    // iterate over each element in the stack
    for (size_t i = 0; i < p_stack->offset; i++)
        
        // Call the function (casting away const as the interface expects non-const)
        pfn_fori((void *)p_stack->_p_data[i], i);

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

int stack_pack ( void *p_buffer, stack *p_stack, fn_pack *pfn_element )
{
    
    // argument check
    if ( p_stack     == (void *) 0 ) goto no_stack;
    if ( pfn_element == (void *) 0 ) return 0;

    // initialized data 
    char *p = p_buffer;
	
    // lock
    mutex_lock(&p_stack->_lock);

    // pack the size
    p += pack_pack(p, "%i64", p_stack->size);

    // pack the offset
    p += pack_pack(p, "%i64", p_stack->offset);

    // iterate through the stack
    for (size_t i = 0; i < p_stack->offset; i++)

		// pack the element
        p += pfn_element(p, p_stack->_p_data[i]);

    // unlock
    mutex_unlock(&p_stack->_lock);

    // success
    return p - (char *)p_buffer;

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
        }
    }
}

int stack_unpack ( stack **pp_stack, void *p_buffer, fn_unpack *pfn_element )
{
    
    // argument check
    if ( pp_stack    == (void *) 0 ) goto no_stack;
    if ( pfn_element == (void *) 0 ) return 0;

    // initialized data
    stack *p_stack = NULL;
    char *p = p_buffer;
    size_t size = 0;
    size_t off = 0;

    // unpack the size
    p += pack_unpack(p, "%i64", &size);

    // unpack the offset
    p += pack_unpack(p, "%i64", &off);

    // construct a stack
    stack_construct(&p_stack, size);

	// iterate through the elements in the stack
    for (size_t i = 0; i < off; i++)
    {
        
        // initialized data
        char _result[1024] = { 0 };
        void *p_element = NULL;
        size_t len_result = pfn_element(_result, p);

        // advance the buffer
        p += len_result;

        // allocate memory for the element
        p_element = realloc(0, len_result),

        // copy the memory
        memcpy(p_element, _result, len_result),
        
        // add the element to the stack
        stack_push(p_stack, p_element);
    }

    // return the stack to the caller
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
        }
    }
}

hash64 stack_hash ( stack *p_stack, fn_hash64 *pfn_element )
{

    // argument check
    if ( p_stack == (void *) 0 ) goto no_stack;
    if ( pfn_element == (void *) 0 ) goto no_pfn_element;

    // initialized data
    hash64     result     = 0;

    // iterate through each element in the stack
    for (size_t i = 0; i < p_stack->offset; i++)
        result ^= pfn_element(p_stack->_p_data[i], 8);

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

int stack_destroy ( stack **const pp_stack )
{

	// argument check
	if ( pp_stack == (void *) 0 ) goto no_stack;

	// initialized data
	stack *p_stack = *pp_stack;

	// error checking
	if ( p_stack == (void *) 0 ) goto pointer_to_null_pointer;

	// lock
    mutex_lock(&p_stack->_lock);

	// No more pointer for caller
	*pp_stack = 0;

	// unlock
    mutex_unlock(&p_stack->_lock);

	// Destroy the mutex
    mutex_destroy(&p_stack->_lock);
	
	// Free the stack
	p_stack = realloc(p_stack, 0);

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

			pointer_to_null_pointer:
				#ifndef NDEBUG
					log_error("[stack] Parameter \"pp_stack\" points to null pointer in call to function \"%s\"\n", __FUNCTION__);
				#endif

				// error
				return 0;
		}
	}
}
