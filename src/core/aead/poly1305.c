/** ! 
 * Poly1305 implementation
 * 
 * @file src/core/chacha20/poly1305.c
 * 
 * @author Jacob Smith
 */

// header file
#include <core/poly1305.h>

// type definitions
typedef unsigned _BitInt(256) u256;

// function declarations
/** !
 * Clamp a poly1305 key  
 * 
 * @param key the key
 * 
 * @return 1 on success, 0 on error
 */
int poly1305_clamp ( poly1305_one_time_key key );

void u256_print ( u256 x )
{

    // static data
    static const char hex[] = "0123456789abcdef";

    // initialized data
    int started = 0;

    // print characters
    for (int i = 63; i >= 0; --i)
    {

        // initialized data
        unsigned digit = (unsigned)((x >> (i * 4)) & 0xF);

        // print the character
        if ( digit != 0 || started || i == 0 )
            putchar(hex[digit]),
            started = 1;
    }

    // formatting
    putchar('\n');

    // done
    return;
}

void poly1305_block_print ( void *p_block, size_t len )
{

    // initialized data
    unsigned char *p_m = (unsigned char *)p_block;

    // print each byte
    for (signed i = (signed)len - 1; i > -1; i--)
        printf("%02x", p_m[i]);

    // formatting
    putchar('\n');
    
    // done
    return;
}

// function definitions
int poly1305_clamp ( poly1305_one_time_key key )
{

    // clamp
    key[3]  &= 0b00001111,
    key[7]  &= 0b00001111,
    key[11] &= 0b00001111,
    key[15] &= 0b00001111,
    key[4]  &= 0b11111100,
    key[8]  &= 0b11111100,
    key[12] &= 0b11111100;

    // success
    return 1;
}

int poly1305_mac 
(
    const unsigned char   *p_message, 
    size_t                 len, 
    poly1305_tag           tag, 
    poly1305_one_time_key  key
)
{

    // static data
    #ifdef POLY1305_DEBUG
    static int block_no = 1;
    #endif

    // initialized data
    size_t off = 0;
    u256   r = 0,
           s = 0,
           a = 0;
    #ifdef POLY1305_DEBUG
        u256 s_temp = 0, r_temp = 0;
    #endif

    #ifdef POLY1305_DEBUG

        // reset the block number
        block_no = 1;

        // print s
        printf("s as a 128-bit number: ");
        for ( int i = 0; i < 16; i++ ) 
            s_temp |= (u256) key[i + 16] << (8 * i);
        u256_print(s_temp);

        // print r before clamping
        printf("r before clamping: ");
        for (int i = 0; i < 16; i++) 
            printf("%02x%c", key[i], (i == 15) ? '\n' : ':');
    #endif

    // clamp the key
    poly1305_clamp(key);

    #ifdef POLY1305_DEBUG

        // print r after clamp
        printf("Clamped r as a number: ");
        for ( int i = 0; i < 16; i++ ) 
            r_temp |= (u256) key [i] << (8 * i);
        u256_print(r_temp);
    #endif

    // compute r
    for ( int i = 0; i < 16; i++ ) 
        r |= (u256) key [i + 0] << (8 * i);

    // compute s
    for ( int i = 0; i < 16; i++ ) 
        s |= (u256) key [i + 16] << (8 * i);

    // compute the message authentication code
    while ( off < len )
    {

        // initialized data
        u256 n = 0;
        size_t b = ( len - off ) < 16 ? ( len - off ) : 16;
       
        #ifdef POLY1305_DEBUG

            // print the block number
            printf("\nBlock #%d\n", block_no);
            
            // print the accumulator
            printf("Acc = "), u256_print(a);

            // print the content of the block
            printf("Block = "), poly1305_block_print((void *)(p_message + off), b);
        #endif
        
        // compute n
        for ( size_t i = 0; i < b; i++)
            n |= (u256) p_message[off + i] << (8 * i);
        n |= (u256)1 << (8 * b);
            
        #ifdef POLY1305_DEBUG

            // print the block with the 1 byte
            printf("Block with 0x01 byte = "), u256_print(n);

            // print the sum of the accumulator and block
            printf("Acc + block = "), u256_print(a + n);
        #endif
        
        // update the accumulator
        a = (a + n) * r;

        #ifdef POLY1305_DEBUG

            // print accumulator + block * r
            printf("(Acc + Block) * r =\n"), u256_print(a);
        #endif

        // modulo 2^130-5
        a = a % (((u256)1 << 130) - 5);

        #ifdef POLY1305_DEBUG

            // print the accumulator 
            printf("Acc = ((Acc + Block) * r) %% P = "), u256_print(a);
            block_no++;
        #endif
        
        // step
        off += b;
    }

    a += s;

    #ifdef POLY1305_DEBUG
        printf("\nAcc + s = ");
        u256_print(a);
    #endif

    for ( int i = 0; i < 16; i++ )
        tag[i] = (unsigned char)((a >> (8 * i)) & 0xff);

    #ifdef POLY1305_DEBUG
        printf("Tag: ");
        for (int i = 0; i < 16; i++) printf("%02x%c", tag[i], (i == 15) ? '\n' : ':');
    #endif

    // success
    return 1;
}

int poly1305_key_generate
(
    poly1305_one_time_key *p_key, 
    chacha20_key           key, 
    chacha20_nonce         nonce,
    int                    block
)
{

    // argument check
    if ( NULL == p_key ) goto no_key;

    // initialized data
    chacha20 *p_chacha20 = NULL; 
    chacha20_state _s = { 0 };
    
    // construct
    chacha20_construct(&p_chacha20, key, nonce, block);

    // setup
    chacha20_setup(p_chacha20);

    // build the key
    chacha20_round(p_chacha20);

    // store the internal state
    chacha20_state_get(&_s, p_chacha20);

    // return the one time key to the caller
    memcpy(p_key, _s, sizeof(poly1305_one_time_key));

    // success
    return 1;

    // error handling
    {

        // argument errors
        {
            no_key:
                #ifndef NDEBUG
                    log_error("[poly1305] Null pointer provided for parameter \"p_key\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
    }
}
