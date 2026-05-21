/** !
 * base64 implementation
 * 
 * @file src/reflection/base64/base64.c 
 * 
 * @author Jacob Smith
 */

// header
#include <reflection/base64.h>

// structure definitions
struct base64_stream_s
{
    void  *p_buffer;
    size_t size;
    size_t cursor;
    size_t read_cursor;
    unsigned char remainder[4];
    size_t remainder_len;
};

// type definitions
typedef struct base64_stream_s base64_stream;

// forward declarations
/// read
fn_stream_read base64_stream_read;

/// write
fn_stream_write base64_stream_encode_write;
fn_stream_write base64_stream_decode_write;

/// size
fn_stream_size base64_stream_size;

/// flush
fn_stream_flush base64_stream_encode_flush;
fn_stream_flush base64_stream_decode_flush;

/// seek
fn_stream_seek base64_stream_seek;

/// close
fn_stream_close base64_stream_close;

// data
static const char base_64_encoding_characters[] = 
{
    'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 
    'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 
    'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 
    'Y', 'Z', 'a', 'b', 'c', 'd', 'e', 'f', 
    'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 
    'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 
    'w', 'x', 'y', 'z', '0', '1', '2', '3', 
    '4', '5', '6', '7', '8', '9', '+', '/'  
};
static const char base_64_decoding_characters[] = 
{
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x3E, 0x00, 0x00, 0x00, 0x3F, 
    0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x3A, 0x3B, 
    0x3C, 0x3D, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 
    0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 
    0x0F, 0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 
    0x17, 0x18, 0x19, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x1A, 0x1B, 0x1C, 0x1D, 0x1E, 0x1F, 0x20, 
    0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28, 
    0x29, 0x2A, 0x2B, 0x2C, 0x2D, 0x2E, 0x2F, 0x30, 
    0x31, 0x32, 0x33, 0x00, 0x00, 0x00, 0x00, 0x00  
};

