# [gsdk](../../../README.md) > [core](../core.md) > secure socket

## Authenticated and encrypted network communication
 
 > 1 [Examples](#examples)
 >
 > 2 [Definitions](#definitions)
 >
 >> 2.1 [Type definitions](#type-definitions)
 >>
 >> 2.2 [Function declarations](#function-declarations)

 ## Examples
 To run the example program, execute the following commands

 ```bash
 $ ./src/utilities/certificate.sh       # generate certificates
 ```

 ```bash
 $ ./build/utilities/secure_time_server 
 ```

 ```bash
 $ ./build/utilities/secure_time_client 
 ```

 ## Definitions

 ### Type definitions
```c
// type definitions
/// certificate
typedef struct certificate_s certificate;

/// secure socket
typedef struct secure_socket_s secure_socket;
typedef int(fn_secure_socket_accept)( secure_socket *p_secure_socket, socket_ip_address ip_address, socket_port port_number, void *const p_parameter );
```

 ### Function declarations
 #### certificate function declarations
 ```c
// function declarations
/// constructor
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
int certificate_print ( certificate *p_certificate );

/// accessors
int certificate_get_subject    ( certificate *p_certificate, const char         **pp_subject );
int certificate_public_key_get ( certificate *p_certificate, ed25519_public_key  *p_public_key );

/// sign
int certificate_sign ( certificate *p_certificate, ed25519_public_key *p_public_key, ed25519_private_key *p_private_key );

/// verify
int certificate_verify       ( certificate  *p_certificate, certificate *p_issuer );
int certificate_chain_verify ( certificate **pp_chain     , size_t       count   , certificate *p_trust_root );

/// reflection
int certificate_pack   ( void         *p_buffer      , certificate *p_certificate );
int certificate_unpack ( certificate **pp_certificate, void        *p_buffer );

/// destructor
int certificate_destroy ( certificate **pp_certificate );
 ```

 #### secure socket function declarations
 ```c
// function declarations
/// constructor
int secure_socket_construct
(
    secure_socket       **pp_secure_socket,
    socket_tcp            tcp_socket,
    bool                  is_server,
    certificate          *p_certificate,
    ed25519_private_key  *p_private_key
);

/// connect
int secure_socket_connect
(
    secure_socket       **pp_secure_socket,
    socket_ip_address     ip_address,
    socket_port           port,
    certificate          *p_certificate,
    ed25519_private_key  *p_private_key
);

/// listen
int secure_socket_listen
(
    socket_tcp               tcp_server_socket,
    fn_secure_socket_accept *pfn_callback,
    certificate             *p_certificate,
    ed25519_private_key     *p_private_key,
    void                    *p_parameter
);

/// send 
int secure_socket_send ( secure_socket *p_secure_socket, const void *p_data, size_t len );

/// receive
int secure_socket_receive ( secure_socket *p_secure_socket, void *p_buffer, size_t buffer_len );

/// destructor
int secure_socket_destroy ( secure_socket **pp_secure_socket );
 ```
