#include <core/interfaces.h>

void *default_allocator ( void *p_pointer, unsigned long long size )
{

    // argument check
    if
    (
        NULL == p_pointer &&
        0    == size
    ) goto double_free_or_empty_alloc;

    // initialized data
    void *p_result = NULL;

    // free
    if ( 0 == size ) free(p_pointer);

    // allocate 
    else if ( NULL == p_pointer ) p_result = malloc(size);

    // reallocate
    else
    {

        // store the result
        p_result = realloc(p_pointer, size);

        // error check
        if ( NULL == p_result )
        {
            free(p_pointer);
            goto double_free_or_empty_alloc;
        }
    }

    // default
    return p_result;

    // argument errors
    {
        double_free_or_empty_alloc:
            printf("[gsdk] detected a double free or an empty allocation in call to function \"%s\"\n", __FUNCTION__);
            return NULL;
    }
}

int default_comparator ( const void *p_a, const void *p_b )
{

    // default
    return p_b - p_a;
}

int default_equality ( const void *const p_a, const void *const p_b )
{

    // default
    return p_a == p_b;
}

void *default_key_accessor ( const void *const p_value )
{

    // default
    return (void *) p_value;
}

hash64 default_hash ( const void *const k, unsigned long long l )
{

    // initialized data
    hash64  ret = 0;
    char   *p   = (char *) k;

    // hash
    for (size_t i = 0; i < l; i++)
        ret += p[i],
        ret *= ( p[i] > 0 ) ? p[i] : l;

    // default
    return ret;
}
