/** ! 
 * ChaCha20 implementation
 * 
 * @file src/core/aead/chacha20.c
 * 
 * @author Jacob Smith
 */

// header file
#include <core/chacha20.h>

// structure definitions
struct chacha20_s
{
    chacha20_state   state;
    chacha20_key     key;
    chacha20_nonce   nonce;
    int              count;
    int              block;
    
};

// type definitions
typedef unsigned _BitInt(130) u130;

// function declarations
/** !
 * ChaCha20 quarter round
 * 
 * @param a a
 * @param b b
 * @param c c
 * @param d d
 * 
 * @return void
 */
void chacha20_quarter_round
(
    unsigned int *a,
    unsigned int *b,
    unsigned int *c,
    unsigned int *d
);

// function definitions
void chacha20_block_print ( chacha20_state block )
{

    // print block row
    for (size_t i = 0; i < 4; i++)
    {
        
        // print block column
        for (size_t j = 0; j < 4; j++)
            printf("%08lx ", block[i*4+j]);
        
        // formatting
        putchar('\n');
    }

    // formatting
    putchar('\n');
    
    // done
    return;
}

void chacha20_block_byte_print ( chacha20_state block )
{

    // initialized data
    unsigned char *b = (unsigned char *)block;

    // print block row
    for (size_t i = 0; i < 4; i++)
    {
        
        // print block column
        for (size_t j = 0; j < 16; j++)
            printf("%02hhx ", b[i*16+j]);
        
        // formatting
        putchar('\n');
    }

    // formatting
    putchar('\n');
    
    // done
    return;
}

