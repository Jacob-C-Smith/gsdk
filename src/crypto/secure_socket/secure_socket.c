/** ! 
 * Secure socket implementation
 * 
 * @file src/crypto/secure_socket/secure_socket.c
 * 
 * @author Jacob Smith
 */

// header file
#include <crypto/secure_socket.h>

// structure definitions
struct secure_stream_state_s
{
    socket_tcp tcp_socket;
    
    struct 
    {
        aead          *p_aead; 
        unsigned char *p_buffer;
        size_t         cursor;
        size_t         frame_size;
    } send;

    struct
    {
        aead          *p_aead; 
        unsigned char *p_buffer;
        size_t         cursor;
        size_t         available;
    } receive;
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
typedef struct secure_stream_state_s secure_stream_state;

// function declarations
fn_stream_read secure_stream_read;
fn_stream_write secure_stream_write;
fn_stream_size secure_stream_size;
fn_stream_flush secure_stream_flush;
fn_stream_seek secure_stream_seek;
fn_stream_close secure_stream_close;

// function definitions
int secure_socket_handshake ( secure_stream_state *p_secure_stream_state, bool is_server, certificate *p_certificate, ed25519_private_key *p_private_key )
{
    
    // argument check
    if ( NULL == p_secure_stream_state ) goto no_secure_stream_state;
    if ( NULL ==         p_certificate ) goto no_certificate;

    // initialized data
    char                 _plain_buf[1024] = { 0 };
    x25519_private_key   priv             = { 0 };
    x25519_public_key    pub              = { 0 };
    x25519_public_key    peer_pub         = { 0 };
    x25519_shared_secret ss               = { 0 };
    sha512_state         s                = { 0 };
    sha512_hash          h1               = { 0 };
    sha512_hash          h2               = { 0 };
    chacha20_key         send_key         = { 0 };
    chacha20_nonce       send_nonce       = { 0 };
    chacha20_key         receive_key      = { 0 };
    chacha20_nonce       receive_nonce    = { 0 };

    // construct a key pair
    if ( 0 == x25519_key_pair_construct(&pub, &priv) ) goto failed_to_construct_key_pair;

    // construct a hasher
    sha512_construct(&s);
    
    // server 
    if ( is_server ) 
    {

        // initialized data
        stream             *p_stream    = NULL;
        ed25519_public_key  _public_key = { 0 };
        ed25519_signature   _signature  = { 0 };

        // construct stream
        stream_from_buffer(&p_stream, _plain_buf, sizeof(_plain_buf));

        // store the public key from the certificate
        if ( 0 == certificate_public_key_get(p_certificate, &_public_key) ) goto failed_to_get_public_key;

        // sign the public key
        if ( 0 == ed25519_sign(&_signature, (const unsigned char *)&pub, sizeof(pub), &_public_key, p_private_key) ) goto failed_to_sign_public_key;
        
        // pack the ephemeral public key
        x25519_public_key_pack(p_stream, &pub);

        // pack the signature of the ephemeral public key
        ed25519_signature_pack(p_stream, &_signature);

        // pack the certificate
        certificate_pack(p_stream, p_certificate);

        // debug
        #ifdef SECURE_SOCKET_DEBUG
            log_info("X25519    : "), x25519_public_key_print(&pub);
            log_info("ED25519   : "), ed25519_public_key_print(&_public_key);
            log_info("Signature : "), ed25519_signature_print(&_signature);
        #endif

        // receive client hello
        if ( 0 == socket_tcp_receive(p_secure_stream_state->tcp_socket, peer_pub, sizeof(x25519_public_key)) ) goto failed_to_receive_client_hello;

        // update the hasher
        sha512_update(&s, (const unsigned char *)peer_pub, sizeof(x25519_public_key));

        // send server hello
        if ( 0 == socket_tcp_send(p_secure_stream_state->tcp_socket, _plain_buf, stream_tell(p_stream)) ) goto failed_to_send_server_hello;

        // update the hasher
        sha512_update(&s, (const unsigned char *)_plain_buf, stream_tell(p_stream));

        // release the stream
        stream_destroy(&p_stream);
    }
    
    // client
    else
    {

        // initialized data
        stream             *p_stream             = NULL;
        ed25519_public_key  _server_key          = { 0 };
        ed25519_signature   _signature           = { 0 };
        certificate        *p_server_certificate = NULL;

        // send client hello
        if ( 0 == socket_tcp_send(p_secure_stream_state->tcp_socket, pub, sizeof(x25519_public_key)) ) goto failed_to_send_client_hello;

        // update the hasher
        sha512_update(&s, (const unsigned char *)pub, sizeof(x25519_public_key));
        
        // receive server hello
        if ( 0 == socket_tcp_receive(p_secure_stream_state->tcp_socket, _plain_buf, sizeof(_plain_buf)) ) goto failed_to_receive_server_hello;

        // construct stream
        stream_from_buffer(&p_stream, _plain_buf, sizeof(_plain_buf));

        // unpack the public key
        x25519_public_key_unpack(&peer_pub, p_stream);

        // unpack the signature
        ed25519_signature_unpack(&_signature, p_stream);

        // unpack the certificate
        certificate_unpack(&p_server_certificate, p_stream);

        // update the hasher
        sha512_update(&s, (const unsigned char *)_plain_buf, stream_tell(p_stream));

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

        // release the stream
        stream_destroy(&p_stream);
    }
    
    // derive the shared secret
    if ( 0 == x25519_shared_secret_derive(&priv, &peer_pub, &ss) ) goto failed_to_derive_shared_secret;

    // derive h1
    sha512_update(&s, (const unsigned char *)&ss, sizeof(x25519_shared_secret));
    sha512_final(&s, h1);

    // derive h2
    sha512_construct(&s);
    sha512_update(&s, (const unsigned char *)&h1, sizeof(sha512_hash));
    sha512_final(&s, h2);

    // zero keys
    memset(&priv, 0, sizeof(x25519_private_key)),
    memset(&peer_pub, 0, sizeof(x25519_public_key));

    // assign keys based on role
    if ( is_server )
    {
        memcpy(send_key,      h1 + 0,  32);
        memcpy(receive_key,   h1 + 32, 32);
        memcpy(send_nonce,    h2 + 0,  12);
        memcpy(receive_nonce, h2 + 12, 12);
    }
    else
    {
        memcpy(receive_key,   h1 + 0,  32);
        memcpy(send_key,      h1 + 32, 32);
        memcpy(receive_nonce, h2 + 0,  12);
        memcpy(send_nonce,    h2 + 12, 12);
    }

    // construct the aead contexts
    if ( 0 == aead_construct(&p_secure_stream_state->send.p_aead, send_key, send_nonce) ) goto failed_to_construct_aead;
    if ( 0 == aead_construct(&p_secure_stream_state->receive.p_aead, receive_key, receive_nonce) ) goto failed_to_construct_aead;
    
    // success
    return 1;

    // error handling
    {

        // argument errors
        {
            no_secure_stream_state:
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

            failed_to_receive_client_hello:
                #ifndef NDEBUG
                    log_error("[secure socket] Failed to receive client hello in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;

            failed_to_send_server_hello:
                #ifndef NDEBUG
                    log_error("[secure socket] Failed to send server hello in call to function \"%s\"\n", __FUNCTION__);
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
    secure_bridge *p_ctx    = (secure_bridge *)p_parameter;
    stream        *p_stream = NULL;

    // construct a secure stream
    if ( 0 == secure_stream_construct(&p_stream, new_tcp_sock, true, p_ctx->cert, p_ctx->priv) ) goto failed_to_construct_secure_stream;

    // done
    return p_ctx->pfn_user_callback(p_stream, ip, port, p_ctx->user_param);

    // error handling
    {

        // secure socket errors
        {
            failed_to_construct_secure_stream:
                #ifndef NDEBUG
                    log_error("[secure socket] Failed to construct secure stream in call to function \"%s\"\n", __FUNCTION__);
                #endif
                
                // release the tcp socket
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
        .cert              = p_certificate,
        .priv              = p_private_key,
        .user_param        = p_parameter
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

int secure_socket_connect
(
    stream              **pp_stream,
    socket_ip_address     ip_address,
    socket_port           port,
    certificate          *p_certificate,
    ed25519_private_key  *p_private_key
)
{

    // initialized data
    socket_tcp  tcp_socket = -1;
    stream     *p_stream = NULL;

    // create a tcp socket
    if ( 0 == socket_tcp_connect(&tcp_socket, ip_address._type, ip_address, port) ) goto failed_to_connect_tcp_socket;

    // construct a secure stream
    if ( 0 == secure_stream_construct(&p_stream, tcp_socket, false, p_certificate, p_private_key) ) goto failed_to_construct_secure_stream;

    // return a pointer to the caller
    *pp_stream = p_stream;

    // success
    return 1;

    // error handling
    {

        // socket errors
        {
            failed_to_connect_tcp_socket:
                #ifndef NDEBUG
                    log_error("[secure socket] Failed to connect tcp socket in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // release the socket
                socket_tcp_destroy(&tcp_socket);

                // error
                return 0;
        }

        // secure socket errors
        {
            failed_to_construct_secure_stream:
                #ifndef NDEBUG
                    log_error("[secure socket] Failed to construct secure stream in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // release the socket
                socket_tcp_destroy(&tcp_socket);

                // error
                return 0;
        }
    }
}

int secure_stream_construct
(
    stream              **pp_stream,
    socket_tcp            tcp_socket,
    bool                  is_server,
    certificate          *p_certificate,
    ed25519_private_key  *p_private_key
)
{

    // argument check
    if ( NULL == pp_stream ) goto no_stream;

    // initialized data
    stream              *p_stream = NULL;
    secure_stream_state *p_state  = NULL;
    void                *p_send_buffer = NULL;
    void                *p_receive_buffer = NULL;

    // allocate memory for the stream
    p_stream = default_allocator(NULL, sizeof(stream));
    if ( NULL == p_stream ) goto no_mem;

    // allocate memory for the state
    p_state = default_allocator(NULL, sizeof(secure_stream_state));
    if ( NULL == p_state ) goto no_mem;

    // allocate memory for send buffer 
    p_send_buffer = default_allocator(NULL, 16384);
    if ( NULL == p_send_buffer ) goto no_mem;

    // allocate memory for receive buffer 
    p_receive_buffer = default_allocator(NULL, 16384);
    if ( NULL == p_receive_buffer ) goto no_mem;

    // populate state
    *p_state = (secure_stream_state)
    {
        .tcp_socket = tcp_socket,
        .send = 
        {
            .p_aead     = NULL,
            .p_buffer   = p_send_buffer,
            .cursor     = 0,
            .frame_size = 16384
        },
        .receive = 
        {
            .p_aead     = NULL,
            .p_buffer   = p_receive_buffer,
            .cursor     = 0,
            .available  = 0
        }
    };

    // handshake
    if ( 0 == secure_socket_handshake(p_state, is_server, p_certificate, p_private_key) ) goto failed_to_handshake;

    // populate stream
    *p_stream = (stream)
    {
        .p_data    = p_state,
        .type      = STREAM_TYPE_BUFFER, 
        .size      = 0,
        .cursor    = 0,
        .pfn_read  = secure_stream_read,
        .pfn_write = secure_stream_write,
        .pfn_size  = secure_stream_size,
        .pfn_flush = secure_stream_flush,
        .pfn_seek  = secure_stream_seek,
        .pfn_close = secure_stream_close,
    };

    // construct a lock
    mutex_create(&p_stream->_lock);

    // return pointer to caller
    *pp_stream = p_stream;

    // success
    return 1;

    // error handling
    {

        // argument errors
        {
            no_stream:
                #ifndef NDEBUG
                    log_error("[secure socket] Null pointer provided for parameter \"pp_stream\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }

        // handshake errors
        {
            failed_to_handshake:
                #ifndef NDEBUG
                    log_error("[secure socket] Failed to handshake in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // release the aead contexts
                if ( p_state->send.p_aead ) aead_destroy(&p_state->send.p_aead);
                if ( p_state->receive.p_aead ) aead_destroy(&p_state->receive.p_aead);

                // release buffers
                p_receive_buffer = default_allocator(p_receive_buffer, 0);
                p_send_buffer = default_allocator(p_send_buffer, 0);
                p_state  = default_allocator(p_state, 0);
                p_stream = default_allocator(p_stream, 0);

                // error
                return 0;
        }

        // standard library errors
        {
            no_mem:
                #ifndef NDEBUG
                    log_error("[interfaces] Failed to allocate memory in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // release buffers
                if ( p_receive_buffer ) default_allocator(p_receive_buffer, 0);
                if ( p_send_buffer ) default_allocator(p_send_buffer, 0);
                if ( p_state ) default_allocator(p_state, 0);
                if ( p_stream ) default_allocator(p_stream, 0);

                // error
                return 0;
        }
    }
}

int secure_stream_write ( stream *p_stream, void *p_data, size_t size )
{

    // initialized data
    secure_stream_state *p_state = p_stream->p_data;
    size_t               written = 0;
    size_t               remaining = size;

    // write 
    while ( remaining > 0 )
    {
        
        // initialized data
        size_t capacity = -1;
        size_t to_write = -1;

        // flush?
        if ( p_state->send.cursor == p_state->send.frame_size )
            if ( 0 == secure_stream_flush(p_stream) ) break;

        // compute capacity
        capacity = p_state->send.frame_size - p_state->send.cursor;

        // compute remaining
        to_write = ( capacity < remaining ) ? capacity : remaining;

        // copy
        memcpy(p_state->send.p_buffer + p_state->send.cursor, (unsigned char *)p_data + written, to_write);

        // update cursor
        p_state->send.cursor += to_write;

        // update written
        written += to_write;

        // update remaining
        remaining -= to_write;
    }

    // success
    return (int) written;
}

int secure_stream_flush ( stream *p_stream )
{

    // initialized data
    secure_stream_state *p_state      = p_stream->p_data;
    unsigned char       *p_ciphertext = NULL;
    poly1305_tag         tag          = { 0 };
    uint64_t             n_len        = (uint64_t)p_state->send.cursor;

    // empty?
    if ( p_state->send.cursor == 0 ) return 1;

    // allocate memory for ciphertext
    p_ciphertext = default_allocator(NULL, p_state->send.cursor + sizeof(n_len) + sizeof(tag)); 
    if ( NULL == p_ciphertext ) goto no_mem;

    // encrypt, passing the length as AAD
    if ( 0 == aead_encrypt(p_ciphertext + sizeof(n_len), p_state->send.p_aead, tag, &n_len, sizeof(n_len), p_state->send.p_buffer, p_state->send.cursor) ) goto failed_to_encrypt;

    // store the length
    memcpy(p_ciphertext, &n_len, sizeof(n_len));
    
    // store the tag
    memcpy(p_ciphertext+sizeof(n_len)+p_state->send.cursor, tag, sizeof(tag));

    // send 
    if ( 0 == socket_tcp_send(p_state->tcp_socket, p_ciphertext, p_state->send.cursor + sizeof(n_len) + sizeof(tag)) ) goto failed_to_send;
    
    // reset cursor
    p_state->send.cursor = 0;

    // release ciphertext
    p_ciphertext = default_allocator(p_ciphertext, 0);

    // success
    return 1;

    // error handling
    {
        failed_to_encrypt:
            #ifndef NDEBUG
                log_error("[secure stream] Failed to encrypt message in call to function \"%s\"\n", __FUNCTION__);
            #endif

            // release the ciphertext buffer
            p_ciphertext = default_allocator(p_ciphertext, 0);

            // error
            return 0;

        failed_to_send:
            #ifndef NDEBUG
                log_error("[secure stream] Failed to send encrypted frame in call to function \"%s\"\n", __FUNCTION__);
            #endif

            // release the ciphertext buffer
            p_ciphertext = default_allocator(p_ciphertext, 0);

            // error
            return 0;

        no_mem:
            #ifndef NDEBUG
                log_error("[secure stream] Failed to allocate memory in call to function \"%s\"\n", __FUNCTION__);
            #endif

            // error
            return 0;
    }
}

int secure_stream_read ( stream *p_stream, void *p_data, size_t size )
{

    // initialized data
    secure_stream_state *p_state = p_stream->p_data;
    size_t               to_read = 0;
    unsigned char       *ciphertext = NULL;

    // read a new frame
    if ( p_state->receive.available == 0 )
    {

        // initialized data
        uint64_t       n_len = 0;
        poly1305_tag   tag   = { 0 };
        int            r     = 0;

        // read the length
        r = socket_tcp_receive(p_state->tcp_socket, &n_len, sizeof(n_len));
        
        // EOF?
        if ( 0 == r ) return 0;

        // error check
        if ( sizeof(n_len) != r ) goto failed_to_receive;
        
        // error check frame bounds
        if ( n_len > p_state->send.frame_size ) goto frame_too_large;

        // allocate memory for ciphertext
        ciphertext = default_allocator(NULL, (size_t)n_len);
        if ( NULL == ciphertext ) goto no_mem;

        // read the ciphertext
        if ( 0 == socket_tcp_receive(p_state->tcp_socket, ciphertext, (size_t)n_len) ) goto failed_to_receive_ciphertext;

        // read the tag
        if ( 0 == socket_tcp_receive(p_state->tcp_socket, tag, sizeof(tag)) ) goto failed_to_receive_tag;

        // decrypt the message into the receive buffer
        if ( 0 == aead_decrypt(p_state->receive.p_buffer, p_state->receive.p_aead, tag, &n_len, sizeof(n_len), ciphertext, (size_t)n_len) ) goto failed_to_decrypt;

        // release the ciphertext buffer
        ciphertext = default_allocator(ciphertext, 0);

        // update state
        p_state->receive.available = (size_t)n_len;
        p_state->receive.cursor    = 0;
    }

    // copy from receive buffer
    to_read = ( size < p_state->receive.available ) ? size : p_state->receive.available;
    memcpy(p_data, p_state->receive.p_buffer + p_state->receive.cursor, to_read);

    // update state
    p_state->receive.cursor += to_read;
    p_state->receive.available -= to_read;

    // update stream cursor
    p_stream->cursor += to_read;

    // success
    return (int)to_read;

    // error handling
    {
        failed_to_receive:
        failed_to_receive_ciphertext:
        failed_to_receive_tag:
            #ifndef NDEBUG
                log_error("[secure stream] Failed to receive data in call to function \"%s\"\n", __FUNCTION__);
            #endif

            // release the ciphertext buffer
            if ( ciphertext ) 
                ciphertext = default_allocator(ciphertext, 0);

            // error
            return 0;

        frame_too_large:
            #ifndef NDEBUG
                log_error("[secure stream] Frame size exceeded maximum in call to function \"%s\"\n", __FUNCTION__);
            #endif

            // error
            return 0;

        failed_to_decrypt:
            #ifndef NDEBUG
                log_error("[secure stream] Failed to decrypt message (cryptographic failure) in call to function \"%s\"\n", __FUNCTION__);
            #endif

            // release the ciphertext buffer
            if ( ciphertext ) 
                ciphertext = default_allocator(ciphertext, 0);

            // error
            return 0;

        no_mem:
            #ifndef NDEBUG
                log_error("[secure stream] Failed to allocate memory in call to function \"%s\"\n", __FUNCTION__);
            #endif

            // error
            return 0;
    }
}

int secure_stream_size ( stream *p_stream )
{

    // unused
    (void) p_stream;

    // error
    return -1;
}

int secure_stream_seek ( stream *p_stream, long offset, enum stream_seek_e whence )
{

    // unused
    (void) p_stream;
    (void) offset;
    (void) whence;

    // error
    return 0;
}

int secure_stream_close ( stream *p_stream )
{

    // initialized data
    secure_stream_state *p_state = p_stream->p_data;

    // flush the stream
    secure_stream_flush(p_stream);

    // destroy AEAD contexts
    aead_destroy(&p_state->send.p_aead);
    aead_destroy(&p_state->receive.p_aead);

    // release buffers
    p_state->send.p_buffer = default_allocator(p_state->send.p_buffer, 0);
    p_state->receive.p_buffer = default_allocator(p_state->receive.p_buffer, 0);

    // destroy tcp socket
    socket_tcp_destroy(&p_state->tcp_socket);

    // release state
    p_state = default_allocator(p_state, 0);

    // clear pointer
    p_stream->p_data = p_state;

    // success
    return 1;
}
