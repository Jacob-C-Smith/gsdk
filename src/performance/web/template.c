#include <web/template.h>

int web_template_create ( web_template **const pp_web_template )
{

    // argument check
    if ( pp_web_template == (void *) 0 ) goto no_web_template;

    // initialized data
    web_template *p_web_template = realloc(0, sizeof(web_template));

    // error check
    if ( p_web_template == (void *) 0 ) goto no_mem;

    // Zero set the web template
    memset(p_web_template, 0, sizeof(web_template));

    // return a pointer to the caller
    *pp_web_template = p_web_template;

    // success
    return 1;

    // error handling
    {

        // argument errors
        {
            no_web_template:
                #ifndef NDEBUG
                    printf("[web] [template] Null pointer provided for parameter \"pp_web_template\" in call to function \"%s\"\n", __FUNCTION__);
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

int web_template_from_json ( web_template **const pp_web_template, const json_value *const p_value )
{

    // argument check
    if ( pp_web_template == (void *) 0 ) goto no_web_template;
    if ( p_value         == (void *) 0 ) goto no_value;

    // initialized data
    web_template *p_web_template = 0;
    json_value   *p_path         = 0;

    // Parse the json value
    if ( p_value->type == JSON_VALUE_OBJECT )
    {

        // initialized data
        dict *p_dict = p_value->object;

        // Get the path to the template file
        p_path = dict_get(p_dict, "path");

        // error checking
        // TODO
    }

    // Parse the json properties
    {

        // Parse the file path
        if ( p_path->type == JSON_VALUE_STRING )
        {
            
        }
        // TODO: Default
    }

    // return a pointer to the caller
    *pp_web_template = p_web_template;

    // success
    return 1;

    // error handling
    {

        // argument errors
        {
            no_web_template:
                #ifndef NDEBUG
                    printf("[web] [template] Null pointer provided for parameter \"pp_web_template\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
            
            no_value:
                #ifndef NDEBUG
                    printf("[web] [template] Null pointer provided for parameter \"p_value\" in call to function \"%s\"\n", __FUNCTION__);
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