int chacha20_setup ( chacha20 *p_chacha20 )
{

    // argument check
    if ( NULL == p_chacha20 ) goto no_chacha20;

    // set up the state
    pack_pack(&p_chacha20->state, "%4i32%8i32%i32%3i32",

        // "expand 32-byte k"
        0x61707865, 0x3320646e, 0x79622d32, 0x6b206574,
        
        // the key
        p_chacha20->key[0], p_chacha20->key[1], p_chacha20->key[2], p_chacha20->key[3],
        p_chacha20->key[4], p_chacha20->key[5], p_chacha20->key[6], p_chacha20->key[7],

        // the block number
        p_chacha20->block, 
        
        // the nonce
        p_chacha20->nonce[0], p_chacha20->nonce[1], p_chacha20->nonce[2]
    );

    // debug
    #ifdef CHACHA20_DEBUG
        log_info("Setup: \n");
        chacha20_block_print(p_chacha20->state);
    #endif
    
    // success
    return 1;

    // error handling
    {

        // argument errors
        {
            no_chacha20:
                #ifndef NDEBUG
                    log_error("[chacha20] Null pointer provided for parameter \"p_chacha20\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
    }
}

int chacha20_nonce_set ( chacha20 *p_chacha20, chacha20_nonce nonce )
{

    memcpy(p_chacha20->nonce, nonce, sizeof(chacha20_nonce));

    // success
    return 1;
}

int chacha20_state_get ( chacha20_state *p_state, chacha20 *p_chacha20 )
{

    memcpy(p_state, p_chacha20->state, sizeof(chacha20_state));

    return 1;
}

int chacha20_construct
(
    chacha20       **pp_chacha20, 
    chacha20_key      key, 
    chacha20_nonce    nonce, 
    int               block
)
{

    // argument check
    if ( NULL == pp_chacha20 ) goto no_chacha20;

    // initialized data
    chacha20 *p_chacha20 = NULL;

    // allocate memory
    p_chacha20 = default_allocator(NULL, sizeof(chacha20));
    if ( NULL == p_chacha20 ) goto no_mem;

    // populate the struct
    *p_chacha20 = (chacha20)
    {
        .state = { 0 },
        .key = 
        {
            key[0], key[1], key[2], key[3],
            key[4], key[5], key[6], key[7],
        },
        .nonce = { nonce[0], nonce[1], nonce[2], },
        .block = block
    }; 

    // return a pointer to the caller
    *pp_chacha20 = p_chacha20;

    // success
    return 1;

    // error handling
    {

        // argument errors
        {
            no_chacha20:
                #ifndef NDEBUG
                    log_error("[chacha20] Null pointer provided for parameter \"pp_chacha20\" in call to function \"%s\"\n", __FUNCTION__);
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

int chacha20_process ( void *p_ciphertext, chacha20 *p_chacha20, void *p_plaintext, size_t size )
{

    // argument check
    if ( NULL == p_chacha20 ) goto no_chacha20;

    // initialized data 
    size_t  items    = size / 64;
    size_t  residual = size % 64;
    char   *p_p      = (char *) p_plaintext;
    char   *p_c      = (char *) p_ciphertext;

    // process each full block
    for (size_t i = 0; i < items; i++)
    {

        // initialized data
        char ciphertext[64] = { 0 };

        // setup
        chacha20_setup(p_chacha20);

        // build the block key
        chacha20_round(p_chacha20);

        // copy the block key
        memcpy(ciphertext, p_chacha20->state, sizeof(chacha20_state));

        // compute and store ciphertext
        for (size_t i = 0; i < sizeof(chacha20_state); i++)
            ciphertext[i] = p_p[i] ^ ciphertext[i];
        
        // debug
        #ifdef CHACHA20_DEBUG
            log_info("\nCiphertext: \n");
            chacha20_block_byte_print((unsigned int *)ciphertext);
        #endif

        memcpy(p_c, ciphertext, sizeof(chacha20_state));

        // step
        p_c += sizeof(chacha20_state),
        p_p += sizeof(chacha20_state),
        p_chacha20->block++;
    }

    // process the residual
    if ( residual )
    {

        // initialized data
        char ciphertext[64] = { 0 };

        // setup
        chacha20_setup(p_chacha20);

        // debug
        #ifdef CHACHA20_DEBUG
            log_info("Setup: \n");
            chacha20_block_print(p_chacha20->state);
        #endif

        // build the block key
        chacha20_round(p_chacha20);

        // copy the block key
        memcpy(ciphertext, p_chacha20->state, residual);

        // compute and store ciphertext
        for (size_t i = 0; i < residual; i++)
            ciphertext[i] = p_p[i] ^ ciphertext[i];
        
        // debug
        #ifdef CHACHA20_DEBUG
            log_info("\nCiphertext: \n");
            chacha20_block_byte_print((unsigned int *)ciphertext);
        #endif

        memcpy(p_c, ciphertext, residual);

        // step
        p_c += sizeof(chacha20_state),
        p_p += sizeof(chacha20_state);
    }

    // success
    return 1;

    // error handling
    {

        // argument errors
        {
            no_chacha20:
                #ifndef NDEBUG
                    log_error("[chacha20] Null pointer provided for parameter \"p_chacha20\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
    }
}

int chacha20_encrypt ( void *p_ciphertext, chacha20 *p_chacha20, void *p_plaintext, size_t size ) 
{
    
    // argument check
    if ( NULL == p_chacha20 ) goto no_chacha20;

    // done
    return chacha20_process(p_ciphertext, p_chacha20, p_plaintext, size); 

    // error handling
    {

        // argument errors
        {
            no_chacha20:
                #ifndef NDEBUG
                    log_error("[chacha20] Null pointer provided for parameter \"p_chacha20\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
    }
}

int chacha20_decrypt ( void *p_plaintext, chacha20 *p_chacha20, void *p_ciphertext, size_t size ) 
{
    
    // argument check
    if ( NULL == p_chacha20 ) goto no_chacha20;

    // done
    return chacha20_process(p_plaintext, p_chacha20, p_ciphertext, size); 

    // error handling
    {

        // argument errors
        {
            no_chacha20:
                #ifndef NDEBUG
                    log_error("[chacha20] Null pointer provided for parameter \"p_chacha20\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
    }
}

int chacha20_seek ( chacha20 *p_chacha20, unsigned int block )
{

    // set the cursor
    p_chacha20->block = block;

    // success
    return 1;
}

int chacha20_tell ( chacha20 *p_chacha20, unsigned int *p_block )
{

    // return a pointer to the caller
    *p_block = p_chacha20->block;

    // success
    return 1;
}

int chacha20_destroy ( chacha20 **pp_chacha20 )
{

    // argument check
    if ( NULL == pp_chacha20 ) goto no_chacha20;

    // initialized data
    chacha20 *p_chacha20 = *pp_chacha20;

    // no more pointer for caller
    *pp_chacha20 = NULL;

    // release the chacha20 cipher
    p_chacha20 = default_allocator(p_chacha20, 0);

    // success
    return 1;

    // error handling
    {

        // argument errors
        {
            no_chacha20:
                #ifndef NDEBUG
                    log_error("[chacha20] Null pointer provided for parameter \"pp_chacha20\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
    }
}

int chacha20_round ( chacha20 *p_chacha20 )
{
    
    // initialized data
    unsigned int x[16] = { 0 };

    // copy the current chacha20 state
    memcpy(x, &p_chacha20->state, sizeof(chacha20_state));
    
    // 10 iterations; 4 column quarter rounds and 4 diagonal quarter rounds
    for ( int i = 0; i < 10; i++ )
    
        chacha20_quarter_round(&x[0], &x[4], &x[8] , &x[12]),
        chacha20_quarter_round(&x[1], &x[5], &x[9] , &x[13]),
        chacha20_quarter_round(&x[2], &x[6], &x[10], &x[14]),
        chacha20_quarter_round(&x[3], &x[7], &x[11], &x[15]),

        chacha20_quarter_round(&x[0], &x[5], &x[10], &x[15]),
        chacha20_quarter_round(&x[1], &x[6], &x[11], &x[12]),
        chacha20_quarter_round(&x[2], &x[7], &x[8] , &x[13]),
        chacha20_quarter_round(&x[3], &x[4], &x[9] , &x[14]);
    

    // accumulate
    for ( int i = 0; i < 16; i++ ) x[i] += p_chacha20->state[i];
    
    #ifdef CHACHA20_DEBUG
        log_info("Key stream: \n");
        chacha20_block_print(x);
        chacha20_block_byte_print(x);
    #endif

    // store the state after the round operation
    memcpy(&p_chacha20->state, x, sizeof(chacha20_state));
    
    // done
    return 1;
}

void chacha20_quarter_round 
( 
    unsigned int *a,
    unsigned int *b,
    unsigned int *c,
    unsigned int *d
)
{

    // add rotate xor
    *a += *b, *d ^= *a, *d = (*d << 16) | (*d >> 16),
    *c += *d, *b ^= *c, *b = (*b << 12) | (*b >> 20),
    *a += *b, *d ^= *a, *d = (*d << 8) | (*d >> 24),
    *c += *d, *b ^= *c, *b = (*b << 7) | (*b >> 25);

    // done
    return;
}
