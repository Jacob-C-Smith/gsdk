/** ! 
 * AEAD implementation
 * 
 * @file src/core/aead/aead.c
 * 
 * @author Jacob Smith
 */

// header file
#include <core/aead.h>

// structure definitions
struct aead_s
{
    chacha20_key    key;
    chacha20_nonce  nonce;
    size_t          sequence;
    chacha20       *p_chacha20;
};

// function definitions
int aead_construct
(
    aead           **pp_aead,
    chacha20_key     key,
    chacha20_nonce   nonce
)
{

    // argument check
    if ( NULL == pp_aead ) goto no_aead;

    // initialized data
    aead     *p_aead     = NULL;
    chacha20 *p_chacha20 = NULL;

    // allocate memory
    p_aead = default_allocator(NULL, sizeof(aead));
    if ( NULL == p_aead ) goto no_mem;

    // construct a chacha20 cipher
    if ( 0 == chacha20_construct(
        &p_chacha20,
        key,
        nonce,
        0
    ) ) 
        goto failed_to_construct_chacha20;    

    // populate the struct
    *p_aead = (aead)
    {
        .key = 
        {
            key[0], key[1], key[2], key[3],
            key[4], key[5], key[6], key[7],
        },
        .nonce = { nonce[0], nonce[1], nonce[2], },
        .sequence   = 0,
        .p_chacha20 = p_chacha20,
    };

    // return a pointer to the caller
    *pp_aead = p_aead;

    // success
    return 1;

    // error handling
    {

        // argument errors
        {
            no_aead:
                #ifndef NDEBUG
                    log_error("[aead] Null pointer provided for parameter \"pp_aead\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }

        // chacha20 errors
        {
            failed_to_construct_chacha20:
                #ifndef NDEBUG
                    log_error("[aead] Failed to construct chacha20 cipher in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // release the aead
                p_aead = default_allocator(p_aead, 0);
                
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

int aead_encrypt
( 
    void         *p_ciphertext, 
    aead         *p_aead, 
    poly1305_tag  tag, 

    const void *p_aad,       size_t aad_len, 
    const void *p_plaintext, size_t len
)
{

    // argument check
    if ( NULL == p_aead ) goto no_aead;

    // initialized data
    poly1305_one_time_key _one_time_key = { 0 };
    chacha20_nonce        current_nonce = { 0 };
    unsigned long long   *p_seq_part    = NULL;
    unsigned char        *p_data        = NULL;
    size_t                aad_pad       = (16 - (aad_len % 16)) % 16;
    size_t                text_pad      = (16 - (len % 16)) % 16;
    size_t                data_len      = 0;
    size_t                offset        = 0;

    // construct the nonce
    memcpy(current_nonce, p_aead->nonce, sizeof(chacha20_nonce));
    p_seq_part = (unsigned long long *)&current_nonce[1];
    *p_seq_part ^= p_aead->sequence;

    // generate the one time key
    poly1305_key_generate(&_one_time_key, p_aead->key, current_nonce, 0);

    // seek block 1
    chacha20_seek(p_aead->p_chacha20, 1);

    // set the nonce
    chacha20_nonce_set(p_aead->p_chacha20, current_nonce);

    // encrypt
    chacha20_encrypt(p_ciphertext, p_aead->p_chacha20, (void *)p_plaintext, len);

    // store the length of the message authentication code 
    data_len = (aad_len + aad_pad) + (len + text_pad) + 16;

    // allocate memory for the message authentication code 
    p_data = default_allocator(NULL, data_len);
    if ( NULL == p_data ) goto no_mem;

    // zero set
    memset(p_data, 0, data_len);

    // copy the aad
    memcpy(p_data + offset, p_aad, aad_len);
    offset += aad_len + aad_pad;

    // copy the ciphertext
    memcpy(p_data + offset, p_ciphertext, len);
    offset += len + text_pad;

    // pack the aad
    pack_pack(p_data + offset, "%8i64%8i64", (uint64_t)aad_len, (uint64_t)len);

    // compute the poly1305 message authentication code 
    poly1305_mac(p_data, data_len, tag, _one_time_key);

    // increment the sequence counter
    p_aead->sequence++;

    // release the message authentication code 
    p_data = default_allocator(p_data, 0);
    
    // success
    return 1;

    // error handling
    {

        // argument errors
        {
            no_aead:
                #ifndef NDEBUG
                    log_error("[aead] Null pointer provided for parameter \"p_aead\" in call to function \"%s\"\n", __FUNCTION__);
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

int aead_sequence_set ( aead *p_aead, size_t sequence )
{

    // argument check
    if ( NULL == p_aead ) goto no_aead;

    // set the sequence counter
    p_aead->sequence = sequence;

    // success
    return 1;

    // error handling
    {

        // argument errors
        {
            no_aead:
                #ifndef NDEBUG
                    log_error("[aead] Null pointer provided for parameter \"p_aead\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
    }
}

int aead_decrypt
( 
    void         *p_plaintext, 
    aead         *p_aead, 
    poly1305_tag  tag, 
    
    void *p_aad       , size_t aad_len, 
    void *p_ciphertext, size_t len
)
{

    // argument check
    if ( NULL == p_aead ) goto no_aead;

    // initialized data
    poly1305_one_time_key _one_time_key = { 0 };
    chacha20_nonce        current_nonce = { 0 };
    unsigned long long   *p_seq_part    = NULL;
    unsigned char        *p_data        = NULL;
    size_t                aad_pad       = (16 - (aad_len % 16)) % 16;
    size_t                text_pad      = (16 - (len % 16)) % 16;
    size_t                data_len      = 0;
    poly1305_tag          computed_tag  = { 0 };
    size_t                offset        = 0;

    // construct the nonce
    memcpy(current_nonce, p_aead->nonce, sizeof(chacha20_nonce));
    p_seq_part = (unsigned long long *)&current_nonce[1];
    *p_seq_part ^= p_aead->sequence;

    // generate the one time key
    poly1305_key_generate(&_one_time_key, p_aead->key, current_nonce, 0);

    // store the length of the message authentication code 
    data_len = (aad_len + aad_pad) + (len + text_pad) + 16;

    // allocate memory for the message authentication code 
    p_data = default_allocator(NULL, data_len);
    if ( NULL == p_data ) goto no_mem;

    // zero set
    memset(p_data, 0, data_len);

    // copy the aad
    memcpy(p_data + offset, p_aad, aad_len);
    offset += aad_len + aad_pad;

    // copy the ciphertext
    memcpy(p_data + offset, p_ciphertext, len);
    offset += len + text_pad;

    // pack the aad
    pack_pack(p_data + offset, "%8i64%8i64", (uint64_t)aad_len, (uint64_t)len);

    // compute the poly1305 message authentication code 
    poly1305_mac(p_data, data_len, computed_tag, _one_time_key);

    // release the message authentication code 
    default_allocator(p_data, 0);

    // verify the tag
    if ( 0 != memcmp(computed_tag, tag, 16) ) goto tag_verification_failed;

    // seek block 1
    chacha20_seek(p_aead->p_chacha20, 1);

    // set the nonce
    chacha20_nonce_set(p_aead->p_chacha20, current_nonce);

    // decrypt
    chacha20_decrypt(p_plaintext, p_aead->p_chacha20, (void *)p_ciphertext, len);

    // increment the sequence counter
    p_aead->sequence++;
    
    // success
    return 1;

    // error handling
    {

        // argument errors
        {
            no_aead:
                #ifndef NDEBUG
                    log_error("[aead] Null pointer provided for parameter \"p_aead\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }

        // tag verification errors
        {
            tag_verification_failed:
                #ifndef NDEBUG
                    log_error("[aead] Tag verification failed in call to function \"%s\"\n", __FUNCTION__);
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


int aead_destroy ( aead **pp_aead )
{

    // argument check
    if ( NULL == pp_aead ) goto no_aead;

    // initialized data
    aead *p_aead = *pp_aead;

    // no more pointer for caller
    *pp_aead = NULL;

    // destroy the chacha20 cipher
    chacha20_destroy(&p_aead->p_chacha20);

    // release the aead
    p_aead = default_allocator(p_aead, 0);

    // success
    return 1;

    // error handling
    {

        // argument errors
        {
            no_aead:
                #ifndef NDEBUG
                    log_error("[aead] Null pointer provided for parameter \"pp_aead\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
    }
}
