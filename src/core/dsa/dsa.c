/** ! 
 * Digital Signature Algorithm
 *
 * @file dsa.c
 *
 * @author Jacob Smith
 */

// header
#include <core/dsa.h>

// function definitions
int dsa_sign
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
    void          *p_signature   = NULL;

    // construct a sha256 hasher
    sha256_construct(&_sha256_state);

    // feed it
    sha256_update(&_sha256_state, p_data, data_size);

    // digest it
    sha256_final(&_sha256_state, hash);

    // allocate memory for the signature
    p_signature = default_allocator(0, sizeof(i2048));

    // error check
    if ( NULL == p_signature ) goto no_mem;

    // encrypt the hash with the private key to create the signature
    if ( 0 == dec(hash, p_signature, p_public_key, p_private_key) ) goto failed_to_sign;

    // return the signature
    *pp_signature = p_signature;

    // success
    return 1;

    // error handling
    {
        no_private_key:
            #ifndef NDEBUG
                printf("[dsa] Null pointer provided for parameter \"p_private_key\" in call to function \"%s\"\n", __FUNCTION__);
            #endif

            // error
            return 0;

        no_data:
            #ifndef NDEBUG
                printf("[dsa] Null pointer provided for parameter \"p_data\" in call to function \"%s\"\n", __FUNCTION__);
            #endif

            // error
            return 0;

        no_data_size:
            #ifndef NDEBUG
                printf("[dsa] Zero provided for parameter \"data_size\" in call to function \"%s\"\n", __FUNCTION__);
            #endif

            // error
            return 0;

        no_signature:
            #ifndef NDEBUG
                printf("[dsa] Null pointer provided for parameter \"pp_signature\" in call to function \"%s\"\n", __FUNCTION__);
            #endif

            // error
            return 0;

        no_mem:
            #ifndef NDEBUG
                printf("[dsa] Failed to allocate memory in call to function \"%s\"\n", __FUNCTION__);
            #endif

            // error
            return 0;

        failed_to_sign:
            #ifndef NDEBUG
                printf("[dsa] Failed to sign data in call to function \"%s\"\n", __FUNCTION__);
            #endif

            // release the signature
            p_signature = default_allocator(p_signature, 0);

            // error
            return 0;
    }
}

int dsa_verify
(
    public_key *p_public_key, 
    private_key *p_private_key, 
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
    sha256_state _sha256_state = { 0 };
    sha256_hash computed_hash = { 0 };
    i2048 decrypted_hash_val = { 0 };

    // construct a sha256 hasher
    sha256_construct(&_sha256_state);

    // feed it
    sha256_update(&_sha256_state, p_data, data_size);

    // digest it
    sha256_final(&_sha256_state, computed_hash);

    // Decrypt the signature with the public key
    if ( 0 == enc((void *)p_signature, &decrypted_hash_val, p_public_key) ) goto failed_to_verify;

    // Compare the decrypted hash with the computed hash
    if ( 0 != memcmp(&decrypted_hash_val, computed_hash, sizeof(sha256_hash)) ) goto failed_to_verify;

    // success
    return 1;

    // error handling
    {
        no_public_key:
            #ifndef NDEBUG
                printf("[dsa] Null pointer provided for parameter \"p_public_key\" in call to function \"%s\"\n", __FUNCTION__);
            #endif
            
            // error
            return 0;

        no_data:
            #ifndef NDEBUG
                printf("[dsa] Null pointer provided for parameter \"p_data\" in call to function \"%s\"\n", __FUNCTION__);
            #endif
            
            // error
            return 0;

        no_data_size:
            #ifndef NDEBUG
                printf("[dsa] Zero provided for parameter \"data_size\" in call to function \"%s\"\n", __FUNCTION__);
            #endif
            
            // error
            return 0;

        no_signature:
            #ifndef NDEBUG
                printf("[dsa] Null pointer provided for parameter \"p_signature\" in call to function \"%s\"\n", __FUNCTION__);
            #endif
            
            // error
            return 0;

        failed_to_verify:
            #ifndef NDEBUG
                printf("[dsa] Signature verification failed in call to function \"%s\"\n", __FUNCTION__);
            #endif
            
            // error
            return 0;
    }
}
