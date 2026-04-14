/** ! 
 * Certificate interface
 * 
 * @file src/core/secure_socket/certificate.h
 * 
 * @author Jacob Smith
 */

// header guard
#pragma once

// standard library
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <time.h>

// core
#include <core/log.h>
#include <core/hash.h>
#include <core/interfaces.h>
#include <core/ed25519.h>
#include <core/pack.h>

// structure declarations
struct certificate_s;

// type definitions
typedef struct certificate_s certificate;

// function declarations
/// constructor
/** !
 * Construct a certificate
 * 
 * @param pp_certificate result
 * @param p_subject      to whom the certificate refers to
 * @param p_public_key   the public key
 * @param issuer         the fingerprint of the issuer
 * @param not_before     when the certificate becomes valid
 * @param not_after      when the certificate becomes invalid
 * @param is_ca          true IF certificate is a certificate authority ELSE false
 * 
 * @return 1 on success, 0 on error
 */
int certificate_construct
(
    certificate        **pp_certificate,
    const char          *p_subject,
    ed25519_public_key  *p_public_key,
    sha256_hash          issuer,
    long                 not_before,
    long                 not_after,
    bool                 is_ca
);

/// print
/** !
 * Print a certificate
 * 
 * @param p_certificate the certificate
 * 
 * @return 1 on success, 0 on error
 */
int certificate_print ( certificate *p_certificate );

/// accessors
/** !
 * Get the subject of a certificate
 * 
 * @param p_certificate the certificate
 * @param pp_subject    result
 * 
 * @return 1 on success, 0 on error
 */
int certificate_get_subject ( certificate *p_certificate, const char **pp_subject );

/** !
 * Get the public key of a certificate
 * 
 * @param p_certificate the certificate
 * @param p_public_key  result
 * 
 * @return 1 on success, 0 on error
 */
int certificate_public_key_get ( certificate *p_certificate, ed25519_public_key *p_public_key );

/// sign
/** !
 * Sign a certificate
 * 
 * @param p_certificate the certificate
 * @param p_public_key  the public key
 * @param p_private_key the private key
 * 
 * @return 1 on success, 0 on error
 */
int certificate_sign ( certificate *p_certificate, ed25519_public_key *p_public_key, ed25519_private_key *p_private_key );

/// verify
/** !
 * Verify a certificate
 * 
 * @param p_certificate the certificate
 * @param p_issuer      the certificate of the issuer IF NOT NULL else p_certificate is a CA
 * 
 * @return 1 on success, 0 on error
 */
int certificate_verify ( certificate *p_certificate, certificate *p_issuer );

/** !
 * Verify a certificate chain
 * 
 * @param pp_chain     a list of certificates
 * @param count        the quantity of certificates in the certificate chain
 * @param p_trust_root the trust root
 * 
 * @return 1 on success, 0 on error
 */
int certificate_chain_verify ( certificate **pp_chain, size_t count, certificate *p_trust_root );

/// reflection
/** !
 * Pack a certificate into a buffer
 * 
 * @param p_buffer      the buffer
 * @param p_certificate the certificate
 * 
 * @return bytes written on success, 0 on error
 */
int certificate_pack ( void *p_buffer, certificate *p_certificate );

/** !
 * Unpack a buffer into a certificate
 * 
 * @param pp_certificate result
 * @param p_buffer       the buffer
 * 
 * @return bytes read on success, 0 on error
 */
int certificate_unpack ( certificate **pp_certificate, void *p_buffer );

/// destructor
/** !
 * Destroy a certificate
 * 
 * @param pp_certificate pointer to certificate pointer
 * 
 * @return 1 on success, 0 on error
 */
int certificate_destroy ( certificate **pp_certificate );
