# [gsdk](../../../README.md) > [core](../core.md) > digital signature

## Sign and Verify data

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
 $ ./build/examples/digital_signature_example
 ```

 ## Definitions
 ### Function declarations
 ```c
// function declarations
/// sign
int digital_signature_sign 
(
    private_key  *p_private_key, 
    public_key   *p_public_key, 
    const void   *p_data, 
    size_t        data_size, 
    void        **pp_signature
);

/// verify
int digital_signature_verify 
( 
    public_key *      p_public_key, 
    const void *const p_data, 
    size_t            data_size,
    const void *const p_signature
);
```
