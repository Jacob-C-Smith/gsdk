#include <web/resource.h>

int web_resource_create ( web_resource **pp_web_resource )
{

    // argument check
    if ( pp_web_resource == (void *) 0 ) goto no_web_resource;

    // initialized data
    web_resource *p_web_resource = realloc(0, sizeof(web_resource));

    // error check
    if ( p_web_resource == (void *) 0 ) goto no_mem;

    // Zero set
    memset(p_web_resource, 0, sizeof(web_resource));

    // return a pointer to the caller
    *pp_web_resource = p_web_resource;

    // success
    return 1;

    // error handling
    {

        // argument handling
        {
            no_web_resource:
                #ifndef NDEBUG
                    printf("[web] [resource] Null pointer provided for parameter \"pp_web_resource\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }

        // standard library errors
        {
            no_mem:
                #ifndef NDEBUG
                    printf("[Standard Library] Failed to allocate memory in call to function \"%s\"\n", __FUNCTION__);
                #endif
                
                // error
                return 0;
        }
    }
}

int web_resource_from_json ( web_resource **pp_web_resource, json_value *p_value )
{

    // argument check
    if ( pp_web_resource == (void *) 0 ) goto no_web_resource;
    if ( p_value         == (void *) 0 ) goto no_value;

    // initialized data
    web_resource *p_web_resource = (void *) 0;
    size_t resource_length = 0;
    
    // Allocate a server resource
    if ( web_resource_create(&p_web_resource) ) goto failed_to_allocate_web_resource;

    // Parse the json value as an object
    if ( p_value->type == JSON_VALUE_OBJECT )
    {
        
        // initialized data
        dict *p_dict = p_value->object;
        const json_value *p_name  = dict_get(p_dict, "name"),
                         *p_path  = dict_get(p_dict, "path"),
                         *p_cache = dict_get(p_dict, "cache");

        // error check
        if ( !( p_name && p_path ) ) goto missing_properties;

        // Parse the name
        if ( p_name->type == JSON_VALUE_STRING )
        {

            // initialized data
            size_t len = strlen(p_name->string);

            // error checking 
            if ( len == 0   ) goto name_too_short;
            if ( len >= 255 ) goto name_too_long;

            // Copy the name
            strncpy(p_web_resource->name, p_name->string, len);
        } 
        
        // Default
        else goto name_wrong_type;

        // Parse the path
        if ( p_path->type == JSON_VALUE_STRING ) 
        {

            // initialized data
            size_t len = strlen(p_path->string);

            // error checking 
            if ( len == 0   ) goto path_too_short;
            if ( len >= 255 ) goto path_too_long;

            // Copy the name
            strncpy(p_web_resource->path, p_path->string, len);
        } 

        // Default
        goto path_wrong_type;

        // Parse the cache flag. Default to true
        p_web_resource->cache = ( p_cache->type == JSON_VALUE_BOOLEAN ) ? p_cache->boolean : true;

    }

    // Finish constructing the resource
    resource_length = web_load_file(p_web_resource->path, 0, true);

    // error check
    if ( resource_length == 0 ) goto failed_to_open_file; 

    // Allocate memory for file
    p_web_resource->content = realloc(0, sizeof(char) * (resource_length + 1));

    // error check
    if ( p_web_resource->content == (void *) 0 ) goto no_mem;

    // Read the file
    if ( web_load_file(p_web_resource->path, &p_web_resource->content, true) == 0 ) goto failed_to_read_file;

    // Store the length of the resource
    p_web_resource->length = resource_length;

    // return a pointer to the caller
    *pp_web_resource = p_web_resource;

    // success
    return 1;

    // error handling
    no_web_resource:
    no_value:
    failed_to_allocate_web_resource:
    missing_properties:
    name_too_short:
    name_too_long:
    name_wrong_type:
    path_too_short:
    path_too_long:
    path_wrong_type:
    failed_to_open_file:
    failed_to_read_file:
    no_mem:

        // error
        return 0;
}

int web_resource_destroy ( web_resource **pp_web_resource )
{

    // argument check
    if ( pp_web_resource == (void *) 0 ) goto no_web_resource;

    // initialized data
    web_resource *p_web_resource = *pp_web_resource;

    // No more pointer for caller
    *pp_web_resource = 0;

    // Clean up
    p_web_resource = realloc(p_web_resource, 0);

    // success
    return 1;

    // error handling
    {

        // argument errors
        {
            no_web_resource:
                #ifndef NDEBUG
                    printf("[web] [resource] Null pointer provided for parameter \"pp_web_resource\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
    }
}
