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

static const unsigned long long k512[] =
{
    0x428a2f98d728ae22ULL, 0x7137449123ef65cdULL, 0xb5c0fbcfec4d3b2fULL, 0xe9b5dba58189dbbcULL,
    0x3956c25bf348b538ULL, 0x59f111f1b605d019ULL, 0x923f82a4af194f9bULL, 0xab1c5ed5da6d8118ULL,
    0xd807aa98a3030242ULL, 0x12835b0145706fbeULL, 0x243185be4ee4b28cULL, 0x550c7dc3d5ffb4e2ULL,
    0x72be5d74f27b896fULL, 0x80deb1fe3b1696b1ULL, 0x9bdc06a725c71235ULL, 0xc19bf174cf692694ULL,
    0xe49b69c19ef14ad2ULL, 0xefbe4786384f25e3ULL, 0x0fc19dc68b8cd5b5ULL, 0x240ca1cc77ac9c65ULL,
    0x2de92c6f592b0275ULL, 0x4a7484aa6ea6e483ULL, 0x5cb0a9dcbd41fbd4ULL, 0x76f988da831153b5ULL,
    0x983e5152ee66dfabULL, 0xa831c66d2db43210ULL, 0xb00327c898fb213fULL, 0xbf597fc7beef0ee4ULL,
    0xc6e00bf33da88fc2ULL, 0xd5a79147930aa725ULL, 0x06ca6351e003826fULL, 0x142929670a0e6e70ULL,
    0x27b70a8546d22ffcULL, 0x2e1b21385c26c926ULL, 0x4d2c6dfc5ac42aedULL, 0x53380d139d95b3dfULL,
    0x650a73548baf63deULL, 0x766a0abb3c77b2a8ULL, 0x81c2c92e47edaee6ULL, 0x92722c851482353bULL,
    0xa2bfe8a14cf10364ULL, 0xa81a664bbc423001ULL, 0xc24b8b70d0f89791ULL, 0xc76c51a30654be30ULL,
    0xd192e819d6ef5218ULL, 0xd69906245565a910ULL, 0xf40e35855771202aULL, 0x106aa07032bbd1b8ULL,
    0x19a4c116b8d2d0c8ULL, 0x1e376c085141ab53ULL, 0x2748774cdf8eeb99ULL, 0x34b0bcb5e19b48a8ULL,
    0x391c0cb3c5c95a63ULL, 0x4ed8aa4ae3418acbULL, 0x5b9cca4f7763e373ULL, 0x682e6ff3d6b2b8a3ULL,
    0x748f82ee5defb2fcULL, 0x78a5636f43172f60ULL, 0x84c87814a1f0ab72ULL, 0x8cc702081a6439ecULL,
    0x90befffa23631e28ULL, 0xa4506cebde82bde9ULL, 0xbef9a3f7b2c67915ULL, 0xc67178f2e372532bULL,
    0xca273eceea26619cULL, 0xd186b8c721c0c207ULL, 0xeada7dd6cde0eb1eULL, 0xf57d4f7fee6ed178ULL,
    0x06f067aa72176fbaULL, 0x0a637dc5a2c898a6ULL, 0x113f9804bef90daeULL, 0x1b710b35131c471bULL,
    0x28db77f523047d84ULL, 0x32caab7b40c72493ULL, 0x3c9ebe0a15c9bebcULL, 0x431d67c49c100d4cULL,
    0x4cc5d4becb3e42b6ULL, 0x597f299cfc657e2aULL, 0x5fcb6fab3ad6faecULL, 0x6c44198c4a475817ULL
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

int sha512_transform ( sha512_state *p_sha512_state, const unsigned char *p_data )
{

    // argument check
    if ( NULL == p_sha512_state ) goto no_state;
    if ( NULL ==         p_data ) goto no_data;

    // initialized data
    unsigned long long a = 0, b  = 0, c  = 0,
                       d = 0, e  = 0, f  = 0,
                       g = 0, h  = 0,
                       t1 = 0, t2 = 0; 
    unsigned long long m[80];
    int i, j;

    for 
    (
        i = 0, j = 0;
        i < 16;
        ++i, j += 8
    )
    {
        m[i] = ( (unsigned long long)p_data[j + 0] << 56 )
             | ( (unsigned long long)p_data[j + 1] << 48 )
             | ( (unsigned long long)p_data[j + 2] << 40 )
             | ( (unsigned long long)p_data[j + 3] << 32 )
             | ( (unsigned long long)p_data[j + 4] << 24 )
             | ( (unsigned long long)p_data[j + 5] << 16 )
             | ( (unsigned long long)p_data[j + 6] <<  8 )
             | ( (unsigned long long)p_data[j + 7] <<  0 );
    }

    for
    (
        ;
        i < 80;
        ++i
    )
        m[i] = ((((m[i - 2]) >> (19)) | ((m[i - 2]) << (64 - (19)))) ^ (((m[i - 2]) >> (61)) | ((m[i - 2]) << (64 - (61)))) ^ ((m[i - 2]) >> 6)) 
             + m[i - 7] 
             + ((((m[i - 15]) >> (1)) | ((m[i - 15]) << (64 - (1)))) ^ (((m[i - 15]) >> (8)) | ((m[i - 15]) << (64 - (8)))) ^ ((m[i - 15]) >> 7)) 
             + m[i - 16];

    // initialize the state
    a = p_sha512_state->_state[0], b = p_sha512_state->_state[1],
    c = p_sha512_state->_state[2], d = p_sha512_state->_state[3],
    e = p_sha512_state->_state[4], f = p_sha512_state->_state[5],
    g = p_sha512_state->_state[6], h = p_sha512_state->_state[7];

    // iterate through the data
    for (i = 0; i < 80; ++i)
    {

        // calculate the transformation
        t1 = h + ((((e) >> (14)) | ((e) << (64 - (14)))) ^ (((e) >> (18)) | ((e) << (64 - (18)))) ^ (((e) >> (41)) | ((e) << (64 - (41))))) + (((e) & (f)) ^ (~(e) & (g))) + k512[i] + m[i],
        
        t2 = ((((a) >> (28)) | ((a) << (64 - (28)))) ^ (((a) >> (34)) | ((a) << (64 - (34)))) ^ (((a) >> (39)) | ((a) << (64 - (39))))) + (((a) & (b)) ^ ((a) & (c)) ^ ((b) & (c)));
        
        // update the state
        h = g, g = f, f = e, e =  d + t1,
        d = c, c = b, b = a, a = t1 + t2;
    }

    // update the state
    p_sha512_state->_state[0] += a, p_sha512_state->_state[1] += b,
    p_sha512_state->_state[2] += c, p_sha512_state->_state[3] += d,
    p_sha512_state->_state[4] += e, p_sha512_state->_state[5] += f,
    p_sha512_state->_state[6] += g, p_sha512_state->_state[7] += h;

    // success
    return 1;

    // error handling
    {

        // argument errors
        {
            no_state:
                #ifndef NDEBUG
                    log_error("[sha] Null pointer provided for parameter \"p_sha512_state\" in call to function \"%s\"\n", __FUNCTION__);
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

int sha512_construct ( sha512_state *p_sha512_state )
{

    // argument check
    if ( NULL == p_sha512_state ) goto no_state;

    // populate the result
    *p_sha512_state = (sha512_state)
    {
        .datalen = 0,
        .bitlen  = 0,
        ._data   = { 0 },
        ._state  =
        {
            0x6a09e667f3bcc908ULL, 0xbb67ae8584caa73bULL,
            0x3c6ef372fe94f82bULL, 0xa54ff53a5f1d36f1ULL,
            0x510e527fade682d1ULL, 0x9b05688c2b3e6c1fULL,
            0x1f83d9abfb41bd6bULL, 0x5be0cd19137e2179ULL
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
                    log_error("[sha] Null pointer provided for parameter \"p_sha512_state\" in call to function \"%s\"\n", __FUNCTION__);
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

int sha512_update ( sha512_state *p_sha512_state, const unsigned char *data, size_t len )
{
    
    // argument check
    if ( NULL == p_sha512_state ) goto no_state;
    if ( NULL ==           data ) goto no_data;
    if ( 0    ==           len  ) goto no_length;

    // iterate through the data
    for (size_t i = 0; i < len; ++i)
    {

        // store a byte
        p_sha512_state->_data[p_sha512_state->datalen] = data[i],
        p_sha512_state->datalen++;

        // keep reading until there are 128 bytes
        if ( 128 != p_sha512_state->datalen ) continue;
        
        // transform the data
        sha512_transform(p_sha512_state, p_sha512_state->_data);

        // update the counters
        p_sha512_state->bitlen += 1024,
        p_sha512_state->datalen = 0;
    }

    // success
    return 1;

    // error handling
    {

        // argument errors
        {
            no_state:
                #ifndef NDEBUG
                    log_error("[sha] Null pointer provided for parameter \"p_sha512_state\" in call to function \"%s\"\n", __FUNCTION__);
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

int sha512_final ( sha512_state *p_sha512_state, unsigned char *hash)
{

    // argument check
    if ( NULL == p_sha512_state ) goto no_state;
    if ( NULL ==           hash ) goto no_hash;

    // initialized data
    unsigned int i = p_sha512_state->datalen;

    // append the padding
    if (p_sha512_state->datalen < 112)
    {

        // set the first padding bit
        p_sha512_state->_data[i++] = 0x80;
        
        // pad the rest with zeros
        while (i < 128) p_sha512_state->_data[i++] = 0x00;
    }

    else
    {
        p_sha512_state->_data[i++] = 0x80;
        
        while (i < 128) p_sha512_state->_data[i++] = 0x00;
        
        sha512_transform(p_sha512_state, p_sha512_state->_data);
        memset(p_sha512_state->_data, 0, 112);
    }

    // accumulate the length
    p_sha512_state->bitlen += p_sha512_state->datalen * 8;

    for (int j = 0; j < 8; ++j)
        p_sha512_state->_data[127 - j] = (p_sha512_state->bitlen >> (8 * j)) & 0xff;
    
    // transform the data
    sha512_transform(p_sha512_state, p_sha512_state->_data);

    for (i = 0; i < 8; ++i) 
        for (int j = 0; j < 8; ++j) 
            hash[i * 8 + j] = (p_sha512_state->_state[i] >> (56 - j * 8)) & 0xff;

    // success
    return 1;

    // error handling
    {

        // argument errors
        {
            no_state:
                #ifndef NDEBUG
                    log_error("[sha] Null pointer provided for parameter \"p_sha512_state\" in call to function \"%s\"\n", __FUNCTION__);
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

hash64 sha512_hash64 ( const void *const k, unsigned long long l )
{

    // argument check
    if ( k == (void *) 0 ) goto no_k;

    // initialize the hasher
    sha512_state state = { 0 };
    sha512_hash  hash  = { 0 };
    hash64       result = 0;

    // construct a hasher
    sha512_construct(&state);

    // feed the hasher
    if ( l ) sha512_update(&state, k, l);

    // digest the input
    sha512_final(&state, hash);

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
    for (size_t i = 0; i < sizeof(sha256_hash); i++) 

        // ... one byte at a time
        printf("%02x", _hash[i]);

    // success
    return 1;
}

int sha512_print ( sha512_hash _hash )
{

    // print the hash ...
    for (size_t i = 0; i < sizeof(sha512_hash); i++) 

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

int sha512_pack ( void *p_buffer, sha512_hash _hash )
{
    
    // argument check
    if ( NULL == p_buffer ) goto no_buffer;

    // initialized data
    char *p = p_buffer;
    
    // pack the hash
    p += pack_pack(
        p, "%64i8",
        _hash[0] , _hash[1] , _hash[2] , _hash[3],
        _hash[4] , _hash[5] , _hash[6] , _hash[7],
        _hash[8] , _hash[9] , _hash[10], _hash[11],
        _hash[12], _hash[13], _hash[14], _hash[15],
        _hash[16], _hash[17], _hash[18], _hash[19],
        _hash[20], _hash[21], _hash[22], _hash[23],
        _hash[24], _hash[25], _hash[26], _hash[27],
        _hash[28], _hash[29], _hash[30], _hash[31],
        _hash[32], _hash[33], _hash[34], _hash[35],
        _hash[36], _hash[37], _hash[38], _hash[39],
        _hash[40], _hash[41], _hash[42], _hash[43],
        _hash[44], _hash[45], _hash[46], _hash[47],
        _hash[48], _hash[49], _hash[50], _hash[51],
        _hash[52], _hash[53], _hash[54], _hash[55],
        _hash[56], _hash[57], _hash[58], _hash[59],
        _hash[60], _hash[61], _hash[62], _hash[63]
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

int sha512_unpack ( sha512_hash *p_hash, void *p_buffer )
{

    // argument check
    if ( NULL ==   p_hash ) goto no_hash;
    if ( NULL == p_buffer ) goto no_buffer;
 
    // initialized data
    char *p       = p_buffer;
    char  buf[64] = { 0 };

    // unpack the hash
    p += pack_unpack(
        p, "%64i8",
        buf,
        &p_hash[0] , &p_hash[1] , &p_hash[2] , &p_hash[3],
        &p_hash[4] , &p_hash[5] , &p_hash[6] , &p_hash[7],
        &p_hash[8] , &p_hash[9] , &p_hash[10], &p_hash[11],
        &p_hash[12], &p_hash[13], &p_hash[14], &p_hash[15],
        &p_hash[16], &p_hash[17], &p_hash[18], &p_hash[19],
        &p_hash[20], &p_hash[21], &p_hash[22], &p_hash[23],
        &p_hash[24], &p_hash[25], &p_hash[26], &p_hash[27],
        &p_hash[28], &p_hash[29], &p_hash[30], &p_hash[31],
        &p_hash[32], &p_hash[33], &p_hash[34], &p_hash[35],
        &p_hash[36], &p_hash[37], &p_hash[38], &p_hash[39],
        &p_hash[40], &p_hash[41], &p_hash[42], &p_hash[43],
        &p_hash[44], &p_hash[45], &p_hash[46], &p_hash[47],
        &p_hash[48], &p_hash[49], &p_hash[50], &p_hash[51],
        &p_hash[52], &p_hash[53], &p_hash[54], &p_hash[55],
        &p_hash[56], &p_hash[57], &p_hash[58], &p_hash[59],
        &p_hash[60], &p_hash[61], &p_hash[62], &p_hash[63]
    );

    // error check
    if ( 64 != (p - (char*) p_buffer) ) goto failed_to_unpack_hash;

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
