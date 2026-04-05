/** ! 
 * Ed25519 implementation
 * 
 * @file src/core/ed25519/ed25519.c
 * 
 * @author Jacob Smith
 */

// header file
#include <core/ed25519.h>

// preprocessor definitions
#define RED "\033[91m"
#define BLUE "\033[94m"
#define RESET "\033[0m"

// type definitions
typedef unsigned _BitInt(256) i256;
typedef unsigned _BitInt(512) i512;

// constant data
static const i256 Q  = 0x7FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFEDWB;
static const i256 D  = 0x52036CEE2B6FFE738CC740797779E89800700A4D4141D8AB75EB4DCA135978A3WB;
static const i256 I  = 0x2B8324804FC1DF0B2B4D00993DFBD7A72F431806AD2FE478C4EE1B274A0EA0B0WB;
static const i256 Bx = 0x216936D3CD6E53FEC0A4E231FDD6DC5C692CC7609525A7B2C9562D608F25D51AWB;
static const i256 By = 0x6666666666666666666666666666666666666666666666666666666666666658WB;
static const i256 L  = 0x1000000000000000000000000000000014DEF9DEA2F79CD65812631A5CF5D3EDWB;

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
 * Recover the x-coordinate on a twisted Edwards curve given a y-coordinate
 * 
 * @param y the y-coordinate
 * 
 * @return the x-coordinate
 */
i256 xrecover ( i256 y );

/** !
 * Add two points on a twisted Edwards curve
 * 
 * @param P  the first point
 * @param _Q the second point
 * @param R  the result
 * 
 * @return void
 */
void edwards ( i256 P[2], i256 _Q[2], i256 R[2] );

/** !
 * Scalar multiplication of a point on a twisted Edwards curve
 * 
 * @param P the point
 * @param e the scalar
 * @param R the result
 * 
 * @return void
 */
void scalarmult ( i256 P[2], i256 e, i256 R[2] );

/** !
 * Get the n'th bit of some data
 * 
 * @param h the data
 * @param i the n'th bit
 * 
 * @return 1 IF set, 0 IF clear
 */
int bit ( const unsigned char *h, int i );

/** !
 * Store a 256-bit integer in a buffer
 * 
 * @param y   the integer
 * @param out the buffer
 * 
 * @return void
 */
void encodeint ( i256 y, unsigned char *out );

/** !
 * Store a point on a twisted edwards curve in a buffer
 * 
 * @param P   the point
 * @param out the buffer
 * 
 * @return void
 */
void encodepoint ( i256 P[2], unsigned char *out );

/** !
 * Load a 256-bit integer from a buffer
 * 
 * @param s the buffer
 * 
 * @return the 256-bit integer
 */
i256 decodeint ( const unsigned char *s );

/** !
 * Load a point on a twisted edwards curve from a buffer
 * 
 * @param s the buffer
 * @param P the result
 * 
 * @return 1 on success, 0 on error
 */
int decodepoint ( const unsigned char *s, i256 P[2] );

/** !
 * Hash a message with SHA512
 * 
 * @param m   the message
 * @param len the length of the message
 * @param out the result
 * 
 * @return void
 */
void H ( const unsigned char *m, size_t len, unsigned char *out );

/** !
 * Compute a cryptographic scalar for a message
 * 
 * @param m the message
 * @param len the length of the message
 * 
 * @return the "Hint"
 */
i256 Hint ( const unsigned char *m, size_t len );

/** !
 * Derive the public key from the private key
 * 
 * @param sk the private key
 * @param pk the public key
 * 
 * @return void
 */
void public_key_derive ( const unsigned char *sk, unsigned char *pk );

// function definitions
i256 add_mod ( i256 a, i256 b, i256 m ) { return (a + b) % m; }
i256 sub_mod ( i256 a, i256 b, i256 m ) { return (a - b + m) % m; }
i256 mul_mod ( i256 a, i256 b, i256 m ) { return ((i512)a * b) % m; }
i256 sqr_mod ( i256 a, i256 m )         { return mul_mod(a, a, m); }
i256 inv     ( i256 x )                 { return expmod(x, Q - 2, Q); }

