/** ! 
 * Digital Signature 
 *
 * @file digital_signature.c
 *
 * @author Jacob Smith
 */

// header
#include <core/digital_signature.h>

// function definitions
int digital_signature_sign
(
    private_key  *p_private_key, 
    public_key   *p_public_key, 
    const void   *const p_data, 
    size_t        data_size, 
    void        **pp_signature
)
{
    // argument check
    if ( p_private_key == (void *) 0 ) goto no_private_key;
    if ( p_data        == (void *) 0 ) goto no_data;
    if ( data_size     ==          0 ) goto no_data_size;
    if ( pp_signature  == (void *) 0 ) goto no_signature;

    // initialized data
    sha256_state   _sha256_state = { 0 };
    sha256_hash    hash          = { 0 };
    void          *p_hash_val    = NULL; // packed hash block (RSA size)
    void          *p_signature   = NULL;

    // construct a sha256 hasher
    sha256_construct(&_sha256_state);

    // feed it
    sha256_update(&_sha256_state, p_data, data_size);

    // digest it
    sha256_final(&_sha256_state, hash);

    // compute RSA block size from key type
    size_t rsa_block_size = sizeof(((public_key *)0)->n);

    // allocate memory for the signature (RSA block size)
    p_signature = default_allocator(0, rsa_block_size);

    // error check
    if ( NULL == p_signature ) goto no_mem;

    // Allocate and zero a block, then pack the 32-byte SHA256 into the lower bytes
    p_hash_val = default_allocator(0, rsa_block_size);
    if ( NULL == p_hash_val ) goto no_mem;
    memset(p_hash_val, 0, rsa_block_size);
    memcpy(p_hash_val, hash, sizeof(sha256_hash));

    // "Sign" by raising to the private exponent (RSA decrypt)
    if ( 0 == rsa_decrypt(p_hash_val, p_signature, p_public_key, p_private_key) ) goto failed_to_sign;

    // return the signature
    *pp_signature = p_signature;

    // success
    return 1;

    // error handling
    {
        no_private_key:
            #ifndef NDEBUG
                log_error("[digital signature] Null pointer provided for parameter \"p_private_key\" in call to function \"%s\"\n", __FUNCTION__);
            #endif

            // error
            return 0;

        no_data:
            #ifndef NDEBUG
                log_error("[digital signature] Null pointer provided for parameter \"p_data\" in call to function \"%s\"\n", __FUNCTION__);
            #endif

            // error
            return 0;

        no_data_size:
            #ifndef NDEBUG
                log_error("[digital signature] Zero provided for parameter \"data_size\" in call to function \"%s\"\n", __FUNCTION__);
            #endif

            // error
            return 0;

        no_signature:
            #ifndef NDEBUG
                log_error("[digital signature] Null pointer provided for parameter \"pp_signature\" in call to function \"%s\"\n", __FUNCTION__);
            #endif

            // error
            return 0;

        no_mem:
            #ifndef NDEBUG
                log_error("[digital signature] Failed to allocate memory in call to function \"%s\"\n", __FUNCTION__);
            #endif

            // error
            return 0;

        failed_to_sign:
            #ifndef NDEBUG
                log_error("[digital signature] Failed to sign data in call to function \"%s\"\n", __FUNCTION__);
            #endif

            // release the signature
            p_signature = default_allocator(p_signature, 0);
            // release temp hash block
            p_hash_val  = default_allocator(p_hash_val, 0);

            // error
            return 0;
    }
}

int digital_signature_verify
(
    public_key *p_public_key, 
    const void *p_data, 
    size_t data_size, 
    const void *p_signature
)
{

    // argument check
    if ( p_public_key == (void *) 0 ) goto no_public_key;
    if ( p_data       == (void *) 0 ) goto no_data;
    if ( data_size    == 0          ) goto no_data_size;
    if ( p_signature  == (void *) 0 ) goto no_signature;

    // initialized data
    sha256_state _sha256_state    = { 0 };
    sha256_hash  computed_hash    = { 0 };
    void        *p_hash_val       = NULL; // packed hash block (RSA size)
    void        *p_decrypted_hash = NULL;

    // construct a sha256 hasher
    sha256_construct(&_sha256_state);

    // feed it
    sha256_update(&_sha256_state, p_data, data_size);

    // digest it
    sha256_final(&_sha256_state, computed_hash);

    // Recover the signed hash with the public key (RSA encrypt)
    size_t rsa_block_size = sizeof(((public_key *)0)->n);
    p_decrypted_hash = default_allocator(0, rsa_block_size);
    if ( NULL == p_decrypted_hash ) goto failed_to_verify;
    if ( 0 == rsa_encrypt((void *)p_signature, p_decrypted_hash, p_public_key) ) goto failed_to_verify;

    // Pack the freshly computed hash into an RSA-sized block for comparison
    p_hash_val = default_allocator(0, rsa_block_size);
    if ( NULL == p_hash_val ) goto failed_to_verify;
    memset(p_hash_val, 0, rsa_block_size);
    memcpy(p_hash_val, computed_hash, sizeof(sha256_hash));

    // Compare full RSA block (both are zero-padded beyond 32 bytes)
    if ( 0 != memcmp(p_decrypted_hash, p_hash_val, rsa_block_size) ) goto failed_to_verify;

    // success
    // free temporaries
    p_decrypted_hash = default_allocator(p_decrypted_hash, 0);
    p_hash_val       = default_allocator(p_hash_val, 0);
    return 1;

    // error handling
    {
        no_public_key:
            #ifndef NDEBUG
                log_error("[digital signature] Null pointer provided for parameter \"p_public_key\" in call to function \"%s\"\n", __FUNCTION__);
            #endif
            
            // error
            return 0;

        no_data:
            #ifndef NDEBUG
                log_error("[digital signature] Null pointer provided for parameter \"p_data\" in call to function \"%s\"\n", __FUNCTION__);
            #endif
            
            // error
            return 0;

        no_data_size:
            #ifndef NDEBUG
                log_error("[digital signature] Zero provided for parameter \"data_size\" in call to function \"%s\"\n", __FUNCTION__);
            #endif
            
            // error
            return 0;

        no_signature:
            #ifndef NDEBUG
                log_error("[digital signature] Null pointer provided for parameter \"p_signature\" in call to function \"%s\"\n", __FUNCTION__);
            #endif
            
            // error
            return 0;

        failed_to_verify:
            #ifndef NDEBUG
                log_error("[digital signature] Signature verification failed in call to function \"%s\"\n", __FUNCTION__);
            #endif
            
            // free temporaries if allocated
            if ( p_decrypted_hash ) p_decrypted_hash = default_allocator(p_decrypted_hash, 0);
            if ( p_hash_val )       p_hash_val       = default_allocator(p_hash_val, 0);

            // error
            return 0;
    }
}
