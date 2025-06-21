// header guard
#pragma once

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

// forward declarations
struct web_server_s;
struct web_resource_s;
struct web_route_s;
struct web_template_s;

// type definitions
typedef struct web_server_s   web_server;
typedef struct web_resource_s web_resource;
typedef struct web_route_s    web_route;
typedef struct web_template_s web_template;

/**!
 * Return the size of a file IF buffer == 0 ELSE read a file into buffer
 * 
 * @param path path to the file
 * @param buffer buffer
 * @param binary_mode "wb" IF true ELSE "w"
 * 
 * @return 1 on success, 0 on error
 */
size_t web_load_file ( const char *path, void *buffer, bool binary_mode );
