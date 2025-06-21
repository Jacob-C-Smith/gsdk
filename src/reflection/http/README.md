# http
[![CMake](https://github.com/Jacob-C-Smith/http/actions/workflows/cmake.yml/badge.svg)](https://github.com/Jacob-C-Smith/http/actions/workflows/cmake.yml)

 Parse and serialize HTTP requests and responses

 > 1 [Download](#download)
 >
 > 2 [Build](#build)
 >
 > 3 [Example](#example)
 >
 > 4 [Tester](#tester)
 >
 > 5 [Definitions](#definitions)
 >
 >> 5.1 [Type definitions](#type-definitions)
 >>
 >> 5.2 [Function definitions](#function-definitions)

 ## Download
 To download http, execute the following command
 ```bash
 $ git clone https://github.com/Jacob-C-Smith/http --recurse-submodules
 ```
 ## Build
 To build on UNIX like machines, execute the following commands in the same directory
 ```bash
 $ cd http
 $ cmake .
 $ make
 ```
  This will build the example program, and dynamic / shared libraries

  To build http for Windows machines, open the base directory in Visual Studio, and build your desired target(s)
 ## Example
 TODO
 
 ## Tester
 TODO

 ## Definitions
 
 ### Type definitions
 ```c
 typedef enum http_request_type_e    http_request_type;
 typedef enum http_response_status_e http_response_status;
 ```
 ### Function definitions

 ```c
 // Serializers
 int http_serialize_request (
     char              *request_text, 
     http_request_type  request_type,
     const char        *path,
     const char        *format,
     ...
 );
 
 // TODO: Implement
 // int http_serialize_response (  );

 // TODO: Parsers
 // int http_parse_request ( );
 // int http_parse_response ( );
 ```

