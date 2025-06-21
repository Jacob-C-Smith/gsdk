// header guard
#pragma once

// standard library
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

// sync submodule
#include <sync/sync.h>

// socket submodule
#include <socket/socket.h>

// dict submodule
#include <dict/dict.h>

// array submodule
#include <array/array.h>

// json submodule
#include <json/json.h>

// queue submodule
#include <queue/queue.h>

// stack submodule
#include <stack/stack.h>

// tuple submodule
#include <tuple/tuple.h>

// http submodule
#include <http/http.h>

// Web
#include <web/web.h>
#include <web/route.h>
#include <web/resource.h>

struct web_server_s
{
    struct
    {
        unsigned short port;
        socket_tcp     socket;
    } server;

    struct
    {
        bool running;
    } context;

    dict *routes;
    dict *resources;
};

/** !
 *  Allocate memory for an HTTP server
 * 
 * @param pp_web_server return
 * 
 * @return 1 on success, 0 on error
 */
int web_server_create ( web_server **const pp_web_server );

/** !
 *  Construct an HTTP server from a config file
 * 
 * @param pp_web_server return
 * @param p_path        path to config file
 * 
 * @return 1 on success, 0 on error 
 * 
 */
int web_server_construct ( web_server **const pp_web_server, const char *const p_path );

/** !
 *  Construct an HTTP server from a json value
 * 
 * @param pp_web_servrer return
 * @param p_value        the json value
 * 
 * @param pp_web_server return
*/
int web_server_from_json_value ( web_server **const pp_web_server, const json_value *const p_value );

/** !
 *  TCP accept callback
 * 
 * @param _socket      the new socket
 * @param ip_address   the ip address
 * @param port         the port
 * @param p_web_server the http server receiving the request
 * 
 * @return 1 on success, 0 on error
 */
int web_server_accept ( socket_tcp _socket, unsigned long ip_address, unsigned short port, web_server *const p_web_server );

/** !
 *  Start an HTTP server
 * 
 * @param p_web_server the web server
 * 
 * @return 1 on success, 0 on error
 */
int web_server_start ( web_server *const p_web_server );

/** !
 *  Destroy and deallocate an HTTP server and all it's contents.
 * 
 * @param pp_web_server return
 * 
 * @return 1 on success, 0 on error 
 */
int web_server_destroy ( web_server **const pp_web_server );