i256 expmod ( i256 b, i256 e, i256 m )
{

    // initialized data
    i256 t = 1;

    // process the exponent
    while (e > 0)
    {

        // odd exponent 
        if (e % 2 == 1) t = mul_mod(t, b, m);

        // square
        b = sqr_mod(b, m);

        // shift
        e /= 2;
    }
    
    // done
    return t;
}

i256 xrecover ( i256 y )
{

    // initialized data
    i256 y2 = 0;
    i256 num = 0, den = 0;
    i256 xx = 0, x = 0, x2 = 0;

    // y2 = (y^2) mod Q
    y2  = sqr_mod(y, Q);

    // num = (y^2 - 1) mod Q
    num = sub_mod(y2, 1, Q);

    // den = (d * y^2 + 1) mod Q
    den = add_mod(
        mul_mod(D, y2, Q),
        1,
        Q
    );
    
    // xx = (num / den) mod Q
    xx = mul_mod(
        num, 
        inv(den),
        Q
    );
    
    // NOTE: This is a fun party trick from section 5.1.3 of the RFC
    // x = sqrt(xx)
    x = expmod(xx, (Q + 3) / 8, Q);
    
    // correct x
    x2 = sqr_mod(x, Q);
    if (sub_mod(x2, xx, Q) != 0) 
        x = mul_mod(x, I, Q);
    
    // force x to be even
    if (x % 2 != 0) 
        x = Q - x;

    // done
    return x;
}

void edwards ( i256 P[2], i256 _Q[2], i256 R[2] )
{

    // initialized data
    i256 x1 =  P[0], y1 =  P[1];
    i256 x2 = _Q[0], y2 = _Q[1];
    i256 x1y2 = 0, x2y1  = 0,
         y1y2 = 0, x1x2  = 0;
    i256 x_num = 0, x_den = 0;
    i256 y_num = 0, y_den = 0;
    i256 temp  = 0;

    // compute intermediate products
    x1y2 = mul_mod(x1, y2, Q),
    x2y1 = mul_mod(x2, y1, Q),
    y1y2 = mul_mod(y1, y2, Q),
    x1x2 = mul_mod(x1, x2, Q);

    // compute the denominator term
    temp = mul_mod(
        D, 
        mul_mod(
            x1x2, 
            y1y2, 
            Q
        ), 
        Q
    );

    // compute x
    x_num = add_mod(x1y2, x2y1, Q),
    x_den = add_mod(1, temp, Q);

    // compute y
    y_num = add_mod(y1y2, x1x2, Q),
    y_den = sub_mod(1, temp, Q);
    
    // store the results
    R[0] = mul_mod(
        x_num, 
        inv(x_den), 
        Q
    ),

    R[1] = mul_mod(
        y_num, 
        inv(y_den), 
        Q
    );

    // done
    return;
}

void scalarmult ( i256 P[2], i256 e, i256 R[2] )
{

    // initialized data
    i256 _Q[2] = { P[0], P[1] };

    // store initial point
    R[0] = 0; R[1] = 1;
    
    while (e > 0)
    {

        // initialized data
        i256 T[2] = { 0, 0 };

        // odd
        if (e % 2 == 1)
        {
            
            // initialized data
            i256 T[2] = { 0, 0 };

            // add points
            edwards(R, _Q, T);

            // store result
            R[0] = T[0], 
            R[1] = T[1];
        }

        // add points
        edwards(_Q, _Q, T);

        // store result
        _Q[0] = T[0],
        _Q[1] = T[1];

        // iterate
        e /= 2;
    }
}

int bit ( const unsigned char *h, int i )
{

    // done
    return (h[i / 8] >> (i % 8)) & 1;
}

void encodeint ( i256 y, unsigned char *out )
{

    // serialize the integer
    for (int i = 0; i < 32; i++)
        out[i] = (unsigned char)(y >> (i * 8));
    
    // done
    return;
}

void encodepoint ( i256 P[2], unsigned char *out )
{

    // initialized data
    i256 x = P[0], y = P[1];

    // encode y
    encodeint(y, out);

    // encode the parity of x
    if ((x & 1) == 1) out[31] |= 0x80;
}

