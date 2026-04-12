/** ! 
 * X25519 implementation
 * 
 * @file src/core/x25519/x25519.c
 * 
 * @author Jacob Smith
 */

// header file
#include <core/x25519.h>

// type definitions
typedef unsigned _BitInt(256) i256;
typedef unsigned _BitInt(512) i512;

// constant data
static const i256 Q   = 0x7FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFEDWB;
static const i256 A24 = 121665WB;

// function declarations
/** !
 * Modular exponentiation by squaring
 * 
 * @param b the base
 * @param e the exponent
 * @param m the modulus
 * 
 * @return (b ^ e) mod m 
 */
i256 expmod ( i256 b, i256 e, i256 m );

/** !
 * Modular inverse 
 * 
 * @param x the number
 * 
 * @return the modular inverse of x mod 2^255-19
 */
i256 inv ( i256 x );

/** !
 * Conditional swap
 * 
 * @param b swap if b is 1
 * @param x first value
 * @param y second value
 * 
 * @return void
 */
void cswap ( int b, i256 *x, i256 *y );

/** !
 * Montgomery ladder for X25519
 * 
 * @param k   the scalar
 * @param u   the u coordinate of the point
 * @param out result
 * 
 * @return void
 */
void x25519_ladder ( const unsigned char *k, const unsigned char *u, unsigned char *out );

// function definitions
i256 add_mod ( i256 a, i256 b, i256 m ) { return (a + b) % m; }
i256 sub_mod ( i256 a, i256 b, i256 m ) { return (a - b + m) % m; }
i256 mul_mod ( i256 a, i256 b, i256 m ) { return (i256)(((i512)a * (i512)b) % (i512)m); }
i256 sqr_mod ( i256 a, i256 m )         { return mul_mod(a, a, m); }
i256 inv     ( i256 x )                 { return expmod(x, Q - 2, Q); }

i256 expmod ( i256 b, i256 e, i256 m )
{

    // initialized data
    i256 t = 1;

    // process the exponent
    while ( e > 0 )
    {

        // odd exponent 
        if ( e % 2 == 1 ) t = mul_mod(t, b, m);

        // square
        b = sqr_mod(b, m);

        // shift
        e /= 2;
    }
    
    // done
    return t;
}

void cswap ( int b, i256 *x, i256 *y )
{

    // initialized data
    i256 mask = (i256)(-(unsigned _BitInt(256))b); 
    i256 diff = (*x ^ *y) & mask;

    // swap
    *x ^= diff;
    *y ^= diff;
}

void x25519_ladder ( const unsigned char *k, const unsigned char *u, unsigned char *out )
{

    // initialized data
    i256 x_1 = 0;
    i256 res = 0;
    i256 x_2 = 1, z_2 = 0;
    i256 x_3 = 0, z_3 = 1;
    int swap = 0;
    unsigned char u_copy[32] = { 0 };
    unsigned char k_copy[32] = { 0 };

    // make a copy of u
    memcpy(u_copy, u, 32);

    // make a copy of k
    memcpy(k_copy, k, 32);

    // clamp u
    u_copy[31] &= 127;

    // swap the endianness of u
    for (int i = 0; i < 32; i++) 
        x_1 |= ((i256)u_copy[i]) << (i * 8);

    // make a copy of x_1
    x_3 = x_1;

    // clamp k
    k_copy[0] &= 248;
    k_copy[31] &= 127;
    k_copy[31] |= 64;

    // montgomery ladder, as described in RFC 7748, section 5
    for (int t = 254; t >= 0; t--)
    {

        // initialized data
        int k_t = (k_copy[t / 8] >> (t % 8)) & 1;
        i256 A  = 0,
             AA = 0,
             B  = 0,
             BB = 0,
             E  = 0,
             C  = 0,
             D  = 0,
             DA = 0,
             CB = 0;

        // swap
        swap ^= k_t;
        cswap(swap, &x_2, &x_3);
        cswap(swap, &z_2, &z_3);
        swap = k_t;

        // a = x_2 + z_2
        A = add_mod(x_2, z_2, Q);

        // aa = a^2
        AA = sqr_mod(A, Q);

        // b = x_2 - z_2
        B = sub_mod(x_2, z_2, Q);

        // bb = b^2
        BB = sqr_mod(B, Q);

        // e = a^2 - b^2
        E = sub_mod(AA, BB, Q);

        // c = x_3 + z_3
        C = add_mod(x_3, z_3, Q);

        // d = x_3 - z_3
        D = sub_mod(x_3, z_3, Q);

        // da = d * a
        DA = mul_mod(D, A, Q);

        // cb = c * b
        CB = mul_mod(C, B, Q);

        // x_3 = (da + cb)^2
        x_3 = sqr_mod
        (
            add_mod(DA, CB, Q), 
            Q
        );

        // z_3 = x_1 * (da - cb)^2
        z_3 = mul_mod
        (
            x_1, 
            sqr_mod(
                sub_mod(DA, CB, Q),
                Q
            ),
            Q
        );

        // x_2 = a^2 * b^2
        x_2 = mul_mod(AA, BB, Q);

        // z_2 = e * (a^2 + A24 * e)
        z_2 = mul_mod
        (
            E, 
            add_mod
            (
                AA, 
                mul_mod(A24, E, Q),
                Q
            ),
            Q
        );
    }

    // final swap
    cswap(swap, &x_2, &x_3);
    cswap(swap, &z_2, &z_3);

    // compute the result
    res = mul_mod
    (
        x_2, 
        inv(z_2),
        Q
    );

    // swap the endianness of the result
    for (int i = 0; i < 32; i++)
        out[i] = (unsigned char)(res & 0xFF),
        res >>= 8;

    // done
    return;
}

