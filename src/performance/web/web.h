// Header guard
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

// Set the reallocator for the dict submodule
#ifdef DICT_REALLOC
    #undef DICT_REALLOC
    #define DICT_REALLOC(p, sz) realloc(p, sz)
#endif

// Set the reallocator for the array submodule
#ifdef ARRAY_REALLOC
    #undef ARRAY_REALLOC
    #define ARRAY_REALLOC(p, sz) realloc(p, sz)
#endif

// Set the reallocator for the JSON submodule
#ifdef JSON_REALLOC
    #undef JSON_REALLOC
    #define JSON_REALLOC(p, sz) realloc(p, sz)
#endif

// Set the reallocator for the queue submodule
#ifdef QUEUE_REALLOC
    #undef QUEUE_REALLOC
    #define QUEUE_REALLOC(p, sz) realloc(p, sz)
#endif

// Set the reallocator for the stack submodule
#ifdef STACK_REALLOC
    #undef STACK_REALLOC
    #define STACK_REALLOC(p, sz) realloc(p, sz)
#endif

// Set the reallocator for the tuple submodule
#ifdef TUPLE_REALLOC
    #undef TUPLE_REALLOC
    #define TUPLE_REALLOC(p, sz) realloc(p, sz)
#endif

// Memory management macro
#ifndef WEB_REALLOC
    #define WEB_REALLOC(p, sz) realloc(p,sz)
#endif

// Forward declarations
struct web_server_s;
struct web_resource_s;
struct web_route_s;
struct web_template_s;

// Type definitions
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
