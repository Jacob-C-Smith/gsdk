#include <graph/graph.h>

// graph
#include <data/graph_prototypes.h>

// preprocessor macros
#define GRAPH_TYPE(storage, type) ( storage << 2 | type )
#define GRAPH_TYPE_STRING(type) ( _graph_type_strings[type] )
#define GRAPH_STORAGE_STRING(storage) ( _graph_storage_type_strings[storage] )

int graph_construct ( graph **pp_graph,
    enum graph_type_e _type,
    enum graph_type_e _storage
)
{

    // argument check
    if ( pp_graph ==                 (void *) 0 ) goto no_graph;
    if ( _storage == GRAPH_STORAGE_TYPE_INVALID ) goto invalid_storage_type;

    // initialized data
    graph *p_graph = realloc(0, sizeof(graph));

    // Clone the graph 
    memcpy(p_graph, &_graph_prototypes[ GRAPH_TYPE(_type, _storage) ], sizeof(graph));

    // return a pointer to the caller
    *pp_graph = p_graph; 

    // success
    return 1;

    no_graph:
    invalid_storage_type:
        return 0;
}

int graph_vertex_add ( graph *p_graph, void *p_value )
{
    log_info("[graph] %s(%p) --> ", __FUNCTION__, p_value);
    
    int ret = p_graph->storage.pfn_graph_vertex_add(p_graph, p_value);

    return ret;
}

int graph_depth_first_search ( graph *p_graph, char _a[], char _b[] )
{

    if ( p_graph->operations.pfn_graph_depth_first_search == (void *) 0 ) goto no_depth_first_search_implemented;

    int ret = p_graph->operations.pfn_graph_depth_first_search(p_graph, _a, _b);
    
    // success
    return ret;

    no_depth_first_search_implemented:
        log_error("[graph] [search] [dfs] No depth first search implementation for %s in call to function \"%s\"\n", GRAPH_TYPE_STRING(p_graph->_type), __FUNCTION__);
        return 0;
}

