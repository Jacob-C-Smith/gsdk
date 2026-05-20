/** !
 * Stream implementation
 *
 * @file src/core/stream/stream.c
 * 
 * @author Jacob Smith
 */

// header file
#include <core/stream.h>

// forward declarations
/// read
fn_stream_read stream_read_buffer;
fn_stream_read stream_read_file;
fn_stream_read stream_read_socket_tcp;

/// write
fn_stream_write stream_write_buffer;
fn_stream_write stream_write_dynamic_buffer;
fn_stream_write stream_write_file;
fn_stream_write stream_write_socket_tcp;

/// size
fn_stream_size stream_size_buffer;
fn_stream_size stream_size_file;
fn_stream_size stream_size_socket_tcp;

/// flush
fn_stream_flush stream_flush_buffer;
fn_stream_flush stream_flush_file;
fn_stream_flush stream_flush_socket_tcp;

/// seek
fn_stream_seek stream_seek_buffer;
fn_stream_seek stream_seek_file;
fn_stream_seek stream_seek_socket_tcp;

/// close
fn_stream_close stream_close_buffer;
fn_stream_close stream_close_dynamic_buffer;
fn_stream_close stream_close_file;
fn_stream_close stream_close_socket_tcp;

// function definitions
int stream_from_path
( 
    stream     **pp_stream, 
    const char  *p_path
)
{

    // argument check
    if ( NULL == pp_stream ) goto no_stream;
    if ( NULL ==    p_path ) goto no_path;

    // initialized data
    stream *p_stream = NULL;
    FILE   *p_f      = NULL;
    size_t  size     = -1;

    // open the file
    p_f = fopen(p_path, "wb+");
    if ( NULL == p_f ) goto no_file;

    // allocate memory for a stream
    p_stream = default_allocator(0, sizeof(stream));
    if ( NULL == p_stream ) goto no_mem;

    // get the file size
    fseek(p_f, 0, SEEK_END),
    size = ftell(p_f),
    fseek(p_f, 0, SEEK_SET);

    // populate the stream structure
    *p_stream = (stream)
    {
        .p_data    = p_f,
        .type      = STREAM_TYPE_FILE,
        .size      = size,
        .cursor    = 0,
        .pfn_read  = stream_read_file,
        .pfn_write = stream_write_file,
        .pfn_size  = stream_size_file,
        .pfn_flush = stream_flush_file,
        .pfn_seek  = stream_seek_file,
        .pfn_close = stream_close_file,
    };

    // construct a lock
    mutex_create(&p_stream->_lock);

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

        // standard library errors
        {
            no_file:
                #ifndef NDEBUG
                    log_error("[stream] Failed to open file \"%s\" in call to function \"%s\"\n", p_path, __FUNCTION__);
                #endif

                // error
                return 0;

            no_mem:
                #ifndef NDEBUG
                    log_error("[interfaces] Failed to allocate memory in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
    }
}

int stream_from_file 
( 
    stream **pp_stream, 
    FILE    *p_f
)
{

    // argument check
    if ( NULL == pp_stream ) goto no_stream;
    if ( NULL ==       p_f ) goto no_file;

    // initialized data
    stream *p_stream = NULL;
    size_t  size     = -1;

    // allocate memory for a stream
    p_stream = default_allocator(0, sizeof(stream));
    if ( NULL == p_stream ) goto no_mem;

    // get the file size
    fseek(p_f, 0, SEEK_END);
    size = ftell(p_f);
    fseek(p_f, 0, SEEK_SET);

    // populate the stream structure
    *p_stream = (stream)
    {
        .p_data = p_f,
        .type   = STREAM_TYPE_FILE,
        .size   = size,
        .cursor = 0,
        .pfn_read  = stream_read_file,
        .pfn_write = stream_write_file,
        .pfn_size  = stream_size_file,
        .pfn_flush = stream_flush_file,
        .pfn_seek  = stream_seek_file,
        .pfn_close = stream_close_file,
    };

    // construct a lock
    mutex_create(&p_stream->_lock);

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
                
            no_file:
                #ifndef NDEBUG
                    log_error("[stream] Null pointer provided for parameter \"p_file\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }

        // standard library errors
        {
            no_mem:
                #ifndef NDEBUG
                    log_error("[interfaces] Failed to allocate memory in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
    }
}

int stream_from_buffer
( 
    stream **pp_stream, 
    void    *p_buffer, 
    size_t  size
)
{

    // argument check
    if ( NULL == pp_stream ) goto no_stream;
    if ( NULL ==  p_buffer ) goto no_buffer;
    if ( 0    ==      size ) goto no_size;

    // initialized data
    stream *p_stream = NULL;

    // allocate memory for a stream
    p_stream = default_allocator(0, sizeof(stream));
    if ( NULL == p_stream ) goto no_mem;

    // populate the stream structure
    *p_stream = (stream)
    {
        .p_data    = p_buffer,
        .type      = STREAM_TYPE_BUFFER,
        .size      = size,
        .cursor    = 0,
        .pfn_read  = stream_read_buffer,
        .pfn_write = stream_write_buffer,
        .pfn_size  = stream_size_buffer,
        .pfn_flush = stream_flush_buffer,
        .pfn_seek  = stream_seek_buffer,
        .pfn_close = stream_close_buffer,
    };

    // construct a lock
    mutex_create(&p_stream->_lock);

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
                
            no_buffer:
                #ifndef NDEBUG
                    log_error("[stream] Null pointer provided for parameter \"p_buffer\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
                
            no_size:
                #ifndef NDEBUG
                    log_error("[stream] Null pointer provided for parameter \"size\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }

        // standard library errors
        {
            no_mem:
                #ifndef NDEBUG
                    log_error("[interfaces] Failed to allocate memory in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
    }
}

int stream_from_dynamic_buffer ( stream **pp_stream )
{

    // argument check
    if ( NULL == pp_stream ) goto no_stream;

    // initialized data
    stream *p_stream = NULL;
    void   *p_buffer = NULL;
    size_t  size     = 1024;

    // allocate memory for a buffer
    p_buffer = default_allocator(0, size);
    if ( NULL == p_buffer ) goto no_mem;

    // allocate memory for a stream
    p_stream = default_allocator(0, sizeof(stream));
    if ( NULL == p_stream ) goto no_mem;

    // populate the stream structure
    *p_stream = (stream)
    {
        .p_data    = p_buffer,
        .type      = STREAM_TYPE_DYNAMIC_BUFFER,
        .size      = size,
        .cursor    = 0,
        .pfn_read  = stream_read_buffer,
        .pfn_write = stream_write_dynamic_buffer,
        .pfn_size  = stream_size_buffer,
        .pfn_flush = stream_flush_buffer,
        .pfn_seek  = stream_seek_buffer,
        .pfn_close = stream_close_dynamic_buffer,
    };

    // construct a lock
    mutex_create(&p_stream->_lock);

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
                    log_error("[interfaces] Failed to allocate memory in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
    }
}

int stream_from_tcp_socket 
( 
    stream     **pp_stream, 
    socket_tcp    _socket 
)
{

    // argument check
    if ( NULL == pp_stream ) goto no_stream;

    // initialized data
    stream *p_stream = NULL;

    // allocate memory for a stream
    p_stream = default_allocator(0, sizeof(stream));
    if ( NULL == p_stream ) goto no_mem;

    // populate the stream structure
    *p_stream = (stream)
    {
        .p_data    = (void *)(size_t)_socket,
        .type      = STREAM_TYPE_BUFFER,
        .size      = -1,
        .cursor    = 0,

        .pfn_read  = stream_read_socket_tcp,
        .pfn_write = stream_write_socket_tcp,
        .pfn_size  = stream_size_socket_tcp,
        .pfn_flush = stream_flush_socket_tcp,
        .pfn_seek  = stream_seek_socket_tcp,
        .pfn_close = stream_close_socket_tcp,
    };

    // construct a lock
    mutex_create(&p_stream->_lock);

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
                    log_error("[interfaces] Failed to allocate memory in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
    }
}

int stream_read ( stream *p_stream, void *p_data, size_t size )
{

    // argument check
    if ( NULL == p_stream ) goto no_stream;
    if ( NULL ==   p_data ) goto no_data;

    // initialized data
    int result = -1;

    // lock
    mutex_lock(&p_stream->_lock);

    // read the data
    result = p_stream->pfn_read(p_stream, p_data, size);

    // unlock
    mutex_unlock(&p_stream->_lock);

    // success
    return result;

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
                
            no_data:
                #ifndef NDEBUG
                    log_error("[stream] Null pointer provided for parameter \"p_data\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
    }
}

int stream_write ( stream *p_stream, void *p_data, size_t size )
{

    // argument check
    if ( NULL == p_stream ) goto no_stream;
    if ( NULL ==   p_data ) goto no_data;

    // initialized data
    int result = -1;

    // lock
    mutex_lock(&p_stream->_lock);

    // read the data
    result = p_stream->pfn_write(p_stream, p_data, size);

    // unlock
    mutex_unlock(&p_stream->_lock);

    // success
    return result;

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
                
            no_data:
                #ifndef NDEBUG
                    log_error("[stream] Null pointer provided for parameter \"p_data\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
    }
}

int stream_flush ( stream *p_stream )
{

    // argument check
    if ( NULL == p_stream ) goto no_stream;

    // initialized data
    int result = -1;

    // lock
    mutex_lock(&p_stream->_lock);

    // read the data
    result = p_stream->pfn_flush(p_stream);

    // unlock
    mutex_unlock(&p_stream->_lock);

    // success
    return result;

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
    }
}

int stream_seek ( stream *p_stream, long offset, enum stream_seek_e whence )
{

    // argument check
    if ( NULL ==           p_stream ) goto no_stream;
    if ( NULL == p_stream->pfn_seek ) return 0;

    // initialized data
    int result = -1;

    // lock
    mutex_lock(&p_stream->_lock);

    // seek
    result = p_stream->pfn_seek(p_stream, offset, whence);

    // unlock
    mutex_unlock(&p_stream->_lock);

    // success
    return result;

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
    }
}

int stream_tell ( stream *p_stream )
{

    // argument check
    if ( NULL == p_stream ) goto no_stream;
    
    // initialized data
    size_t cursor = -1;

    // lock
    mutex_lock(&p_stream->_lock);

    // tell
    cursor = p_stream->cursor;

    // unlock
    mutex_unlock(&p_stream->_lock);

    // success
    return (int) cursor;

    // error handling
    {
        
        // argument errors
        {
            no_stream:
                #ifndef NDEBUG
                    log_error("[stream] Null pointer provided for parameter \"p_stream\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return -1;
        }
    }
}

bool stream_eof ( stream *p_stream )
{

    // argument check
    if ( NULL == p_stream ) goto no_stream;

    // initialized data
    bool eof = false;

    // lock
    mutex_lock(&p_stream->_lock);

    // eof?
    eof = ( p_stream->size > 0 ) ? ( p_stream->cursor >= p_stream->size ) : false;

    // unlock
    mutex_unlock(&p_stream->_lock);

    // success
    return eof;

    // error handling
    {
        
        // argument errors
        {
            no_stream:
                #ifndef NDEBUG
                    log_error("[stream] Null pointer provided for parameter \"p_stream\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return true;
        }
    }
}

int stream_peek ( stream *p_stream, void *p_data, size_t size )
{

    // argument check
    if ( NULL == p_stream ) goto no_stream;
    if ( NULL ==   p_data ) goto no_data;

    // initialized data
    int read = -1;

    // lock
    mutex_lock(&p_stream->_lock);

    // seek 
    p_stream->pfn_seek(p_stream, 0, STREAM_SEEK_CURSOR);

    // read the data
    read = p_stream->pfn_read(p_stream, p_data, size);

    // seek back
    p_stream->pfn_seek(p_stream, -(long)read, STREAM_SEEK_CURSOR);

    // unlock
    mutex_unlock(&p_stream->_lock);

    // success
    return read;

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

            no_data:
                #ifndef NDEBUG
                    log_error("[stream] Null pointer provided for parameter \"p_data\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
    }
}

int stream_copy ( stream *p_destination, stream *p_source, size_t size )
{

    // argument check
    if ( NULL == p_destination ) goto no_destination;
    if ( NULL ==      p_source ) goto no_source;

    // initialized data
    char   buffer[4096]  = { 0 };
    size_t total_written = 0;

    // copy
    while ( total_written < size )
    {

        // initialized data
        size_t to_read = ( size - total_written > sizeof(buffer) ) ? sizeof(buffer) : (size - total_written);
        int bytes_read = stream_read(p_source, buffer, to_read);
        int bytes_written = -1;

        // error check
        if ( bytes_read <= 0 ) break;

        // write
        bytes_written = stream_write(p_destination, buffer, bytes_read);

        // error check
        if ( bytes_written <= 0 ) break;

        // update written
        total_written += bytes_written;
    }

    // success
    return (int) total_written;

    // error handling
    {

        // argument errors
        {
            no_destination:
                #ifndef NDEBUG
                    log_error("[stream] Null pointer provided for parameter \"p_destination\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;

            no_source:  
                #ifndef NDEBUG
                    log_error("[stream] Null pointer provided for parameter \"p_source\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
    }
}

int stream_pipe ( stream *p_destination, stream *p_source )
{

    // argument check
    if ( NULL == p_destination ) goto no_destination;
    if ( NULL ==      p_source ) goto no_source;

    // initialized data
    char   buffer[4096]  = { 0 };
    size_t total_written = 0;

    // pipe
    while ( !stream_eof(p_source) )
    {

        // initialized data
        int bytes_read    = stream_read(p_source, buffer, sizeof(buffer));
        int bytes_written = -1;

        // error check
        if ( bytes_read <= 0 ) break;

        // write
        bytes_written = stream_write(p_destination, buffer, bytes_read);

        // error check
        if ( bytes_written <= 0 ) break;

        // update written
        total_written += bytes_written;
    }

    // success
    return (int) total_written;

    // error handling
    {

        // argument errors
        {
            no_destination:
                #ifndef NDEBUG
                    log_error("[stream] Null pointer provided for parameter \"p_destination\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;

            no_source:  
                #ifndef NDEBUG
                    log_error("[stream] Null pointer provided for parameter \"p_source\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
    }
}

int stream_size ( stream *p_stream )
{

    // argument check
    if ( NULL == p_stream ) goto no_stream;

    // initialized data
    int result = 0;

    // lock
    mutex_lock(&p_stream->_lock);

    // store the size
    result = p_stream->pfn_size(p_stream);

    // unlock
    mutex_unlock(&p_stream->_lock);

    // success
    return result;

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
    }
}

int stream_destroy ( stream **pp_stream )
{

    // argument check
    if ( NULL ==  pp_stream ) goto no_stream;

    // initialized data
    stream *p_stream = *pp_stream;

    // fast fail
    if ( NULL == p_stream ) return 1;

    // lock
    mutex_lock(&p_stream->_lock);

    // close
    p_stream->pfn_close(p_stream);

    // unlock
    mutex_unlock(&p_stream->_lock);

    // destroy the lock
    mutex_destroy(&p_stream->_lock);

    // free the stream
    default_allocator(p_stream, 0);

    // clear the pointer
    *pp_stream = NULL;

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

int stream_read_file ( stream *p_stream, void *p_data, size_t size ) 
{ 
    
    // initialized data
    int read = fread(p_data, 1, size, (FILE *) p_stream->p_data); 
    
    // update cursor
    if ( read > 0 ) p_stream->cursor += read;
    
    // success
    return read;
}

int stream_write_file ( stream *p_stream, void *p_data, size_t size ) 
{ 

    // initialized data
    int written = fwrite(p_data, 1, size, (FILE *) p_stream->p_data); 

    // update cursor
    if ( written > 0 ) p_stream->cursor += written;

    // success
    return written;
}

int stream_flush_file ( stream *p_stream ) 
{ 

    // success
    return ( 0 == fflush((FILE *) p_stream->p_data) ); 
}

int stream_seek_file ( stream *p_stream, long offset, enum stream_seek_e whence )
{

    // initialized data
    int _whence = -1;
    int result = -1;

    // strategy
    switch ( whence )
    {
        case STREAM_SEEK_SET:    _whence = SEEK_SET; break;
        case STREAM_SEEK_CURSOR: _whence = SEEK_CUR; break;
        case STREAM_SEEK_END:    _whence = SEEK_END; break;
        default:                 return 0;
    }

    // seek
    result = fseek((FILE *) p_stream->p_data, offset, _whence);

    // error check
    if ( 0 == result ) p_stream->cursor = ftell((FILE *) p_stream->p_data);

    // success
    return ( 0 == result );
}

int stream_close_file ( stream *p_stream )
{

    // success
    return ( 0 == fclose((FILE *) p_stream->p_data) );
}

int stream_read_buffer ( stream *p_stream, void *p_data, size_t size ) 
{ 

    // initialized data
    size_t to_read = ( p_stream->cursor + size > p_stream->size ) ? ( p_stream->size - p_stream->cursor ) : size;

    // error check
    if ( 0 == to_read ) return 0;

    // copy
    memcpy(p_data, (char *) p_stream->p_data + p_stream->cursor, to_read); 

    // update cursor
    p_stream->cursor += to_read;

    // success
    return (int) to_read; 
}

int stream_write_buffer ( stream *p_stream, void *p_data, size_t size ) 
{ 
    
    // initialized data
    size_t written = 0;
    size_t remaining = size;
    size_t capacity = 0;
    size_t to_write = 0;

    // write everything
    while ( remaining > 0 )
    {

        // flush it
        if ( p_stream->cursor == p_stream->size ) 
            if ( 0 == p_stream->pfn_flush(p_stream) ) break;
 
        // store capacity
        capacity = p_stream->size - p_stream->cursor;

        // amount to write
        to_write = (capacity < remaining) ? capacity : remaining;

        // copy
        memcpy((char *) p_stream->p_data + p_stream->cursor, (char *) p_data + written, to_write);

        // update cursor
        p_stream->cursor += to_write;

        // update written bytes
        written += to_write;

        // update remaining bytes
        remaining -= to_write;
    }

    // success
    return (int) written;
}

int stream_flush_buffer ( stream *p_stream ) 
{ 

    // unused
    (void) p_stream;

    // success
    return 0;
}

int stream_seek_buffer ( stream *p_stream, long offset, enum stream_seek_e whence )
{

    // initialized data
    size_t new_cursor = 0;

    // strategy
    switch ( whence )
    {
        case STREAM_SEEK_SET:

            // error check
            if ( offset < 0 ) return 0;

            // set the new cursor
            new_cursor = (size_t)offset;

            // done
            break;

        case STREAM_SEEK_CURSOR:

            // error check
            if ( offset < 0 && (size_t)-offset > p_stream->cursor ) return 0;

            // set the new cursor
            new_cursor = p_stream->cursor + offset;

            // done
            break;

        case STREAM_SEEK_END:

            // error check
            if ( offset < 0 && (size_t)-offset > p_stream->size ) return 0;

            // set the new cursor
            new_cursor = p_stream->size + offset;

            // done
            break;

        // default
        default: return 0;
    }

    // error check
    if ( new_cursor > p_stream->size ) return 0;

    // update cursor
    p_stream->cursor = new_cursor;

    // success
    return 1;
}

int stream_close_buffer ( stream *p_stream )
{ 

    // unused
    (void) p_stream;

    // success
    return 1; 
}

int stream_write_dynamic_buffer ( stream *p_stream, void *p_data, size_t size ) 
{ 
    
    // grow?
    if ( p_stream->cursor + size > p_stream->size )
    {

        // initialized data
        size_t new_size = ( p_stream->size * 2 > p_stream->cursor + size ) ? ( p_stream->size * 2 ) : ( p_stream->cursor + size );

        // resize the buffer
        p_stream->p_data = default_allocator(p_stream->p_data, new_size);

        // error check
        if ( NULL == p_stream->p_data ) return 0;

        // update size
        p_stream->size = new_size;
    }

    // copy
    memcpy((char *) p_stream->p_data + p_stream->cursor, p_data, size);

    // update cursor
    p_stream->cursor += size;

    // success
    return (int) size;
}

int stream_close_dynamic_buffer ( stream *p_stream )
{

    // release the buffer
    p_stream->p_data = default_allocator(p_stream->p_data, 0);

    // success
    return 1;
}

int stream_size_file ( stream *p_stream ) 
{ 

    // initialized data
    FILE *p_f = p_stream->p_data;
    size_t size = 0;

    // get the file size
    fseek(p_f, 0, SEEK_END);
    size = ftell(p_f);
    fseek(p_f, 0, SEEK_SET);

    // update the file size
    p_stream->size = size;

    // success
    return (int) p_stream->size; 
}

int stream_size_buffer ( stream *p_stream ) 
{ 
    
    // success
    return p_stream->size;
}

int stream_read_socket_tcp ( stream *p_stream, void *p_data, size_t size ) 
{ 

    // initialized data
    socket_tcp _socket = (socket_tcp)(size_t)p_stream->p_data;
    int bytes_read = socket_tcp_receive(_socket, p_data, size);

    // update cursor
    if ( bytes_read > 0 ) p_stream->cursor += bytes_read;

    // success
    return bytes_read; 
}

int stream_write_socket_tcp ( stream *p_stream, void *p_data, size_t size ) 
{ 

    // initialized data
    socket_tcp _socket = (socket_tcp)(size_t)p_stream->p_data;
    int bytes_written = socket_tcp_send(_socket, p_data, size);

    // update cursor
    if ( bytes_written > 0 ) p_stream->cursor += bytes_written;

    // success
    return bytes_written; 
}

int stream_size_socket_tcp ( stream *p_stream ) 
{ 

    // unused
    (void) p_stream;

    // success
    return -1;
}

int stream_flush_socket_tcp ( stream *p_stream ) 
{ 

    // unused
    (void) p_stream;
    
    // success
    return 1;
}

int stream_seek_socket_tcp ( stream *p_stream, long offset, enum stream_seek_e whence )
{

    // unused
    (void) p_stream;
    (void) offset;
    (void) whence;

    // error
    return 0;
}

int stream_close_socket_tcp ( stream *p_stream )
{

    // unused
    (void) p_stream;

    // success
    return 1;
}
