/** !
 * Digital Signature 
 *
 * @file src/core/digital_signature/digital_signature.h
 *
 * @author Jacob Smith
 */

// header guard
#pragma once

// standard library
#include <stdio.h>
#include <stdlib.h>

// core
#include <core/rsa.h>
#include <core/sha.h>

// function declarations
/// sign
/** !
 * Sign a block of data with a private key
 *
 * @param p_private_key the private key
 * @param p_data        the data to sign
 * @param data_size     the size of the data in bytes
 * @param pp_signature  the signature
 *
 * @return 1 on success, 0 on error
 */
int digital_signature_sign 
(
    private_key  *p_private_key, 
    public_key   *p_public_key, 
    const void   *p_data, 
    size_t        data_size, 
    void        **pp_signature
);

/// verify
/** !
 * Verify a signature with a public key
 *
 * @param p_public_key the public key
 * @param p_data       the data
 * @param data_size    the size of the data in bytes
 * @param p_signature  the signature
 *
 * @return 1 on success, 0 on error
 */
int digital_signature_verify 
( 
    public_key *      p_public_key, 
    const void *const p_data, 
    size_t            data_size,
    const void *const p_signature
);