int graph_breadth_first_search ( graph *p_graph, char _a[], char _b[] )
{

    // argument check
    if ( p_graph == (void *) 0 ) goto no_graph;

    // Is BFS available?
    if ( p_graph->operations.pfn_graph_breadth_first_search == (void *) 0 ) goto no_breadth_first_search_implemented;

    // success
    return p_graph->operations.pfn_graph_breadth_first_search(p_graph, _a, _b);
    
    // error handling
    {

        // argument errors
        {
            no_graph:
                #ifndef NDEBUG
                    log_error("[graph] Null pointer provided for parameter \"p_graph\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }

        // Graph errors
        {
            no_breadth_first_search_implemented:
                #ifndef NDEBUG
                    log_error("[graph] [search] [bfs] No breadth first search implementation for %s in call to function \"%s\"\n", GRAPH_TYPE_STRING(p_graph->_type), __FUNCTION__);
                #endif

                // error
                return 0;
        }
    }
}

int graph_minimum_spanning_tree ( graph *p_graph )
{

    if ( p_graph->operations.pfn_graph_minimum_spanning_tree == (void *) 0 ) goto no_minimum_spanning_tree_implemented;

    int ret = p_graph->operations.pfn_graph_minimum_spanning_tree(p_graph);
    
    // success
    return ret;

    no_minimum_spanning_tree_implemented:
                log_error("[graph] [span] Minimum Spanning Trees are unsupported for %s graphs in call to function \"%s\"\n", GRAPH_TYPE_STRING(p_graph->_type), __FUNCTION__);
        return 0;
}

int graph_cycle_finder ( graph *p_graph )
{

    // argument check
    if ( p_graph == (void *) 0 ) goto no_graph;

    // Is BFS available?
    if ( p_graph->operations.pfn_graph_cycle_finder == (void *) 0 ) goto no_cycle_finder_implemented;

    // success
    return p_graph->operations.pfn_graph_cycle_finder(p_graph);
    
    // error handling
    {

        // argument errors
        {
            no_graph:
                log_error("[graph] Null pointer provided for parameter \"p_graph\" in call to function \"%s\"\n", __FUNCTION__);

                // error
                return 0;
        }

        // Graph errors
        {
            no_cycle_finder_implemented:
                log_error("[graph] [cycle] Cycle finder is unsupported for %s graphs in call to function \"%s\"\n", GRAPH_TYPE_STRING(p_graph->_type), __FUNCTION__);

                // error
                return 0;
        }
    }
}

int graph_single_source_shortest_path ( graph *p_graph )
{

    // argument check
    if ( p_graph == (void *) 0 ) goto no_graph;

    // Is BFS available?
    if ( p_graph->operations.pfn_graph_single_source_shortest_path == (void *) 0 ) goto no_single_source_shortest_path_implemented;

    // success
    return p_graph->operations.pfn_graph_single_source_shortest_path(p_graph);
    
    // error handling
    {

        // argument errors
        {
            no_graph:
                log_error("[graph] Null pointer provided for parameter \"p_graph\" in call to function \"%s\"\n", __FUNCTION__);

                // error
                return 0;
        }

        // Graph errors
        {
            no_single_source_shortest_path_implemented:
                log_error("[graph] [sssp] Single source shortest path finder is unsupported for %s graphs in call to function \"%s\"\n", GRAPH_TYPE_STRING(p_graph->_type), __FUNCTION__);

                // error
                return 0;
        }
    }
}

int graph_all_pairs_shortest_path ( graph *p_graph )
{

    // argument check
    if ( p_graph == (void *) 0 ) goto no_graph;

    // Is All Pairs Shortest Paths available?
    if ( p_graph->operations.pfn_graph_all_pairs_shortest_path == (void *) 0 ) goto no_all_pairs_shortest_path_implemented;

    // success
    return p_graph->operations.pfn_graph_all_pairs_shortest_path(p_graph);
    
    // error handling
    {

        // argument errors
        {
            no_graph:
                log_error("[graph] Null pointer provided for parameter \"p_graph\" in call to function \"%s\"\n", __FUNCTION__);

                // error
                return 0;
        }

        // Graph errors
        {
            no_all_pairs_shortest_path_implemented:
                log_error("[graph] [apsp] All pairs shortest paths is unsupported for %s graphs in call to function \"%s\"\n", GRAPH_TYPE_STRING(p_graph->_type), __FUNCTION__);

                // error
                return 0;
        }
    }
}

int graph_maximum_flow ( graph *p_graph )
{

    // argument check
    if ( p_graph == (void *) 0 ) goto no_graph;

    // Is Maximum Flow available?
    if ( p_graph->operations.pfn_graph_maximum_flow == (void *) 0 ) goto no_maximum_flow_implemented;

    // success
    return p_graph->operations.pfn_graph_maximum_flow(p_graph);
    
    // error handling
    {

        // argument errors
        {
            no_graph:
                log_error("[graph] Null pointer provided for parameter \"p_graph\" in call to function \"%s\"\n", __FUNCTION__);

                // error
                return 0;
        }

        // Graph errors
        {
            no_maximum_flow_implemented:
                log_error("[graph] [flow] Maximum flow is unsupported for %s graphs in call to function \"%s\"\n", GRAPH_TYPE_STRING(p_graph->_type), __FUNCTION__);

                // error
                return 0;
        }
    }
}

int graph_info ( graph *p_graph )
{
    
    // argument check
    if ( p_graph == (void *) 0 ) goto no_graph;
    
    // Print the graph properties
    printf("%s %s graph\n", GRAPH_TYPE_STRING(p_graph->_type), GRAPH_STORAGE_STRING(p_graph->_storage_type));

    // success
    return p_graph->operations.pfn_graph_info(p_graph);
    
    // error handling
    {

        // argument errors
        {
            no_graph:
                log_error("[graph] Null pointer provided for parameter \"p_graph\" in call to function \"%s\"\n", __FUNCTION__);

                // error
                return 0;
        }
    }
}

int graph_destruct  ( graph **pp_graph )
{

    // argument check
    if ( pp_graph  == (void *) 0 ) goto no_graph;
    if ( *pp_graph == (void *) 0 ) goto invalid_storage_type;

    // initialized data
    //graph *p_graph = realloc(0, sizeof(graph));


    // return a pointer to the caller
    *pp_graph = (void *) 0; 

    // success
    return 1;

    no_graph:
    invalid_storage_type:
        return 0;
}
