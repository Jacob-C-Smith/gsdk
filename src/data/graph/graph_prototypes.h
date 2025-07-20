/** !
 * Graph prototypes
 * 
 * @file include/graph/graph_prototypes.h
 * 
 * @author Jacob Smith
 */
#pragma once

// graph
#include <graph/graph.h>
#include <data/adjacency_matrix.h>
#include <data/adjacency_list.h>
// #include <graph/graph_edge_list.h>

// preprocessor macros
#define GRAPH_TYPE(storage, type) ( storage << 2 | type )
#define GRAPH_TYPE_STRING(type) ( _graph_type_strings[type] )
#define GRAPH_STORAGE_STRING(storage) ( _graph_storage_type_strings[storage] )

static int graph_prototype_dfs_stub ( graph *p_graph )
{

    // Log the warning
    log_warning("[graph] [dfs] Operation not implemented\n");

    // Failure
    return 0;
}

static int graph_prototype_bfs_stub ( graph *p_graph )
{

    // Log the warning
    log_warning("[graph] [bfs] Operation not implemented\n");

    // Failure
    return 0;
}

static int graph_prototype_span_stub ( graph *p_graph )
{

    // Log the warning
    log_warning("[graph] [span] Operation not implemented\n");

    // Failure
    return 0;
}

static int graph_prototype_cycle_stub ( graph *p_graph )
{

    // Log the warning
    log_warning("[graph] [cycle] Operation not implemented\n");

    // Failure
    return 0;
}

static int graph_prototype_sssp_stub ( graph *p_graph )
{

    // Log the warning
    log_warning("[graph] [sssp] Operation not implemented\n");

    // Failure
    return 0;
}

static int graph_prototype_apsp_stub ( graph *p_graph )
{

    // Log the warning
    log_warning("[graph] [apsp] Operation not implemented\n");

    // Failure
    return 0;
}

static int graph_prototype_max_flow_operation ( graph *p_graph )
{

    // Log the warning
    log_warning("[graph] [flow] Operation not implemented\n");

    // Failure
    return 0;
}

static int graph_prototype_unsupported_operation ( graph *p_graph )
{

    // Log the warning
    log_error("[graph] Unsupported operation\n");

    // Failure
    return 0;
}


