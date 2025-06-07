/** !
 *  Defines a web resource
 * 
 * @file web/resource.h
 * 
 * @author Jacob Smith
 */

// header guard
#pragma once

// standard library
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

// web
#include <web/web.h>

// Structure definitions
struct web_resource_s
{
    char name[255+1];
    char path[2047+1];
    char *content;
    bool cache;
    size_t length;
};

/** !
 * Allocate memory for a web resource
 * 
 * @param pp_web_resource return
 * 
 * @return 1 on success, 0 on error
 */
int web_resource_create ( web_resource **pp_web_resource );

/** !
 * Construct a web resource from a json value 
 * 
 * @param pp_web_resource return
 * @param p_value         pointer to json value
 * 
 * @return 1 on success, 0 on error
 */
int web_resource_from_json ( web_resource **pp_web_resource, json_value *p_value );

/** !
 * Destroy a web resource
 * 
 * @param pp_web_resource pointer to web resource pointer
 * 
 * @return 1 on success, 0 on error
*/
int web_resource_destroy ( web_resource **pp_web_resource ); 