int base64_encode_stream_create ( stream **pp_stream )
{

    // argument check
    if ( NULL == pp_stream ) goto no_stream;

    // initialized data
    stream        *p_stream        = NULL;
    base64_stream *p_base64_stream = NULL;
    void          *p_buffer        = NULL;
    
    // allocate memory for a stream
    p_stream = default_allocator(NULL, sizeof(stream));
    if ( NULL == p_stream ) goto no_mem;

    // allocate memory for a base64 stream 
    p_base64_stream = default_allocator(NULL, sizeof(base64_stream));
    if ( NULL == p_base64_stream ) goto no_mem;

    // allocate memory for a buffer
    p_buffer = default_allocator(NULL, 1024);
    if ( NULL == p_buffer ) goto no_mem;

    // populate the base64 stream 
    *p_base64_stream = (base64_stream)
    {
        .p_buffer      = p_buffer,
        .size          = 1024,
        .cursor        = 0,
        .read_cursor   = 0,
        .remainder_len = 0,
    };

    // populate the stream structure
    *p_stream = (stream)
    {
        .p_data    = p_base64_stream,
        .type      = STREAM_TYPE_ENCODER,
        .size      = 0,
        .cursor    = 0,
        .pfn_read  = base64_stream_read,
        .pfn_write = base64_stream_encode_write,
        .pfn_size  = base64_stream_size,
        .pfn_flush = base64_stream_encode_flush,
        .pfn_seek  = base64_stream_seek,
        .pfn_close = base64_stream_close,
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
                    log_error("[base64] Null pointer provided for parameter \"pp_stream\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }

        // standard library errors
        {
            no_mem:
                #ifndef NDEBUG
                    log_error("[base64] Failed to allocate memory in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
    }
}

int base64_decode_stream_create ( stream **pp_stream )
{

    // argument check
    if ( NULL == pp_stream ) goto no_stream;

    // initialized data
    stream        *p_stream        = NULL;
    base64_stream *p_base64_stream = NULL;
    void          *p_buffer        = NULL;

    // allocate memory for a stream
    p_stream = default_allocator(NULL, sizeof(stream));
    if ( NULL == p_stream ) goto no_mem;

    // allocate memory for a base64 stream 
    p_base64_stream = default_allocator(NULL, sizeof(base64_stream));
    if ( NULL == p_base64_stream ) goto no_mem;

    // allocate memory for a buffer
    p_buffer = default_allocator(NULL, 1024);
    if ( NULL == p_buffer ) goto no_mem;

    // populate the base64 stream 
    *p_base64_stream = (base64_stream)
    {
        .p_buffer      = p_buffer,
        .size          = 1024,
        .cursor        = 0,
        .read_cursor   = 0,
        .remainder_len = 0,
    };

    // populate the stream structure
    *p_stream = (stream)
    {
        .p_data    = p_base64_stream,
        .type      = STREAM_TYPE_DECODER,
        .size      = 0,
        .cursor    = 0,
        .pfn_read  = base64_stream_read,
        .pfn_write = base64_stream_decode_write,
        .pfn_size  = base64_stream_size,
        .pfn_flush = base64_stream_decode_flush,
        .pfn_seek  = base64_stream_seek,
        .pfn_close = base64_stream_close,
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
                    log_error("[base64] Null pointer provided for parameter \"pp_stream\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }

        // standard library errors
        {
            no_mem:
                #ifndef NDEBUG
                    log_error("[base64] Failed to allocate memory in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
    }
}

int base64_encode ( char *const p_output, const void *const p_data, size_t len )
{

    // argument errors
    if ( NULL == p_output ) goto no_output;
    if ( NULL ==   p_data ) goto no_data;

    // initialized data
    size_t output_length = ( 4 * ( ( len + 2 ) / 3 ) );

    // iterate through len bytes of p_data
    for (size_t i = 0, j = 0; i < len; i += 3, j += 4)
    {

        // initialized data
        unsigned long part = (unsigned long)( ( (i + 0 < len) ? ( (unsigned char*)p_data)[i + 0] : 0 ) << 0x10 ) | 
                             (unsigned long)( ( (i + 1 < len) ? ( (unsigned char*)p_data)[i + 1] : 0 ) << 0x08 ) |
                             (unsigned long)( ( (i + 2 < len) ? ( (unsigned char*)p_data)[i + 2] : 0 ) << 0x00 ) ;

        // write this fragment to the output
        p_output[j + 0] =                 base_64_encoding_characters[(part >> (3 * 6)) & 0x3F],
        p_output[j + 1] =                 base_64_encoding_characters[(part >> (2 * 6)) & 0x3F],
        p_output[j + 2] = (i + 1 < len) ? base_64_encoding_characters[(part >> (1 * 6)) & 0x3F] : '=',
        p_output[j + 3] = (i + 2 < len) ? base_64_encoding_characters[(part >> (0 * 6)) & 0x3F] : '=';
    }

    // insert a null terminator at the end of the output
    p_output[output_length] = '\0';
    
    // success
    return 1;

    // error handling
    {

        // argument errors
        {
            no_data:
                #ifndef NDEBUG
                    log_error("[base64] Null pointer provided for parameter \"p_data\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;

            no_output:
                #ifndef NDEBUG
                    log_error("[base64] Null pointer provided for parameter \"p_output\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
    }
}

int base64_decode ( void *const p_output, const char *const p_data, size_t len )
{

    // argument errors
    if ( NULL == p_output ) goto no_output;
    if ( NULL ==   p_data ) goto no_data;

    // iterate through len bytes of p_data
    for ( size_t i = 0, j = 0; i < len; i+=4, j+=3 )
    {

        // initialized data
        unsigned long long part = (unsigned long long)( ( ( i + 0 < len ) && ( (unsigned char)p_data[i+0] < 128 ) ? base_64_decoding_characters[(unsigned char)p_data[i+0]] : 0 ) << 3 * 6 ) + 
                                  (unsigned long long)( ( ( i + 1 < len ) && ( (unsigned char)p_data[i+1] < 128 ) ? base_64_decoding_characters[(unsigned char)p_data[i+1]] : 0 ) << 2 * 6 ) +
                                  (unsigned long long)( ( ( i + 2 < len ) && ( (unsigned char)p_data[i+2] < 128 ) ? base_64_decoding_characters[(unsigned char)p_data[i+2]] : 0 ) << 1 * 6 ) +
                                  (unsigned long long)( ( ( i + 3 < len ) && ( (unsigned char)p_data[i+3] < 128 ) ? base_64_decoding_characters[(unsigned char)p_data[i+3]] : 0 ) << 0 * 6 );

        // write this fragment to the output
        if ( i + 1 < len ) ((unsigned char *const)p_output)[j+0] = (part >> 2 * 8) & 0xFF;
        if ( i + 2 < len && p_data[i+2] != '=' ) ((unsigned char *const)p_output)[j+1] = (part >> 1 * 8) & 0xFF;
        if ( i + 3 < len && p_data[i+3] != '=' ) ((unsigned char *const)p_output)[j+2] = (part >> 0 * 8) & 0xFF;
    }
    
    // success
    return 1;

    // error handling
    {

        // argument errors
        {
            no_data:
                #ifndef NDEBUG
                    log_error("[base64] Null pointer provided for parameter \"p_data\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;

            no_output:
                #ifndef NDEBUG
                    log_error("[base64] Null pointer provided for parameter \"p_output\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
    }
}

int base64_stream_read ( stream *p_stream, void *p_data, size_t size )
{

    // initialized data
    base64_stream *p_base64_stream = p_stream->p_data;
    size_t         available       = p_base64_stream->cursor - p_base64_stream->read_cursor;
    size_t         to_read         = ( available < size ) ? available : size;

    // error check
    if ( 0 == to_read ) return 0;

    // copy
    memcpy
    (
        p_data, 
        (char *) p_base64_stream->p_buffer + p_base64_stream->read_cursor, 
        to_read
    );

    // update read cursor
    p_base64_stream->read_cursor += to_read;

    // update stream cursor
    p_stream->cursor = p_base64_stream->read_cursor;

    // success
    return (int) to_read;
}

int base64_stream_encode_write ( stream *p_stream, void *p_data, size_t size )
{

    // initialized data
    base64_stream *p_base64_stream = p_stream->p_data;
    size_t         total_len       = p_base64_stream->remainder_len + size;
    size_t         blocks          = total_len / 3;
    size_t         to_encode       = blocks * 3;
    size_t         encoded_len     = blocks * 4;

    // edge case
    if ( 0 == blocks )
    {

        // copy
        memcpy
        (
            p_base64_stream->remainder + p_base64_stream->remainder_len, 
            p_data, 
            size
        );

        // update remainder length
        p_base64_stream->remainder_len += size;

        // success
        return (int) size;
    }

    // grow the buffer
    if ( p_base64_stream->cursor + encoded_len > p_base64_stream->size )
    {

        // initialized data
        size_t new_size = 
        ( p_base64_stream->size * 2 > p_base64_stream->cursor + encoded_len ) 
            ? ( p_base64_stream->size * 2 ) 
            : ( p_base64_stream->cursor + encoded_len );

        // resize the buffer
        p_base64_stream->p_buffer = default_allocator(p_base64_stream->p_buffer, new_size);
        if ( NULL == p_base64_stream->p_buffer ) return 0;

        // update the size
        p_base64_stream->size = new_size;
    }

    // encode
    {

        // initialized data
        unsigned char *p_src = default_allocator(NULL, to_encode);

        // error check
        if ( NULL == p_src ) return 0;

        // copy remainder
        memcpy(p_src, p_base64_stream->remainder, p_base64_stream->remainder_len);

        // copy data
        memcpy(p_src + p_base64_stream->remainder_len, p_data, to_encode - p_base64_stream->remainder_len);

        // encode
        base64_encode
        (
            (char *) p_base64_stream->p_buffer + p_base64_stream->cursor, 
            p_src, 
            to_encode
        );

        // release the source buffer
        p_src = default_allocator(p_src, 0);
    }

    // update the cursors
    p_base64_stream->cursor += encoded_len,
    p_stream->size = p_base64_stream->cursor;

    // update remainder
    {

        // initialized data
        size_t consumed = to_encode - p_base64_stream->remainder_len;

        // store the remainder
        p_base64_stream->remainder_len = size - consumed;

        // copy
        memcpy
        (
            p_base64_stream->remainder, 
            (char *) p_data + consumed, 
            p_base64_stream->remainder_len
        );
    }

    // success
    return (int) size;
}

int base64_stream_decode_write ( stream *p_stream, void *p_data, size_t size )
{

    // initialized data
    base64_stream *p_base64_stream    = p_stream->p_data;
    size_t         total_len          = p_base64_stream->remainder_len + size;
    size_t         blocks             = total_len / 4;
    size_t         to_decode          = blocks * 4;
    size_t         decoded_len        = blocks * 3; 
    size_t         actual_decoded_len = 0;

    // edge case
    if ( 0 == blocks )
    {

        // copy
        memcpy
        (
            p_base64_stream->remainder + p_base64_stream->remainder_len, 
            p_data, 
            size
        );

        // update the remainder 
        p_base64_stream->remainder_len += size;

        // success
        return (int) size;
    }

    // resize the buffer
    if ( p_base64_stream->cursor + decoded_len > p_base64_stream->size )
    {

        // initialized data
        size_t new_size = ( p_base64_stream->size * 2 > p_base64_stream->cursor + decoded_len ) 
            ? ( p_base64_stream->size * 2 ) 
            : ( p_base64_stream->cursor + decoded_len );

        // resize the buffer
        p_base64_stream->p_buffer = default_allocator(p_base64_stream->p_buffer, new_size);
        if ( NULL == p_base64_stream->p_buffer ) return 0;

        // store the new size
        p_base64_stream->size = new_size;
    }

    // decode
    {

        // initialized data
        char *p_src = default_allocator(NULL, to_decode + 1);

        // error check
        if ( NULL == p_src ) return 0;

        // copy remainder
        memcpy
        (
            p_src, 
            p_base64_stream->remainder, 
            p_base64_stream->remainder_len
        );

        // copy data
        memcpy
        (
            p_src + p_base64_stream->remainder_len, 
            p_data, 
            to_decode - p_base64_stream->remainder_len
        );

        // store a null terminator 
        p_src[to_decode] = '\0';

        // store the actual decoded length
        actual_decoded_len = decoded_len;

        // pad
        if ( p_src[to_decode - 1] == '=' ) actual_decoded_len--;
        if ( p_src[to_decode - 2] == '=' ) actual_decoded_len--;

        // decode
        base64_decode((unsigned char *) p_base64_stream->p_buffer + p_base64_stream->cursor, p_src, to_decode);

        // release the source buffer
        p_src = default_allocator(p_src, 0);

        // update cursors
        p_base64_stream->cursor += actual_decoded_len;
    }

    // update the size of the stream
    p_stream->size = p_base64_stream->cursor;

    // update remainder
    {

        // initialized data
        size_t consumed = to_decode - p_base64_stream->remainder_len;

        // store the remainder
        p_base64_stream->remainder_len = size - consumed;

        // copy
        memcpy
        (
            p_base64_stream->remainder, 
            (char *) p_data + consumed, 
            p_base64_stream->remainder_len
        );
    }

    // success
    return (int) size;
}

int base64_stream_size ( stream *p_stream )
{

    // initialized data
    base64_stream *p_base64_stream = p_stream->p_data;

    // success
    return (int) (p_base64_stream->cursor - p_base64_stream->read_cursor);
}

int base64_stream_encode_flush ( stream *p_stream )
{

    // initialized data
    base64_stream *p_base64_stream = p_stream->p_data;

    // error check
    if ( 0 == p_base64_stream->remainder_len ) return 1;

    // resize the buffer
    if ( p_base64_stream->cursor + 4 > p_base64_stream->size )
    {

        // resize the buffer
        p_base64_stream->p_buffer = default_allocator(p_base64_stream->p_buffer, p_base64_stream->size + 4);
        if ( NULL == p_base64_stream->p_buffer ) return 0;

        // update the size
        p_base64_stream->size += 4;
    }

    // encode the remainder
    base64_encode
    (
        (char *) p_base64_stream->p_buffer + p_base64_stream->cursor, 
        p_base64_stream->remainder, 
        p_base64_stream->remainder_len
    );

    // update cursors
    p_base64_stream->cursor += 4,
    p_stream->size = p_base64_stream->cursor,
    p_base64_stream->remainder_len = 0;

    // success
    return 1;
}

int base64_stream_decode_flush ( stream *p_stream )
{

    // unused
    (void) p_stream;

    // success
    return 1;
}

int base64_stream_seek ( stream *p_stream, long offset, enum stream_seek_e whence )
{

    // unused
    (void) p_stream;
    (void) offset;
    (void) whence;

    // error
    return 0;
}

int base64_stream_close ( stream *p_stream )
{

    // initialized data
    base64_stream *p_base64_stream = p_stream->p_data;

    // release the buffer
    p_base64_stream->p_buffer = default_allocator(p_base64_stream->p_buffer, 0);

    // release the base64 stream
    p_base64_stream = default_allocator(p_base64_stream, 0);

    // success
    return 1;
}
