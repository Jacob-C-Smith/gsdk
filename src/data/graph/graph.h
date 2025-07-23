/** !
 * Graph header
 * 
 * @file include/graph/graph.h
 * 
 * @author Jacob Smith
 */

// header guard
#pragma once

// standard library
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// core
#include <core/log.h>
#include <core/pack.h>
#include <core/interfaces.h>
#include <core/hash.h>

// enumerations
enum graph_type_e 
{

    // NOTE: Storage type. 
    GRAPH_STORAGE_TYPE_INVALID          = 0, 
    GRAPH_STORAGE_TYPE_ADJACENCY_MATRIX = 1, 
    GRAPH_STORAGE_TYPE_ADJACENCY_LIST   = 2, 
    GRAPH_STORAGE_TYPE_EDGE_LIST        = 3, 

    // NOTE: Bit 0 set -> directed IF set ELSE undirected
    //       Bit 1 set -> weighted IF set ELSE unweighted
    GRAPH_TYPE_UNWEIGHTED_UNDIRECTED = 0, 
    GRAPH_TYPE_UNWEIGHTED_DIRECTED   = 1, 
    GRAPH_TYPE_WEIGHTED_UNDIRECTED   = 2, 
    GRAPH_TYPE_WEIGHTED_DIRECTED     = 3, 

    GRAPH_STORAGE_TYPE_QUANTITY = 4,
    GRAPH_TYPE_QUANTITY         = 4,
    GRAPH_COMBINATION_QUANTITY = GRAPH_STORAGE_TYPE_QUANTITY * GRAPH_TYPE_QUANTITY
};

// structure declarations
struct graph_s;
struct adjacency_matrix_s;
struct adjacency_list_s;
struct edge_list_s;

// union definitions
union edge_u
{
    struct 
    {
        void *p_value;
    } _unweighted_undirected;

    struct 
    {
        double weight;
        void *p_value;
    } _weighted_undirected;

    struct 
    {
        void *p_value;
    } _unweighted_directed;

    struct 
    {
        double weight;
        void *p_value;
    } _weighted_directed;
};

// type definitions
/// graph
typedef struct graph_s graph;

/// adjacency matrix
typedef struct adjacency_matrix_s adjacency_matrix;

/// adjacency list
typedef struct adjacency_list_s adjacency_list;

/// node
typedef union node_u node;

/// interfaces
typedef int (fn_graph_vertex_add)    ( graph *p_graph, void *p_value );
typedef int (fn_graph_vertex_remove) ( graph **pp_graph, void *p_value );
typedef int (fn_graph_vertex_search) ( graph *p_graph, void *p_value, void **pp_value );
typedef int (fn_graph_vertex_count)  ( graph *p_graph, size_t *p_count );
typedef int (fn_graph_vertex_info)   ( graph *p_graph, void *p_value, char _name[], size_t name_size, void **pp_value );

typedef int (fn_graph_edge_add)    ( graph **pp_graph, void *p_a_value, void *p_b_value, double weight );
typedef int (fn_graph_edge_remove) ( graph **pp_graph, void *p_a_value, void *p_b_value );
typedef int (fn_graph_edge_search) ( graph *p_graph, void *p_a_value, void *p_b_value, double *p_weight );
typedef int (fn_graph_edge_count)  ( graph *p_graph, size_t *p_count );
typedef int (fn_graph_edge_info)   ( graph *p_graph, void *p_a_value, void *p_b_value, double *p_weight );

typedef int (fn_graph_depth_first_search)           ( graph *p_graph, void *p_a_key, void *p_b_key );
typedef int (fn_graph_breadth_first_search)         ( graph *p_graph, void *p_a_key, void *p_b_key );
typedef int (fn_graph_cycle_finder)                 ( graph *p_graph );
typedef int (fn_graph_minimum_spanning_tree)        ( graph *p_graph );
typedef int (fn_graph_single_source_shortest_path)  ( graph *p_graph );
typedef int (fn_graph_all_pairs_shortest_path)      ( graph *p_graph );
typedef int (fn_graph_maximum_flow)                 ( graph *p_graph );
typedef int (fn_graph_info)                         ( graph *p_graph );

// structure definitions
/// node
;

struct adjacency_list_node_s {
    void *p_vertex;                     // Pointer to the vertex data
    union edge_u edge;                  // Edge information
    struct adjacency_list_node_s *next; // Pointer to the next node in the list
};

