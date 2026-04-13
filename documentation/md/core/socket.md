# [gsdk](../../../README.md) > [core](../core.md) > socket

## Cross platform networking primatives
 
 > 1 [Examples](#examples)
 >
 >> 1.1 [TCP](#tcp)
 >>
 >> 1.2 [UDP](#udp)
 >
 > 2 [Definitions](#definitions)
 >
 >> 2.1 [Type definitions](#type-definitions)
 >>
 >> 2.2 [Function declarations](#function-declarations)

 ## Examples
 ### TCP
 To run the example program, execute the following commands
 
 ```bash
 $ ./build/utilities/time_server 
 ```

 ```bash
 $ ./build/utilities/time_client 
 ```

 ### UDP

 ```bash
 $ ./build/utilities/echo_server 
 ```

 ```bash
 $ ./build/utilities/echo_client 
 ```

 ## Definitions

 ### Type definitions
```c
// type definitions
/// socket
typedef struct socket_ip_address_s socket_ip_address;
typedef unsigned short socket_port;

/// tcp
typedef int socket_tcp;
typedef int(*fn_socket_tcp_accept)( socket_tcp _socket_tcp, socket_ip_address ip_address, socket_port port_number, void *const p_parameter );

/// udp
typedef int socket_udp;
typedef int(*fn_socket_udp_accept)( socket_udp _socket_udp, socket_ip_address ip_address, socket_port port_number, void *const p_parameter );
```

 ### Function declarations
 #### socket function declarations
 ```c
// function declarations
/// resolve
int socket_resolve_host ( socket_ip_address *p_ip_addresses, size_t limit, const char *restrict p_hostname );

/// print
int socket_ip_address_print ( socket_ip_address ip_address );
 ```

 #### tcp function declarations
 ```c
// function declarations
/// construct
int socket_tcp_create ( socket_tcp *const p_socket_tcp, enum socket_address_family_e address_family, socket_port port_number );

/// listen
int socket_tcp_listen ( socket_tcp _socket_tcp, fn_socket_tcp_accept pfn_tcp_accept_callback, void *const p_parameter );

/// receive
int socket_tcp_receive ( socket_tcp _socket_tcp, void *const p_buffer, size_t buffer_len );

/// send
int socket_tcp_send ( socket_tcp _socket_tcp, const void *const p_buffer, size_t buffer_len );

/// connect
int socket_tcp_connect ( socket_tcp *const p_socket_tcp, enum socket_address_family_e address_family, socket_ip_address ip_address, socket_port port_number );

/// destroy
int socket_tcp_destroy ( socket_tcp *p_socket_tcp );
 ```

 #### udp function declarations
 ```c
// function declarations
/// construct
int socket_udp_create ( socket_udp *const p_socket_udp, enum socket_address_family_e address_family, socket_port port_number );

/// listen
int socket_udp_listen ( socket_udp _socket_udp, fn_socket_udp_accept pfn_udp_accept_callback, void *const p_parameter );

/// receive
int socket_udp_receive_from ( socket_udp _socket_udp, void *const p_buffer, size_t buffer_len, socket_ip_address *const p_ip_address, socket_port *const p_port );

/// send
int socket_udp_send_to ( socket_udp _socket_udp, const void *const p_buffer, size_t buffer_len, socket_ip_address ip_address, socket_port port );

/// connect
int socket_udp_connect ( socket_udp *const p_socket_udp, enum socket_address_family_e address_family, socket_ip_address ip_address, socket_port port_number );

/// destroy
int socket_udp_destroy ( socket_udp *p_socket_udp );
 ```
