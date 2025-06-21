# json
[![CMake](https://github.com/Jacob-C-Smith/json/actions/workflows/cmake.yml/badge.svg)](https://github.com/Jacob-C-Smith/json/actions/workflows/cmake.yml)

**Dependencies:**\
[![dict](https://github.com/Jacob-C-Smith/dict/actions/workflows/cmake.yml/badge.svg)](https://github.com/Jacob-C-Smith/dict/actions/workflows/cmake.yml)
[![array](https://github.com/Jacob-C-Smith/array/actions/workflows/cmake.yml/badge.svg?branch=main)](https://github.com/Jacob-C-Smith/array/actions/workflows/cmake.yml)
[![hash-cache](https://github.com/Jacob-C-Smith/hash-cache/actions/workflows/cmake.yml/badge.svg)](https://github.com/Jacob-C-Smith/hash-cache/actions/workflows/cmake.yml) 
[![sync](https://github.com/Jacob-C-Smith/sync/actions/workflows/cmake.yml/badge.svg)](https://github.com/Jacob-C-Smith/sync/actions/workflows/cmake.yml)
[![log](https://github.com/Jacob-C-Smith/log/actions/workflows/cmake.yml/badge.svg)](https://github.com/Jacob-C-Smith/log/actions/workflows/cmake.yml)

 A JSON parser / serializer written in C. 
 
 > 0 [Try it](#try-it)
 >
 > 1 [Download](#download)
 >
 > 2 [Build](#build)
 >
 > 3 [Example](#example)
 >
 >> 3.1 [Example input](#example-input)
 >>
 >> 3.2 [Example output](#example-output)
 >
 > 4 [Tester](#tester)
 >
 > 5 [Definitions](#definitions)
 >
 >> 5.1 [Type definitions](#type-definitions)
 >>
 >> 5.2 [Function definitions](#function-definitions)
 >>
 >> 5.3 [Macro definitions](#macro-definitinos)

## Try it
[![Open in GitHub Codespaces](https://github.com/codespaces/badge.svg)](https://codespaces.new/Jacob-C-Smith/json?quickstart=1)

Wait for a few moments, then click the play button on the bottom of the window. This will run the example program.

 ## Download
 To download json, execute the following command
 ```bash
 $ git clone https://github.com/Jacob-C-Smith/json --recurse-submodules
 ```

 ## Build
 To build on UNIX like machines, execute the following commands in the same directory
 ```bash
 $ cd json
 $ cmake .
 $ make
 ```
  This will build the example program, and dynamic / shared libraries

  To build json for Windows machines, open the base directory in Visual Studio, and build your desired target(s)
 ## Example
 **NOTE: If you are running the tester program on Windows, you may have to copy example files to the ```Debug``` or ```Release``` directories.**
 The example program parses the input file, and writes the parsed tokens to stdout. To run the example program, execute this command.
 ```
 ./json_example file1.json [file2.json ... fileN.json]
 ```
 ### Example input 
 ```json
{
    "name"      : "Jacob Smith",
    "age"       : 20,
    "height"    : 1.775,
    "dog"       : {
        "name"  : "Eddie",
        "sex"   : "Male",
        "breed" : "Terrier"
    },
    "interests" : [
        "Computer science",
        "3D modeling",
        "Organic chemistry",
        "Mathematics",
        "Computer games",
        "Epistemology"
    ]
}
 ```
[example.json](example.json)

 ### Example output
 ```
--- example.json ---
{"name":"Jacob Smith","age":20,"height":1.775,"dog":{"name":"Eddie","sex":"Male","breed":"Terrier"},"interests":["Computer science","3D modeling","Organic chemistry","Mathematics","Computer games","Epistemology"]}
 ```
 [Source](main.c)
 
 ## Tester
 **NOTE: If you are running the tester program on Windows, you may have to copy the ```serial test cases``` and ```parse test cases``` directories to the ```Debug``` or ```Release``` directories.**

 To run the tester program, execute this command after building
 ```
 $ ./json_test
 ```
 [Source](json_test.c)
 
 [Tester output](test_output.txt)
 ## Definitions
 
 ### Type definitions
 ```c
 typedef struct json_value_s json_value;
 ```
 ### Function definitions

 ```c
// Parser
int json_value_parse ( char *text, char **return_pointer, json_value **const pp_value );

// Serializer
int json_value_serialize ( const json_value *const p_value, char *_buffer );
int json_value_print     ( const json_value *const p_value );
int json_value_fprint    ( const json_value *const p_value, FILE *p_f );

// Destructor
void json_value_free ( json_value *p_value );
 ```

