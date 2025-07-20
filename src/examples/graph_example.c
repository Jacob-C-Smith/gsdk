/** !
 * Graph example
 * 
 * @file main.c
 * 
 * @author Jacob Smith
 */

// standard library
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <graph/graph.h>

// preprocessor macros
#define GRAPH_TYPE(storage, type) ( storage << 2 | type )
#define GRAPH_TYPE_STRING(type) ( _graph_type_strings[type] )
#define GRAPH_STORAGE_STRING(storage) ( _graph_storage_type_strings[storage] )

int main ( int argc, const char *argv[] )
{

    graph *p_graph = (void *) 0;

    graph_construct(&p_graph,
        GRAPH_TYPE_UNWEIGHTED_DIRECTED,
        GRAPH_STORAGE_TYPE_ADJACENCY_MATRIX
    );

    graph_vertex_add(p_graph, "A", 0xffffffffffffffff);
    graph_vertex_add(p_graph, "B", 0xffffffffffffffff);
    graph_vertex_add(p_graph, "C", 0xffffffffffffffff);

    // Print the graph
    graph_info(p_graph);

    // success
    return EXIT_SUCCESS;
}