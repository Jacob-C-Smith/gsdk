// Header guard
#pragma once 

// Web
#include <web/web.h>

struct web_template_s
{
    int i;
};

/** !
 * Allocate memory for an HTML template
 * 
 * @param pp_web_template return
 * 
 * @return 1 on success, 0 on error
 */
int web_template_create ( web_template **const pp_web_template );

/** !
 * Construct an HTML template from a json value
 * 
 * @param pp_web_template return
 * @param p_value         the json value
 * 
 * @return 1 on success, 0 on error
 */
int web_template_from_json ( web_template **const pp_web_template, const json_value *const p_value );
