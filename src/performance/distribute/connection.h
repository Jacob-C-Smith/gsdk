/** !
 * High level abstractions for connection
 *
 * @file distribute/connection.h
 * 
 * @author Jacob Smith
 */

// header guard
#pragma once

// Header files
#include <distribute/distribute.h>

// Socket module
#include <socket/socket.h>

// Preprocessor definitions
#define DISTRIBUTE_CONNECTION_MAX_BUFFER_SIZE 65535

// Forward declarations
struct connection_s;

// type definitions
typedef struct connection_s connection;

// Function declarations
/** !
 * Allocate a connection 
 * 
 * @param pp_connection return
 * 
 * @return 1 on success, 0 on error
 */
int connection_create ( const connection **const pp_connection );

/** ! 
 * Initiate a connection to a host 
 * 
 * @param pp_connection return
 * @param p_hostname    hostname string
 * @param port          the port number
 * 
 * @return 1 on success, 0 on error
 */
int connection_initiate_hostname ( const connection **const pp_connection, const char *p_hostname, const short port );

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
 * Read some data from a connection 
 * 
 * @param p_connection the connection
 * @param p_data       pointer to buffer
 * @param size         quantity of bytes to recieve
 * 
 * @return 1 on success, 0 on error
 */
int connection_read ( connection *p_connection, void *p_data, size_t size );
