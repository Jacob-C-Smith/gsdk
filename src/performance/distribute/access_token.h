/** !
 * Access token interface
 *
 * @file performance/access_token.h
 * 
 * @author Jacob Smith
 */

// header guard
#pragma once

// core 
#include <core/log.h>
#include <core/rsa.h>
#include <core/sha.h>
#include <core/interfaces.h>
#include <core/digital_signature.h>

// reflection
#include <reflection/base64.h>
#include <reflection/json.h>

// forward declarations
struct access_token_s;

// type definitions
typedef struct access_token_s access_token;

// structure definitions
struct access_token_s
{
    public_key  *p_public_key;
    private_key *p_private_key;
    json_value  *p_header_value;
    json_value  *p_payload_value;
    sha256_hash  _sha_hash;
};

// function declarations
/// constructors
int access_token_construct
(
    access_token **pp_access_token,
    public_key    *p_public_key,
    private_key   *p_private_key,
    json_value    *p_header_value,
    json_value    *p_payload_value
);

/// verify
int access_token_verify
(
    const char *token_string,
    public_key *p_public_key,
    access_token **pp_access_token
);

/// destructors
int access_token_destroy ( access_token **pp_access_token );