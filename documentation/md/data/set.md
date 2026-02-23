# [gsdk](../../../README.md) > [data](../data.md) > set

## Mutable, unique collection

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
 >> 5.2 [Function declarations](#function-declarations)
 
 ## Example
 To run the example program, execute this command
 ```
 $ ./build/examples/set_example
 ```

 ## Tester
 To run the tester program, execute this command
 ```
 $ ./build/tests/set_test
 ```

 ## Definitions
 ### Type definitions
 ```c
 typedef struct set_s set;
 ```
 ### Function declarations
 ```c
// allocaters
int set_create ( set **const pp_set );

// constructors
int  set_construct     ( set **const pp_set, size_t             size );
int  set_from_elements ( set **const pp_set, const void **const pp_elements, size_t size );
int  set_union         ( set **const pp_set, const set   *const p_a        , const  set *const p_b );
int  set_difference    ( set **const pp_set, const set   *const p_a        , const  set *const p_b );
int  set_intersection  ( set **const pp_set, const set   *const p_a        , const  set *const p_b );

// accessors
bool set_isdisjoint ( const set *const p_a, const set *const p_b );
bool set_issubset   ( const set *const p_a, const set *const p_b );
bool set_issuperset ( const set *const p_a, const set *const p_b );

// mutators
int  set_add                 ( set *const p_set , const void  *      p_element );
void set_discard             ( set *const p_set , void        *      p_element );
int  set_difference_update   ( set *const p_a   , const set   *const p_b );
int  set_intersection_update ( set *const p_a   , const set   *const p_b );
int  set_update              ( set *const p_a   , const set   *const p_b );
int  set_pop                 ( set *const p_set , void       **const pp_value );
int  set_remove              ( set *const p_set , void        *const p_value );

// Clear elements
int  set_clear      ( set *const p_set );
int  set_free_clear ( set *const p_set , void (*pfn_free_func) );

// Shallow copy
int  set_copy ( const set *const p_set , set **const pp_set );

// destructors
int  set_destroy ( set **const pp_set );
```