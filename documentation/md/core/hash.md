# [gsdk](../../../README.md) > [core](../core.md) > hash

## Hashing functions

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

```bash
./build/examples/hash_example
```

### Example output

```
╭──────────────╮
│ hash example │
╰──────────────╯

This example hashes a string using many hashing functions. Each hash is
printed to standard out.

crc64("Hi mom!") = 0x0455209B3BBBD123
fvn64("Hi mom!") = 0x6C72AB36E527EC8C
mmh64("Hi mom!") = 0xB76CD4925A814FCA
xxh64("Hi mom!") = 0x0C51FC09DF2257FB
```

[Source](main.c)

## Tester

To run the example program, execute this command

```bash
./build/tests/hash_test
```

## Definitions

### Type definitions
```c
// Data
typedef unsigned long long hash64;
typedef struct cache_s cache;
typedef struct hash_table_s hash_table;

// Functions
typedef hash64 (fn_hash64) ( const void *const k, size_t l );
```

### Function definitions
```c
// Hashing
hash64 hash_fnv64 ( const void *const k, size_t l );
hash64 hash_mmh64 ( const void *const k, size_t l );
hash64 hash_crc64 ( const void *const k, size_t l );
```
