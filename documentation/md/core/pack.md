# [gsdk](../../../README.md) > [core](../core.md) > pack

## Pack and unpack binary data

 > 0 [Motivation](#motivation)
 >
 > 1 [Example](#example)
 >
 > 2 [Tester](#tester)
 >
 > 3 [Definitions](#definitions)
 >
 >> 3.1 [Type definitions](#type-definitions)
 >>
 >> 3.2 [Function declarations](#function-declarations)

## Motivation
 I wanted a mechanism to reflect binary data, and format strings fit the bill. 

### Format specifier prototype
The format specifiers prototype I came up with is 

```
%[length]specifier[bits]
```

#### length
The length defines a quantity of specifiers that are to be read sequentially

#### specifiers
| specifier | output  | example        |
| --------- | ------- | -------------- |
| i         | integer | 123            |
| f         | float   | 3.1415         |  
| s         | string  | "Hello mother" |

#### bits
| bits   | i                | f            |  
| ------ | ---------------  | ------------ |
| 8      | ```char```       | ❌           |
| 16     | ```short```      | ❌           |
| 32     | ```long```       | ```float```  |
| 64     | ```long long```  | ```double``` |

#### Format string examples

| format string        | Description                                              |
| -------------------- | -------------------------------------------------------- | 
| ```%3s```            | Three sequential strings                                 |
| ```%3i8```           | Three sequential characters                              |
| ```%i16 %i32 %i64``` | A short, followed by a long, followed by a long long     |
| ```%3f32```          | Three sequential floats, suitable for a vec3 type        |
| ```%2f64```          | Two sequential doubles, suitable for an imaginary number |
        

## Example
 To run the example program, execute this command
 ```
 $ ./build/examples/pack_example
 ```

## Tester
 To run the tester program, execute this command
 ```
 $ ./build/tests/pack_test
```

## Definitions
### Type definitions
```c
// type definitions
typedef int (fn_pack)   ( void *p_buffer, const void *const p_value );
typedef int (fn_unpack) ( void *p_value, void *p_buffer );
```

### Function declarations
```c 
// function declarations
/// pack
size_t pack_pack ( void *p_buffer, const char *restrict format, ... );

/// unpack
size_t pack_unpack ( void *p_buffer, const char *restrict format, ... );
```
