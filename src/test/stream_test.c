/** !
 * stream tester
 * 
 * @file src/test/stream_test.c
 *
 * @author Jacob Smith
*/

// standard library
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

// gsdk
/// core
#include <core/log.h>
#include <core/sync.h>
#include <core/test.h>
#include <core/stream.h>

// function declarations
/// scenario constructors
fn_scenario_constructor construct_buffer_stream;
fn_scenario_constructor construct_file_stream;
fn_scenario_constructor construct_file_ptr_stream;
fn_scenario_constructor construct_dynamic_stream;

/// test cases
fn_test_case test_stream_write_read;
fn_test_case test_stream_seek_tell;
fn_test_case test_stream_peek;
fn_test_case test_stream_eof;
fn_test_case test_stream_size;
fn_test_case test_stream_copy;
fn_test_case test_stream_pipe;
fn_test_case test_stream_large_data;
fn_test_case test_stream_boundary_checks;
fn_test_case test_stream_zero_byte;
fn_test_case test_stream_random_access;
fn_test_case test_stream_null_args;
fn_test_case test_stream_interleaved;
fn_test_case test_stream_overlapping;
fn_test_case test_stream_realloc_stress;
fn_test_case test_stream_underflow_check;

/// allocators
fn_allocator destruct_stream;

// data
static char _buffer[4096] = { 0 };
#define TEST_FILE_PATH "test_stream.bin"

// test
/// cases
test_case _stream_test_cases[] = 
{
    TEST_CASE("write/read"     , test_stream_write_read     , NULL, TEST_RESULT_ONE),
    TEST_CASE("seek/tell"      , test_stream_seek_tell      , NULL, TEST_RESULT_ONE),
    TEST_CASE("peek"           , test_stream_peek           , NULL, TEST_RESULT_ONE),
    TEST_CASE("eof"            , test_stream_eof            , NULL, TEST_RESULT_ONE),
    TEST_CASE("size"           , test_stream_size           , NULL, TEST_RESULT_ONE),
    TEST_CASE("copy"           , test_stream_copy           , NULL, TEST_RESULT_ONE),
    TEST_CASE("pipe"           , test_stream_pipe           , NULL, TEST_RESULT_ONE),
    TEST_CASE("large data"     , test_stream_large_data     , NULL, TEST_RESULT_ONE),
    TEST_CASE("boundary checks", test_stream_boundary_checks, NULL, TEST_RESULT_ONE),
    TEST_CASE("zero-byte op"   , test_stream_zero_byte      , NULL, TEST_RESULT_ONE),
    TEST_CASE("random access"  , test_stream_random_access  , NULL, TEST_RESULT_ONE),
    TEST_CASE("null arguments" , test_stream_null_args      , NULL, TEST_RESULT_ONE),
    TEST_CASE("interleaved"    , test_stream_interleaved    , NULL, TEST_RESULT_ONE),
    TEST_CASE("overlapping"    , test_stream_overlapping    , NULL, TEST_RESULT_ONE),
    TEST_CASE("realloc stress" , test_stream_realloc_stress , NULL, TEST_RESULT_ONE),
    TEST_CASE("underflow check", test_stream_underflow_check, NULL, TEST_RESULT_ONE),
};

/// scenarios
test_scenario _scenarios[] = 
{
    TEST_SCENARIO("buffer stream"   , _buffer       , _stream_test_cases, construct_buffer_stream  , destruct_stream),
    TEST_SCENARIO("file path stream", TEST_FILE_PATH, _stream_test_cases, construct_file_stream    , destruct_stream),
    TEST_SCENARIO("file ptr stream" , TEST_FILE_PATH, _stream_test_cases, construct_file_ptr_stream, destruct_stream),
    TEST_SCENARIO("dynamic stream"  , NULL          , _stream_test_cases, construct_dynamic_stream , destruct_stream),
};

/// suites
test_suite _suite = TEST_SUITE("stream", _scenarios);

