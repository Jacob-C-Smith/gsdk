/** !
 * High level abstractions for remote procedure calls
 *
 * @file distribute/rpc.c
 * 
 * @author Jacob Smith
 */

// header files
#include <performance/rpc.h>

// structure declarations
struct rpc_entry_s
{
    fn_rpc_handler *pfn_handler;
    fn_pack        *pfn_request_pack;
    fn_unpack      *pfn_request_unpack;
    fn_pack        *pfn_response_pack;
    fn_unpack      *pfn_response_unpack;
};

// type definitions
typedef struct rpc_entry_s rpc_entry;

// data
static dict *p_rpc_registry = NULL;

/** !
 * The callback for handling a new connection. Spawns a thread to handle RPCs.
 * 
 * @param p_connection the new connection
 * 
 * @return 1 on success, 0 on error
 */
int rpc_dispatch_thread_launcher( connection *p_connection );

/** !
 * The main loop for an RPC worker thread. Reads requests and calls handlers.
 * 
 * @param p_connection_raw the connection to handle
 * 
 * @return NULL
 */
void *rpc_dispatch_worker ( void *p_connection_raw );

// function definitions
void rpc_init ( void )
{
    // state check
    if ( p_rpc_registry != NULL ) return;

    // Construct a dictionary for the RPC registry
    dict_construct(&p_rpc_registry, 256, NULL);

    // done
    return;
}

