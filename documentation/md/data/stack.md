# [gsdk](../../../README.md) > [data](../data.md) > stack

## LIFO data structure

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
 $ ./build/examples/stack_example
 ```

 ## Definitions

 ### Type definitions
 ```c
 typedef struct stack_s stack;
 ```

 ### Function declarations

 ```c 
// constructors 
int stack_construct ( const stack **const pp_stack, size_t size );

// mutators
int stack_push ( stack *const p_stack, const void *const        p_value );
int stack_pop  ( stack *const p_stack, const void *      *const ret );

// accessors
int stack_peek ( const stack *const p_stack, const void **const ret );

// iterators
int stack_fori ( stack *const p_stack, fn_fori *pfn_fori );

// reflection
int stack_pack   ( void   *p_buffer, stack *p_stack , fn_pack   *pfn_element );
int stack_unpack ( stack **pp_stack, void  *p_buffer, fn_unpack *pfn_element );

// hash
hash64 stack_hash ( stack *p_stack, fn_hash64 *pfn_element );

// destructors
int stack_destroy ( stack **const pp_stack );
```