// standard library
#include <stdio.h>
#include <stdlib.h>

// gsdk
#include <data/set.h>

void print_element ( void *const p_string, size_t index )
{
    printf("%s, ", p_string);
}

// entry point
int main ( int argc, const char* argv[] )
{

    // initialized data
    set *p_a                = (void *) 0;
    set *p_b                = (void *) 0;
    set *p_a_union_b        = (void *) 0;
    set *p_a_difference_b   = (void *) 0;
    set *p_a_intersection_b = (void *) 0;
    
    // Construct a set with 4 elements
    if ( set_construct(&p_a, 4, (fn_equality *)strcmp) == 0 ) goto failed_to_construct_set;
    
    // Construct a set with 4 elements
    if ( set_construct(&p_b, 4, (fn_equality *)strcmp) == 0 ) goto failed_to_construct_set;

    // Add elements to set a
    set_add(p_a, "6");
    set_add(p_a, "1");
    set_add(p_a, "5");
    set_add(p_a, "9");
    
    // Add elements to set b
    set_add(p_b, "5");
    set_add(p_b, "9");
    set_add(p_b, "8");
    set_add(p_b, "3");

    // Compute the union of set a and set b
    set_union(&p_a_union_b, p_a, p_b, (fn_equality *)strcmp);
    
    // Compute the difference of set a and set b
    set_difference(&p_a_difference_b, p_a, p_b, (fn_equality *)strcmp);
    
    // Compute the intersection of set a and set b
    set_intersection(&p_a_intersection_b, p_a, p_b, (fn_equality *)strcmp);

    // Print each set
    printf("a:                { ");
    set_foreach_i(p_a, &print_element);
    
    printf("}\nb:                { ");
    set_foreach_i(p_b, &print_element);

    printf("}\na union b:        { ");
    set_foreach_i(p_a_union_b, &print_element);

    printf("}\na difference b:   { ");
    set_foreach_i(p_a_difference_b, &print_element);

    printf("}\na intersection b: { ");
    set_foreach_i(p_a_intersection_b, &print_element);
    printf("}\n ");

    // Destruct the sets
    if ( set_destroy(&p_a)                == 0 ) goto failed_to_destroy_set;
    if ( set_destroy(&p_b)                == 0 ) goto failed_to_destroy_set;
    if ( set_destroy(&p_a_union_b)        == 0 ) goto failed_to_destroy_set;
    if ( set_destroy(&p_a_difference_b)   == 0 ) goto failed_to_destroy_set;
    if ( set_destroy(&p_a_intersection_b) == 0 ) goto failed_to_destroy_set;

    // success
    return EXIT_SUCCESS;

    // error handling
    {

        // Set errors
        {
            failed_to_construct_set:
                printf("Failed to construct set!\n");

                // error
                return EXIT_FAILURE;

            failed_to_destroy_set:
                printf("Failed to destroy set!\n");

                // error
                return EXIT_FAILURE;
        }
    }
}