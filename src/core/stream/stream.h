/** !
 * Stream interface
 *
 * @file src/core/stream/stream.h 
 * 
 * @author Jacob Smith
 */

// header guard
#pragma once

// standard library
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

// gsdk
/// core
#include <core/log.h>
#include <core/hash.h>
#include <core/sync.h>
#include <core/interfaces.h>

// enumeration definitions
enum stream_type_e
{
    STREAM_TYPE_FILE           = 0,
    STREAM_TYPE_BUFFER         = 1,
    STREAM_TYPE_DYNAMIC_BUFFER = 2,
    STREAM_TYPE_QUANTITY       = 3
};

enum stream_seek_e
{
    STREAM_SEEK_SET      = 0,
    STREAM_SEEK_CURSOR   = 1,
    STREAM_SEEK_END      = 2,
    STREAM_SEEK_QUANTITY = 3
};

// structure declarations
struct stream_s;

// type definitions
typedef struct stream_s stream;
typedef int (fn_stream_read)  ( stream *p_stream, void *p_data, size_t size );
typedef int (fn_stream_write) ( stream *p_stream, void *p_data, size_t size );
typedef int (fn_stream_size)  ( stream *p_stream );
typedef int (fn_stream_flush) ( stream *p_stream );
typedef int (fn_stream_seek)  ( stream *p_stream, long offset, enum stream_seek_e whence );
typedef int (fn_stream_close) ( stream *p_stream );

// structure definitions
struct stream_s
{
    void               *p_data;
    enum stream_type_e  type;
    size_t              size;
    size_t              cursor;
    mutex               _lock;
    
    fn_stream_read     *pfn_read;
    fn_stream_write    *pfn_write; 
    fn_stream_size     *pfn_size;
    fn_stream_flush    *pfn_flush; 
    fn_stream_seek     *pfn_seek;
    fn_stream_close    *pfn_close;
};

// function declarations
/// constructors
/** !
 * Construct a stream from a file path
 * 
 * @param pp_stream result
 * @param p_path    path to file
 * 
 * @return 1 on success, 0 on error
 */
int stream_from_path
( 
    stream     **pp_stream, 
    const char  *p_path
);

/** !
 * Construct a stream from a FILE
 * 
 * @param pp_stream result
 * @param p_f       the FILE
 * 
 * @return 1 on success, 0 on error
 */
int stream_from_file 
( 
    stream **pp_stream, 
    FILE    *p_f
);

/** !
 * Construct a stream from a buffer
 * 
 * @param pp_stream result
 * @param p_buffer  the buffer
 * 
 * @return 1 on success, 0 on error
 */
int stream_from_buffer 
( 
    stream **pp_stream, 
    void    *p_buffer, 
    size_t   size
);

/** !
 * Construct a dynamic stream
 * 
 * @param pp_stream result
 * 
 * @return 1 on success, 0 on error
 */
int stream_from_dynamic_buffer ( stream **pp_stream );

/// read
/** !
 * Read from a stream
 * 
 * @param p_stream the stream
 * @param p_data   result
 * @param size     the quantity of bytes to read
 * 
 * @return bytes read on success, 0 on error
 */
int stream_read ( stream *p_stream, void *p_data, size_t size );

/** !
 * Read from a stream without advancing the cursor
 * 
 * @param p_stream the stream
 * @param p_data   result
 * @param size     the quantity of bytes to read
 * 
 * @return bytes read on success, 0 on error
 */
int stream_peek ( stream *p_stream, void *p_data, size_t size );

/// write
/** !
 * Write to a stream
 * 
 * @param p_stream the stream
 * @param p_data   result
 * @param size     the quantity of bytes to write
 * 
 * @return bytes written on success, 0 on error
 */
int stream_write ( stream *p_stream, void *p_data, size_t size );

/// flush
/** !
 * Flush a stream
 * 
 * @param p_stream the stream
 * 
 * @return 1 on success, 0 on error
 */
int stream_flush ( stream *p_stream );

/// cursor
/** !
 * Set the position of the cursor
 * 
 * @param p_stream the stream
 * @param offset   the position
 * @param whence   one of < STREAM_SEEK_SET | STREAM_SEEK_CURSOR | STREAM_SEEK_END >
 * 
 * @return the position of the cursor on success, -1 on error
 */
int stream_seek ( stream *p_stream, long offset, enum stream_seek_e whence );

/** !
 * Get the position of the cursor
 * 
 * @param p_stream the stream
 * 
 * @return the position of the cursor on success, -1 on error
 */
int stream_tell ( stream *p_stream );

/// eof
/** !
 * Is a stream at the end?
 * 
 * @param p_stream the stream
 * 
 * @return true IF end of file ELSE false
 */
bool stream_eof ( stream *p_stream );

/// copy
/** !
 * Copy a fixed size of data 
 * 
 * @param p_destination the destination
 * @param p_source      the source
 * @param size          the quantity of bytes to copy
 * 
 * @return bytes copied on success, 0 on error
 */
int stream_copy ( stream *p_destination, stream *p_source, size_t size );

/** !
 * Pipe data from a stream, to a stream
 * 
 * @param p_destination the destination
 * @param p_source      the source
 * 
 * @return bytes piped on success, 0 on error
 */
int stream_pipe ( stream *p_destination, stream *p_source );

/// size
/** !
 * Get the size of a stream
 * 
 * @param p_stream the stream
 * 
 * @return the size of the stream on success, 0 on error
 */
int stream_size ( stream *p_stream );

/// destructors
/** !
 * Destroy a stream
 * 
 * @param pp_stream pointer to stream pointer
 * 
 * @return 1 on success, 0 on error
 */
int stream_destroy ( stream **pp_stream );
