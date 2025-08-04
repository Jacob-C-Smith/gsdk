/** ! 
 * Stream implementation
 * 
 * @file src/core/stream.c
 * 
 * @author Jacob Smith
 */

// standard library
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <limits.h>
#include <time.h>

// posix
#include <fcntl.h>
#include <sys/stat.h>

// core
#include <core/log.h>
#include <core/stream.h>

// function declarations
/** !
 * Read a FILE
 * 
 * @param p_stream the stream
 * @param buffer   the buffer to read into
 * @param size     the size of the buffer
 * 
 * @return number of bytes read, 0 on error
 */
int stream_read_FILE ( stream *p_stream, void *buffer, size_t size );

/** !
 * Write a FILE
 * 
 * @param p_stream the stream
 * @param buffer   the buffer to write from
 * @param size     the size of the buffer
 * 
 * @return number of bytes written, 0 on error
 */
int stream_write_FILE ( stream *p_stream, const void *buffer, size_t size );

/** !
 * Close a FILE
 * 
 * @param p_stream the stream
 * 
 * @return 1 on success, 0 on error
 */
int stream_close_FILE ( stream *p_stream );

/** !
 * Read from a socket
 * 
 * @param p_stream the stream
 * @param buffer   the buffer to read into
 * @param size     the size of the buffer
 * 
 * @return number of bytes read, 0 on error
 */
int stream_read_socket ( stream *p_stream, void *buffer, size_t size );

/** !
 * Write to a socket
 * 
 * @param p_stream the stream
 * @param buffer   the buffer to write from
 * @param size     the size of the buffer
 * 
 * @return number of bytes written, 0 on error
 */
int stream_write_socket ( stream *p_stream, const void *buffer, size_t size );

/** !
 * Close a socket
 * 
 * @param p_stream the stream
 * 
 * @return 1 on success, 0 on error
 */
int stream_close_socket ( stream *p_stream );

