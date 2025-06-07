#include "vector.h"

static bool initialized = false;
struct {
    const char *type;
    pack_pair _pack_pair;
} _vector_pack_pairs[] =
{
    {
        .type       = "vec3",
        ._pack_pair = (pack_pair)
        {
            .pfn_pack   = vec3_pack,
            .pfn_unpack = vec3_unpack
        }
    },
    {
        .type       = "transform",
        ._pack_pair = (pack_pair)
        {
            .pfn_pack   = transform_pack,
            .pfn_unpack = transform_unpack
        }
    }
};

void vector_init ( void )
{

    // Initialized data
    size_t pack_unpack_count = sizeof(_vector_pack_pairs) / ( sizeof(const char *) + sizeof(pack_pair) );

    // Iterate through each pack/unpack
    for (size_t i = 0; i < pack_unpack_count; i++)

        // Register the pack/unpack function
        pack_register(_vector_pack_pairs[i].type, _vector_pack_pairs[i]._pack_pair);

    // Done
    return;
}

int vec3_pack ( void *p_buffer, const void *const p_data )
{
    
    // Initialized data
    const vec3 *const p_vec3 = p_data;
    
    // Done
    return pack_pack(p_buffer, "%3f32", p_vec3->x, p_vec3->y, p_vec3->z);
}

int vec3_unpack ( void *p_data, void *p_buffer )
{
    
    // Initialized data
    const vec3 *const p_vec3 = p_data;
    
    // Done
    return pack_unpack(p_buffer, "%3f32", &p_vec3->x, &p_vec3->y, &p_vec3->z);
}

void vec3_print ( vec3 _vec3 )
{

    // Format
    printf ("< %g, %g, %g >",
        _vec3.x,
        _vec3.y,
        _vec3.z
    );

    // Done
    return;
}

int transform_pack ( void *p_buffer, const void *const p_data )
{
    
    // Initialized data
    transform *p_transform = (transform *) p_data;
    void *o_buffer = p_buffer;

    // Pack some data
    o_buffer += pack("vec3", o_buffer, &p_transform->loc),
    o_buffer += pack("vec3", o_buffer, &p_transform->rot),
    o_buffer += pack("vec3", o_buffer, &p_transform->sca);

    // Done
    return o_buffer - p_buffer;
}

int transform_unpack ( void *p_data, void *p_buffer )
{
    
    // Initialized data
    transform *const p_transform = p_data;
    void *o_buffer = p_buffer;

    // Unpack some data
    o_buffer += unpack("vec3", &p_transform->loc, o_buffer);
    o_buffer += unpack("vec3", &p_transform->rot, o_buffer);
    o_buffer += unpack("vec3", &p_transform->sca, o_buffer);

    // Done
    return (int)(o_buffer - p_buffer);
}

void transform_print ( transform _transform )
{

    // Format
    vec3_print(_transform.loc);
    printf(", ");
    vec3_print(_transform.rot);
    printf(", ");
    vec3_print(_transform.sca);
    
    // Done
    return;
}

void vector_exit ( void )
{
    
    // State check
    if ( initialized == false ) return;

    // Initialized data
    size_t pack_unpack_count = sizeof(_vector_pack_pairs) / ( sizeof(const char *) + sizeof(pack_pair) );

    // Iterate through each pack/unpack
    for (size_t i = 0; i < pack_unpack_count; i++)

        // Unegister the pack/unpack function
        pack_unregister(_vector_pack_pairs[i].type);

    // Done
    return;

    // Clear the initialized flag
    initialized = false;

    // Done
    return;
}