// Prototypes
graph _graph_prototypes [] =
{

    // Invalid graphs
    [ 0 ] = (graph) { 0 },
    [ 1 ]   = (graph) { 0 },
    [ 500 ]   = (graph) { 0 },
    [ GRAPH_TYPE( GRAPH_STORAGE_TYPE_INVALID, GRAPH_TYPE_WEIGHTED_DIRECTED ) ]     = (graph) { 0 },
    
    // Unweighted undirected graphs
    [ GRAPH_TYPE( GRAPH_TYPE_UNWEIGHTED_UNDIRECTED, GRAPH_STORAGE_TYPE_ADJACENCY_MATRIX ) ] = (graph)
    { 
        ._type         = GRAPH_TYPE_UNWEIGHTED_UNDIRECTED, 
        ._storage_type = GRAPH_STORAGE_TYPE_ADJACENCY_MATRIX,
        .storage = 
        {
            .pfn_graph_vertex_add = adjacency_matrix_vertex_add,
        },
        .operations = 
        {
            .pfn_graph_depth_first_search    = (void *) graph_prototype_dfs_stub,
            .pfn_graph_breadth_first_search  = (void *) graph_prototype_bfs_stub,
            .pfn_graph_minimum_spanning_tree = (void *) graph_prototype_span_stub,
            .pfn_graph_cycle_finder          = (void *) graph_prototype_cycle_stub,
            .pfn_graph_info                  = (void *) adjacency_matrix_print
        }
    },
    [ GRAPH_TYPE( GRAPH_TYPE_UNWEIGHTED_UNDIRECTED, GRAPH_STORAGE_TYPE_ADJACENCY_LIST ) ] = (graph)
    { 
        ._type         = GRAPH_TYPE_UNWEIGHTED_UNDIRECTED, 
        ._storage_type = GRAPH_STORAGE_TYPE_ADJACENCY_LIST,
        .storage =
        {
            .pfn_graph_vertex_add = adjacency_list_vertex_add
        },
        .operations = 
        {
            .pfn_graph_depth_first_search    = (void *) graph_prototype_dfs_stub,
            .pfn_graph_breadth_first_search  = (void *) graph_prototype_bfs_stub,
            .pfn_graph_minimum_spanning_tree = (void *) graph_prototype_span_stub,
            .pfn_graph_cycle_finder          = (void *) graph_prototype_cycle_stub,
            .pfn_graph_info                  = (void *) graph_adjacency_list_info
        }
    },
    [ GRAPH_TYPE( GRAPH_TYPE_UNWEIGHTED_UNDIRECTED, GRAPH_STORAGE_TYPE_EDGE_LIST ) ] = (graph)
    { 
        ._type         = GRAPH_TYPE_UNWEIGHTED_UNDIRECTED, 
        ._storage_type = GRAPH_STORAGE_TYPE_EDGE_LIST,
        .storage = 
        {
            .pfn_graph_vertex_add = graph_edge_list_vertex_add
        },
        .operations = 
        {
            .pfn_graph_depth_first_search    = (void *) graph_prototype_dfs_stub,
            .pfn_graph_breadth_first_search  = (void *) graph_prototype_bfs_stub,
            .pfn_graph_minimum_spanning_tree = (void *) graph_prototype_span_stub,
            .pfn_graph_cycle_finder          = (void *) graph_prototype_cycle_stub
        }
    },
    
    // Unweighted directed graphs
    [ GRAPH_TYPE( GRAPH_TYPE_UNWEIGHTED_DIRECTED, GRAPH_STORAGE_TYPE_ADJACENCY_MATRIX ) ] = (graph)
    { 
        ._type         = GRAPH_TYPE_UNWEIGHTED_DIRECTED, 
        ._storage_type = GRAPH_STORAGE_TYPE_ADJACENCY_MATRIX,
        .storage = 
        {
            .pfn_graph_vertex_add = adjacency_matrix_vertex_add
        },
        .operations = 
        {
            .pfn_graph_depth_first_search   = (void *) graph_prototype_dfs_stub,
            .pfn_graph_breadth_first_search = (void *) graph_prototype_bfs_stub,
            .pfn_graph_cycle_finder         = (void *) graph_prototype_cycle_stub,
            .pfn_graph_info                 = (void *) adjacency_matrix_print
        }
    },
    [ GRAPH_TYPE( GRAPH_TYPE_UNWEIGHTED_DIRECTED, GRAPH_STORAGE_TYPE_ADJACENCY_LIST ) ] = (graph)
    { 
        ._type         = GRAPH_TYPE_UNWEIGHTED_DIRECTED, 
        ._storage_type = GRAPH_STORAGE_TYPE_ADJACENCY_LIST,
        .storage =
        {
            .pfn_graph_vertex_add = graph_adjacency_list_vertex_add
        },
        .operations = 
        {
            .pfn_graph_depth_first_search   = (void *) graph_prototype_dfs_stub,
            .pfn_graph_breadth_first_search = (void *) graph_prototype_bfs_stub,
            .pfn_graph_cycle_finder         = (void *) graph_prototype_cycle_stub,
            .pfn_graph_info                 = (void *) graph_adjacency_list_info
        }
    },
    [ GRAPH_TYPE( GRAPH_TYPE_UNWEIGHTED_DIRECTED, GRAPH_STORAGE_TYPE_EDGE_LIST ) ] = (graph)
    { 
        ._type         = GRAPH_TYPE_UNWEIGHTED_DIRECTED, 
        ._storage_type = GRAPH_STORAGE_TYPE_EDGE_LIST,
        .storage = 
        {
            .pfn_graph_vertex_add = graph_edge_list_vertex_add
        },
        .operations = 
        {
            .pfn_graph_depth_first_search   = (void *) graph_prototype_dfs_stub,
            .pfn_graph_breadth_first_search = (void *) graph_prototype_bfs_stub,
            .pfn_graph_cycle_finder         = (void *) graph_prototype_cycle_stub
        }
    },
    
    // Weighted undirected graphs
    [ GRAPH_TYPE( GRAPH_TYPE_WEIGHTED_UNDIRECTED, GRAPH_STORAGE_TYPE_ADJACENCY_MATRIX ) ] = (graph)
    { 
        ._type         = GRAPH_TYPE_WEIGHTED_UNDIRECTED, 
        ._storage_type = GRAPH_STORAGE_TYPE_ADJACENCY_MATRIX,
        .storage = 
        {
            .pfn_graph_vertex_add = adjacency_matrix_vertex_add
        },
        .operations = 
        {
            .pfn_graph_depth_first_search    = (void *) graph_prototype_dfs_stub,
            .pfn_graph_breadth_first_search  = (void *) graph_prototype_bfs_stub,
            .pfn_graph_minimum_spanning_tree = (void *) graph_prototype_span_stub,
            .pfn_graph_cycle_finder          = (void *) graph_prototype_cycle_stub,
            .pfn_graph_info                  = (void *) adjacency_matrix_print

        }
    },
    [ GRAPH_TYPE( GRAPH_TYPE_WEIGHTED_UNDIRECTED, GRAPH_STORAGE_TYPE_ADJACENCY_LIST ) ] = (graph)
    { 
        ._type         = GRAPH_TYPE_WEIGHTED_UNDIRECTED, 
        ._storage_type = GRAPH_STORAGE_TYPE_ADJACENCY_LIST,
        .storage =
        {
            .pfn_graph_vertex_add = graph_adjacency_list_vertex_add
        },
        .operations = 
        {
            .pfn_graph_depth_first_search    = (void *) graph_prototype_dfs_stub,
            .pfn_graph_breadth_first_search  = (void *) graph_prototype_bfs_stub,
            .pfn_graph_minimum_spanning_tree = (void *) graph_prototype_span_stub,
            .pfn_graph_cycle_finder          = (void *) graph_prototype_cycle_stub,
            .pfn_graph_info                  = (void *) graph_adjacency_list_info
        }
    },
    [ GRAPH_TYPE( GRAPH_TYPE_WEIGHTED_UNDIRECTED, GRAPH_STORAGE_TYPE_EDGE_LIST ) ] = (graph)
    { 
        ._type         = GRAPH_TYPE_WEIGHTED_UNDIRECTED, 
        ._storage_type = GRAPH_STORAGE_TYPE_EDGE_LIST,
        .storage = 
        {
            .pfn_graph_vertex_add = graph_edge_list_vertex_add
        },
        .operations = 
        {
            .pfn_graph_depth_first_search    = (void *) graph_prototype_dfs_stub,
            .pfn_graph_breadth_first_search  = (void *) graph_prototype_bfs_stub,
            .pfn_graph_minimum_spanning_tree = (void *) graph_prototype_span_stub,
            .pfn_graph_cycle_finder          = (void *) graph_prototype_cycle_stub
        }
    },

    // Weighted directed graphs
    [ GRAPH_TYPE( GRAPH_TYPE_WEIGHTED_DIRECTED, GRAPH_STORAGE_TYPE_ADJACENCY_MATRIX ) ] = (graph)
    { 
        ._type         = GRAPH_TYPE_WEIGHTED_DIRECTED, 
        ._storage_type = GRAPH_STORAGE_TYPE_ADJACENCY_MATRIX,
        .storage = 
        {
            .pfn_graph_vertex_add = adjacency_matrix_vertex_add
        },
        .operations = 
        {
            .pfn_graph_depth_first_search          = (void *) graph_prototype_dfs_stub,
            .pfn_graph_breadth_first_search        = (void *) graph_prototype_bfs_stub,
            .pfn_graph_cycle_finder                = (void *) graph_prototype_cycle_stub,
            .pfn_graph_single_source_shortest_path = (void *) graph_prototype_sssp_stub,
            .pfn_graph_all_pairs_shortest_path     = (void *) graph_prototype_apsp_stub,
            .pfn_graph_maximum_flow                = (void *) graph_prototype_max_flow_operation,
            .pfn_graph_info                        = (void *) adjacency_matrix_print
        }
    },
    [ GRAPH_TYPE( GRAPH_TYPE_WEIGHTED_DIRECTED, GRAPH_STORAGE_TYPE_ADJACENCY_LIST ) ] = (graph)
    { 
        ._type         = GRAPH_TYPE_WEIGHTED_DIRECTED, 
        ._storage_type = GRAPH_STORAGE_TYPE_ADJACENCY_LIST,
        .storage = 
        {
            .pfn_graph_vertex_add = graph_adjacency_list_vertex_add
        },
        .operations = 
        {
            .pfn_graph_depth_first_search          = (void *) graph_prototype_dfs_stub,
            .pfn_graph_breadth_first_search        = (void *) graph_prototype_bfs_stub,
            .pfn_graph_minimum_spanning_tree       = (void *) graph_prototype_span_stub,
            .pfn_graph_cycle_finder                = (void *) graph_prototype_cycle_stub,
            .pfn_graph_single_source_shortest_path = (void *) graph_prototype_sssp_stub,
            .pfn_graph_all_pairs_shortest_path     = (void *) graph_prototype_apsp_stub,
            .pfn_graph_maximum_flow                = (void *) graph_prototype_max_flow_operation,
            .pfn_graph_info                        = (void *) graph_adjacency_list_info
        }
    },
    [ GRAPH_TYPE( GRAPH_TYPE_WEIGHTED_DIRECTED, GRAPH_STORAGE_TYPE_EDGE_LIST ) ] = (graph)
    { 
        ._type         = GRAPH_TYPE_WEIGHTED_DIRECTED, 
        ._storage_type = GRAPH_STORAGE_TYPE_EDGE_LIST,
        .storage = 
        {
            .pfn_graph_vertex_add = graph_edge_list_vertex_add
        },
        .operations = 
        {
            .pfn_graph_depth_first_search          = (void *) graph_prototype_dfs_stub,
            .pfn_graph_breadth_first_search        = (void *) graph_prototype_bfs_stub,
            .pfn_graph_cycle_finder                = (void *) graph_prototype_cycle_stub,
            .pfn_graph_single_source_shortest_path = (void *) graph_prototype_sssp_stub,
            .pfn_graph_all_pairs_shortest_path     = (void *) graph_prototype_apsp_stub,
            .pfn_graph_maximum_flow                = (void *) graph_prototype_max_flow_operation
        }
    }
};
