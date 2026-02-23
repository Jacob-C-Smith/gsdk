# json

 ## A JSON parser / serializer written in C. 
 
 > 1 [Example](#example)
 >
 >> 1.1 [Example input](#example-input)
 >>
 >> 1.2 [Example output](#example-output)
 >
 > 2 [Tester](#tester)
 >
 > 3 [Definitions](#definitions)
 >
 >> 3.1 [Type definitions](#type-definitions)
 >>
 >> 3.2 [Function declarations](#function-declarations)

 ## Example
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
 ### Function declarations

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

