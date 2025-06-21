/** !
 * High level abstractions for connection
 *
 * @file distribute/distribute.h
 * 
 * @author Jacob Smith
 */

// header files
#include <distribute/connection.h>

// structure definitions
struct connection_s
{
    socket_tcp _tcp_socket;

    struct
    {
        size_t size,
               read;
        char _buffer [DISTRIBUTE_CONNECTION_MAX_BUFFER_SIZE];
    } in;

    struct
    {
        size_t size,
               read;
        char _buffer [DISTRIBUTE_CONNECTION_MAX_BUFFER_SIZE];
    } out;
};

// function definitions
int connection_create ( const connection **const pp_connection )
{

    // argument check
    if ( pp_connection == (void *) 0 ) goto no_connection;

    // initialized data
    connection *p_connection = (void *) 0;

    // error check
    if ( realloc((void *) 0, sizeof(connection)) == (void *) 0 ) goto no_mem;

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
                    printf("[distribute] [connection] Null pointer provided for parametr \"pp_connection\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }

        // standard library errors
        {
            no_mem:
                #ifndef NDEBUG
                    printf("[standard library] Failed to allocate memory in call to function \"%s\"\n", __FUNCTION__);
                #endif
                
                // error
                return 0;
        }
    }
}

int connection_initiate_hostname ( const connection **const pp_connection, const char *p_hostname, const short port )
{

    // argument check
    if ( pp_connection == (void *) 0 ) goto no_connection;
    if ( p_hostname    == (void *) 0 ) goto no_hostname;
    if ( port          ==          0 ) goto no_port;

    // initialized data
    connection *p_connection = (void *) 0,
                 _connection = { 0 };
    socket_tcp   _socket     = 0;

    // error check
    if ( connection_create(&p_connection) == (void *) 0 ) goto no_mem;

    // Connect to a socket
    if ( socket_tcp_connect(&_socket, 0x0100007f, port) ) goto failed_to_create_socket;

    // Populate the connection struct
    _connection = (connection)
    {
        ._tcp_socket = _socket,
        .in = 
        {
            ._buffer = { 0 },
            .size    = DISTRIBUTE_CONNECTION_MAX_BUFFER_SIZE
        },
        .out = 
        {
            ._buffer = { 0 },
            .size    = DISTRIBUTE_CONNECTION_MAX_BUFFER_SIZE
        }
    };

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
                    printf("[distribute] [connection] Null pointer provided for parameter \"pp_connection\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;

            no_hostname:
                #ifndef NDEBUG
                    printf("[distribute] [connection] Null pointer provided for parameter \"p_hostname\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;

            no_port:
                #ifndef NDEBUG
                    printf("[distribute] [connection] Parameter \"port\" can not equal zero in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }

        // Socket errors
        {
            failed_to_create_socket:
                #ifndef NDEBUG
                    printf("[socket] Failed to construct socket in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }

        // standard library errors
        {
            no_mem:
                #ifndef NDEBUG
                    printf("[standard library] Failed to allocate memory in call to function \"%s\"\n", __FUNCTION__);
                #endif
                
                // error
                return 0;
        }
    }
}

int connection_write ( connection *p_connection, void *p_data, size_t size )
{

    // TODO: Argument check
    //

    // TODO: Error check
    // Send some data
    socket_tcp_send(p_connection->_tcp_socket, p_data, size);

    // success
    return 1;

    // error handling
    {

        // argument errors
        {

            // error
            return 0;
        }
    }
}

int connection_read ( connection *p_connection, void *p_data, size_t size )
{

    // TODO: Argument check
    //

    // TODO: Error check
    // Send some data
    socket_tcp_receive(p_connection->_tcp_socket, p_connection->in._buffer, p_connection->in.size);

    // success
    return 1;

    // error handling
    {

        // argument errors
        {

            // error
            return 0;
        }
    }
}