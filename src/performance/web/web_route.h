/** !
 *  Defines a route for resources on a webpage 
 * 
 */

// header guard
#pragma once

// standard library
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#include <web/web.h>

struct web_route_s
{
    char name[512];
    char path[1024];
};


/** !
 * Allocate memory for a server route
 * 
 * @param pp_web_route return
 * 
 * @return 1 on success, 0 on error
 */
int web_route_create ( web_route **const pp_web_route );

/** !
 * Construct a server route from a file
 * 
 * @param pp_web_route return
 * @param p_path       path to file
 * 
 * @return 1 on success, 0 on error 
 */
int web_route_construct ( web_route **const pp_web_route, const char *const p_path );

/** !
 * Construct a server route from a json value
 * 
 * @param pp_web_route return
 * @param p_value      the json value
 * 
 * @return 1 on success, 0 on error
 */
int web_route_from_json ( web_route **const pp_web_route, const json_value *const p_value );

/** !
 * Destroy and deallocate a server route and all it's contents.
 * 
 * @param pp_web_route the web route, return
 * 
 * @return 1 on success, 0 on error 
 */
int web_route_destroy ( web_route **const pp_web_route );
