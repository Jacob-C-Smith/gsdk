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

// log module
#include <core/log.h>

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

// type definitions
typedef struct graph_s graph;
typedef struct graph_edge_create_info_s graph_edge_create_info;

typedef union  node_u node;

typedef int (fn_graph_vertex_add)                   ( graph **pp_graph, char _name[], void *p_value );
typedef int (fn_graph_depth_first_search)           ( graph *p_graph, char _a[], char _b[] );
typedef int (fn_graph_breadth_first_search)         ( graph *p_graph, char _a[], char _b[] );
typedef int (fn_graph_cycle_finder)                 ( graph *p_graph );
typedef int (fn_graph_minimum_spanning_tree)        ( graph *p_graph );
typedef int (fn_graph_single_source_shortest_path)  ( graph *p_graph );
typedef int (fn_graph_all_pairs_shortest_path)      ( graph *p_graph );
typedef int (fn_graph_maximum_flow)                 ( graph *p_graph );
typedef int (fn_graph_info)                         ( graph *p_graph );

struct node_unweighted_undirected_s
{
    char  _name  [63+1];
    void *p_value;
};

union node_u
{
    struct node_unweighted_undirected_s _unweighted_undirected;
};

// 
struct adjacency_matrix_s 
{
    enum graph_type_e _type;
    size_t size;
    node _data [ ];
};

struct adjacency_list_s 
{
    size_t size;
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
    } operations;


    struct 
    {
        fn_graph_vertex_add *pfn_graph_vertex_add;

        union
        {
            struct adjacency_matrix_s _adjacency_matrix;
            struct adjacency_list_s   _adjacency_list;
            struct edge_list_s        _edge_list;
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

// Constructors
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
int graph_vertex_add ( graph **pp_graph, char _name[], void *p_value );

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

// Info
/** !
 * Print a graph
 * 
 * @param p_graph the graph
 * 
 * @return 1 on success, 0 on error
 */
int graph_info ( graph *p_graph );
