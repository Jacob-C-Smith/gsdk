/** !
 * High level abstractions for connection
 *
 * @file performance/connection.h
 * 
 * @author Jacob Smith
 */

// header guard
#pragma once

// core 
#include <core/log.h>
#include <core/socket.h>
#include <core/interfaces.h>

// preprocessor definitions
#define DISTRIBUTE_CONNECTION_MAX_BUFFER_SIZE 65535

// forward declarations
struct connection_s;

// type definitions
typedef struct connection_s connection;
typedef int (fn_connection_accept)(connection *p_connection);

// function declarations
/** ! 
 * Initiate a connection to a host 
 * 
 * @param pp_connection return
 * @param p_hostname    hostname string
 * @param port          the port number
 * 
 * @return 1 on success, 0 on error
 */
int connection_construct ( connection **const pp_connection, const char *p_hostname, const short port );

/** !
 * Listen for connections on a port
 *
 * @param pp_connection       return
 * @param port                the port number
 * @param pfn_accept_callback the callback function to be invoked when a connection is accepted
 *
 * @return 1 on success, 0 on error
 */
int connection_listen ( connection **pp_connection, const short port, fn_connection_accept *pfn_accept_callback );

/** ! 
 * Write some data to a connection 
 * 
 * @param p_connection the connection
 * @param p_data       pointer to data
 * @param size         quantity of bytes to write
 * 
 * @return 1 on success, 0 on error
 */
int connection_write ( connection *p_connection, void *p_data, size_t size );

/** ! 
 * Read some data from a connection. This is a blocking call that waits for a complete message.
 * 
 * @param p_connection the connection
 * @param p_data       pointer to a buffer to store the message
 * @param p_size       in: size of the buffer, out: size of the received message
 * 
 * @return 1 on success, 0 on error or if the connection was closed
 */
int connection_read ( connection *p_connection, void *p_data, size_t *p_size );

/** !
 * Destroy a connection
 *
 * @param pp_connection pointer to the connection pointer
 *
 * @return 1 on success, 0 on error
 */
int connection_destroy(connection **pp_connection);