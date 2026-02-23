# [gsdk](../../../README.md) > [core](../core.md) > sha

## Secure Hash Algorithm (256/512)

 > 1 [Example](#example)
 >
 > 2 [Tester](#tester)
 >
 > 3 [Definitions](#definitions)
 >
 >> 3.1 [Type definitions](#type-definitions)
 >>
 >> 3.2 [Function declarations](#function-declarations)

 ## Example
 To run the example program, execute this command
```
$ ./build/examples/sha_example
```

## Tester
 To run the tester program, execute this command
```
$ ./build/tests/sha_test
```

 ## Definitions
 ### Type definitions
```c
// type definitions
typedef struct sha256_state_s sha256_state;
typedef unsigned char sha256_hash[32];

typedef struct sha512_state_s sha512_state;
typedef unsigned char sha512_hash[64];
```

### Function declarations
 ```c
// function declarations
/// constructors
int sha256_construct ( sha256_state *p_sha256_state );
int sha512_construct ( sha512_state *p_sha512_state );

/// state
int sha256_update ( sha256_state *p_sha256_state, const unsigned char *data, size_t len );
int sha512_update ( sha512_state *p_sha512_state, const unsigned char *data, size_t len );
int sha256_final  ( sha256_state *p_sha256_state, unsigned char       *hash );
int sha512_final  ( sha512_state *p_sha512_state, unsigned char       *hash );

/// log
int sha256_print ( sha256_hash _hash );
int sha512_print ( sha512_hash _hash );

/// hash64
hash64 sha256_hash64 ( const void *const k, unsigned long long l );
hash64 sha512_hash64 ( const void *const k, unsigned long long l );

/// reflection
int sha256_pack   ( void        *p_buffer     , sha256_hash  _hash );
int sha512_pack   ( void        *p_buffer     , sha512_hash  _hash );
int sha256_unpack ( sha256_hash *p_sha256_hash, void        *p_buffer );
int sha512_unpack ( sha512_hash *p_sha512_hash, void        *p_buffer );
```
