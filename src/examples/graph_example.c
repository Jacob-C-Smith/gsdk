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

    graph_construct(&p_graph, GRAPH_TYPE_UNWEIGHTED_DIRECTED, GRAPH_STORAGE_TYPE_ADJACENCY_MATRIX);

    graph_vertex_add(&p_graph, "A", 0xffffffffffffffff);
    graph_vertex_add(&p_graph, "B", 0xffffffffffffffff);
    graph_vertex_add(&p_graph, "C", 0xffffffffffffffff);

    // Print the graph
    graph_info(p_graph);

    /*
    for (size_t i = 0; i < 4; i++)
    {

        printf("=== %s ===\n", GRAPH_TYPE_STRING(i));

        for (size_t j = 2; j < 4; j++)
        {   

            // Construct a graph
            graph_construct(&p_graph, GRAPH_TYPE_UNWEIGHTED_DIRECTED, GRAPH_STORAGE_TYPE_ADJACENCY_MATRIX);

            // Storage
            {

                
                // Add a few edges
                //

                // graph_edge_add(p_graph, "1 -> 2", "1", "2");

                // Test node adjacency
                // 

                //

                // Remove verts

                // Remove edges
            }

            // Operations
            {
                
                // Depth first search
                graph_depth_first_search(p_graph, "a", "b");
                
                // Breadth first search
                graph_breadth_first_search(p_graph, "a", "b");

                // Minimum spanning tree
                graph_minimum_spanning_tree(p_graph);

                // Cycle finder
                graph_cycle_finder(p_graph);
                
                graph_single_source_shortest_path(p_graph);

                graph_all_pairs_shortest_path(p_graph);

                graph_maximum_flow(p_graph);
            }

            // Serialize the graph
            //
            
            //graph_serialize_json(p_graph);

            // Destruct a graph
            //

            //graph_destruct(&p_graph);

            putchar('\n');
        }
    }
    
    */
    // success
    return EXIT_SUCCESS;
}