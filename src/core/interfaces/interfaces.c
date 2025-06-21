#include <core/interfaces.h>

void *default_allocator ( void *p_pointer, unsigned long long size )
{

    // Default
    return realloc(p_pointer, size);
}

int default_comparator ( const void *p_a, const void *p_b )
{

    // Default
    return p_b - p_a;
}

int default_equality ( const void *const p_a, const void *const p_b )
{

    // Default
    return p_a == p_b;
}

void *default_key_accessor ( const void *const p_value )
{

    // Default
    return (void *) p_value;
}
