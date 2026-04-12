# [gsdk](../../../README.md) > [core](../core.md) > AEAD

## Authenticated Encryption with Associated Data
 
 > 1 [Examples](#examples)
 >
 >> 1.1 [ChaCha20](#chacha20)
 >>
 >> 1.2 [Poly1305](#poly1305)
 >>
 >> 1.3 [AEAD](#aead)
 >
 > 2 [Definitions](#definitions)
 >
 >> 2.1 [Preprocessor definitions](#preprocessor-definitions)
 >>
 >> 2.1 [Type definitions](#type-definitions)
 >>
 >> 2.2 [Function declarations](#function-declarations)

 ## Examples
 ### ChaCha20
 To run the example program, execute the following command
 ```bash
 $ ./build/examples/chacha20_example
 ```

 ### Poly1305
 To run the example program, execute the following command
 ```bash
 $ ./build/examples/poly1305_example
 ```

 ### AEAD
 To run the example program, execute the following command
 ```bash
 $ ./build/examples/aead_example
 ```

 ## Definitions
 ### Preprocessor definitions
```c
// preprocessor definitions
#define CHACHA20_DEBUG // verbose ChaCha20 logging
#define POLY1305_DEBUG // verbose Poly1305 logging
```

 ### Type definitions
```c
// type definitions
/// chacha20
typedef unsigned int chacha20_key  [8];
typedef unsigned int chacha20_nonce[3];
typedef unsigned int chacha20_state[16];
typedef struct chacha20_s chacha20;

/// poly1305
typedef unsigned char poly1305_one_time_key[32];
typedef unsigned char poly1305_tag         [16];

/// aead
typedef struct aead_s aead;
```

 ### Function declarations
 #### ChaCha20 function declarations
 ```c
// function declarations
/// constructors
int chacha20_construct
(
    chacha20       **pp_chacha20, 
    chacha20_key     key, 
    chacha20_nonce   nonce,
    int              block
);

/// encrypt
int chacha20_encrypt ( void *p_ciphertext, chacha20 *p_chacha20, void *p_plaintext, size_t len );

/// decrypt
int chacha20_decrypt ( void *p_plaintext, chacha20 *p_chacha20, void *p_ciphertext, size_t len );

/// seek
int chacha20_seek ( chacha20 *p_chacha20, unsigned int block );

/// tell
int chacha20_tell ( chacha20 *p_chacha20, unsigned int *p_block );

/// round
int chacha20_round ( chacha20 *p_chacha20 );

/// setup
int chacha20_setup ( chacha20 *p_chacha20 );

/// mutators
int chacha20_nonce_set ( chacha20 *p_chacha20, chacha20_nonce nonce );

/// accessors
int chacha20_state_get ( chacha20_state *p_state, chacha20 *p_chacha20 );

/// destructors
int chacha20_destroy ( chacha20 **pp_chacha20 );
 ```

 #### Poly1305 function declarations
 ```c
// function declarations
/// message authentication
int poly1305_mac 
(
    const unsigned char   *p_message, 
    size_t                 len, 
    poly1305_tag           tag, 
    poly1305_one_time_key  key
);

/// key generation
int poly1305_key_generate
(
    poly1305_one_time_key *p_key, 
    chacha20_key           key, 
    chacha20_nonce         nonce,
    int                    block
);
 ```

 #### AEAD function declarations
 ```c
// function declarations
/// constructors
int aead_construct
(
    aead           **pp_aead,
    chacha20_key     key,
    chacha20_nonce   nonce
);

/// encrypt
int aead_encrypt
(
    void         *p_ciphertext,
    aead         *p_aead,
    poly1305_tag  tag,

    const void *p_aad,       size_t aad_len,
    const void *p_plaintext, size_t len
);

/// seek
int aead_sequence_set ( aead *p_aead, size_t sequence );

/// decrypt
int aead_decrypt
( 
    void         *p_plaintext, 
    aead         *p_aead, 
    poly1305_tag  tag, 
    
    void *p_aad       , size_t aad_len, 
    void *p_ciphertext, size_t len
);

/// destructors
int aead_destroy ( aead **pp_aead );
 ```