int rpc_register
(
    const char     *p_name, 
    fn_rpc_handler *pfn_handler,
    fn_pack        *pfn_request_pack,
    fn_unpack      *pfn_request_unpack,
    fn_pack        *pfn_response_pack,
    fn_unpack      *pfn_response_unpack
)
{

    // singleton check
    if ( NULL == p_rpc_registry ) rpc_init();

    // argument check
    if ( NULL ==              p_name ) goto no_name;
    if ( NULL ==         pfn_handler ) goto no_handler;
    if ( NULL ==    pfn_request_pack ) goto no_request_pack;
    if ( NULL ==  pfn_request_unpack ) goto no_request_unpack;
    if ( NULL ==   pfn_response_pack ) goto no_response_pack;
    if ( NULL == pfn_response_unpack ) goto no_response_unpack;

    // initialized data
    rpc_entry *p_entry = malloc(sizeof(rpc_entry));

    // error check
    if ( NULL == p_entry ) goto no_mem;

    // populate the struct
    *p_entry = (rpc_entry)
    {
        .pfn_handler         = pfn_handler,
        .pfn_request_pack    = pfn_request_pack,
        .pfn_request_unpack  = pfn_request_unpack,
        .pfn_response_pack   = pfn_response_pack,
        .pfn_response_unpack = pfn_response_unpack
    };

    // add the rpc entry to the registry
    if ( 0 == dict_add(p_rpc_registry, p_name, p_entry) ) goto failed_to_register_rpc;

    // success
    return 1;

    // error handling
    {

        // argument errors
        {
            no_name:
                #ifndef NDEBUG
                    log_error("[distribute] [rpc] Null pointer provided for parameter \"name\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;

            no_handler:
                #ifndef NDEBUG
                    log_error("[distribute] [rpc] Null pointer provided for parameter \"pfn_handler\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;

            no_request_pack:
                #ifndef NDEBUG
                    log_error("[distribute] [rpc] Null pointer provided for parameter \"pfn_request_pack\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;

            no_request_unpack:
                #ifndef NDEBUG
                    log_error("[distribute] [rpc] Null pointer provided for parameter \"pfn_request_unpack\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;

            no_response_pack:
                #ifndef NDEBUG
                    log_error("[distribute] [rpc] Null pointer provided for parameter \"pfn_response_pack\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;

            no_response_unpack:
                #ifndef NDEBUG
                    log_error("[distribute] [rpc] Null pointer provided for parameter \"pfn_response_unpack\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;

            no_unpack:
                #ifndef NDEBUG
                    log_error("[distribute] [rpc] Null pointer provided for parameter \"pfn_unpack\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }

        // dict errors
        {
            failed_to_register_rpc:
                #ifndef NDEBUG
                    log_error("[distribute] [rpc] Failed to register RPC \"%s\" in call to function \"%s\"\n", p_name, __FUNCTION__);
                #endif

                // error
                return 0;
        }

        // standard library errors
        {
            no_mem:
                #ifndef NDEBUG
                    log_error("[digital signature] Failed to allocate memory in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
    }
}

int rpc_server_listen ( short port )
{

    // initialized data
    connection *p_connection = NULL;

    // listen for connections
    if ( 0 == connection_listen(&p_connection, port, (fn_connection_accept *)rpc_dispatch_thread_launcher) ) goto failed_to_listen;

    // success
    return 1;

    // error handling
    {

        // socket errors
        {
            failed_to_construct_socket:
                #ifndef NDEBUG
                    log_error("[distribute] [rpc] Failed to construct socket in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }

        // distribute errors
        {
            failed_to_listen:
                #ifndef NDEBUG
                    log_error("[distribute] [rpc] Failed to listen on socket in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
    }
}

int rpc_dispatch_thread_launcher( connection *p_connection )
{
    // argument check
    if ( NULL == p_connection ) return 0;

    // initialized data
    parallel_thread *p_thread = NULL;

    // start a worker thread to handle the RPC lifecycle for this connection
    if ( 0 == parallel_thread_start(&p_thread, rpc_dispatch_worker, (void *)p_connection) )
    {
        // on failure, close the connection
        connection_destroy(&p_connection);
        return 0;
    }

    // success
    return 1;
}

void *rpc_dispatch_worker ( void *p_connection_raw )
{

    // initialized data
    connection     *p_connection       = (connection *)p_connection_raw;
    char            _rpc_name[64]      = { 0 };
    char            request_buf[4096]  = { 0 },
                    response_buf[4096] = { 0 },
                   *p_request_buf      = request_buf;
    size_t          request_size       = 0,
                    response_size      = 0;
    rpc_entry      *p_rpc_entry        = NULL;
    void           *p_response         = NULL,
                   *p_request          = NULL,
                   *p_response_buf     = response_buf + 8;
    char            _param[4096]       = { 0 };

    // read the request
    if ( 0 == connection_read(p_connection, request_buf, &request_size) ) goto failed_to_read_size;

    // parse the rpc name
    p_request_buf += pack_unpack(p_request_buf, "%s", _rpc_name);

    // log the rpc
    log_info("rpc '%s' received\n", _rpc_name);

    // look up the RPC handler
    p_rpc_entry = dict_get(p_rpc_registry, _rpc_name);

    // unpack the request
    p_rpc_entry->pfn_request_unpack(_param, p_request_buf);

    // invoke the handler
    p_response = p_rpc_entry->pfn_handler(p_connection_raw, _param);

    // pack the response
    response_size = p_rpc_entry->pfn_response_pack(p_response_buf, p_response);

    // header should contain total message size (header + payload)
    *(size_t *)response_buf = response_size + sizeof(size_t);
    response_size += sizeof(size_t);

    // write the response
    if ( 0 == connection_write(p_connection, response_buf, response_size) ) goto failed_to_write_response;

    // success
    return (void *) 1;

    // error handling
    {

        // distribute errors
        {
            failed_to_read_size:
                #ifndef NDEBUG
                    log_error("[distribute] [rpc] Failed to read request size in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return NULL;

            failed_to_write_response:
                #ifndef NDEBUG
                    log_error("[distribute] [rpc] Failed to write response in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return NULL;
        }
    }
}

int rpc_call 
(
    connection *p_connection,
    const char *p_name,
    void       *p_args, 
    void       *p_response_buffer,
    size_t     *p_response_size
)
{

    // argument check
    if ( NULL ==    p_connection ) goto no_connection;
    if ( NULL ==          p_name ) goto no_name;
    if ( NULL ==          p_args ) goto no_args;
    if ( NULL == p_response_buffer ) goto no_response;
    if ( NULL == p_response_size ) goto no_response_size;

    // initialized data
    rpc_entry *p_rpc_entry        = dict_get(p_rpc_registry, p_name);
    char       request_buf[4096]  = { 0 },
               response_buf[4096] = { 0 };
    size_t     request_size       = 0,
               response_size      = 0;
    void      *p_request          = request_buf + 8;
    void      *p_response         = p_response_buffer;

    // serialize the RPC name
    p_request += pack_pack(p_request, "%s", p_name);

    // serialize the rpc request data
    p_request += p_rpc_entry->pfn_request_pack(p_request, p_args);

    // compute the request size
    request_size = (size_t)p_request - (size_t)request_buf;

    // error check
    if ( 0 == request_size ) goto failed_to_serialize_request;

    // store the size of the request
    *(size_t *)request_buf = request_size;

    // send the request
    if ( 0 == connection_write(p_connection, request_buf, request_size) ) goto failed_to_write_request;

    // read the response
    if ( 0 == connection_read(p_connection, response_buf, &response_size) ) goto failed_to_read_response;

    // parse the response
    response_size = p_rpc_entry->pfn_response_unpack(p_response, response_buf);
    if ( 0 == response_size ) goto failed_to_parse_response;

    // return the size to the caller
    *p_response_size = response_size;

    // success
    return 1;

    // error handling
    {

        // argument errors
        {
            no_connection:
                #ifndef NDEBUG
                    log_error("[distribute] [rpc] Null pointer provided for parameter \"pp_connection\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;

            no_name:
                #ifndef NDEBUG
                    log_error("[distribute] [rpc] Null pointer provided for parameter \"p_name\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;

            no_args:
                #ifndef NDEBUG
                    log_error("[distribute] [rpc] Null pointer provided for parameter \"p_args\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;

        no_response:
                #ifndef NDEBUG
            log_error("[distribute] [rpc] Null pointer provided for parameter \"p_response_buffer\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;

            no_response_size:
                #ifndef NDEBUG
                    log_error("[distribute] [rpc] Null pointer provided for parameter \"p_response_size\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }

        // distribute errors
        {
            failed_to_write_request:
                #ifndef NDEBUG
                    log_error("[distribute] [rpc] Failed to write request in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;

            failed_to_read_response:
                #ifndef NDEBUG
                    log_error("[distribute] [rpc] Failed to read response in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
    
        // pack errors
        {
            failed_to_serialize_request:
                #ifndef NDEBUG
                    log_error("[distribute] [rpc] Failed to serialize request in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
            
            failed_to_parse_response:
                #ifndef NDEBUG
                    log_error("[distribute] [rpc] Failed to parse response in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
    }
}