struct adjacency_list_s {
    size_t size;                        // Number of vertices in the graph
    size_t edge_count;                  // Number of edges in the graph
    struct {
        void *p_vertex;                 // Vertex data
        struct adjacency_list_node_s *p_head; // Head of the adjacency list for this vertex
    } *p_vertices;                      // Array of vertices and their adjacency lists
};

struct adjacency_matrix_s {
    size_t size;
    size_t edge_count;
    double **matrix;
    void **p_vertices;
};

struct edge_list_s
{
    size_t size;
};

struct graph_s
{
    enum graph_type_e _storage_type;
    enum graph_type_e _type;

    struct 
    {
        fn_graph_depth_first_search          *pfn_graph_depth_first_search;
        fn_graph_breadth_first_search        *pfn_graph_breadth_first_search;
        fn_graph_minimum_spanning_tree       *pfn_graph_minimum_spanning_tree;
        fn_graph_cycle_finder                *pfn_graph_cycle_finder;
        fn_graph_single_source_shortest_path *pfn_graph_single_source_shortest_path;
        fn_graph_all_pairs_shortest_path     *pfn_graph_all_pairs_shortest_path;
        fn_graph_maximum_flow                *pfn_graph_maximum_flow;
        fn_graph_info                        *pfn_graph_info;
        fn_graph_vertex_info                 *pfn_graph_vertex_info;
        fn_graph_edge_info                   *pfn_graph_edge_info;
    } operations;


    struct 
    {
        fn_graph_vertex_add *pfn_graph_vertex_add;

        union
        {
            adjacency_matrix _adjacency_matrix;
            adjacency_list _adjacency_list;
            // struct edge_list_s        _edge_list;
        };
    } storage;
};

static const char *_graph_storage_type_strings [ 4 ] = 
{
    [GRAPH_STORAGE_TYPE_INVALID]          = "invalid graph",
    [GRAPH_STORAGE_TYPE_ADJACENCY_MATRIX] = "adjacency matrix",
    [GRAPH_STORAGE_TYPE_ADJACENCY_LIST]   = "adjacency list",
    [GRAPH_STORAGE_TYPE_EDGE_LIST]        = "edge list"
};

static const char *_graph_type_strings [ 4 ] =
{
    [GRAPH_TYPE_UNWEIGHTED_UNDIRECTED] = "unweighted undirected",
    [GRAPH_TYPE_UNWEIGHTED_DIRECTED]   = "unweighted directed",
    [GRAPH_TYPE_WEIGHTED_UNDIRECTED]   = "weighted undirected",
    [GRAPH_TYPE_WEIGHTED_DIRECTED]     = "weighted directed"
};

// constructors
/** !
 * Construct a graph
 * 
 * @param pp_graph result
 * @param _type    the type of the graph < (un)weighted / (un)directed >
 * @param _storage the storage solution  < adjacency matrix / adjacency list / edge list >
 * 
 * @return 1 on success, 0 on error
 */
int graph_construct ( graph **pp_graph, enum graph_type_e _type, enum graph_type_e _storage );

// Storage
/** !
 *  Add a vertex to a graph
 * 
 * @param pp_graph pointer to graph pointer
 * @param p_name   the name of the vertex
 * @param p_value  the value of the vertex
 * 
 * @return 1 on success, 0 on error
 */
int graph_vertex_add ( graph *p_graph, void *p_value );

// Search
/** !
 *  Search for a vertex in the graph using depth first search
 * 
 * @param p_graph  the graph
 * @param p_source the name of the source
 * @param p_value  the value of the vertex
 * 
 * @return 1 on success, 0 on error
 */
int graph_depth_first_search ( graph *p_graph, char *p_source, char *p_value );

/** !
 *  Search for a vertex in the graph using breadth first search
 * 
 * @param p_graph  the graph
 * @param p_source the name of the source
 * @param p_value  the value of the vertex
 * 
 * @return 1 on success, 0 on error
 */
int graph_breadth_first_search ( graph *p_graph, char *p_source, char *p_value );

int graph_cycle_finder ( graph *p_graph );

int graph_minimum_spanning_tree ( graph *p_graph );

int graph_single_source_shortest_path ( graph *p_graph );

int graph_all_pairs_shortest_path ( graph *p_graph );

int graph_maximum_flow ( graph *p_graph );

// info
/** !
 * Print a graph
 * 
 * @param p_graph the graph
 * 
 * @return 1 on success, 0 on error
 */
int graph_info ( graph *p_graph );
