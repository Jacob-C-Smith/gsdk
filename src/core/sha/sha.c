/** ! 
 * SHA implementation
 * 
 * @file src/core/sha/sha.c
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

// core
#include <core/log.h>
#include <core/sha.h>

// data
static const unsigned int k[] =
{
    0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5,
    0x3956c25b, 0x59f111f1, 0x923f82a4, 0xab1c5ed5,
    0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3,
    0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174,
    0xe49b69c1, 0xefbe4786, 0x0fc19dc6, 0x240ca1cc,
    0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da,
    0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7,
    0xc6e00bf3, 0xd5a79147, 0x06ca6351, 0x14292967,
    0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13,
    0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85,
    0xa2bfe8a1, 0xa81a664b, 0xc24b8b70, 0xc76c51a3,
    0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070,
    0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5,
    0x391c0cb3, 0x4ed8aa4a, 0x5b9cca4f, 0x682e6ff3,
    0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208,
    0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2
};

int sha256_transform ( sha256_state *p_sha256_state, const unsigned char *p_data )
{

    // argument check
    if ( NULL == p_sha256_state ) goto no_state;
    if ( NULL ==         p_data ) goto no_data;

    // initialized data
    unsigned int a = 0, b  = 0, c  = 0,
                 d = 0, e  = 0, f  = 0,
                 g = 0, h  = 0, i  = 0,
                 j = 0,
                 
                 t1 = 0, t2 = 0; 
    unsigned int m[64];

    for 
    (
        i = 0, j = 0;
        i < 16;
        ++i, j += 4
    )
    {
        m[i] = ( p_data[j + 0] << 24 )
             | ( p_data[j + 1] << 16 )
             | ( p_data[j + 2] <<  8 )
             | ( p_data[j + 3] <<  0 );
    }

    for
    (
        ;
        i < 64;
        ++i
    )
        m[i] = ((((m[i - 2]) >> (17)) | ((m[i - 2]) << (32 - (17)))) ^ (((m[i - 2]) >> (19)) | ((m[i - 2]) << (32 - (19)))) ^ ((m[i - 2]) >> 10)) + m[i - 7] + ((((m[i - 15]) >> (7)) | ((m[i - 15]) << (32 - (7)))) ^ (((m[i - 15]) >> (18)) | ((m[i - 15]) << (32 - (18)))) ^ ((m[i - 15]) >> 3)) + m[i - 16];

    // initialize the state
    a = p_sha256_state->_state[0], b = p_sha256_state->_state[1],
    c = p_sha256_state->_state[2], d = p_sha256_state->_state[3],
    e = p_sha256_state->_state[4], f = p_sha256_state->_state[5],
    g = p_sha256_state->_state[6], h = p_sha256_state->_state[7];

    // iterate through the data
    for (i = 0; i < 64; ++i)
        
        // calculate the transformation
        t1 = h + ((((e) >> (6)) | ((e) << (32 - (6)))) ^ (((e) >> (11)) | ((e) << (32 - (11)))) ^ (((e) >> (25)) | ((e) << (32 - (25))))) + (((e) & (f)) ^ (~(e) & (g))) + k[i] + m[i],
        t2 = ((((a) >> (2)) | ((a) << (32 - (2)))) ^ (((a) >> (13)) | ((a) << (32 - (13)))) ^ (((a) >> (22)) | ((a) << (32 - (22))))) + (((a) & (b)) ^ ((a) & (c)) ^ ((b) & (c))),
        
        // update the state
        h = g, g = f, f = e, e =  d + t1,
        d = c, c = b, b = a, a = t1 + t2;

    // update the state
    p_sha256_state->_state[0] += a, p_sha256_state->_state[1] += b,
    p_sha256_state->_state[2] += c, p_sha256_state->_state[3] += d,
    p_sha256_state->_state[4] += e, p_sha256_state->_state[5] += f,
    p_sha256_state->_state[6] += g, p_sha256_state->_state[7] += h;

    // success
    return 1;

    // error handling
    {

        // argument errors
        {
            no_state:
                #ifndef NDEBUG
                    log_error("[sha] Null pointer provided for parameter \"p_sha256_state\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;

            no_data:
                #ifndef NDEBUG
                    log_error("[sha] Null pointer provided for parameter \"p_data\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
    }
}

int sha256_construct ( sha256_state *p_sha256_state )
{

    // argument check
    if ( NULL == p_sha256_state ) goto no_state;

    // populate the result
    *p_sha256_state = (sha256_state)
    {
        .datalen = 0,
        .bitlen  = 0,
        ._data   = { 0 },
        ._state  =
        {
            0x6a09e667, 0xbb67ae85,
            0x3c6ef372, 0xa54ff53a,
            0x510e527f, 0x9b05688c,
            0x1f83d9ab, 0x5be0cd19
        }
    };

    // success
    return 1;

    // error handling
    {

        // argument errors
        {
            no_state:
                #ifndef NDEBUG
                    log_error("[sha] Null pointer provided for parameter \"pp_sha256_state\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
    }
}

int sha256_update ( sha256_state *p_sha256_state, const unsigned char *data, size_t len )
{
    
    // argument check
    if ( NULL == p_sha256_state ) goto no_state;
    if ( NULL ==           data ) goto no_data;
    if ( 0    ==           len  ) goto no_length;

    // iterate through the data
    for (size_t i = 0; i < len; ++i)
    {

        // store a byte
        p_sha256_state->_data[p_sha256_state->datalen] = data[i],
        p_sha256_state->datalen++;

        // keep reading until there are 64 bytes
        if ( 64 != p_sha256_state->datalen ) continue;
        
        // transform the data
        sha256_transform(p_sha256_state, p_sha256_state->_data);

        // update the counters
        p_sha256_state->bitlen += 512,
        p_sha256_state->datalen = 0;
    }

    // success
    return 1;

    // error handling
    {

        // argument errors
        {
            no_state:
                #ifndef NDEBUG
                    log_error("[sha] Null pointer provided for parameter \"pp_sha256_state\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;

            no_data:
                #ifndef NDEBUG
                    log_error("[sha] Null pointer provided for parameter \"data\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;

            no_length:
                #ifndef NDEBUG
                    log_error("[sha] Zero length provided for parameter \"len\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
    }
}

int sha256_final ( sha256_state *p_sha256_state, unsigned char *hash)
{

    // argument check
    if ( NULL == p_sha256_state ) goto no_state;
    if ( NULL ==           hash ) goto no_hash;

    // initialized data
    unsigned int i = p_sha256_state->datalen;

    // append the padding
    if (p_sha256_state->datalen < 56)
    {

        // set the first padding bit
        p_sha256_state->_data[i++] = 0x80;
        
        // pad the rest with zeros
        while (i < 56) p_sha256_state->_data[i++] = 0x00;
    }

    else
    {
        p_sha256_state->_data[i++] = 0x80;
        
        while (i < 64) p_sha256_state->_data[i++] = 0x00;
        
        sha256_transform(p_sha256_state, p_sha256_state->_data);
        memset(p_sha256_state->_data, 0, 56);
    }

    // accumulate the length
    p_sha256_state->bitlen += p_sha256_state->datalen * 8;

    for (int j = 0; j < 8; ++j)
        p_sha256_state->_data[63 - j] = (p_sha256_state->bitlen >> (8 * j)) & 0xff;

    // transform the data
    sha256_transform(p_sha256_state, p_sha256_state->_data);

    for (i = 0; i < 4; ++i) 
        for (int j = 0; j < 8; ++j) 
            for (int k = 0; k < 4; ++k) 
                hash[j * 4 + k] = (p_sha256_state->_state[j] >> (24 - k * 8)) & 0xff;

    // success
    return 1;

    // error handling
    {

        // argument errors
        {
            no_state:
                #ifndef NDEBUG
                    log_error("[sha] Null pointer provided for parameter \"p_sha256_state\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;

            no_hash:
                #ifndef NDEBUG
                    log_error("[sha] Null pointer provided for parameter \"hash\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
    }
}

hash64 sha256_hash64 ( const void *const k, unsigned long long l )
{

    // argument check
    if ( k == (void *) 0 ) goto no_k;

    // initialize the hasher
    sha256_state state = { 0 };
    sha256_hash  hash  = { 0 };
    hash64       result = 0;

    // construct a hasher
    sha256_construct(&state);

    // feed the hasher
    if ( l ) sha256_update(&state, k, l);

    // digest the input
    sha256_final(&state, hash);

    // convert the state to a hash
    result = (((hash64) hash[0]) << 56)
           | (((hash64) hash[1]) << 48)
           | (((hash64) hash[2]) << 40)
           | (((hash64) hash[3]) << 32)
           | (((hash64) hash[4]) << 24)
           | (((hash64) hash[5]) << 16)
           | (((hash64) hash[6]) << 8)
           | (((hash64) hash[7]) << 0);

    // success
    return result;

    // error handling
    {
        no_k:
            #ifndef NDEBUG
                log_error("[sha] Null pointer provided for parameter \"k\" in call to function \"%s\"\n", __FUNCTION__);
            #endif

            // error
            return 0;
    }
}

int sha256_print ( sha256_hash _hash )
{

    // print the hash ...
    for (int i = 0; i < sizeof(sha256_hash); i++) 

        // ... one byte at a time
        printf("%02x", _hash[i]);

    // success
    return 1;
}

int sha256_pack ( void *p_buffer, sha256_hash _hash )
{
    
    // argument check
    if ( NULL == p_buffer ) goto no_buffer;

    // initialized data
    char *p = p_buffer;
    
    // pack the hash
    p += pack_pack(
        p, "%32i8",
        _hash[0] , _hash[1] , _hash[2] , _hash[3],
        _hash[4] , _hash[5] , _hash[6] , _hash[7],
        _hash[8] , _hash[9] , _hash[10], _hash[11],
        _hash[12], _hash[13], _hash[14], _hash[15],
        _hash[16], _hash[17], _hash[18], _hash[19],
        _hash[20], _hash[21], _hash[22], _hash[23],
        _hash[24], _hash[25], _hash[26], _hash[27],
        _hash[28], _hash[29], _hash[30], _hash[31]
    );

    // success
    return p - (char*) p_buffer;

    // error check
    {
        
        // argument errors
        {
            no_buffer:
                #ifndef NDEBUG
                    log_error("[sha] Null pointer provided for parameter \"p_buffer\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
    }
}

int sha256_unpack ( sha256_hash *p_hash, void *p_buffer )
{

    // argument check
    if ( NULL ==   p_hash ) goto no_hash;
    if ( NULL == p_buffer ) goto no_buffer;
 
    // initialized data
    char *p       = p_buffer;
    char  buf[64] = { 0 };

    // unpack the hash
    p += pack_unpack(
        p, "%32i8",
        buf,
        &p_hash[0] , &p_hash[1] , &p_hash[2] , &p_hash[3],
        &p_hash[4] , &p_hash[5] , &p_hash[6] , &p_hash[7],
        &p_hash[8] , &p_hash[9] , &p_hash[10], &p_hash[11],
        &p_hash[12], &p_hash[13], &p_hash[14], &p_hash[15],
        &p_hash[16], &p_hash[17], &p_hash[18], &p_hash[19],
        &p_hash[20], &p_hash[21], &p_hash[22], &p_hash[23],
        &p_hash[24], &p_hash[25], &p_hash[26], &p_hash[27],
        &p_hash[28], &p_hash[29], &p_hash[30], &p_hash[31]
    );

    // error check
    if ( 32 != (p - (char*) p_buffer) ) goto failed_to_unpack_hash;

    // success
    return 1;

    // error handling
    {

        // argument errors
        {
            no_hash:
                #ifndef NDEBUG
                    log_error("[sha] Null pointer provided for parameter \"p_hash\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;

            no_buffer:
                #ifndef NDEBUG
                    log_error("[sha] Null pointer provided for parameter \"p_buffer\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }

        // format errors
        {
            failed_to_unpack_hash:
                #ifndef NDEBUG
                    log_error("[sha] Failed to unpack SHA256 hash from buffer in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
    }
}
