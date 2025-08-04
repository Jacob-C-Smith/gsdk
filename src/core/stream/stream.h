/** ! 
 * Stream interface
 * 
 * @file stream.h
 * 
 * @author Jacob Smith
 */

// header guard
#pragma once

// standard library
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <time.h>
#include <dirent.h>

// core
#include <core/log.h>
#include <core/hash.h>
#include <core/interfaces.h>
#include <core/pack.h>
#include <core/socket.h>

// enumeration definitions
enum stream_type 
{
    STREAM_TYPE_INVALID = 0,
    STREAM_TYPE_FILE,
    STREAM_TYPE_MEMORY,
    STREAM_TYPE_DIRECTORY,
    STREAM_TYPE_TCP_SOCKET,
    STREAM_TYPE_UDP_SOCKET,
    STREAM_TYPE_PIPE,
    STREAM_TYPE_CHAR,
    STREAM_TYPE_BLOCK
};

// structure declarations
struct stream_s;
struct stream_memory_s;
struct stream_udp_socket_s;
struct stream_pipe_s;
struct stream_char_s;
struct stream_block_s;

// type definitions
/// structures
typedef struct stream_s            stream;
typedef struct stream_memory_s     stream_memory;
typedef struct stream_udp_socket_s stream_udp_socket;
typedef struct stream_pipe_s       stream_pipe;
typedef struct stream_char_s       stream_char;
typedef struct stream_block_s      stream_block;

/// functions
typedef int  (fn_stream_read)  ( stream *p_stream, void *buffer, size_t size );
typedef int  (fn_stream_write) ( struct stream_s *stream, const void *buffer, size_t size );
typedef int  (fn_stream_seek)  ( struct stream_s *stream, long        offset, int    whence );
typedef long (fn_stream_tell)  ( struct stream_s *stream );
typedef long (fn_stream_fill)  ( struct stream_s *stream );
typedef long (fn_stream_flush) ( struct stream_s *stream, int fd );
typedef int  (fn_stream_eof)   ( struct stream_s *stream );
typedef int  (fn_stream_close) ( struct stream_s *stream );

// structure definitions
struct stream_s
{
    enum stream_type _type;

    void *p_parameter;

    union
    {
        FILE              *file;
        void              *memory;
        DIR               *directory;
        socket_tcp         _tcp_socket;
        // socket_udp        *udp_socket;
        // int[2]       pipe;
        // stream_char       *char_stream;
        // stream_block      *block;
    } data;

    struct
    {
        int count;
        char *ptr;
        char *base;
        int flag;
        int fd;
        size_t size;
        size_t position;
        int eof;
    } state;

    struct 
    {
        fn_stream_read  *pfn_read;
        fn_stream_write *pfn_write;
        fn_stream_seek  *pfn_seek;
        fn_stream_tell  *pfn_tell;
        fn_stream_fill  *pfn_fill;
        fn_stream_flush *pfn_flush;
        fn_stream_eof   *pfn_eof;
        fn_stream_close *pfn_close;
    } functions;
};

// function declarations
/// constructors
/** !
 * Construct a stream from a path
 * 
 * @param p_stream result
 * @param p_path   path 
 * 
 * @return 1 on success, 0 on error
 */
int stream_from_path ( stream **p_stream, const char *p_path );

/** !
 * Construct a stream from a file descriptor
 * 
 * @param p_stream result
 * @param fd       file descriptor
 * 
 * @return 1 on success, 0 on error
 */
int stream_from_fd ( stream **p_stream, int fd );

/** !
 * Construct a stream from a socket
 * 
 * @param p_stream result
 * @param socket   the socket
 * 
 * @return 1 on success, 0 on error
 */
int stream_from_socket_tcp ( stream **p_stream, socket_tcp socket );

/** !
 * Read from a stream
 * 
 * @param p_stream the stream
 * @param buffer   
 * @param size     size of the buffer
 * 
 * @return number of bytes read, 0 on error
 */
int stream_read ( stream *p_stream, void *buffer, size_t size );

/** !
 * Write to a stream
 * 
 * @param p_stream the stream
 * @param buffer   
 * @param size     size of the buffer
 * 
 * @return number of bytes written, 0 on error
 */
int stream_write ( stream *p_stream, const void *buffer, size_t size );

/// destructors
/** !
 * Destroy a stream
 * 
 * @param pp_stream pointer to stream pointer
 * 
 * @return 1 on success, 0 on error
 */
int stream_destroy ( stream **pp_stream );