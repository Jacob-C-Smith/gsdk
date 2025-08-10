# [gsdk](../../../README.md) > [core](../core.md) > sha

## Secure Hash Algorithm

 > 1 [Example](#example)
 >
 >> 1.1 [Example output](#example-output)
 >
 > 2 [Tester](#tester)
 >
 > 3 [Definitions](#definitions)
 >
 >> 3.1 [Type definitions](#type-definitions)
 >>
 >> 3.2 [Function definitions](#function-definitions)

 ## Example
 To run the example program, execute this command
TODO
 ### Example output
 TODO
 [Source](main.c)
## TODO: Tester

 ## Definitions
 ### Type definitions
```c
// type definitions
typedef struct sha256_state_s sha256_state;
typedef unsigned char sha256_hash[32];
```

### Function definitions
 ```c
/// constructors
int sha256_construct ( sha256_state *p_sha256_state );

/// state
int sha256_update ( sha256_state *p_sha256_state, const unsigned char *data, size_t len );
int sha256_final  ( sha256_state *p_sha256_state, unsigned char       *hash );

/// log
int sha256_print ( sha256_hash _hash );

/// hash64
hash64 sha256_hash64 ( const void *const k, unsigned long long l );

/// pack
int sha256_pack ( void *p_buffer, sha256_hash _hash );

/// unpack
int sha256_unpack ( sha256_hash *p_sha256_hash, void *p_buffer );
```
