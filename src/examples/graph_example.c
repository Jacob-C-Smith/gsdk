/** !
 * Graph example
 *
 * @file src/examples/graph_example.c
 *
 * @author Jacob Smith
 */

// standard library
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <float.h>

// gsdk
/// core
#include <core/log.h>

/// data
#include <data/graph.h>

// structure definitions
struct airport_s 
{
    char   code[4];
    char   city[32];
};

struct flight_s 
{
    double distance;
};

// type definitions
typedef struct airport_s airport;
typedef struct flight_s  flight;

// forward declarations
/// logs
int checkpoint  ( graph *p_graph, const char *p_event );
void apsp_print ( graph *p_graph, double **pp_matrix );
void mst_print  ( graph *p_graph, graph *p_mst );

fn_key_accessor    airport_key_accessor;
fn_comparator      airport_comparator;
fn_foreach         airport_print;
fn_foreach         flight_print;
fn_weight_accessor flight_weight_accessor;

// data
/// working graph
graph *p_graph = NULL;

/// file for reflection
FILE *p_f = NULL;
size_t file_len = 0;

/// hashes
hash64 h1 = 0,
       h2 = 0;

/// example data
airport _vertices[] =
{
    { .code = "SEA", .city = "Seattle" },
    { .code = "SFO", .city = "San Francisco" },
    { .code = "LAX", .city = "Los Angeles" },
    { .code = "SLC", .city = "Salt Lake City" },
    { .code = "DEN", .city = "Denver" },
    { .code = "MSP", .city = "Minneapolis" },
    { .code = "DFW", .city = "Dallas" },
    { .code = "STL", .city = "St. Louis" },
    { .code = "ORD", .city = "Chicago" },
    { .code = "ATL", .city = "Atlanta" },
    { .code = "DCA", .city = "Washington" },
    { .code = "LGA", .city = "New York" },
    { .code = "BOS", .city = "Boston" },
    { .code = "MIA", .city = "Miami" },
};

struct 
{
    char *from,
         *to;
    flight _flight;
} 
_edges[] =
{
    { .from = "SEA", .to = "SFO", (flight) { .distance = 700.0 } },
    { .from = "SFO", .to = "LAX", (flight) { .distance = 350.0 } },
    { .from = "SEA", .to = "SLC", (flight) { .distance = 700.0 } },
    { .from = "LAX", .to = "DEN", (flight) { .distance = 850.0 } },
    { .from = "SLC", .to = "DEN", (flight) { .distance = 400.0 } },
    { .from = "SEA", .to = "MSP", (flight) { .distance = 1400.0 } },
    { .from = "LAX", .to = "DFW", (flight) { .distance = 1200.0 } },
    { .from = "DEN", .to = "MSP", (flight) { .distance = 700.0 } },
    { .from = "MSP", .to = "STL", (flight) { .distance = 450.0 } },
    { .from = "MSP", .to = "ORD", (flight) { .distance = 300.0 } },
    { .from = "STL", .to = "ORD", (flight) { .distance = 250.0 } },
    { .from = "STL", .to = "DFW", (flight) { .distance = 550.0 } },
    { .from = "ORD", .to = "BOS", (flight) { .distance = 850.0 } },
    { .from = "STL", .to = "DCA", (flight) { .distance = 700.0 } },
    { .from = "DFW", .to = "ATL", (flight) { .distance = 700.0 } },
    { .from = "BOS", .to = "LGA", (flight) { .distance = 200.0 } },
    { .from = "LGA", .to = "DCA", (flight) { .distance = 200.0 } },
    { .from = "DCA", .to = "ATL", (flight) { .distance = 550.0 } },
    { .from = "ATL", .to = "MIA", (flight) { .distance = 600.0 } },
};

// entry point
int main ( int argc, const char *argv[] )
{
    
    // unused
    (void) argc;
    (void) argv;

    // #0 - start
    checkpoint(p_graph, "start");

    // #1 - construct a weighted, directed graph 
    {

        // construct an graph
        graph_construct
        (
            &p_graph,                        // result
            GRAPH_ADJACENCY_LIST,            // storage type
            GRAPH_DIRECTED | GRAPH_WEIGHTED, // type

            sizeof(airport), // vertex size
            sizeof(flight),  // edge size

            (fn_key_accessor *)airport_key_accessor, // key accessor
            (fn_comparator *)airport_comparator      // comparator
        );

        // checkpoint
        checkpoint(p_graph, "after construct"),
        putchar('\n');
    }

    // #2 - insert vertices
    {

        // iterate over each vertex ...
        for (size_t i = 0; i < sizeof(_vertices) / sizeof(*_vertices); i++)
            
            // ... store the vertex in the graph
            graph_vertex_add(p_graph, &_vertices[i]);

        // checkpoint
        checkpoint(p_graph, "after insert vertices"),
        putchar('\n');
    }

    // #3 - insert edges
    {

        // add flights to and from airports
        for (size_t i = 0; i < sizeof(_edges) / sizeof(*_edges); i++)
            graph_edge_add(p_graph, _edges[i].from, _edges[i].to, &_edges[i]._flight),
            graph_edge_add(p_graph, _edges[i].to, _edges[i].from, &_edges[i]._flight);
    
        // checkpoint
        checkpoint(p_graph, "after insert edges"),
        putchar('\n');
    }
        
    // #4 - breadth first search
    {

        // print
        log_info("BFS from SEA:\n");

        // traverse the graph
        graph_algorithm_bfs(p_graph, "SEA", airport_print),
        putchar('\n');

        // checkpoint
        checkpoint(p_graph, "after bfs"),
        putchar('\n');
    }

    // #5 - depth first search
    {

        // print
        log_info("DFS from SEA:\n");

        // traverse the graph
        graph_algorithm_dfs(p_graph, "SEA", airport_print),
        putchar('\n');

        // checkpoint
        checkpoint(p_graph, "after dfs"),
        putchar('\n');
    }

    // #4 - destroy
    {

        // destroy the graph
        graph_destroy(&p_graph, NULL, NULL);
        
        // checkpoint
        checkpoint(p_graph, "after destroy"),
        putchar('\n');
    }

    // success
    return EXIT_SUCCESS;
}

int checkpoint ( graph *p_graph, const char *p_event )
{

    // static data
    static int step = 0;

    // print the graph
    if ( NULL == p_graph )
        log_info("#%d - Graph %s: NULL\n", step, p_event);
    else
        log_info("#%d - Graph %s:\n", step, p_event),
        printf("Vertices (%zu)\n", graph_vertex_count(p_graph)),
        printf("Edges (%zu)\n", graph_edge_count(p_graph));
        
    // increment counter
    step++;

    // success
    return 1;
}

void *airport_key_accessor ( const void *const p_value ) 
{

    // initialized data
    const airport *p_airport = p_value;

    // success
    return (void *)p_airport->code;
}

int airport_comparator ( const void *p_a, const void *p_b )
{

    // done
    return strcmp(p_b, p_a);
}

void airport_print ( void *p_element )
{

    // initialized data
    const airport *p_airport = p_element;

    // print the code and city
    printf("  %s: %s\n", p_airport->code, p_airport->city);

    // done
    return;
}

void flight_print ( void *p_element )
{

    // initialized data
    const flight *p_flight = p_element;

    // print the code and city
    printf("  %.0lf\n", p_flight->distance);

    // done
    return;
}

double flight_weight_accessor ( const void *p_edge )
{

    // initialized data
    const flight *p_flight = p_edge;

    // return the distance
    return p_flight->distance;
}
