/** !
 * High level abstractions for connection
 *
 * @file distribute/distribute.c
 *
 * @author Jacob Smith
 */

// header files
#include <performance/connection.h>

// internal callbacks forward declaration
int connection_accept_callback ( socket_tcp _socket_tcp, socket_ip_address ip_address, socket_port port_number, void *const p_parameter );

// structure definitions
struct connection_s
{
    socket_tcp        _tcp_socket;
    socket_ip_address ip_address;
    socket_port       port_number;
};

// function declarations
/** !
 * Allocate a connection 
 * 
 * @param pp_connection return
 * 
 * @return 1 on success, 0 on error
 */
int connection_create ( connection **const pp_connection );

// function definitions
int connection_create ( connection **const pp_connection )
{

    // argument check
    if ( pp_connection == (void *) 0 ) goto no_connection;

    // initialized data
    connection *p_connection = default_allocator(0, sizeof(connection));

    // error check
    if ( p_connection == (void *) 0 ) goto no_mem;

    // return a pointer to the caller
    *pp_connection = p_connection;

    // success
    return 1;

    // error handling
    {

        // argument errors
        {
            no_connection:
                #ifndef NDEBUG
                    log_error("[distribute] [connection] Null pointer provided for parameter \"pp_connection\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }

        // standard library errors
        {
            no_mem:
                #ifndef NDEBUG
                    log_error("[standard library] Failed to allocate memory in call to function \"%s\"\n", __FUNCTION__);
                #endif
                
                // error
                return 0;
        }
    }
}

int connection_construct ( connection **const pp_connection, const char *p_hostname, const short port )
{

    // argument check
    if ( pp_connection == (void *) 0 ) goto no_connection;
    if ( p_hostname    == (void *) 0 ) goto no_hostname;
    if ( port          ==          0 ) goto no_port;

    // initialized data
    connection *p_connection = (void *) 0;
    socket_tcp   _socket     = 0;
    socket_ip_address ip = 0;

    // allocate a connection
    if ( connection_create(&p_connection) == 0 ) goto no_mem;

    // Resolve the hostname
    if ( socket_resolve_host(&ip, p_hostname) == 0) goto failed_to_resolve_host;

    // Connect to a socket
    if ( socket_tcp_connect(&_socket, socket_address_family_ipv4, ip, port) == 0 ) goto failed_to_create_socket;

    // Populate the connection struct
    p_connection->_tcp_socket = _socket;

    // return a pointer to the caller
    *pp_connection = p_connection;

    // success
    return 1;

    // error handling
    {

        // argument errors
        {
            no_connection:
                #ifndef NDEBUG
                    log_error("[distribute] [connection] Null pointer provided for parameter \"pp_connection\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;

            no_hostname:
                #ifndef NDEBUG
                    log_error("[distribute] [connection] Null pointer provided for parameter \"p_hostname\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;

            no_port:
                #ifndef NDEBUG
                    log_error("[distribute] [connection] Parameter \"port\" can not equal zero in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }

        // Socket errors
        {
            failed_to_create_socket:
                #ifndef NDEBUG
                    log_error("[socket] Failed to construct socket in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
            
            failed_to_resolve_host:
                #ifndef NDEBUG
                    log_error("[socket] Failed to resolve host in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }

        // standard library errors
        {
            no_mem:
                #ifndef NDEBUG
                    log_error("[standard library] Failed to allocate memory in call to function \"%s\"\n", __FUNCTION__);
                #endif
                
                // error
                return 0;
        }
    }
}

int connection_listen ( connection **pp_connection, const short port, fn_connection_accept *pfn_accept_callback )
{

    // argument check
    if ( 0    ==                port ) goto no_port;
    if ( NULL == pfn_accept_callback ) goto no_accept_callback;

    // initialized data
    socket_tcp _listening_socket = -1;

    // create a socket
    if ( 0 == socket_tcp_create(&_listening_socket, socket_address_family_ipv4, port) ) goto failed_to_create_socket;

    // log
    log_info("Listening for connections on port %d...\n", port);
    
    // listen for connections
    while ( 1 ) 
        if ( 0 == socket_tcp_listen(
            _listening_socket, 
            connection_accept_callback, 
            pfn_accept_callback
        ) ) goto failed_to_listen;

    // success
    return 1;

    // error handling
    {

        // argument errors
        {
            no_port:
                #ifndef NDEBUG
                    log_error("[distribute] [connection] Parameter \"port\" can not equal zero in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;

            no_accept_callback:
                #ifndef NDEBUG
                    log_error("[distribute] [connection] Null pointer provided for parameter \"pfn_accept_callback\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }

        // socket errors
        {
            failed_to_create_socket:
                #ifndef NDEBUG
                    log_error("[socket] Failed to construct socket in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;

            failed_to_listen:
                #ifndef NDEBUG
                    log_error("[socket] Failed to listen on socket in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
    }
}

int connection_accept_callback
(        
    socket_tcp               _socket_tcp,
    socket_ip_address        ip_address,
    socket_port              port_number, 
    void              *const p_parameter
)
{

    // unused
    (void)ip_address;
    (void)port_number;
    (void)p_parameter;

    // initialized data
    connection *p_connection = NULL;

    // create a new connection
    if ( 0 == connection_create(&p_connection) ) goto failed_to_create_connection;

    // populate the struct
    *p_connection = (connection)
    {
        ._tcp_socket = _socket_tcp,
        .ip_address  = ip_address,
        .port_number = port_number
    };

    p_connection->_tcp_socket = _socket_tcp;

    // done
    return ((int (*)(connection *))p_parameter)(p_connection);

    // error handling
    {
        
        // distribute errors
        failed_to_create_connection:
            #ifndef NDEBUG
                log_error("Failed to create connection for new client\n");
            #endif

            // destroy the socket
            socket_tcp_destroy(&_socket_tcp);

            // error
            return 0;
    }

}

int connection_write ( connection *p_connection, void *p_data, size_t size )
{

    // argument check
    if ( NULL == p_connection ) goto no_connection;
    if ( NULL ==       p_data ) goto no_data;

    // send the data
    if ( 0 == socket_tcp_send(p_connection->_tcp_socket, p_data, size) ) goto failed_to_send_data;

    // success
    return 1;

    // error handling
    {

        // argument errors
        {
            no_connection:
                #ifndef NDEBUG
                    log_error("[distribute] [connection] Null pointer provided for parameter \"p_connection\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
            
            no_data:
                #ifndef NDEBUG
                    log_error("[distribute] [connection] Null pointer provided for parameter \"p_data\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }

        // socket errors
        {
            failed_to_send_data:
                #ifndef NDEBUG
                    log_error("[distribute] [connection] Failed to send data in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
    }
}

int connection_read ( connection *p_connection, void *p_data, size_t *p_size )
{

    // argument check
    if ( NULL == p_connection ) goto no_connection;
    if ( NULL ==       p_data ) goto no_data;
    if ( NULL ==       p_size ) goto no_size;

    // initialized data
    size_t  size       = 0;
    size_t  bytes_read = 0;

    // read the length from the socket
    if ( 0 == socket_tcp_receive(p_connection->_tcp_socket, &size, sizeof(size_t)) ) goto failed_to_read_size;

    // read data
    while ( bytes_read < size )
    {

        // initialized data
        int result = socket_tcp_receive(p_connection->_tcp_socket, (char*)p_data + bytes_read, size - bytes_read);

        // error check
        if (result == 0) goto failed_to_receive;

        // accumulate
        bytes_read += result;
    }

    // return the size to the caller
    *p_size = bytes_read;

    // success
    return 1;

    // error handling
    {

        // argument errors
        {
            no_connection:
                #ifndef NDEBUG
                    log_error("[distribute] [connection] Null pointer provided for parameter \"p_connection\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;

            no_data:
                #ifndef NDEBUG
                    log_error("[distribute] [connection] Null pointer provided for parameter \"p_data\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;

            no_size:
                #ifndef NDEBUG
                    log_error("[distribute] [connection] Null pointer provided for parameter \"p_size\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }

        // socket errors
        {
            failed_to_read_size:
                #ifndef NDEBUG
                    log_error("[distribute] [connection] Failed to read size in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
            
            failed_to_receive:
                #ifndef NDEBUG
                    log_error("[distribute] [connection] Failed to receive data in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }

        // distribute errors
        {
            buffer_too_small:
                #ifndef NDEBUG
                    log_error("[distribute] [connection] Buffer too small for incoming message in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
    }
}

int connection_info ( connection *p_connection )
{

    // argument check
    if ( NULL == p_connection ) goto no_connection;

    // log connection info
    log_info("Connection @%p\n", p_connection);
    log_info("\tIP Address: %hhd.%hhd.%hhd.%hhd\n",
        p_connection->ip_address >> 24,
        p_connection->ip_address >> 16,
        p_connection->ip_address >> 8,
        p_connection->ip_address
    );
    log_info("\tPort Number: %d\n", p_connection->port_number);
    log_info("\tSocket fileno: %d\n", p_connection->_tcp_socket);

    // success
    return 1;

    // error handling
    {

        // argument errors
        {
            no_connection:
                #ifndef NDEBUG
                    log_error("[distribute] [connection] Null pointer provided for parameter \"p_connection\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
    }
}

int connection_destroy ( connection **pp_connection )
{
    if (pp_connection == NULL || *pp_connection == NULL) return 0;

    socket_tcp_destroy(&((*pp_connection)->_tcp_socket));
    *pp_connection = default_allocator(*pp_connection, 0);

    return 1;
}
