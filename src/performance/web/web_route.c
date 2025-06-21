#include <web/route.h>

int web_route_create ( web_route **const pp_web_route )
{

    // Argument check
    if ( pp_web_route == (void *) 0 ) goto no_web_route;

    // Initialized data
    web_route *p_web_route = WEB_REALLOC(0, sizeof(web_route));

    // Error check
    if ( p_web_route == (void *) 0 ) goto no_mem;

    // Zero set the struct
    memset(p_web_route, 0, sizeof(web_route));

    // Return a pointer to the caller
    *pp_web_route = p_web_route;

    // Success
    return 1;

    // Error handling
    {

        // Argument errors
        {
            no_web_route:
                #ifndef NDEBUG
                    printf("[web] Null pointer provided for parameter \"pp_web_server\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }

        // standard library errors
        {
            no_mem:
                #ifndef NDEBUG
                    printf("[Standard Library] Failed to allocate memory in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }
    }
}

int web_route_construct ( web_route **const pp_web_route, const char *const p_path );

int web_route_from_json ( web_route **const pp_web_route, const json_value *const p_value )
{

    // Argument check
    if ( pp_web_route == (void *) 0 ) goto no_web_route;
    if ( p_value      == (void *) 0 ) goto no_value;

    // Initialized data
    web_route  *p_web_route = 0;

    // Allocate a web route
    // TODO
    //if ( web_route_create(&p_web_route) == (void *) 0 ) ;

    // Parse the json object
    if ( p_value->type == JSON_VALUE_STRING ) 

        strcpy(p_web_route->path, p_value->string);

    // Return a pointer to the caller
    *pp_web_route = p_web_route;

    // Success
    return 1;

    // Error handling
    {
        
        // Argument errors
        {
            no_web_route:
                #ifndef NDEBUG
                    printf("[web] Null pointer provided for parameter \"pp_web_server\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;

            no_value:
                #ifndef NDEBUG
                    printf("[web] Null pointer provided for parameter \"p_value\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
                
        }
    }
}

int web_route_destroy ( web_route **const pp_web_route )
{

    // TODO: Implement

    // Error
    return 0;
}
