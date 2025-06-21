// Header
#include <core/hash.h>

// Function declarations
/** !
 * Bitwise left rotate an unsigned 64 bit value by N bits
 * 
 * @param x the unsigned 64 bit value
 * @param r N bits
 * 
 * @return rotated x
 */
unsigned long long rotl64 ( unsigned long long x, signed char r );

// Function definitions
hash64 hash_fnv64 ( const void *const k, unsigned long long l )
{

    // Argument check
    if ( k == (void *) 0 ) goto no_k;
    
    // Constant data
    const unsigned long long p = 0x100000001B3;

    // Initialized data
    unsigned long long h = 0xc6a4a7935bd1e995;
    
    // Compute the hash
    for (size_t i = 0; i < l; i++)
    {

        // XOR the eight least significant bits of the hash
        h ^= (unsigned long long)((char *)k)[i];

        // Multiply the hash by the prime
        h *= p;
    }
    
    // Success
    return h;

    // Error handling
    {

        // Argument error
        {
            no_k:
                #ifndef NDEBUG
                    printf("[hash cache] [hash] Null pointer provided for parameter \"k\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }
    }
}

hash64 hash_mmh64 ( const void* const k, unsigned long long l )
{

    // Argument check
    if ( k == (void *) 0 ) goto no_k;
    
    // Constant data
    const unsigned long long m = 0xc6a4a7935bd1e995;
    const int                r = 47;

    // Initialized data
    unsigned long long   h    = 0x41C64E6D ^ (l * m);
    unsigned long long  *data = (unsigned long long*)k;
    unsigned long long  *end  = (l >> 3) + data;
    unsigned char       *d2   = 0;

    // Compute the hash
    while ( data != end )
    {
        unsigned long long n = *data++;

        n *= m;
        n ^= n >> r;
        n *= m;

        h ^= n;
        h *= m;
    }

    d2 = (unsigned char *)data;

    switch ( l & 7 )
    {
        case 7: h ^= (unsigned long long)(d2[6]) << 48; __attribute__((fallthrough));
        case 6: h ^= (unsigned long long)(d2[5]) << 40; __attribute__((fallthrough));
        case 5: h ^= (unsigned long long)(d2[4]) << 32; __attribute__((fallthrough));
        case 4: h ^= (unsigned long long)(d2[3]) << 24; __attribute__((fallthrough));
        case 3: h ^= (unsigned long long)(d2[2]) << 16; __attribute__((fallthrough));
        case 2: h ^= (unsigned long long)(d2[1]) << 8;  __attribute__((fallthrough));
        case 1: h ^= (unsigned long long)(d2[0]);
            h *= m;
    }

    h ^= h >> r;
    h *= m;
    h ^= h >> r;

    // Success
    return h;

    // Error handling
    {

        // Argument error
        {
            no_k:
                #ifndef NDEBUG
                    printf("[hash cache] [hash] Null pointer provided for parameter \"k\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }
    }
}

hash64 hash_xxh64 ( const void *const k, unsigned long long l )
{

    // Argument check
    if ( k == (void *) 0 ) goto no_k;
    
    // Initialized data
    hash64 h = 0x27D4EB2F165667C5;
    const unsigned char* p = (const unsigned char*)k;
    const unsigned char* const end = p + l;

    // Long input
    if ( l >= 32 )
    {
        
        // Initialized data
        const unsigned char* const limit = end - 32;
        unsigned long long v1 = (0x9E3779B185EBCA87 + 0xC2B2AE3D27D4EB4F);
        unsigned long long v2 = 0xC2B2AE3D27D4EB4F;
        unsigned long long v3 = 0;
        unsigned long long v4 = 0x9E3779B185EBCA87;

        // Compute the
        do
        {
            // Initialized data
            unsigned long long k1 = *(const unsigned long long*)(p);
            unsigned long long k2 = *(const unsigned long long*)(p + 8);
            unsigned long long k3 = *(const unsigned long long*)(p + 16);
            unsigned long long k4 = *(const unsigned long long*)(p + 24);

            v1 += k1 * 0xC2B2AE3D27D4EB4F;
            v1 = rotl64(v1, 31);
            v1 *= 0x9E3779B185EBCA87;

            v2 += k2 * 0xC2B2AE3D27D4EB4F;
            v2 = rotl64(v2, 31);
            v2 *= 0x9E3779B185EBCA87;

            v3 += k3 * 0xC2B2AE3D27D4EB4F;
            v3 = rotl64(v3, 31);
            v3 *= 0x9E3779B185EBCA87;

            v4 += k4 * 0xC2B2AE3D27D4EB4F;
            v4 = rotl64(v4, 31);
            v4 *= 0x9E3779B185EBCA87;

            p += 32;
        }
        
        // Continuation condition
        while ( p <= limit );

        h = rotl64(v1, 1) + rotl64(v2, 7) + rotl64(v3, 12) + rotl64(v4, 18);
        h = (h ^ (rotl64(v1 * 0xC2B2AE3D27D4EB4F, 31) * 0x9E3779B185EBCA87)) * 0x9E3779B185EBCA87 + 0x85EBCA77C2B2AE63;
        h += rotl64(v2 * 0xC2B2AE3D27D4EB4F, 31) * 0x9E3779B185EBCA87;
        h = (h ^ (rotl64(v3 * 0xC2B2AE3D27D4EB4F, 31) * 0x9E3779B185EBCA87)) * 0x9E3779B185EBCA87 + 0x85EBCA77C2B2AE63;
        h += rotl64(v4 * 0xC2B2AE3D27D4EB4F, 31) * 0x9E3779B185EBCA87;
    }

    h += (unsigned long long)l;

    // Compute the hash
    while (p + 8 <= end)
    {

        // Initialized data
        unsigned long long k1 = *(const unsigned long long*)(p);
        
        k1 *= 0xC2B2AE3D27D4EB4F;
        k1 = rotl64(k1, 31);
        k1 *= 0x9E3779B185EBCA87;
        h ^= k1;
        h = rotl64(h, 27) * 0x9E3779B185EBCA87 + 0x85EBCA77C2B2AE63;
        p += 8;
    }

    if ( p + 4 <= end )
        h ^= (unsigned long long)(*(const unsigned long*)(p)) * 0x9E3779B185EBCA87,
        h = rotl64(h, 23) * 0xC2B2AE3D27D4EB4F + 0x165667B19E3779F9,
        p += 4;

    // Compute the last 8 bytes
    while ( p < end )
    {
        h ^= (*p) * 0x27D4EB2F165667C5;
        h = rotl64(h, 11) * 0x9E3779B185EBCA87, p++;
    }

    // Last round
    h ^= h >> 33, h *= 0xC2B2AE3D27D4EB4F,
    h ^= h >> 29, h *= 0x165667B19E3779F9,
    h ^= h >> 32;

    // Success
    return h;

    // Error handling
    {

        // Argument error
        {
            no_k:
                #ifndef NDEBUG
                    printf("[hash cache] [hash] Null pointer provided for parameter \"k\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }
    }
}

hash64 hash_crc64 ( const void *const k, unsigned long long l )
{

    // Argument check
    if ( k == (void *) 0 ) goto no_k;

    // External data
    extern unsigned long long crc64_table[];

    // Initialized data
    const unsigned char *p = k;
    hash64 h = 0xFFFFFFFFFFFFFFFF;

    // Iterate through the input
    for (size_t i = 0; i < l; i++)

        // Update the CRC
        h = crc64_table[(h ^ p[i]) & 0xFF] ^ (h >> 8);

    // Success
    return h ^ 0xFFFFFFFFFFFFFFFF;

    // Error handling
    {

        // Argument error
        {
            no_k:
                #ifndef NDEBUG
                    printf("[hash cache] [hash] Null pointer provided for parameter \"k\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }
    }
}

unsigned long long rotl64 ( unsigned long long x, signed char r )
{

    // Success
    return (x << r) | (x >> (64 - r));
}