// function definitions
int stream_from_path ( stream **pp_stream, const char *p_path )
{

    // argument check
    if ( NULL == pp_stream ) goto no_stream;
    if ( NULL == p_path )    goto no_path;

    // initialized data
    stream *p_stream = NULL;
    int result = open(p_path, O_RDWR | O_CREAT, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);

    // error check
    if ( -1 == result ) goto failed_to_open_stream;

    // construct a stream from the file descriptor
    if ( 0 == stream_from_fd(&p_stream, result) ) goto failed_to_open_stream;

    // return a pointer to the caller
    *pp_stream = p_stream;

    // success
    return 1;

    // error handling
    {

        // argument errors
        {
            no_stream:
                #ifndef NDEBUG
                    log_error("[stream] Null pointer provided for parameter \"pp_stream\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;

            no_path:
                #ifndef NDEBUG
                    log_error("[stream] Null pointer provided for parameter \"p_path\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }

        // stream errors
        {
            failed_to_open_stream:

                // free the stream
                default_allocator(p_stream, 0);

                #ifndef NDEBUG
                    log_error("[stream] Failed to open stream at path \"%s\" in call to function \"%s\"\n", p_path, __FUNCTION__);
                #endif

                // error
                return 0;
        }

        // standard library errors
        {
            no_mem:
                #ifndef NDEBUG
                    log_error("[socket] Failed to allocate memory in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
    }
}

int stream_from_fd ( stream **pp_stream, int fd )
{

    // argument check
    if ( NULL == pp_stream ) goto no_stream;

    // initialized data
    stream *p_stream = default_allocator(0, sizeof(stream));
    struct stat _file = { 0 };

    // file status
    fstat(fd, &_file);

    // store the file descriptor 
    p_stream->state.fd = fd;

    // strategy
    switch ( _file.st_mode & S_IFMT )
    {

        // regular file
        case S_IFREG:
            
            // set the type    
            p_stream->_type = STREAM_TYPE_FILE;

            // set the functions
            p_stream->functions.pfn_read  = stream_read_FILE;
            p_stream->functions.pfn_write = stream_write_FILE;
            p_stream->functions.pfn_close = stream_close_FILE;

            // open a FILE
            p_stream->data.file = fdopen(fd, "r+");

            // store the parameter
            p_stream->p_parameter = p_stream->data.file;

            // done
            break;

        // directory
        case S_IFDIR:
            
            // set the type    
            p_stream->_type = STREAM_TYPE_DIRECTORY;

            // done
            break;

        // socket
        case S_IFSOCK:
            
            // set the type    
            p_stream->_type = STREAM_TYPE_TCP_SOCKET;

            // done
            break;
    }
    
    // // file descriptor
    // p_stream->data.fd = fd;
    
    // return a pointer to the caller
    *pp_stream = p_stream;

    // success
    return 1;

    // error handling
    {

        // argument errors
        {
            no_stream:
                #ifndef NDEBUG
                    log_error("[stream] Null pointer provided for parameter \"pp_stream\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }

        // standard library errors
        {
            no_mem:
                #ifndef NDEBUG
                    log_error("[stream] Failed to allocate memory in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
    }
}

int stream_from_socket_tcp ( stream **pp_stream, socket_tcp socket )
{

    // argument check
    if ( NULL == pp_stream ) goto no_stream;

    // initialized data
    stream *p_stream = default_allocator(0, sizeof(stream));

    // set the type    
    p_stream->_type = STREAM_TYPE_TCP_SOCKET;

    // set the functions
    p_stream->functions.pfn_read  = stream_read_socket;
    p_stream->functions.pfn_write = stream_write_socket;
    p_stream->functions.pfn_close = stream_close_socket;

    // store the socket
    p_stream->data._tcp_socket = socket;

    // store the parameter
    p_stream->p_parameter = (void *)p_stream->data._tcp_socket;

    // return a pointer to the caller
    *pp_stream = p_stream;

    // success
    return 1;

    // error handling
    {

        // argument errors
        {
            no_stream:
                #ifndef NDEBUG
                    log_error("[stream] Null pointer provided for parameter \"pp_stream\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }

        // standard library errors
        {
            no_mem:
                #ifndef NDEBUG
                    log_error("[stream] Failed to allocate memory in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
    }
}

int stream_read ( stream *p_stream, void *buffer, size_t size )
{

    // argument check
    if ( NULL == p_stream ) goto no_stream;

    // initialized data
    size_t bytes_read = 0;
    
    // error check
    if ( NULL == p_stream->functions.pfn_read ) goto unsupported_operation;

    // read
    bytes_read = p_stream->functions.pfn_read(p_stream, buffer, size);

    // error check
    if ( bytes_read < 0 ) goto failed_to_read_stream;
    
    // success
    return bytes_read;

    // error handling
    {

        // argument errors
        {
            no_stream:
                #ifndef NDEBUG
                    log_error("[stream] Null pointer provided for parameter \"p_stream\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
        
        // stream errors
        {
            failed_to_read_stream:
                #ifndef NDEBUG
                    log_error("[stream] Failed to read from stream in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;

            unsupported_operation:
                #ifndef NDEBUG
                    log_error("[stream] Stream 0x...%llx does not support \"%s\" operation\n", (size_t)p_stream & 0xffffff, __FUNCTION__);
                #endif

                // error
                return 0;
        }
    }
}

int stream_read_FILE ( stream *p_stream, void *buffer, size_t size )
{

    // argument check
    if ( NULL == p_stream ) goto no_stream;

    // initialized data
    size_t bytes_read = 0;

    // read
    bytes_read = fread(buffer, 1, size, p_stream->p_parameter);

    // error check
    if ( bytes_read < 0 ) goto failed_to_read_stream;

    // success
    return bytes_read;

    // error handling
    {

        // argument errors
        {
            no_stream:
                #ifndef NDEBUG
                    log_error("[stream] Null pointer provided for parameter \"p_stream\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }

        // stream errors
        {
            failed_to_read_stream:
                #ifndef NDEBUG
                    log_error("[stream] Failed to read from stream in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
    }
}

int stream_write_FILE ( stream *p_stream, const void *buffer, size_t size )
{

    // argument check
    if ( NULL == p_stream ) goto no_stream;

    // initialized data
    size_t bytes_written = 0;

    // write
    bytes_written = fwrite(buffer, 1, size, p_stream->p_parameter);

    // error check
    if ( bytes_written < 0 ) goto failed_to_write_stream;

    // success
    return bytes_written;

    // error handling
    {

        // argument errors
        {
            no_stream:
                #ifndef NDEBUG
                    log_error("[stream] Null pointer provided for parameter \"p_stream\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }

        // stream errors
        {
            failed_to_write_stream:
                #ifndef NDEBUG
                    log_error("[stream] Failed to write to stream in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
    }
}

int stream_close_FILE ( stream *p_stream )
{

    // argument check
    if ( NULL == p_stream ) goto no_stream;

    // close the FILE
    if ( fclose(p_stream->p_parameter) != 0 ) goto failed_to_close_stream;

    // success
    return 1;

    // error handling
    {

        // argument errors
        {
            no_stream:
                #ifndef NDEBUG
                    log_error("[stream] Null pointer provided for parameter \"p_stream\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }

        // stream errors
        {
            failed_to_close_stream:
                #ifndef NDEBUG
                    log_error("[stream] Failed to close stream in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
    }
}

int stream_read_socket ( stream *p_stream, void *buffer, size_t size )
{

    // argument check
    if ( NULL == p_stream ) goto no_stream;

    // initialized data
    size_t bytes_read = 0;

    // read
    bytes_read = socket_tcp_receive((socket_tcp)p_stream->p_parameter, buffer, size);

    // error check
    if ( bytes_read < 0 ) goto failed_to_read_stream;

    // success
    return bytes_read;

    // error handling
    {

        // argument errors
        {
            no_stream:
                #ifndef NDEBUG
                    log_error("[stream] Null pointer provided for parameter \"p_stream\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }

        // stream errors
        {
            failed_to_read_stream:
                #ifndef NDEBUG
                    log_error("[stream] Failed to read from stream in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
    }
}

int stream_write_socket ( stream *p_stream, const void *buffer, size_t size )
{

    // argument check
    if ( NULL == p_stream ) goto no_stream;

    // initialized data
    size_t bytes_written = 0;

    // write
    bytes_written = socket_tcp_send((socket_tcp)p_stream->p_parameter, buffer, size);

    // error check
    if ( bytes_written < 0 ) goto failed_to_write_stream;

    // success
    return bytes_written;

    // error handling
    {

        // argument errors
        {
            no_stream:
                #ifndef NDEBUG
                    log_error("[stream] Null pointer provided for parameter \"p_stream\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }

        // stream errors
        {
            failed_to_write_stream:
                #ifndef NDEBUG
                    log_error("[stream] Failed to write to stream in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
    }
}

int stream_close_socket ( stream *p_stream )
{

    // argument check
    if ( NULL == p_stream ) goto no_stream;

    // close the socket
    // if ( close(p_stream->data.tcp_socket) != 0 ) goto failed_to_close_stream;

    // success
    return 1;

    // error handling
    {

        // argument errors
        {
            no_stream:
                #ifndef NDEBUG
                    log_error("[stream] Null pointer provided for parameter \"p_stream\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }

        // stream errors
        {
            failed_to_close_stream:
                #ifndef NDEBUG
                    log_error("[stream] Failed to close stream in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
    }
}

int stream_write ( stream *p_stream, const void *buffer, size_t size )
{

    // argument check
    if ( NULL == p_stream ) goto no_stream;

    // initialized data
    size_t bytes_written = 0;

    // write
    bytes_written = p_stream->functions.pfn_write(p_stream, buffer, size);

    // error check
    if ( bytes_written < 0 ) goto failed_to_write_stream;

    // success
    return bytes_written;

    // error handling
    {

        // argument errors
        {
            no_stream:
                #ifndef NDEBUG
                    log_error("[stream] Null pointer provided for parameter \"p_stream\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }

        // stream errors
        {
            failed_to_write_stream:
                #ifndef NDEBUG
                    log_error("[stream] Failed to write to stream in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
    }
}

int stream_destroy ( stream **pp_stream )
{
    // argument check
    if ( NULL == pp_stream ) goto no_stream;

    // destroy the stream
    stream *p_stream = *pp_stream;
    
    // nullify the pointer
    *pp_stream = NULL;

    // success
    return 1;

    // error handling
    {
        // argument errors
        {
            no_stream:
                #ifndef NDEBUG
                    log_error("[stream] Null pointer provided for parameter \"pp_stream\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
    }
}