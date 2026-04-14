/** ! 
 * Certificate implementation
 * 
 * @file src/core/secure_socket/certificate.c
 * 
 * @author Jacob Smith
 */

// header file
#include <core/certificate.h>

// structure definitions
struct certificate_s
{
    char               *p_subject;
    ed25519_public_key  public_key;
    sha256_hash         issuer;
    unsigned long       not_before;
    unsigned long       not_after;
    bool                is_ca;
    ed25519_signature   signature;
};

// function definitions
int certificate_construct
(
    certificate        **pp_certificate,
    const char          *p_subject,
    ed25519_public_key  *p_public_key,
    sha256_hash          issuer,
    long                 not_before,
    long                 not_after,
    bool                 is_ca
)
{

    // argument check
    if ( NULL == pp_certificate ) goto no_certificate;
    if ( NULL ==      p_subject ) goto no_subject;
    if ( NULL ==   p_public_key ) goto no_public_key;

    // initialized data
    certificate *p_certificate = NULL;

    // allocate memory for a certificate
    p_certificate = default_allocator(NULL, sizeof(certificate));
    if ( NULL == p_certificate ) goto no_mem;

    // populate the struct
    *p_certificate = (certificate)
    {
        .p_subject  = strdup(p_subject),
        .not_before = not_before,
        .not_after  = not_after,
        .is_ca      = is_ca
    };

    // copy the public key
    memcpy(p_certificate->public_key, p_public_key, sizeof(ed25519_public_key));

    // copy the issuer hash
    memcpy(p_certificate->issuer, issuer, sizeof(sha256_hash));

    // return a pointer to the caller
    *pp_certificate = p_certificate;

    // success
    return 1;

    // error handling
    {
        
        // argument errors
        {
            no_certificate:
                #ifndef NDEBUG
                    log_error("[certificate] Null pointer provided for parameter \"pp_certificate\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;

            no_subject:
                #ifndef NDEBUG
                    log_error("[certificate] Null pointer provided for parameter \"p_subject\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;

            no_public_key:
                #ifndef NDEBUG
                    log_error("[certificate] Null pointer provided for parameter \"p_public_key\" in call to function \"%s\"\n", __FUNCTION__);
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

int certificate_print ( certificate *p_certificate )
{
    
    // argument check
    if ( NULL == p_certificate ) goto no_certificate;

    // print the certificate
    printf("Subject: %s\n", p_certificate->p_subject);
    printf("Public Key: "), ed25519_public_key_print(&p_certificate->public_key);
    printf("Issuer: "), sha256_print(p_certificate->issuer), putchar('\n');
    printf("Not Before: %s", ctime((const time_t *)&p_certificate->not_before));
    printf("Not After: %s", ctime((const time_t *)&p_certificate->not_after));
    printf("Is CA: %s\n", p_certificate->is_ca ? "Yes" : "No");
    printf("Signature: "), ed25519_signature_print(&p_certificate->signature), putchar('\n');

    // success
    return 1;

    // error handling
    {
        no_certificate:
            #ifndef NDEBUG
                log_error("[certificate] Null pointer provided for parameter \"p_certificate\" in call to function \"%s\"\n", __FUNCTION__);
            #endif

            // error
            return 0;
    }
}

int certificate_public_key_get ( certificate *p_certificate, ed25519_public_key *p_public_key )
{

    // argument check
    if ( NULL == p_certificate ) goto no_certificate;
    if ( NULL ==  p_public_key ) goto no_public_key;

    // store the public key
    memcpy(p_public_key, p_certificate->public_key, sizeof(ed25519_public_key));

    // success
    return 1;

    // error handling
    {

        // argument errors
        {
            no_certificate:
                #ifndef NDEBUG
                    log_error("[certificate] Null pointer provided for parameter \"p_certificate\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;

            no_public_key:
                #ifndef NDEBUG
                    log_error("[certificate] Null pointer provided for parameter \"p_public_key\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
    }
}

int certificate_get_subject ( certificate *p_certificate, const char **pp_subject )
{

    // argument check
    if ( NULL == p_certificate ) goto no_certificate;
    if ( NULL ==    pp_subject ) goto no_subject;

    // return a pointer to the caller
    *pp_subject = p_certificate->p_subject;

    // success
    return 1;

    // error handling
    {

        // argument errors
        {
            no_certificate:
                #ifndef NDEBUG
                    log_error("[certificate] Null pointer provided for parameter \"p_certificate\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;

            no_subject:
                #ifndef NDEBUG
                    log_error("[certificate] Null pointer provided for parameter \"pp_subject\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
    }
}

int certificate_sign ( certificate *p_certificate, ed25519_public_key *p_public_key, ed25519_private_key *p_private_key )
{

    // argument check
    if ( NULL == p_certificate ) goto no_certificate;
    if ( NULL ==   p_public_key ) goto no_public_key;
    if ( NULL ==  p_private_key ) goto no_private_key;

    // initialized data
    char _buf[160] = { 0 };
    size_t len = 0;

    // pack the certificate
    len = certificate_pack(_buf, p_certificate);

    // sign the certificate
    ed25519_sign(
        &p_certificate->signature,
        (const unsigned char *)_buf,
        len - sizeof(ed25519_signature),
        p_public_key,
        p_private_key
    );

    // success
    return 1;

    // error handling
    {

        // argument errors
        {
            no_certificate:
                #ifndef NDEBUG
                    log_error("[certificate] Null pointer provided for parameter \"p_certificate\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;

            no_public_key:
                #ifndef NDEBUG
                    log_error("[certificate] Null pointer provided for parameter \"p_public_key\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
            
            no_private_key:
                #ifndef NDEBUG
                    log_error("[certificate] Null pointer provided for parameter \"p_private_key\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
    }
}

int certificate_verify ( certificate *p_certificate, certificate *p_issuer )
{

    // argument check
    if ( NULL == p_certificate ) goto no_certificate;

    // initialized data
    char _buf[160] = { 0 };
    size_t len = 0;
    ed25519_public_key *p_public_key = NULL;

    // CA uses their key
    if ( NULL == p_issuer )
        p_public_key = &p_certificate->public_key;
    
    // Non-CA uses 
    else
    {

        // ensure the issuer is a CA
        if ( false == p_issuer->is_ca ) goto issuer_not_ca;

        // verify that the issuer's subject name matches the certificate's issuer hash
        {

            // initialized data
            sha256_hash _h = { 0 };
            sha256_state s = { 0 };

            // hash the subject
            sha256_construct(&s);
            sha256_update(&s, (const unsigned char *)p_issuer->p_subject, strlen(p_issuer->p_subject));
            sha256_final(&s, _h);

            // ensure the issuer matches
            if ( 0 != memcmp(_h, p_certificate->issuer, sizeof(sha256_hash)) ) goto issuer_mismatch;
        }

        // store the public key of the issuer
        p_public_key = &p_issuer->public_key;
    }

    // pack the certificate
    len = certificate_pack(_buf, p_certificate);

    // done
    return ed25519_verify(
        &p_certificate->signature,
        (const unsigned char *)_buf,
        len - sizeof(ed25519_signature),
        p_public_key
    );

    // error handling
    {

        // argument errors
        {
            no_certificate:
                #ifndef NDEBUG
                    log_error("[certificate] Null pointer provided for parameter \"p_certificate\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }

        // certificate errors
        {
            issuer_not_ca:
                #ifndef NDEBUG
                    log_error("[certificate] Issuer certificate is not a CA in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;

            issuer_mismatch:
                #ifndef NDEBUG
                    log_error("[certificate] Issuer certificate subject does not match certificate issuer hash in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
    }
}

int certificate_chain_verify ( certificate **pp_chain, size_t count, certificate *p_trust_root )
{

    // argument check
    if ( NULL == pp_chain     ) goto no_chain;
    if ( NULL == p_trust_root ) goto no_trust_root;

    // initialized data
    certificate *p_issuer = NULL;

    // iterate through each certificate in the chain
    for (size_t i = 0; i < count; i++)
    {

        // store the issuer for the current certificate
        p_issuer = ( i + 1 < count ) ? pp_chain[i + 1] : p_trust_root;

        // verify the current certificate using its issuer
        if ( 0 == certificate_verify(pp_chain[i], p_issuer) ) goto failed_to_verify_certificate;
    }

    // verify the trust root (it should be self-signed)
    if ( 0 == certificate_verify(p_trust_root, NULL) ) goto failed_to_verify_trust_root;

    // success
    return 1;

    // error handling
    {

        // argument errors
        {
            no_chain:
                #ifndef NDEBUG
                    log_error("[certificate] Null pointer provided for parameter \"pp_chain\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;

            no_trust_root:
                #ifndef NDEBUG
                    log_error("[certificate] Null pointer provided for parameter \"p_trust_root\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }

        // certificate errors
        {
            failed_to_verify_certificate:
                #ifndef NDEBUG
                    log_error("[certificate] Failed to verify certificate chain in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
            
            failed_to_verify_trust_root:
                #ifndef NDEBUG
                    log_error("[certificate] Failed to verify trust root in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
    }
}

int certificate_pack ( void *p_buffer, certificate *p_certificate )
{

    // argument check
    if ( NULL ==      p_buffer ) goto no_buffer;
    if ( NULL == p_certificate ) goto no_certificate;

    // initialized data
    char *p = p_buffer;

    // pack the certificate
    p += pack_pack(p, "%s", p_certificate->p_subject);
    p += ed25519_public_key_pack(p, &p_certificate->public_key);
    p += sha256_pack(p, (unsigned char *)p_certificate->issuer);
    p += pack_pack(p, "%i64", p_certificate->not_before);
    p += pack_pack(p, "%i64", p_certificate->not_after);
    p += pack_pack(p, "%i8", p_certificate->is_ca);
    p += ed25519_signature_pack(p, &p_certificate->signature);

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

            no_certificate:
                #ifndef NDEBUG
                    log_error("[certificate] Null pointer provided for parameter \"p_certificate\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
    }
}

int certificate_unpack ( certificate **pp_certificate, void *p_buffer )
{

    // argument check
    if ( NULL ==       p_buffer ) goto no_buffer;
    if ( NULL == pp_certificate ) goto no_certificate;

    // initialized data
    certificate *p_certificate = NULL;
    char *p = p_buffer;
    char subject[64] = { 0 };
    ed25519_public_key public_key = { 0 };
    ed25519_signature signature = { 0 };
    sha256_hash issuer = { 0 };
    long not_before = 0, 
         not_after  = 0;
    char is_ca;

    // unpack the certificate
    p += pack_unpack(p, "%s", subject);
    p += ed25519_public_key_unpack(&public_key, p);
    p += sha256_unpack(&issuer, p);
    p += pack_unpack(p, "%i64", &not_before);
    p += pack_unpack(p, "%i64", &not_after);
    p += pack_unpack(p, "%i8", &is_ca);
    p += ed25519_signature_unpack(&signature, p);

    // construct a certificate struct
    if ( 0 == certificate_construct(
        &p_certificate,
        subject,
        &public_key,
        issuer,
        not_before,
        not_after,
        (bool)is_ca
    ) ) goto failed_to_construct_certificate;

    // copy the signature
    memcpy(&p_certificate->signature, &signature, sizeof(ed25519_signature));

    // return a pointer to the caller
    *pp_certificate = p_certificate;

    // success
    return p - (char *)p_buffer;

    // error handling
    {

        // argument errors
        {
            no_certificate:
                #ifndef NDEBUG
                log_error("[certificate] Null pointer provided for parameter \"pp_certificate\" in call to function \"%s\"\n", __FUNCTION__);
                #endif
                
                // error
                return 0;

            no_buffer:
                #ifndef NDEBUG
                    log_error("[certificate] Null pointer provided for parameter \"p_buffer\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }

        // certificate errors
        {
            failed_to_construct_certificate:
                #ifndef NDEBUG
                    log_error("[certificate] Failed to construct certificate in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
    }
}

int certificate_destroy ( certificate **pp_certificate )
{

    // argument check
    if ( NULL == pp_certificate ) goto no_certificate;

    // initialized data
    certificate *p_certificate = *pp_certificate;

    // no more pointer for caller
    *pp_certificate = NULL;

    // release the subject
    if ( p_certificate->p_subject ) 
        p_certificate->p_subject = default_allocator(p_certificate->p_subject, 0);

    // free the certificate
    p_certificate = default_allocator(p_certificate, 0);

    // success
    return 1;

    // error handling
    {

        // argument errors
        {
            no_certificate:
                #ifndef NDEBUG
                    log_error("[certificate] Null pointer provided for parameter \"pp_certificate\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
    }
}
