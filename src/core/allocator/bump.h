/** !
 * Bump allocator interface
 *
 * @file bump.h 
 *  
 * @author Jacob Smith
 * /

// standard library
#include <stdio.h>
#include <stdlib.h>

// core
#include <core/log.h>

struct bump_allocator_s
{
    size_t used = 0;
    char   _pool[POOL_SIZE] = { 0 };
}


// function declarations
/** !
 * Bump allocator 
 * 
 * @param size the quantity of bytes to reserve in the pool
 * 
 * @return pointer to usable memory on success, null pointer on error 
 * /
void *bump_alloc ( void *p_ptr, size_t size ) 
{

    // argument check
    if ( size > (POOL_SIZE - used) ) goto too_big;

    // initialized data
    void *ret = (void *) 0;

    // store the return value 
    ret = &_pool[used];

    // update the quantity of memory that has been allocated
    used += size;
    
    // done
    return ret;

    // error handling
    {

        // argument errors
        {
            too_big:
                #ifndef NDEBUG
                    log_error("[allocator] Requested %zu bytes from a %zu byte pool\n", size, )
                #endif

                // error
                return 0;
        }
    }
}
*/