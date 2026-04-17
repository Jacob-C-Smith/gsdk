/** ! 
 * Secure socket implementation
 * 
 * @file src/core/secure_socket/secure_socket.c
 * 
 * @author Jacob Smith
 */

// header file
#include <core/secure_socket.h>

// structure definitions
struct secure_socket_s
{
    socket_tcp      tcp_socket;
    aead           *p_aead;
    chacha20_nonce  nonce; 
};


struct secure_bridge_s
{
    fn_secure_socket_accept *pfn_user_callback;
    certificate             *cert;
    ed25519_private_key     *priv;
    void                    *user_param;
};

// type definitions
typedef struct secure_bridge_s secure_bridge;

// function definitions
int secure_socket_handshake ( secure_socket *p_secure_socket, bool is_server, certificate *p_certificate, ed25519_private_key *p_private_key )
{
    
    // argument check
    if ( NULL == p_secure_socket ) goto no_secure_socket;
    if ( NULL ==   p_certificate ) goto no_certificate;

    // initialized data
    char _plain_buf[1024] = { 0 };

    x25519_private_key   priv      = { 0 };
    x25519_public_key    pub       = { 0 };
    x25519_public_key    peer_pub  = { 0 };
    x25519_shared_secret ss        = { 0 };
    sha512_state         s         = { 0 };
    sha512_hash          h         = { 0 };
    chacha20_key         key       = { 0 };
    chacha20_nonce       nonce     = { 0 };

    // construct a key pair
    if ( 0 == x25519_key_pair_construct(&pub, &priv) ) goto failed_to_construct_key_pair;

    // construct a hasher
    sha512_construct(&s);
    
    // server 
    if ( is_server ) 
    {

        // initialized data
        char *p_p = _plain_buf;
        ed25519_public_key _public_key = { 0 };
        ed25519_signature _signature = { 0 };

        // store the public key from the certificate
        if ( 0 == certificate_public_key_get(p_certificate, &_public_key) ) goto failed_to_get_public_key;

        // sign the public key
        if ( 0 == ed25519_sign(&_signature, (const unsigned char *)&pub, sizeof(pub), &_public_key, p_private_key) ) goto failed_to_sign_public_key;
        
        // pack the ephemeral public key
        p_p += x25519_public_key_pack(p_p, &pub);

        // pack the signature of the ephemeral public key
        p_p += ed25519_signature_pack(p_p, &_signature);

        // pack the certificate
        p_p += certificate_pack(p_p, p_certificate);

        // debug
        #ifdef SECURE_SOCKET_DEBUG
            log_info("X25519    : "), x25519_public_key_print(&pub);
            log_info("ED25519   : "), ed25519_public_key_print(&_public_key);
            log_info("Signature : "), ed25519_signature_print(&_signature);
        #endif

        // receive client hello
        if ( 0 == socket_tcp_receive(p_secure_socket->tcp_socket, peer_pub, sizeof(x25519_public_key)) ) goto failed_to_receive_client_hello;

        // update the hasher
        sha512_update(&s, (const unsigned char *)peer_pub, sizeof(x25519_public_key));

        // send server hello
        if ( 0 == socket_tcp_send(p_secure_socket->tcp_socket, _plain_buf, p_p - _plain_buf) ) goto failed_to_send_server_hello;

        // update the hasher
        sha512_update(&s, (const unsigned char *)_plain_buf, p_p - _plain_buf);
    }
    
    // client
    else
    {

        // initialized data
        char *p_p = _plain_buf;
        ed25519_public_key _server_key = { 0 };
        ed25519_signature _signature = { 0 };
        certificate *p_server_certificate = NULL;

        // send client hello
        if ( 0 == socket_tcp_send(p_secure_socket->tcp_socket, pub, sizeof(x25519_public_key)) ) goto failed_to_send_client_hello;

        // update the hasher
        sha512_update(&s, (const unsigned char *)pub, sizeof(x25519_public_key));
        
        // receive server hello
        if ( 0 == socket_tcp_receive(p_secure_socket->tcp_socket, _plain_buf, sizeof(_plain_buf)) ) goto failed_to_receive_server_hello;

        // unpack the public key
        p_p += x25519_public_key_unpack(&peer_pub, p_p);

        // unpack the signature
        p_p += ed25519_signature_unpack(&_signature, p_p);

        // unpack the certificate
        p_p += certificate_unpack(&p_server_certificate, p_p);

        // update the hasher
        sha512_update(&s, (const unsigned char *)_plain_buf, p_p - _plain_buf);

        // verify the certificate
        if ( 0 == certificate_verify(p_server_certificate, p_certificate ) ) goto failed_to_verify_certificate;

        // store the public key from the certificate
        if ( 0 == certificate_public_key_get(p_server_certificate, &_server_key) ) goto failed_to_get_public_key;

        // verify
        if ( 0 == ed25519_verify(&_signature, (const unsigned char *)&peer_pub, sizeof(x25519_public_key), &_server_key)) goto failed_to_verify_ephemeral_key;
        
        // debug
        #ifdef SECURE_SOCKET_DEBUG
            log_info("X25519       : "), x25519_public_key_print(&pub);
            log_info("ED25519      : "), ed25519_public_key_print(&_server_key);
            log_info("Signature    : "), ed25519_signature_print(&_signature);
        #endif
        
        // destroy the server certificate
        certificate_destroy(&p_server_certificate);
    }
    
    // derive the shared secret
    if ( 0 == x25519_shared_secret_derive(&priv, &peer_pub, &ss) ) goto failed_to_derive_shared_secret;

    // update the hasher
    sha512_update(&s, (const unsigned char *)&ss, sizeof(x25519_shared_secret));

    // zero set keys
    memset(&priv, 0, sizeof(x25519_private_key)),
    memset(&peer_pub, 0, sizeof(x25519_public_key));

    // finalize the hash
    sha512_final(&s, h);

    // debug
    #ifdef SECURE_SOCKET_DEBUG
        log_warning("Shared Secret: "), x25519_shared_secret_print(&ss);
        log_warning("SHA: "), sha512_print((unsigned char *)&h), putchar('\n');
    #endif

    // store the key
    memcpy(key, h, 32);

    // store the nonce
    memcpy(nonce, h + 32, 8);

    // construct the aead
    if ( 0 == aead_construct(&p_secure_socket->p_aead, key, nonce) ) goto failed_to_construct_aead;
    
    // success
    return 1;

    // error handling
    {

        // argument errors
        {
            no_secure_socket:
                #ifndef NDEBUG
                    log_error("[secure socket] Null pointer provided for parameter \"%s\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;

            no_certificate:
                #ifndef NDEBUG
                    log_error("[secure socket] Null pointer provided for parameter \"p_certificate\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
    
        // aead errors
        {
            failed_to_construct_aead:
                #ifndef NDEBUG
                    log_error("[secure socket] Failed to construct aead in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }

        // certificate errors
        {
            failed_to_get_public_key:
                #ifndef NDEBUG
                    log_error("[secure socket] Failed to get public key from certificate in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;

            failed_to_verify_certificate:
                #ifndef NDEBUG
                    log_error("[secure socket] Failed to verify certificate in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }

        // ed25519 errors
        {
            failed_to_sign_public_key:
                #ifndef NDEBUG
                    log_error("[secure socket] Failed to sign public key in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
            
            failed_to_verify_ephemeral_key:
                #ifndef NDEBUG
                    log_error("[secure socket] Failed to verify ephemeral key in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }

        // socket errors
        {
            failed_to_send_client_hello:
                #ifndef NDEBUG
                    log_error("[secure socket] Failed to send client hello in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;

            failed_to_send_server_hello:
                #ifndef NDEBUG
                    log_error("[secure socket] Failed to send server hello in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;

            failed_to_receive_client_hello:
                #ifndef NDEBUG
                    log_error("[secure socket] Failed to receive client hello in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;

            failed_to_receive_server_hello:
                #ifndef NDEBUG
                    log_error("[secure socket] Failed to receive server hello in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }

        // x25519 errors
        {
            failed_to_construct_key_pair:
                #ifndef NDEBUG
                    log_error("[secure socket] Failed to construct key pair in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;

            failed_to_derive_shared_secret:
                #ifndef NDEBUG
                    log_error("[secure socket] Failed to derive shared secret in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
    }
}

int secure_bridge_callback ( socket_tcp new_tcp_sock, socket_ip_address ip, socket_port port, void *p_parameter )
{

    // initialized data
    secure_bridge *p_ctx = (secure_bridge *)p_parameter;
    secure_socket *p_secure_sock = NULL;

    // construct a secure socket
    if ( 0 == secure_socket_construct(&p_secure_sock, new_tcp_sock, true, p_ctx->cert, p_ctx->priv) ) goto failed_to_construct_secure_socket;

    // done
    return p_ctx->pfn_user_callback(p_secure_sock, ip, port, p_ctx->user_param);

    // error handling
    {

        // secure socket errors
        {
            failed_to_construct_secure_socket:
                #ifndef NDEBUG
                    log_error("[secure socket] Failed to construct secure socket in call to function \"%s\"\n", __FUNCTION__);
                #endif
                
                // destroy the tcp socket
                socket_tcp_destroy(&new_tcp_sock);

                // error
                return 0;
        }
    }
}

int secure_socket_listen
(
    socket_tcp               tcp_server_socket,
    fn_secure_socket_accept *pfn_callback,
    certificate             *p_certificate,
    ed25519_private_key     *p_private_key,
    void                    *p_parameter
)
{

    // argument check
    if ( NULL == p_certificate ) goto no_certificate;
    if ( NULL == p_private_key ) goto no_private_key;

    // initialized data
    secure_bridge ctx = 
    {
        .pfn_user_callback = pfn_callback,
        .cert          = p_certificate,
        .priv          = p_private_key,
        .user_param    = p_parameter
    };
    
    // done
    return socket_tcp_listen(tcp_server_socket, secure_bridge_callback, &ctx);

    // error handling
    {

        // argument errors
        {
            no_certificate:
                #ifndef NDEBUG
                    log_error("[secure socket] Null pointer provided for parameter \"p_certificate\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
            
            no_private_key:
                #ifndef NDEBUG
                    log_error("[secure socket] Null pointer provided for parameter \"p_private_key\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
    }
}

// function definitions
int secure_socket_connect
(
    secure_socket       **pp_secure_socket,
    socket_ip_address     ip_address,
    socket_port           port,
    certificate          *p_certificate,
    ed25519_private_key  *p_private_key
)
{

    // initialized data
    socket_tcp tcp_socket;
    secure_socket *p_secure_socket = NULL;

    // create a tcp socket
    if ( 0 == socket_tcp_create(&tcp_socket, ip_address._type, 0) ) goto failed_to_create_tcp_socket;

    // connect
    if ( 0 == socket_tcp_connect(&tcp_socket, ip_address._type, ip_address, port) ) goto failed_to_connect_tcp_socket;

    // construct a secure socket
    if ( 0 == secure_socket_construct(&p_secure_socket, tcp_socket, false, p_certificate, p_private_key) ) goto failed_to_construct_secure_socket;

    // return a pointer to the caller
    *pp_secure_socket = p_secure_socket;

    // success
    return 1;

    // error handling
    {

        // socket errors
        {
            failed_to_create_tcp_socket:
                #ifndef NDEBUG
                    log_error("[secure socket] Failed to create tcp socket in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;

            failed_to_connect_tcp_socket:
                #ifndef NDEBUG
                    log_error("[secure socket] Failed to connect tcp socket in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // destroy the socket
                socket_tcp_destroy(&tcp_socket);

                // error
                return 0;
        }

        // secure socket errors
        {
            failed_to_construct_secure_socket:
                #ifndef NDEBUG
                    log_error("[secure socket] Failed to construct secure socket in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // destroy the socket
                socket_tcp_destroy(&tcp_socket);

                // error
                return 0;
        }
    }
}


int secure_socket_construct
(
    secure_socket       **pp_secure_socket,
    socket_tcp            tcp_socket,
    bool                  is_server,
    certificate          *p_certificate,
    ed25519_private_key  *p_private_key
)
{

    // argument check
    if ( NULL == pp_secure_socket ) return 0;

    // initialized data
    secure_socket *p_secure_socket = NULL;
    
    // allocate a secure socket
    p_secure_socket = default_allocator(NULL, sizeof(secure_socket));
    if ( NULL == p_secure_socket ) return 0;

    // store the tcp socket
    p_secure_socket->tcp_socket = tcp_socket;

    // perform handshake
    if ( 0 == secure_socket_handshake(p_secure_socket, is_server, p_certificate, p_private_key) ) goto failed_to_handshake;

    // return a pointer to the caller
    *pp_secure_socket = p_secure_socket;
    
    // success
    return 1;

    // error handling
    {
        
        // secure socket errors
        {
            failed_to_handshake:
                #ifndef NDEBUG
                    log_error("[secure socket] Handshake failed in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // release the secure socket
                p_secure_socket = default_allocator(p_secure_socket, 0);

                // error
                return 0;
        }
    }
}

int secure_socket_send ( secure_socket *p_secure_socket, const void *p_data, size_t len )
{

    // argument check
    if ( NULL == p_secure_socket ) goto no_secure_socket;
    if ( NULL ==          p_data ) goto no_data;

    // initialized data
    unsigned char *p_ciphertext = NULL;
    poly1305_tag   tag          = { 0 };
    uint64_t       n_len        = (uint64_t)len;
    int            sent         = 0;

    // allocate memory for ciphertext
    p_ciphertext = default_allocator(NULL, len + sizeof(n_len) + sizeof(tag)); 
    if ( NULL == p_ciphertext ) goto no_mem;

    // encrypt, passing the length as AAD
    if ( 0 == aead_encrypt(p_ciphertext+sizeof(n_len), p_secure_socket->p_aead, tag, &n_len, sizeof(n_len), p_data, len) ) goto failed_to_encrypt;

    // store the length
    memcpy(p_ciphertext, &n_len, sizeof(n_len));
    
    // store the tag
    memcpy(p_ciphertext+sizeof(n_len)+len, tag, sizeof(tag));

    // send length prefix, ciphertext, and tag
    sent = socket_tcp_send(p_secure_socket->tcp_socket, p_ciphertext, len+sizeof(n_len)+sizeof(tag));
    
    // release ciphertext
    p_ciphertext = default_allocator(p_ciphertext, 0);

    // success
    return sent;

    // error handling
    {

        // argument errors
        {
            no_secure_socket:
                #ifndef NDEBUG
                    log_error("[secure socket] Null pointer provided for parameter \"p_secure_socket\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;

            no_data:
                #ifndef NDEBUG
                    log_error("[secure socket] Null pointer provided for parameter \"p_data\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }

        // aead errors
        {
            failed_to_encrypt:
                #ifndef NDEBUG
                    log_error("[secure socket] Failed to encrypt message in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // release ciphertext
                p_ciphertext = default_allocator(p_ciphertext, 0);

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

int secure_socket_receive ( secure_socket *p_secure_socket, void *p_buffer, size_t buffer_len )
{

    // argument check
    if ( NULL == p_secure_socket ) goto no_secure_socket;
    if ( NULL ==        p_buffer ) goto no_buffer;

    // initialized data
    uint64_t       n_len      = 0;
    poly1305_tag   tag        = { 0 };
    unsigned char *ciphertext = NULL;
    int r = 0;

    // read the length
    r = socket_tcp_receive(p_secure_socket->tcp_socket, &n_len, sizeof(n_len));
    if ( 0 == r ) return 0;
    if ( sizeof(n_len) != r ) goto failed_to_receive;

    // error check
    if ( n_len > buffer_len ) goto buffer_too_small;

    // allocate memory for the message
    ciphertext = default_allocator(NULL, (size_t)n_len);

    // read the ciphertext
    socket_tcp_receive(p_secure_socket->tcp_socket, ciphertext, (size_t)n_len);

    // read the tag
    socket_tcp_receive(p_secure_socket->tcp_socket, tag, sizeof(tag));
    
    // decrypt the message
    if ( 0 == aead_decrypt(p_buffer, p_secure_socket->p_aead, tag, &n_len, sizeof(n_len), ciphertext, (size_t)n_len) ) goto failed_to_decrypt;

    // release the ciphertext
    ciphertext = default_allocator(ciphertext, 0);

    // success
    return (int)n_len;

    // error handling
    {

        // argument errors
        {
            no_secure_socket:
                #ifndef NDEBUG
                    log_error("[secure socket] Null pointer provided for parameter \"p_secure_socket\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;

            no_buffer:
                #ifndef NDEBUG
                    log_error("[secure socket] Null pointer provided for parameter \"p_buffer\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }

        // socket errors
        {
            failed_to_receive:
                #ifndef NDEBUG
                    log_error("[secure socket] Failed to receive data in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }

        // secure socket errors
        {
            buffer_too_small:
                #ifndef NDEBUG
                    log_error("[secure socket] Buffer was too small for message in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }

        // aead errors
        {
            failed_to_decrypt:
                #ifndef NDEBUG
                    log_error("[secure socket] Failed to decrypt message in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // release the ciphertext
                ciphertext = default_allocator(ciphertext, 0);
                
                // error
                return 0;
        }
    }
}

int secure_socket_destroy ( secure_socket **pp_secure_socket )
{

    // argument check
    if ( NULL == pp_secure_socket ) goto no_secure_socket;

    // initialized data
    secure_socket *p_secure_socket = *pp_secure_socket;

    // no more pointer for caller
    *pp_secure_socket = NULL;

    // destroy AEAD
    if ( p_secure_socket->p_aead ) 
        aead_destroy(&p_secure_socket->p_aead);

    // release memory
    default_allocator(p_secure_socket, 0);

    // success
    return 1;

    // error handling 
    {

        // argument errors
        {
            no_secure_socket:
                #ifndef NDEBUG
                    log_error("[secure socket] Null pointer provided for parameter \"pp_secure_socket\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
    }
}