i256 decodeint ( const unsigned char *s )
{

    // initialized data
    i256 sum = 0;

    // reconstruct the integer
    for (int i = 0; i < 32; i++) 
        sum |= ((i256)s[i]) << (i * 8);
    
    // done 
    return sum;
}

int decodepoint ( const unsigned char *s, i256 P[2] )
{

    // initialized data
    i256 x     = 0, y     = decodeint(s),
         x2    = 0, y2    = 0;
    i256 term1 = 0, term2 = 0, 
         term3 = 0, term4 = 0;
    i256 lhs   = 0;

    // clear the sign of y
    y &= ~(((i256)1) << 255); 
    
    // recover x from y
    x = xrecover(y);

    // correct x
    if ((x & 1) != ((s[31] >> 7) & 1)) x = Q - x;
    
    // x2 = x^2 mod Q
    x2 = sqr_mod(x, Q), 
    
    // y2 = y^2 mod Q
    y2 = sqr_mod(y, Q);
    
    // term1 = -x^2 mod Q
    term1 = sub_mod(0, x2, Q),

    // term2 = y2
    term2 = y2,

    // term3 = -1 mod Q
    term3 = sub_mod(0, 1, Q),

    // term4 = -d * ( x^2 + y^2 ) mod Q
    term4 = sub_mod( 
        0, 
        mul_mod(
            D, 
            mul_mod(
                x2,
                y2,
                Q
            ),
            Q
        ),
        Q
    ); 
    
    // NOTE: 
    //       -x^2 + y^2                 = 1 + d*x^2*y^2
    //       -x^2 + y^2 - 1 - d*x^2*y^2 = 0
    // lhs = -x^2 + y^2 - 1 - d*x^2*y^2
    lhs = add_mod(
        add_mod(
            term1,
            term2,
            Q
        ), 
        add_mod(
            term3,
            term4,
            Q
        ),
        Q
    );
    
    // error check
    if ( lhs != 0 ) return 0; 
    
    // store the result
    P[0] = x, P[1] = y;

    // success
    return 1;
}

void H ( const unsigned char *m, size_t len, unsigned char *out )
{

    // initialized data
    sha512_state state = { 0 };

    // construct a sha hasher
    sha512_construct(&state),

    // feed the hasher
    sha512_update(&state, m, len),

    // wrap up
    sha512_final(&state, out);

    // done
    return;
}

// Hint(m)
i256 Hint ( const unsigned char *m, size_t len )
{

    // initialized data
    unsigned char h[64] = { 0 };
    i512 sum = 0;
    i256 res = 0;

    // hash the message
    H(m, len, h);
    
    // hash -> 512-bit integer
    for (int i = 0; i < 64; i++) 
        sum |= ((i512)h[i]) << (i*8);
    
    // reduce the integer
    res = sum % L;

    // done
    return res;
}

void public_key_derive ( const unsigned char *sk, unsigned char *pk )
{

    // initialized data
    i256          A[2]  = { 0, 0 },
                  B[2]  = { Bx, By };
    i256          a     = 0;
    unsigned char h[64] = { 0 };

    // RFC 8032 Section 5.1.5 > 1
    H(sk, 32, h);
    
    // RFC 8032 Section 5.1.5 > 3
    a = decodeint(h);

    // RFC 8032 Section 5.1.5 > 2
    a &= ~(((i256)0b111)); 
    a &= ~(((i256)1) << 255); 
    a |= ((i256)1) << 254; 
    
    // RFC 8032 > Section 5.1.5 > 4
    scalarmult(B, a, A);

    // store the public key
    encodepoint(A, pk);

    // done
    return;
}

