/** !
 * High level abstractions for remote procedure calls
 *
 * @file performance/rpc.h
 * 
 * @author Jacob Smith
 */

// header guard
#pragma once

// core
#include <core/log.h>

// data
#include <data/dict.h>

// performance
#include <performance/thread.h>
#include <performance/thread_pool.h>
#include <performance/connection.h>

// type definitions
typedef void *(fn_rpc_handler) ( connection *p_connection, void *p_input );

// function declarations
/** !
 * Register a function as an RPC handler
 * 
 * @param name        the name of the RPC
 * @param pfn_handler the function pointer to the handler
 * @param pfn_pack    the function pointer to the packer
 * @param pfn_unpack  the function pointer to the unpacker
 * 
 * @return 1 on success, 0 on error
 */
int rpc_register
(
    const char     *p_name, 
    fn_rpc_handler *pfn_handler,
    fn_pack        *pfn_pack,
    fn_unpack      *pfn_unpack
);

/** !
 * Start an RPC server and listen for connections
 * 
 * @param port the port to listen on
 * 
 * @return 1 on success, 0 on error
 */
int rpc_server_listen ( short port );

/** !
 * Make an RPC call to a server
 * 
 * @param p_connection    the connection to the server
 * @param name            the name of the RPC to call
 * @param p_args          pointer to the arguments buffer
 * @param args_size       size of the arguments buffer
 * @param p_response      pointer to a buffer to store the response
 * @param p_response_size in: size of the response buffer, out: size of the received response
 * 
 * @return 1 on success, 0 on error
 */
int rpc_call 
(
    connection *p_connection,
    const char *p_name,
    void       *p_args, 
    size_t      args_size,
    void      **p_response,
    size_t     *p_response_size
);