// entry point
int main ( int argc, const char *argv[] ) 
{

    // unused
    (void) argc;
    (void) argv;
     
    // run the tests
    test_suite_test(&_suite); 
    
    // done
    return (_suite.counters.total.fails == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}

int construct_buffer_stream ( void **pp_result ) 
{ 

    // clear the buffer
    memset(_buffer, 0, sizeof(_buffer));

    // construct a buffer stream
    return stream_from_buffer((stream **)pp_result, _buffer, sizeof(_buffer));
}

int construct_file_stream ( void **pp_result ) 
{ 

    // construct a file path stream
    return stream_from_path((stream **)pp_result, TEST_FILE_PATH);
}

int construct_file_ptr_stream ( void **pp_result ) 
{ 

    // initialized data
    FILE *p_f = fopen(TEST_FILE_PATH, "wb+");

    // error check
    if ( NULL == p_f ) return 0;

    // construct a file pointer stream
    return stream_from_file((stream **)pp_result, p_f);
}

int construct_dynamic_stream ( void **pp_result )
{

    // construct a dynamic buffer stream
    return stream_from_dynamic_buffer((stream **)pp_result);
}

void *test_stream_write_read ( test_case *p_test_case, void *p_subject ) 
{ 

    // unused
    (void) p_test_case;

    // initialized data
    stream     *p_stream        = (stream *)p_subject;
    char        read_buffer[64] = { 0 };
    const char *data            = "Hello, GSDK!";
    size_t      len             = strlen(data);

    // write the data
    if ( (int)len != stream_write(p_stream, (void *)data, len) ) return NULL;

    // seek start
    if ( -1 == stream_seek(p_stream, 0, STREAM_SEEK_SET) ) return NULL;
    
    // read
    if ( (int)len != stream_read(p_stream, read_buffer, len) ) return NULL;

    // verify
    if ( strncmp(data, read_buffer, len) )  return NULL;

    // success
    return (void *)1;
}

void *test_stream_seek_tell ( test_case *p_test_case, void *p_subject ) 
{ 

    // unused
    (void) p_test_case;

    // initialized data
    stream     *p_stream = (stream *)p_subject;
    const char *data     = "0123456789";
    size_t      len      = strlen(data);
    char        c        = -1;

    // write
    stream_write(p_stream, (void *)data, len);
    
    // tell
    if ( (int)len != stream_tell(p_stream) ) return NULL;
    
    // seek
    if ( -1 == stream_seek(p_stream, 5, STREAM_SEEK_SET)) return NULL;

    // tell
    if ( 5 != stream_tell(p_stream) ) return NULL;
    
    // read
    if ( 1 != stream_read(p_stream, &c, 1) ) return NULL;

    // expect '5'
    if ( '5' != c ) return NULL;
    
    // seek
    if ( -1 == stream_seek(p_stream, -2, STREAM_SEEK_CURSOR) ) return NULL;

    // tell
    if ( 4 != stream_tell(p_stream) ) return NULL;
    
    // read 
    if ( 1 != stream_read(p_stream, &c, 1) ) return NULL;

    // expect '4'
    if ( '4' != c ) return NULL;

    // success
    return (void *)1;
}

void *test_stream_peek ( test_case *p_test_case, void *p_subject ) 
{ 

    // unused
    (void) p_test_case;

    // initialized data
    stream     *p_stream = (stream *)p_subject;
    const char *data     = "PEEK";
    char        c1       = -1, 
                c2       = -1;

    // write
    stream_write(p_stream, (void *)data, 4);

    // seek
    stream_seek(p_stream, 0, STREAM_SEEK_SET);
    
    // peek
    if ( 1 != stream_peek(p_stream, &c1, 1) ) return NULL;

    // expect 'P'
    if ( 'P' != c1 ) return NULL;

    // tell 
    if ( 0 != stream_tell(p_stream) ) return NULL;
    
    // read
    if ( 1 != stream_read(p_stream, &c2, 1) ) return NULL;

    // expect 'P'
    if ( 'P' != c2 ) return NULL;

    // tell
    if ( 1 != stream_tell(p_stream) ) return NULL;

    // success
    return (void *)1;
}

void *test_stream_eof ( test_case *p_test_case, void *p_subject ) 
{ 

    // unused
    (void) p_test_case;

    // initialized data
    stream *p_stream = (stream *)p_subject;
    
    // write
    stream_write(p_stream, "A", 1);
    
    // update the size
    stream_size(p_stream);

    // seek start
    stream_seek(p_stream, 0, STREAM_SEEK_SET);

    // eof? 
    if ( stream_eof(p_stream) ) return NULL;
    
    // seek end
    stream_seek(p_stream, 0, STREAM_SEEK_END);

    // eof?
    if ( !stream_eof(p_stream) ) return NULL;

    // success
    return (void *)1;
}

void *test_stream_size ( test_case *p_test_case, void *p_subject ) 
{ 

    // unused
    (void) p_test_case;

    // initialized data
    stream *p_stream = (stream *)p_subject;
    
    // buffer
    if ( STREAM_TYPE_BUFFER == p_stream->type ) 
    {

        // test the size
        if ( 4096 != stream_size(p_stream) ) return NULL;
    } 
    
    // dynamic buffer
    else if ( STREAM_TYPE_DYNAMIC_BUFFER == p_stream->type ) 
    {

        // test the size
        if ( 1024 > stream_size(p_stream) ) return NULL;
    } 

    // file
    else 
    {

        // write some data
        stream_write(p_stream, "123", 3);

        // test the size
        if ( 3 != stream_size(p_stream) ) return NULL;
    }

    // success
    return (void *)1;
}

void *test_stream_copy ( test_case *p_test_case, void *p_subject ) 
{ 

    // unused
    (void) p_test_case;

    // initialized data
    stream     *p_src           = (stream *)p_subject;
    stream     *p_dest          = NULL;
    const char *data            = "COPY_TEST";
    char        dest_buffer[64] = { 0 };
    char        read_back[64]   = { 0 };
    int         copied          = -1;

    // construct a stream
    stream_from_buffer(&p_dest, dest_buffer, sizeof(dest_buffer));

    // write
    stream_write(p_src, (void *)data, strlen(data));

    // seek
    stream_seek(p_src, 0, STREAM_SEEK_SET);

    // copy
    copied = stream_copy(p_dest, p_src, strlen(data));

    // verify
    if ( (int)strlen(data) != copied ) 
    {

        // release the extra stream
        stream_destroy(&p_dest);

        // error
        return NULL;
    }

    // seek
    stream_seek(p_dest, 0, STREAM_SEEK_SET);

    // read
    stream_read(p_dest, read_back, strlen(data));

    // verify
    if ( strncmp(data, read_back, strlen(data)) )
    {

        // release the extra stream
        stream_destroy(&p_dest);

        // error
        return NULL;
    }

    // release the extra stream
    stream_destroy(&p_dest);

    // success
    return (void *)1;
}

void *test_stream_pipe ( test_case *p_test_case, void *p_subject ) 
{ 

    // unused
    (void) p_test_case;

    // initialized data
    stream     *p_src            = (stream *)p_subject;
    stream     *p_dest           = NULL;
    char        dest_buffer[512] = { 0 };
    char        read_back[512]   = { 0 };
    const char *data             = "PIPE_TEST";

    // construct a stream
    stream_from_buffer(&p_dest, dest_buffer, sizeof(dest_buffer));
    
    // write
    stream_write(p_src, (void *)data, strlen(data));

    // size
    stream_size(p_src);

    // seek
    stream_seek(p_src, 0, STREAM_SEEK_SET);

    if ( sizeof(dest_buffer) < p_src->size )
    {

        // release the extra stream
        stream_destroy(&p_dest);

        // success
        return (void *)1; 
    }

    // pipe
    stream_pipe(p_dest, p_src);
    
    // seek
    stream_seek(p_dest, 0, STREAM_SEEK_SET);

    // read
    stream_read(p_dest, read_back, strlen(data));

    if ( strncmp(data, read_back, strlen(data)) ) 
    {

        // release the extra stream
        stream_destroy(&p_dest);

        // error 
        return NULL;
    }

    // release the extra stream
    stream_destroy(&p_dest);

    // success
    return (void *)1;
}

void *test_stream_large_data ( test_case *p_test_case, void *p_subject ) 
{

    // unused
    (void) p_test_case;

    // initialized data
    stream *p_stream   = (stream *)p_subject;
    size_t  size       = 8192; 
    char   *write_data = default_allocator(NULL, size);
    char   *read_data  = default_allocator(NULL, size);
    size_t  to_write   = -1;

    // populate the buffer
    for (size_t i = 0; i < size; i++) 
        write_data[i] = (char)(i & 0xFF);

    // edge case
    to_write = 
    (
        p_stream->size <  size && 
        p_stream->type != STREAM_TYPE_DYNAMIC_BUFFER
    ) ? p_stream->size : size;

    // write
    if ( (int)to_write != stream_write(p_stream, write_data, to_write) ) 
    {

        // release the buffers
        write_data = default_allocator(write_data, 0); 
        read_data = default_allocator(write_data, 0);

        // error
        return NULL;
    }

    // seek
    stream_seek(p_stream, 0, STREAM_SEEK_SET);

    // read
    if ( (int)to_write != stream_read(p_stream, read_data, to_write) ) 
    {

        // release the buffers
        write_data = default_allocator(write_data, 0); 
        read_data = default_allocator(write_data, 0);

        // error
        return NULL;
    }

    // verify
    if ( memcmp(write_data, read_data, to_write) )
    {

        // release the buffers
        write_data = default_allocator(write_data, 0); 
        read_data = default_allocator(write_data, 0);

        // error
        return NULL;
    }

    // release the buffers
    write_data = default_allocator(write_data, 0); 
    read_data = default_allocator(read_data, 0);

    // success
    return (void *)1;
}

void *test_stream_zero_byte ( test_case *p_test_case, void *p_subject ) 
{

    // unused
    (void) p_test_case;

    // initialized data
    stream *p_stream = (stream *)p_subject;
    char    buf[1]   = { 0 };

    // write
    if ( stream_write(p_stream, NULL, 0) ) return NULL;

    // read
    if ( stream_read(p_stream, buf, 0) ) return NULL;

    // peek
    if ( stream_peek(p_stream, buf, 0) ) return NULL;
    
    // success
    return (void *)1;
}

void *test_stream_random_access ( test_case *p_test_case, void *p_subject ) 
{

    // unused
    (void) p_test_case;

    // initialized data
    stream     *p_stream = (stream *)p_subject;
    const char *data     = "ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    size_t      data_len = strlen(data);
    
    // write
    stream_write(p_stream, (void *)data, data_len);

    // size
    stream_size(p_stream);

    // seed the random number generator 
    srand(12345);

    // random access
    for (int i = 0; i < 100; i++) 
    {

        // initialized data
        int  pos = rand() % (int)data_len;
        char c   = -1;

        // seek
        if ( -1 == stream_seek(p_stream, pos, STREAM_SEEK_SET) ) return NULL;

        // read
        if ( 1 != stream_read(p_stream, &c, 1) ) return NULL;

        // verify
        if ( data[pos] != c ) return NULL;
    }

    // success
    return (void *)1;
}

void *test_stream_null_args ( test_case *p_test_case, void *p_subject ) 
{

    // unused
    (void) p_test_case;
    (void) p_subject;

    // read
    if ( stream_read(NULL, NULL, 10)) return NULL;

    // write
    if ( stream_write(NULL, NULL, 10)) return NULL;

    // seek
    if ( stream_seek(NULL, 0, STREAM_SEEK_SET)) return NULL;

    // tell
    if ( stream_tell(NULL) != -1) return NULL;

    // eof
    if ( stream_eof(NULL) != true) return NULL;

    // size
    if ( stream_size(NULL)) return NULL;

    // destroy
    if ( stream_destroy(NULL)) return NULL;

    // success
    return (void *)1;
}

void *test_stream_boundary_checks ( test_case *p_test_case, void *p_subject ) 
{

    // unused
    (void) p_test_case;

    // initialized data
    stream *p_stream = (stream *)p_subject;
    char    buf[10]  = { 0 };
    int     read     = -1;

    // edge case
    if ( STREAM_TYPE_BUFFER == p_stream->type )
        if ( stream_seek(p_stream, p_stream->size + 1, STREAM_SEEK_SET) ) 
            return NULL;

    // write
    stream_write(p_stream, "0123456789", 10);

    // size
    stream_size(p_stream);

    // seek
    stream_seek(p_stream, p_stream->size - 5, STREAM_SEEK_SET);

    // read
    read = stream_read(p_stream, buf, 10);

    // error check
    if (read > 5) return NULL;

    // seek
    if ( stream_seek(p_stream, -1, STREAM_SEEK_SET) ) return NULL;

    // success
    return (void *)1;
}

void *test_stream_interleaved ( test_case *p_test_case, void *p_subject ) 
{

    // unused
    (void) p_test_case;

    // initialized data
    stream     *p_stream = (stream *)p_subject;
    const char *data1    = "AAAAA";
    const char *data2    = "BBBBB";
    char        buf[6]   = { 0 };

    // write
    stream_write(p_stream, (void *)data1, 5);

    // seek
    stream_seek(p_stream, 0, STREAM_SEEK_SET);

    // peek
    stream_peek(p_stream, buf, 5);

    // verify
    if ( strncmp(buf, data1, 5) ) return NULL;

    // seek
    stream_seek(p_stream, 2, STREAM_SEEK_SET);

    // write
    stream_write(p_stream, (void *)data2, 5);
    
    // size
    stream_size(p_stream);
    
    // seek
    stream_seek(p_stream, 0, STREAM_SEEK_SET);

    // read
    stream_read(p_stream, buf, 2);

    // verify
    if ( strncmp(buf, "AA", 2) ) return NULL;

    // read
    stream_read(p_stream, buf, 5);

    // verify
    if ( strncmp(buf, "BBBBB", 5) ) return NULL;

    // success
    return (void *)1;
}

void *test_stream_overlapping ( test_case *p_test_case, void *p_subject ) 
{

    // unused
    (void) p_test_case;

    // initialized data
    stream *p_stream1 = (stream *)p_subject;
    stream *p_stream2 = NULL;
    char    buf[3]    = { 0 };
    
    // edge case
    if ( STREAM_TYPE_BUFFER != p_stream1->type  ) return (void *)1;

    // construct a stream
    stream_from_buffer(&p_stream2, p_stream1->p_data, p_stream1->size);

    // write
    stream_write(p_stream1, "12345", 5);

    // seek
    stream_seek(p_stream2, 2, STREAM_SEEK_SET);
    
    // read
    stream_read(p_stream2, buf, 3);

    // verify
    if ( strncmp(buf, "345", 3) )
    {

        // release the extra stream
        stream_destroy(&p_stream2);

        // error
        return NULL;
    }

    // release the extra stream
    stream_destroy(&p_stream2);

    // success
    return (void *)1;
}

void *test_stream_realloc_stress ( test_case *p_test_case, void *p_subject )
{

    // unused
    (void) p_test_case;

    // initialized data
    stream *p_stream   = (stream *)p_subject;
    char    chunk[128] = { 0 };
    
    // edge case
    if ( STREAM_TYPE_DYNAMIC_BUFFER != p_stream->type  ) return (void *)1;

    // write chunks
    for (int j = 0; j < 16; j++) 
    {

        // populate the chunk 
        for (int i = 0; i < 128; i++) 
            chunk[i] = (char)((j * 128 + i) % 256);
        
        // write 
        if ( 128 != stream_write(p_stream, chunk, 128) ) return NULL;
    }

    // size
    if ( 2048 != stream_size(p_stream) ) return NULL;

    // seek
    stream_seek(p_stream, 0, STREAM_SEEK_SET);

    // read chunks 
    for (int i = 0; i < 2048; i++)
    {

        // initialized data
        char c = -1;

        // read 
        if ( 1 != stream_read(p_stream, &c, 1) ) return NULL;

        // expect
        if ( (char)(i % 256) != c ) return NULL;
    }

    // success
    return (void *)1;
}

void *test_stream_underflow_check ( test_case *p_test_case, void *p_subject )
{

    // unused
    (void) p_test_case;

    // initialized data
    stream *p_stream = (stream *)p_subject;

    // edge case
    if 
    ( 
        p_stream->type != STREAM_TYPE_BUFFER         && 
        p_stream->type != STREAM_TYPE_DYNAMIC_BUFFER
    ) 

        // success
        return (void *)1;

    // write
    stream_write(p_stream, "ABC", 3);

    // seek +1
    stream_seek(p_stream, 1, STREAM_SEEK_SET);
    
    // seek -1
    if (stream_seek(p_stream, -2, STREAM_SEEK_CURSOR) != 0) return NULL;
    
    // seek -5
    if (stream_seek(p_stream, -5, STREAM_SEEK_SET) != 0) return NULL;

    // success
    return (void *)1;
}

void *destruct_stream ( void *p_pointer, unsigned long long size )
{

    // unused
    (void) size;

    // initialized data
    stream *p_stream = (stream *)p_pointer;

    // release the stream
    if ( p_stream ) 
        stream_destroy(&p_stream);

    // success
    return NULL;
}