int ed25519_key_pair_construct
(
    ed25519_public_key  *p_public_key, 
    ed25519_private_key *p_private_key
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
    if ( read != 32 ) goto failed_to_read_urandom;

    // close urandom
    fclose(urandom);
    
    // derive a public key from the private key
    public_key_derive(
        (const unsigned char *)p_private_key,
        (unsigned char *)p_public_key
    );

    // success
    return 1;

    // error handling
    {

        // argument errors
        {
            no_public_key:
                #ifndef NDEBUG
                    log_error("[ed25519] Null pointer provided for parameter \"p_public_key\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;

            no_private_key:
                #ifndef NDEBUG
                    log_error("[ed25519] Null pointer provided for parameter \"p_private_key\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
    
        // standard library errors
        {
            failed_to_open_urandom:
                #ifndef NDEBUG
                    log_error("[ed25519] Failed to open \"/dev/urandom\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
            
            failed_to_read_urandom:
                #ifndef NDEBUG
                    log_error("[ed25519] Failed to read \"/dev/urandom\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
    }
}

int ed25519_sign
( 
    ed25519_signature         *p_signature, 
    const unsigned char       *p_message, 
    size_t                     message_len, 
    const ed25519_public_key  *p_public_key, 
    const ed25519_private_key *p_private_key
)
{
    
    // argument check
    if ( NULL ==   p_signature ) goto no_signature;
    if ( NULL ==     p_message ) goto no_message;
    if ( NULL ==  p_public_key ) goto no_public_key;
    if ( NULL == p_private_key ) goto no_private_key;

    // initialized data;
    unsigned char h[64] = { 0 };
    i256 a = 0;
    i256 r = 0;
    i256 B[2] = { Bx, By };
    i256 R[2] = { 0, 0 };
    i256 h_ram = 0;
    i256 S = 0;
    unsigned char R_enc[32];
    unsigned char *temp = NULL;

    // allocate memory for the signing process
    temp = default_allocator(0, 32 + message_len);
    if ( NULL == temp ) goto no_mem;

    // RFC 8032 > 5.1.6 > 1
    H((const unsigned char *)p_private_key, 32, h),

    a = decodeint(h);
    a &= ~(((i256)0b111));
    a &= ~(((i256)1) << 255);
    a |= ((i256)1) << 254;
    
    // Append the second half of the digest and the message
    memcpy(temp, h + 32, 32);
    memcpy(temp + 32, p_message, message_len);
    
    // RFC 8032 > 5.1.6 > 2
    r = Hint(temp, 32 + message_len);

    // release the transient allocation
    temp = default_allocator(temp, 0);
    
    // RFC 8032 > 5.1.6 > 3
    scalarmult(B, r, R);
    encodepoint(R, R_enc);

    // allocate memory for h_ram
    temp = default_allocator(0, 32 + 32 + message_len);
    if ( NULL == temp ) goto no_mem;
    
    // copy R
    memcpy(temp, R_enc, 32);
    memcpy(temp + 32, p_public_key, 32);
    memcpy(temp + 64, p_message, message_len);
    
    // RFC 8032 > 5.1.6 > 4
    h_ram = Hint(temp, 32 + 32 + message_len);

    // release the transient allocation
    temp = default_allocator(temp, 0);
    
    // RFC 8032 > 5.1.6 > 5
    S = (i256)( ((i512)r + ((i512)h_ram * (i512)a)) % (i512)L );
    
    // RFC 8032 > 5.1.6 > 6
    memcpy(p_signature, R_enc, 32);
    encodeint(S, ((unsigned char*)p_signature) + 32);
    
    // success
    return 1;

    // error handling
    {

        // argument errors
        {
            no_signature:
                #ifndef NDEBUG
                    log_error("[ed25519] Null pointer provided for parameter \"p_signature\" in call to function \"%s\"\n", __FUNCTION__);
                #endif
                
                // error
                return 0;

            no_message:
                #ifndef NDEBUG
                    log_error("[ed25519] Null pointer provided for parameter \"p_message\" in call to function \"%s\"\n", __FUNCTION__);
                #endif
                
                // error
                return 0;

            no_public_key:
                #ifndef NDEBUG
                    log_error("[ed25519] Null pointer provided for parameter \"p_public_key\" in call to function \"%s\"\n", __FUNCTION__);
                #endif
                
                // error
                return 0;

            no_private_key:
                #ifndef NDEBUG
                    log_error("[ed25519] Null pointer provided for parameter \"p_private_key\" in call to function \"%s\"\n", __FUNCTION__);
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

int ed25519_verify
(
    const ed25519_signature  *p_signature, 
    const unsigned char      *p_message, 
    size_t                    message_len, 
    const ed25519_public_key *p_public_key
)
{

    // argument check
    if ( NULL ==   p_signature ) goto no_signature;
    if ( NULL ==     p_message ) goto no_message;
    if ( NULL ==  p_public_key ) goto no_public_key;

    // initialized data
    i256 R[2]           = { 0, 0 };
    i256 A[2]           = { 0, 0 };
    i256 S              = 0;
    i256 h_ram          = 0;
    i256 B[2]           = { Bx, By };
    i256 SB[2]          = { 0, 0 };
    i256 hA[2]          = { 0, 0 };
    i256 RhA[2]         = { 0, 0 };
    unsigned char *temp = NULL;

    // RFC 8032 > 5.1.7 > 1
    if ( 0 ==  decodepoint((const unsigned char *)p_signature, R) ) return 0;
    if ( 0 == decodepoint((const unsigned char *)p_public_key, A) ) return 0;
    
    // decode S
    S = decodeint(((const unsigned char*)p_signature) + 32);
    if (S >= L) return 0;
    
    // allocate memory for the verification process
    temp = default_allocator(0, 32 + 32 + message_len);
    if ( NULL == temp ) goto no_mem;

    // store the signature
    memcpy(temp, p_signature, 32),
    
    // append the public key
    memcpy(temp + 32, p_public_key, 32),

    // append the message
    memcpy(temp + 64, p_message, message_len);
    
    // RFC 8032 > 5.1.7 > 2
    h_ram = Hint(temp, 32 + 32 + message_len);

    // release the transient allocation
    temp = default_allocator(temp, 0);
    
    // RFC 8032 > 5.1.7 > 3
    scalarmult(B, S, SB),
    scalarmult(A, h_ram, hA),
    edwards(R, hA, RhA);
    
    // done
    return ( SB[0] == RhA[0] && SB[1] == RhA[1] ) ? 1 : 0;

    // error handling
    {

        // argument errors
        {
            no_signature:
                #ifndef NDEBUG
                    log_error("[ed25519] Null pointer provided for parameter \"p_signature\" in call to function \"%s\"\n", __FUNCTION__);
                #endif
                
                // error
                return 0;

            no_message:
                #ifndef NDEBUG
                    log_error("[ed25519] Null pointer provided for parameter \"p_message\" in call to function \"%s\"\n", __FUNCTION__);
                #endif
                
                // error
                return 0;

            no_public_key:
                #ifndef NDEBUG
                    log_error("[ed25519] Null pointer provided for parameter \"p_public_key\" in call to function \"%s\"\n", __FUNCTION__);
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

int ed25519_public_key_print ( ed25519_public_key *p_public_key )
{

    // argument check
    if ( NULL == p_public_key ) goto no_public_key;
    
    // print each byte of the public key
    for (size_t i = 0; i < sizeof(ed25519_public_key); i++)
        printf("%02hhx", ((unsigned char*)p_public_key)[i]);
    
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
                    log_error("[ed25519] Null pointer provided for parameter \"p_public_key\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
    }
}

int ed25519_private_key_print ( ed25519_private_key *p_private_key )
{

    // argument check
    if ( NULL == p_private_key ) goto no_private_key;

    // print each byte of the private key
    for (size_t i = 0; i < sizeof(ed25519_private_key); i++)
        printf("%02hhx", ((unsigned char*)p_private_key)[i]);
    
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
                    log_error("[ed25519] Null pointer provided for parameter \"p_private_key\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
    }
}

int ed25519_signature_print ( ed25519_signature *p_signature )
{

    // argument check
    if ( NULL == p_signature ) goto no_signature;

    // print each byte of the signature
    for (size_t i = 0; i < sizeof(ed25519_signature); i++)
        printf("%02hhx", ((unsigned char*)p_signature)[i]);
    
    // formatting
    putchar('\n');

    // success
    return 1;

    // error handling
    {

        // argument errors
        {
            no_signature:
                #ifndef NDEBUG
                    log_error("[ed25519] Null pointer provided for parameter \"p_signature\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
    }
}

int ed25519_public_key_pack ( void *p_buffer, ed25519_public_key *p_public_key )
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
                    log_error("[ed25519] Null pointer provided for parameter \"p_buffer\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;

            no_public_key:
                #ifndef NDEBUG
                    log_error("[ed25519] Null pointer provided for parameter \"p_public_key\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
    }
}

int ed25519_public_key_unpack ( ed25519_public_key *p_public_key, void *p_buffer )
{

    // argument check
    if ( NULL ==     p_buffer ) goto no_buffer;
    if ( NULL == p_public_key ) goto no_public_key;

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
                log_error("[ed25519] Null pointer provided for parameter \"p_public_key\" in call to function \"%s\"\n", __FUNCTION__);
                #endif
                
                // error
                return 0;

            no_buffer:
                #ifndef NDEBUG
                    log_error("[ed25519] Null pointer provided for parameter \"p_buffer\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
    }
}

int ed25519_private_key_pack ( void *p_buffer, ed25519_private_key *p_private_key )
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
                    log_error("[ed25519] Null pointer provided for parameter \"p_buffer\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;

            no_private_key:
                #ifndef NDEBUG
                    log_error("[ed25519] Null pointer provided for parameter \"p_private_key\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
    }
}

int ed25519_private_key_unpack ( ed25519_private_key *p_private_key, void *p_buffer )
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
                log_error("[ed25519] Null pointer provided for parameter \"p_private_key\" in call to function \"%s\"\n", __FUNCTION__);
                #endif
                
                // error
                return 0;

            no_buffer:
                #ifndef NDEBUG
                    log_error("[ed25519] Null pointer provided for parameter \"p_buffer\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
    }
}

int ed25519_key_pair_pack ( void *p_buffer, ed25519_public_key *p_public_key, ed25519_private_key *p_private_key )
{

    // argument check
    if ( NULL ==      p_buffer ) goto no_buffer;
    if ( NULL ==  p_public_key ) goto no_public_key;
    if ( NULL == p_private_key ) goto no_private_key;

    // initialized data
    char *p = p_buffer;

    // pack the public key
    p += ed25519_public_key_pack(p, p_public_key),

    // pack the private key
    p += ed25519_private_key_pack(p, p_private_key);

    // success
    return p - (char *)p_buffer;

    // error handling
    {

        // argument errors
        {
            no_buffer:
                #ifndef NDEBUG
                    log_error("[ed25519] Null pointer provided for parameter \"p_buffer\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;

            no_public_key:
                #ifndef NDEBUG
                log_error("[ed25519] Null pointer provided for parameter \"p_public_key\" in call to function \"%s\"\n", __FUNCTION__);
                #endif
                
                // error
                return 0;
                
            no_private_key:
                #ifndef NDEBUG
                    log_error("[ed25519] Null pointer provided for parameter \"p_private_key\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
    }
}

int ed25519_key_pair_unpack ( ed25519_public_key *p_public_key, ed25519_private_key *p_private_key, void *p_buffer )
{

    // argument check
    if ( NULL ==  p_public_key ) goto no_public_key;
    if ( NULL == p_private_key ) goto no_private_key;
    if ( NULL ==      p_buffer ) goto no_buffer;

    // initialized data
    char *p = p_buffer;

    // pack the public key
    p += ed25519_public_key_unpack(p_public_key, p),

    // pack the private key
    p += ed25519_private_key_unpack(p_private_key, p);

    // success
    return p - (char *)p_buffer;

    // error handling
    {

        // argument errors
        {
            no_public_key:
                #ifndef NDEBUG
                log_error("[ed25519] Null pointer provided for parameter \"p_public_key\" in call to function \"%s\"\n", __FUNCTION__);
                #endif
                
                // error
                return 0;
                
            no_private_key:
                #ifndef NDEBUG
                    log_error("[ed25519] Null pointer provided for parameter \"p_private_key\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;

            no_buffer:
                #ifndef NDEBUG
                    log_error("[ed25519] Null pointer provided for parameter \"p_buffer\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
    }
}

int ed25519_signature_pack ( void *p_buffer, ed25519_signature *p_signature )
{

    // argument check
    if ( NULL ==    p_buffer ) goto no_buffer;
    if ( NULL == p_signature ) goto no_signature;

    // initialized data
    char          *p   = p_buffer;
    unsigned char *p_s = (unsigned char *) p_signature;

    // pack the signature
    p += pack_pack(
        p, "%64i8",

        p_s[0] , p_s[1] , p_s[2] , p_s[3],
        p_s[4] , p_s[5] , p_s[6] , p_s[7],
        p_s[8] , p_s[9] , p_s[10], p_s[11],
        p_s[12], p_s[13], p_s[14], p_s[15],
        p_s[16], p_s[17], p_s[18], p_s[19],
        p_s[20], p_s[21], p_s[22], p_s[23],
        p_s[24], p_s[25], p_s[26], p_s[27],
        p_s[28], p_s[29], p_s[30], p_s[31],

        p_s[32], p_s[33], p_s[34], p_s[35], 
        p_s[36], p_s[37], p_s[38], p_s[39], 
        p_s[40], p_s[41], p_s[42], p_s[43], 
        p_s[44], p_s[45], p_s[46], p_s[47], 
        p_s[48], p_s[49], p_s[50], p_s[51], 
        p_s[52], p_s[53], p_s[54], p_s[55], 
        p_s[56], p_s[57], p_s[58], p_s[59], 
        p_s[60], p_s[61], p_s[62], p_s[63]
    );

    // success
    return p - (char *)p_buffer;

    // error handling
    {

        // argument errors
        {
            no_buffer:
                #ifndef NDEBUG
                    log_error("[ed25519] Null pointer provided for parameter \"p_buffer\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;

            no_signature:
                #ifndef NDEBUG
                    log_error("[ed25519] Null pointer provided for parameter \"p_signature\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
    }
}

int ed25519_signature_unpack ( ed25519_signature *p_signature, void *p_buffer )
{
    
    // argument check
    if ( NULL == p_signature ) goto no_signature;
    if ( NULL ==    p_buffer ) goto no_buffer;

    // initialized data
    char          *p   = p_buffer;
    unsigned char *p_s = (unsigned char *) p_signature;

    // unpack the signature
    p += pack_unpack(
        p, "%64i8",
        &p_s[0] , &p_s[1] , &p_s[2] , &p_s[3],
        &p_s[4] , &p_s[5] , &p_s[6] , &p_s[7],
        &p_s[8] , &p_s[9] , &p_s[10], &p_s[11],
        &p_s[12], &p_s[13], &p_s[14], &p_s[15],
        &p_s[16], &p_s[17], &p_s[18], &p_s[19],
        &p_s[20], &p_s[21], &p_s[22], &p_s[23],
        &p_s[24], &p_s[25], &p_s[26], &p_s[27],
        &p_s[28], &p_s[29], &p_s[30], &p_s[31],

        &p_s[32], &p_s[33], &p_s[34], &p_s[35], 
        &p_s[36], &p_s[37], &p_s[38], &p_s[39], 
        &p_s[40], &p_s[41], &p_s[42], &p_s[43], 
        &p_s[44], &p_s[45], &p_s[46], &p_s[47], 
        &p_s[48], &p_s[49], &p_s[50], &p_s[51], 
        &p_s[52], &p_s[53], &p_s[54], &p_s[55], 
        &p_s[56], &p_s[57], &p_s[58], &p_s[59], 
        &p_s[60], &p_s[61], &p_s[62], &p_s[63]
    );

    // success
    return p - (char *)p_buffer;

    // error handling
    {

        // argument errors
        {
            no_signature:
                #ifndef NDEBUG
                log_error("[ed25519] Null pointer provided for parameter \"p_signature\" in call to function \"%s\"\n", __FUNCTION__);
                #endif
                
                // error
                return 0;

            no_buffer:
                #ifndef NDEBUG
                    log_error("[ed25519] Null pointer provided for parameter \"p_buffer\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
    }
}
 