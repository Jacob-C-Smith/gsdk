# [gsdk](../../../README.md) > [core](../core.md) > hash

## Abstractions for I/O

 > 1 [Example](#example)
 >
 > 2 [Tester](#tester)
 >
 > 3 [Definitions](#definitions)
 >
 >> 3.1 [Enumeration definitions](#enumeration-definitions)
 >> 
 >> 3.2 [Type definitions](#type-definitions)
 >> 
 >> 3.3 [Function declarations](#function-declarations)

 ## Example
 To run the example program, execute this command
 ```
$ ./build/examples/stream_example
 ```
 ## Tester
 To run the tester program, execute this command
 ```
$ ./build/tests/stream_test
 ```
 
 ## Definitions
 ### Enumeration definitions
 ```c
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
 ```

### Type definitions
 ```c
// type definitions
typedef struct stream_s stream;
typedef int (fn_stream_read)  ( stream *p_stream, void *p_data, size_t size );
typedef int (fn_stream_write) ( stream *p_stream, void *p_data, size_t size );
typedef int (fn_stream_size)  ( stream *p_stream );
typedef int (fn_stream_flush) ( stream *p_stream );
typedef int (fn_stream_seek)  ( stream *p_stream, long offset, enum stream_seek_e whence );
typedef int (fn_stream_close) ( stream *p_stream );
 ```

 ### Function declarations
 ```c
// function declarations
/// constructors
int stream_from_path
( 
    stream     **pp_stream, 
    const char  *p_path
);

int stream_from_file 
( 
    stream **pp_stream, 
    FILE    *p_f
);

int stream_from_buffer 
( 
    stream **pp_stream, 
    void    *p_buffer, 
    size_t   size
);

int stream_from_dynamic_buffer ( stream **pp_stream );

/// read
int stream_read ( stream *p_stream, void *p_data, size_t size );
int stream_peek ( stream *p_stream, void *p_data, size_t size );

/// write
int stream_write ( stream *p_stream, void *p_data, size_t size );

/// flush
int stream_flush ( stream *p_stream );

/// cursor
int stream_seek ( stream *p_stream, long offset, enum stream_seek_e whence );
int stream_tell ( stream *p_stream );

/// eof
bool stream_eof ( stream *p_stream );

/// copy
int stream_copy ( stream *p_destination, stream *p_source, size_t size );
int stream_pipe ( stream *p_destination, stream *p_source );

/// size
int stream_size ( stream *p_stream );

/// destructors
int stream_destroy ( stream **pp_stream );
 ```
