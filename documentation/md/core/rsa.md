# [gsdk](../../../README.md) > [core](../core.md) > rsa

## RSA primitives 
  
 > 1 [Example](#example)
 >
 > 2 [Definitions](#definitions)
 >
 >> 2.1 [Type definitions](#type-definitions)
 >>
 >> 2.2 [Function declarations](#function-declarations)

 ## Example
 To run the example program, execute this command
 ```
 $ ./build/examples/rsa_example
 ```
 
 ## Definitions
 ### Type definitions
 ```c
// type definitions
typedef struct public_key_s  public_key;
typedef struct private_key_s private_key;
 ```
 
 ### Function declarations
 ```c 
// function declarations
/// constructors
int key_pair_construct
(
    public_key  **pp_public_key, 
    private_key **pp_private_key
);
int key_pair_from_files
(
    public_key  **pp_public_key,
    private_key **pp_private_key,
    const char   *p_public_key_path,
    const char   *p_private_key_path 
);

/// print
int print_public_key       ( public_key  *p_public_key );
int print_public_key_short ( public_key  *p_public_key );
int print_private_key      ( private_key *p_private_key );

/// reflection
int public_key_pack    ( void        *p_buffer     , public_key  *p_public_key );
int public_key_unpack  ( public_key  *p_public_key , void        *p_buffer );
int private_key_pack   ( void        *p_buffer     , private_key *p_private_key );
int private_key_unpack ( private_key *p_private_key, void        *p_buffer );

/// encryption
int rsa_encrypt
(
    void       *p_x,
    void       *p_y,
    public_key *p_public_key
);
int rsa_oaep_encrypt
(
    public_key          *p_public_key,
    const unsigned char *p_msg       , size_t msg_len,
    const unsigned char *p_label     , size_t label_len,
    unsigned char       *p_out       , size_t *p_out_len
);

/// decryption
int rsa_decrypt 
(
    void        *p_y,
    void        *p_z,
    public_key  *p_public_key,
    private_key *p_private_key
);
int rsa_oaep_decrypt 
(
    public_key          *p_public_key , private_key *p_private_key,
    const unsigned char *p_cipher_text, size_t       cipher_text_len,
    const unsigned char *p_label      , size_t       label_len,
    unsigned char       *p_out        , size_t      *p_out_len
);
 ```