int x25519_key_pair_construct 
(
    x25519_public_key  *p_public_key, 
    x25519_private_key *p_private_key
)
{

    // argument check
    if ( NULL ==  p_public_key ) goto no_public_key;
    if ( NULL == p_private_key ) goto no_private_key;
    
    // initialized data
    FILE *urandom = fopen("/dev/urandom", "rb");
    int read = 0;

    // error check
    if ( NULL == urandom ) goto failed_to_open_urandom;
    
    // read a random key
    read = fread(p_private_key, 1, 32, urandom);
    
    // close urandom
    fclose(urandom);

    // error check
    if ( read != 32 ) goto failed_to_read_urandom;

    // derive public key
    return x25519_public_key_derive(p_private_key, p_public_key);

    // error handling
    {

        // argument errors
        {
            no_public_key:
                #ifndef NDEBUG
                    log_error("[x25519] Null pointer provided for parameter \"p_public_key\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;

            no_private_key:
                #ifndef NDEBUG
                    log_error("[x25519] Null pointer provided for parameter \"p_private_key\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }

        // standard library errors
        {
            failed_to_open_urandom:
                #ifndef NDEBUG
                    log_error("[standard library] Failed to open \"/dev/urandom\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
            
            failed_to_read_urandom:
                #ifndef NDEBUG
                    log_error("[standard library] Failed to read \"/dev/urandom\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
    }
}

int x25519_public_key_derive 
(
    x25519_private_key *p_private_key, 
    x25519_public_key  *p_public_key
)
{

    // argument check
    if ( NULL == p_private_key ) goto no_private_key;
    if ( NULL ==  p_public_key ) goto no_public_key;

    // initialized data
    unsigned char u[32] = { 9 };

    // derive the public key
    x25519_ladder
    (
        (const unsigned char *)p_private_key, 
        u, 
        (unsigned char *)p_public_key
    );

    // success
    return 1;

    // error handling
    {

        // argument errors
        {
            no_private_key:
                #ifndef NDEBUG
                    log_error("[x25519] Null pointer provided for parameter \"p_private_key\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;

            no_public_key:
                #ifndef NDEBUG
                    log_error("[x25519] Null pointer provided for parameter \"p_public_key\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
    }
}

int x25519_shared_secret_derive
(
    x25519_private_key   *p_private_key, 
    x25519_public_key    *p_public_key, 
    x25519_shared_secret *p_shared_secret
)
{
    // argument check
    if ( NULL ==  p_private_key ) goto no_private_key;
    if ( NULL ==   p_public_key ) goto no_public_key;
    if ( NULL == p_shared_secret ) goto no_shared_secret;

    // derive the shared secret
    x25519_ladder
    (
        (const unsigned char *)p_private_key, 
        (const unsigned char *)p_public_key, 
        (unsigned char *)p_shared_secret
    );

    // success
    return 1;

    // error handling
    {

        // argument errors
        {
            no_private_key:
                #ifndef NDEBUG
                    log_error("[x25519] Null pointer provided for parameter \"p_private_key\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;

            no_public_key:
                #ifndef NDEBUG
                    log_error("[x25519] Null pointer provided for parameter \"p_public_key\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;

            no_shared_secret:
                #ifndef NDEBUG
                    log_error("[x25519] Null pointer provided for parameter \"p_shared_secret\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
    }
}

int x25519_public_key_print ( x25519_public_key *p_public_key )
{

    // argument check
    if ( NULL == p_public_key ) goto no_public_key;

    // print each byte of the public key
    for (size_t i = 0; i < 32; i++) 
        printf("%02x", (*p_public_key)[i]);

    // formatting
    putchar('\n');

    // success
    return 1;

    // error handling
    {

        // argument errors
        {
            no_public_key:
                #ifndef NDEBUG
                    log_error("[x25519] Null pointer provided for parameter \"p_public_key\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
    }
}

int x25519_private_key_print ( x25519_private_key *p_private_key )
{

    // argument check
    if ( NULL == p_private_key ) goto no_private_key;

    // print each byte of the private key
    for (size_t i = 0; i < 32; i++) 
        printf("%02x", (*p_private_key)[i]);

    // formatting
    putchar('\n');

    // success
    return 1;

    // error handling
    {

        // argument errors
        {
            no_private_key:
                #ifndef NDEBUG
                    log_error("[x25519] Null pointer provided for parameter \"p_private_key\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
    }
}

int x25519_shared_secret_print ( x25519_shared_secret *p_shared_secret )
{

    // argument check
    if ( NULL == p_shared_secret ) goto no_shared_secret;

    // print each byte of the shared secret
    for (size_t i = 0; i < 32; i++) 
        printf("%02x", (*p_shared_secret)[i]);

    // formatting
    putchar('\n');

    // success
    return 1;

    // error handling
    {

        // argument errors
        {
            no_shared_secret:
                #ifndef NDEBUG
                    log_error("[x25519] Null pointer provided for parameter \"p_shared_secret\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
    }
}

int x25519_public_key_pack ( void *p_buffer, x25519_public_key *p_public_key )
{

    // argument check
    if ( NULL ==     p_buffer ) goto no_buffer;
    if ( NULL == p_public_key ) goto no_public_key;

    // initialized data
    char          *p   = p_buffer;
    unsigned char *p_p = (unsigned char *) p_public_key;

    // pack the public key
    p += pack_pack(
        p, "%32i8",

        p_p[0] , p_p[1] , p_p[2] , p_p[3],
        p_p[4] , p_p[5] , p_p[6] , p_p[7],
        p_p[8] , p_p[9] , p_p[10], p_p[11],
        p_p[12], p_p[13], p_p[14], p_p[15],
        p_p[16], p_p[17], p_p[18], p_p[19],
        p_p[20], p_p[21], p_p[22], p_p[23],
        p_p[24], p_p[25], p_p[26], p_p[27],
        p_p[28], p_p[29], p_p[30], p_p[31]
    );

    // success
    return p - (char *)p_buffer;    

    // error handling
    {

        // argument errors
        {
            no_buffer:
                #ifndef NDEBUG
                    log_error("[x25519] Null pointer provided for parameter \"p_buffer\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;

            no_public_key:
                #ifndef NDEBUG
                    log_error("[x25519] Null pointer provided for parameter \"p_public_key\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
    }
}

int x25519_public_key_unpack ( x25519_public_key *p_public_key, void *p_buffer )
{

    // argument check
    if ( NULL == p_public_key ) goto no_public_key;
    if ( NULL ==     p_buffer ) goto no_buffer;

    // initialized data
    char          *p   = p_buffer;
    unsigned char *p_p = (unsigned char *) p_public_key;

    // unpack the public key
    p += pack_unpack(
        p, "%32i8",
        &p_p[0] , &p_p[1] , &p_p[2] , &p_p[3],
        &p_p[4] , &p_p[5] , &p_p[6] , &p_p[7],
        &p_p[8] , &p_p[9] , &p_p[10], &p_p[11],
        &p_p[12], &p_p[13], &p_p[14], &p_p[15],
        &p_p[16], &p_p[17], &p_p[18], &p_p[19],
        &p_p[20], &p_p[21], &p_p[22], &p_p[23],
        &p_p[24], &p_p[25], &p_p[26], &p_p[27],
        &p_p[28], &p_p[29], &p_p[30], &p_p[31]
    );

    // success
    return p - (char *)p_buffer;

    // error handling
    {

        // argument errors
        {
            no_public_key:
                #ifndef NDEBUG
                    log_error("[x25519] Null pointer provided for parameter \"p_public_key\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;

            no_buffer:
                #ifndef NDEBUG
                    log_error("[x25519] Null pointer provided for parameter \"p_buffer\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
    }
}

int x25519_private_key_pack ( void *p_buffer, x25519_private_key *p_private_key )
{

    // argument check
    if ( NULL ==      p_buffer ) goto no_buffer;
    if ( NULL == p_private_key ) goto no_private_key;

    // initialized data
    char          *p   = p_buffer;
    unsigned char *p_p = (unsigned char *) p_private_key;

    // pack the private key
    p += pack_pack(
        p, "%32i8",

        p_p[0] , p_p[1] , p_p[2] , p_p[3],
        p_p[4] , p_p[5] , p_p[6] , p_p[7],
        p_p[8] , p_p[9] , p_p[10], p_p[11],
        p_p[12], p_p[13], p_p[14], p_p[15],
        p_p[16], p_p[17], p_p[18], p_p[19],
        p_p[20], p_p[21], p_p[22], p_p[23],
        p_p[24], p_p[25], p_p[26], p_p[27],
        p_p[28], p_p[29], p_p[30], p_p[31]
    );

    // success
    return p - (char *)p_buffer;

    // error handling
    {

        // argument errors
        {
            no_buffer:
                #ifndef NDEBUG
                    log_error("[x25519] Null pointer provided for parameter \"p_buffer\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;

            no_private_key:
                #ifndef NDEBUG
                    log_error("[x25519] Null pointer provided for parameter \"p_private_key\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
    }
}

int x25519_private_key_unpack ( x25519_private_key *p_private_key, void *p_buffer )
{

    // argument check
    if ( NULL == p_private_key ) goto no_private_key;
    if ( NULL ==      p_buffer ) goto no_buffer;

    // initialized data
    char          *p   = p_buffer;
    unsigned char *p_p = (unsigned char *) p_private_key;

    // unpack the private key
    p += pack_unpack(
        p, "%32i8",
        &p_p[0] , &p_p[1] , &p_p[2] , &p_p[3],
        &p_p[4] , &p_p[5] , &p_p[6] , &p_p[7],
        &p_p[8] , &p_p[9] , &p_p[10], &p_p[11],
        &p_p[12], &p_p[13], &p_p[14], &p_p[15],
        &p_p[16], &p_p[17], &p_p[18], &p_p[19],
        &p_p[20], &p_p[21], &p_p[22], &p_p[23],
        &p_p[24], &p_p[25], &p_p[26], &p_p[27],
        &p_p[28], &p_p[29], &p_p[30], &p_p[31]
    );

    // success
    return p - (char *)p_buffer;

    // error handling
    {

        // argument errors
        {
            no_private_key:
                #ifndef NDEBUG
                    log_error("[x25519] Null pointer provided for parameter \"p_private_key\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;

            no_buffer:
                #ifndef NDEBUG
                    log_error("[x25519] Null pointer provided for parameter \"p_buffer\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
    }
}

int x25519_key_pair_pack ( void *p_buffer, x25519_public_key *p_public_key, x25519_private_key *p_private_key )
{

    // argument check
    if ( NULL ==     p_buffer ) goto no_buffer;
    if ( NULL ==  p_public_key ) goto no_public_key;
    if ( NULL == p_private_key ) goto no_private_key;

    // initialized data
    char *p = p_buffer;

    // pack the public key
    p += x25519_public_key_pack(p, p_public_key),

    // pack the private key
    p += x25519_private_key_pack(p, p_private_key);

    // success
    return p - (char *)p_buffer;

    // error handling
    {

        // argument errors
        {
            no_buffer:
                #ifndef NDEBUG
                    log_error("[x25519] Null pointer provided for parameter \"p_buffer\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;

            no_public_key:
                #ifndef NDEBUG
                    log_error("[x25519] Null pointer provided for parameter \"p_public_key\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;

            no_private_key:
                #ifndef NDEBUG
                    log_error("[x25519] Null pointer provided for parameter \"p_private_key\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
    }
}

int x25519_key_pair_unpack ( x25519_public_key *p_public_key, x25519_private_key *p_private_key, void *p_buffer )
{

    // argument check
    if ( NULL ==  p_public_key ) goto no_public_key;
    if ( NULL == p_private_key ) goto no_private_key;
    if ( NULL ==      p_buffer ) goto no_buffer;

    // initialized data
    char *p = p_buffer;

    // pack the public key
    p += x25519_public_key_unpack(p_public_key, p),

    // pack the private key
    p += x25519_private_key_unpack(p_private_key, p);

    // success
    return p - (char *)p_buffer;

    // error handling
    {

        // argument errors
        {
            no_public_key:
                #ifndef NDEBUG
                    log_error("[x25519] Null pointer provided for parameter \"p_public_key\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;

            no_private_key:
                #ifndef NDEBUG
                    log_error("[x25519] Null pointer provided for parameter \"p_private_key\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;

            no_buffer:
                #ifndef NDEBUG
                    log_error("[x25519] Null pointer provided for parameter \"p_buffer\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
    }
}

int x25519_shared_secret_pack ( void *p_buffer, x25519_shared_secret *p_shared_secret )
{

    // argument check
    if ( NULL ==        p_buffer ) goto no_buffer;
    if ( NULL == p_shared_secret ) goto no_shared_secret;

    // initialized data
    char          *p   = p_buffer;
    unsigned char *p_p = (unsigned char *) p_shared_secret;

    // pack the shared secret
    p += pack_pack(
        p, "%32i8",

        p_p[0] , p_p[1] , p_p[2] , p_p[3],
        p_p[4] , p_p[5] , p_p[6] , p_p[7],
        p_p[8] , p_p[9] , p_p[10], p_p[11],
        p_p[12], p_p[13], p_p[14], p_p[15],
        p_p[16], p_p[17], p_p[18], p_p[19],
        p_p[20], p_p[21], p_p[22], p_p[23],
        p_p[24], p_p[25], p_p[26], p_p[27],
        p_p[28], p_p[29], p_p[30], p_p[31]
    );

    // success
    return p - (char *)p_buffer;

    // error handling
    {

        // argument errors
        {
            no_buffer:
                #ifndef NDEBUG
                    log_error("[x25519] Null pointer provided for parameter \"p_buffer\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;

            no_shared_secret:
                #ifndef NDEBUG
                    log_error("[x25519] Null pointer provided for parameter \"p_shared_secret\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
    }
}

int x25519_shared_secret_unpack ( x25519_shared_secret *p_shared_secret, void *p_buffer )
{

    // argument check
    if ( NULL == p_shared_secret ) goto no_shared_secret;
    if ( NULL ==        p_buffer ) goto no_buffer;

    // initialized data
    char          *p   = p_buffer;
    unsigned char *p_p = (unsigned char *) p_shared_secret;

    // unpack the shared secret
    p += pack_unpack(
        p, "%32i8",
        &p_p[0] , &p_p[1] , &p_p[2] , &p_p[3],
        &p_p[4] , &p_p[5] , &p_p[6] , &p_p[7],
        &p_p[8] , &p_p[9] , &p_p[10], &p_p[11],
        &p_p[12], &p_p[13], &p_p[14], &p_p[15],
        &p_p[16], &p_p[17], &p_p[18], &p_p[19],
        &p_p[20], &p_p[21], &p_p[22], &p_p[23],
        &p_p[24], &p_p[25], &p_p[26], &p_p[27],
        &p_p[28], &p_p[29], &p_p[30], &p_p[31]
    );

    // success
    return p - (char *)p_buffer;

    // error handling
    {

        // argument errors
        {
            no_shared_secret:
                #ifndef NDEBUG
                    log_error("[x25519] Null pointer provided for parameter \"p_shared_secret\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;

            no_buffer:
                #ifndef NDEBUG
                    log_error("[x25519] Null pointer provided for parameter \"p_buffer\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
    }
}
