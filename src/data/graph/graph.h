/** !
 * Graph interface
 *
 * @file src/data/graph/graph.h 
 * 
 * @author Jacob Smith
 */

// header guard
#pragma once

// standard library
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

// gsdk
/// core
#include <core/log.h>
#include <core/hash.h>
#include <core/pack.h>
#include <core/sync.h>
#include <core/interfaces.h>

// enumeration definitions
enum graph_storage_type_e
{
    GRAPH_ADJACENCY_MATRIX = 0,
    GRAPH_ADJACENCY_LIST   = 1,
    GRAPH_EDGE_LIST        = 2,
    GRAPH_QUANTITY         = 3,
};

enum graph_edge_type_e
{
    GRAPH_UNDIRECTED = 0b00,
    GRAPH_UNWEIGHTED = 0b00,
    GRAPH_DIRECTED   = 0b01,
    GRAPH_WEIGHTED   = 0b10,
};

// forward declarations
struct graph_s;

// type definitions
typedef struct graph_s graph;

typedef double(fn_weight_accessor)(const void *p_edge);

typedef size_t(fn_graph_vertex_count)(void *const p_graph);
typedef int(fn_graph_vertex_search)(void *const p_graph, const void *p_key, void **pp_vertex);
typedef int(fn_graph_vertex_add)(void *const p_graph, const void *p_vertex);
typedef int(fn_graph_vertex_remove)(void *const p_graph, const void *p_key, void **pp_vertex, fn_allocator *pfn_allocator_vertex, fn_allocator *pfn_allocator_edge);
typedef int(fn_graph_vertex_foreach)(void *const p_graph, fn_foreach *pfn_foreach);
typedef int(fn_graph_vertex_degree)(void *const p_graph, const void *p_key, size_t *p_in_degree, size_t *p_out_degree);
typedef int(fn_graph_neighbors_get)(void *const p_graph, const void *p_key, size_t *p_count, void ***ppp_neighbors);

typedef size_t(fn_graph_edge_count)(void *const p_graph);
typedef int(fn_graph_edge_search)(void *const p_graph, const void *p_from, const void *p_to, void **pp_edge);
typedef int(fn_graph_edge_add)(void *const p_graph, const void *p_from, const void *p_to, const void *p_edge);
typedef int(fn_graph_edge_remove)(void *const p_graph, const void *p_from, const void *p_to, void **pp_edge, fn_allocator *pfn_allocator_edge);
typedef int(fn_graph_edge_foreach)(void *const p_graph, fn_foreach *pfn_foreach);

typedef int(fn_graph_pack)(void *p_buffer, void *p_graph, fn_pack *pfn_vertex, fn_pack *pfn_edge);
typedef int(fn_graph_unpack)(void **pp_graph, void *p_buffer, fn_unpack *pfn_vertex, fn_unpack *pfn_edge, fn_key_accessor *pfn_key_accessor, fn_comparator *pfn_comparator);
typedef hash64(fn_graph_hash)(void *const p_graph, fn_hash64 *pfn_hash64);

typedef int(fn_graph_destroy)(void **const pp_graph, fn_allocator *pfn_allocator_vertex, fn_allocator *pfn_allocator_edge);

// structure definitions
struct graph_s
{
    void *p_graph;

    enum graph_storage_type_e _type;
    enum graph_edge_type_e    _edge_type;

    size_t vertex_size;
    size_t edge_size;

    fn_key_accessor *pfn_key_accessor;
    fn_comparator   *pfn_comparator;

    fn_graph_vertex_search  *pfn_vertex_search;
    fn_graph_vertex_add     *pfn_vertex_add;
    fn_graph_vertex_remove  *pfn_vertex_remove;
    fn_graph_vertex_count   *pfn_vertex_count;
    fn_graph_vertex_foreach *pfn_vertex_foreach;
    fn_graph_vertex_degree  *pfn_vertex_degree;
    fn_graph_neighbors_get  *pfn_neighbors_get;

    fn_graph_edge_search    *pfn_edge_search;
    fn_graph_edge_add       *pfn_edge_add;
    fn_graph_edge_remove    *pfn_edge_remove;
    fn_graph_edge_count     *pfn_edge_count;
    fn_graph_edge_foreach   *pfn_edge_foreach;

    fn_graph_pack           *pfn_pack;
    fn_graph_unpack         *pfn_unpack;
    fn_graph_hash           *pfn_hash;

    fn_graph_destroy        *pfn_destroy;
};

// function declarations
/// constructors
/** !
 * Construct a graph
 * 
 * @param pp_graph         result
 * @param _storage_type    one of < GRAPH_ADJACENCY_MATRIX | GRAPH_ADJACENCY_LIST | GRAPH_EDGE_LIST >
 * @param _edge_type       one of < GRAPH_UNDIRECTED | GRAPH_UNWEIGHTED | GRAPH_DIRECTED | GRAPH_WEIGHTED >
 * @param vertex_size      the size of a vertex in bytes
 * @param edge_size        the size of an edge in bytes
 * @param pfn_key_accessor pointer to vertex key accessor function
 * @param pfn_comparator   pointer to vertex comparator function
 * 
 * @return 1 on success, 0 on error
 */
int graph_construct 
(
    graph **const pp_graph,

    enum graph_storage_type_e _storage_type,
    enum graph_edge_type_e    _edge_type,

    unsigned long long vertex_size,
    unsigned long long edge_size,

    fn_key_accessor *pfn_key_accessor,
    fn_comparator   *pfn_comparator
);

/// accessors
/** !
 * Search for a vertex
 * 
 * @param p_graph   the graph
 * @param p_key     the key
 * @param pp_vertex result IF NOT NULL ELSE unused
 * 
 * @return 1 on success, 0 on error
 */
int graph_vertex_search 
( 
    graph       *p_graph, 
    const void  *p_key, 
    void       **pp_vertex
);

/** !
 * Get the quantity of vertices in a graph
 * 
 * @param p_graph the graph
 * 
 * @return the quantity of vertices in a graph
 */
size_t graph_vertex_count ( graph *p_graph );

/** !
 * Search for an edge
 * 
 * @param p_graph the graph
 * @param p_from  the key of the source node
 * @param p_to    the key of the destination node 
 * @param pp_edge result IF NOT NULL ELSE unused
 * 
 * @return 1 on success, 0 on error
 */
int graph_edge_search 
( 
    graph       *p_graph, 
    const void  *p_from, 
    const void  *p_to, 
    void       **pp_edge 
);

/** !
 * Get the quantity of edges in a graph
 * 
 * @param p_graph the graph
 * 
 * @return the quantity of edges in a graph
 */
size_t graph_edge_count ( graph *p_graph );

/** !
 * Get the quantity of incoming and outgoing edges on a vertex
 * 
 * @param p_graph      the graph
 * @param p_key        the key of the vertex
 * @param p_in_degree  result IF NOT NULL ELSE unused
 * @param p_out_degree result IF NOT NULL ELSE unused
 * 
 * @return 1 on success, 0 on error
 */
int graph_vertex_degree
(
    graph *p_graph,
    const void *p_key,
    size_t *p_in_degree,
    size_t *p_out_degree
);

/** !
 * Get a vertex’s neighbors
 * 
 * @param p_graph       the graph
 * @param p_key         the key of the vertex
 * @param p_count       result IF NOT NULL ELSE unused
 * @param ppp_neighbors result IF NOT NULL ELSE unused
 * 
 * @return 1 on success, 0 on error
 */
int graph_neighbors_get
(
    graph *p_graph,
    const void *p_key,
    size_t *p_count,
    void ***ppp_neighbors
);

/// mutators
/** !
 * Add a vertex to a graph
 * 
 * @param p_graph  the graph
 * @param p_vertex the vertex
 * 
 * @return 1 on success, 0 on error
 */
int graph_vertex_add 
( 
    graph *const  p_graph, 
    const void   *p_vertex 
);

/** !
 * Remove a vertex from a graph
 * 
 * @param p_graph              the graph
 * @param p_key                the key of the vertex
 * @param pp_vertex            result IF NOT NULL ELSE unused
 * @param pfn_allocator_vertex pointer to vertex deallocator IF NOT NULL ELSE unused
 * @param pfn_allocator_edge   pointer to edge deallocator IF NOT NULL ELSE unused
 * 
 * @return 1 on success, 0 on error
 */
int graph_vertex_remove 
( 
    graph *const   p_graph, 
    const void    *p_key, 
    void         **pp_vertex, 
    fn_allocator  *pfn_allocator_vertex, 
    fn_allocator  *pfn_allocator_edge
);

/** !
 * Add an edge to a graph
 * 
 * @param p_graph the graph
 * @param p_from  the key of the source vertex
 * @param p_to    the key of the destination vertex
 * @param p_edge  the edge
 * 
 * @return 1 on success, 0 on error
 */
int graph_edge_add 
( 
    graph      *p_graph, 
    const void *p_from, 
    const void *p_to, 
    void       *p_edge
);

/** !
 * Remove an edge from a graph
 * 
 * @param p_graph            the graph
 * @param p_from             the key of the source vertex
 * @param p_to               the key of the destination vertex
 * @param pp_edge            result IF NOT NULL ELSE unused
 * @param pfn_allocator_edge pointer to edge deallocator IF NOT NULL ELSE unused
 * 
 * @return 1 on success, 0 on error
 */
int graph_edge_remove 
( 
    graph         *p_graph, 
    const void    *p_from, 
    const void    *p_to, 
    void         **pp_edge,
    fn_allocator  *pfn_allocator_edge
);

/// iterator
/** !
 * Call function on every vertex in a graph
 *
 * @param p_graph     the graph
 * @param pfn_foreach pointer to foreach function
 * 
 * @return 1 on success, 0 on error
 */
int graph_vertex_foreach
(
    graph      *p_graph,
    fn_foreach *pfn_foreach
);

/** !
 * Call function on every edge in a graph
 *
 * @param p_graph     the graph
 * @param pfn_foreach pointer to foreach function
 * 
 * @return 1 on success, 0 on error
 */
int graph_edge_foreach
(
    graph      *p_graph,
    fn_foreach *pfn_foreach
);

/// reflection
/** !
 * Pack a graph into a buffer
 * 
 * @param p_buffer   the buffer
 * @param p_graph    the graph
 * @param pfn_vertex pointer to pack function IF not null ELSE default
 * @param pfn_edge   pointer to pack function IF not null ELSE default
 * 
 * @return bytes written on success, 0 on error
 */
int graph_pack
(
    void *p_buffer, 
    graph *p_graph,

    fn_pack *pfn_vertex,
    fn_pack *pfn_edge
);

/** !
 * Unpack a buffer into a graph
 * 
 * @param pp_graph         result
 * @param p_buffer         the buffer
 * @param pfn_vertex       pointer to unpack function IF not null ELSE default
 * @param pfn_edge         pointer to unpack function IF not null ELSE default
 * @param pfn_key_accessor pointer to vertex key accessor function
 * @param pfn_comparator   pointer to vertex comparator function
 * 
 * @return bytes read on success, 0 on error
 */
int graph_unpack
(
    graph **pp_graph,
    void *p_buffer, 

    fn_unpack *pfn_vertex,
    fn_unpack *pfn_edge,

    fn_key_accessor *pfn_key_accessor,
    fn_comparator   *pfn_comparator
);

/** !
 * Compute a 64-bit hash of a graph
 * 
 * @param p_graph    the graph
 * @param pfn_hash64 hashing function applied to each vertex and node
 * 
 * @return hash on success, 0 on error
 */
hash64 graph_hash ( graph *const p_graph, fn_hash64 *pfn_hash64 );

/// destructors
/** !
 * Destroy a graph
 * 
 * @param pp_graph             pointer to graph pointer
 * @param pfn_allocator_vertex pointer to vertex deallocator IF NOT NULL ELSE unused
 * @param pfn_allocator_edge   pointer to edge deallocator IF NOT NULL ELSE unused
 * 
 * @return 1 on success, 0 on error
 */
int graph_destroy
(
    graph **pp_graph,
    
    fn_allocator *pfn_allocator_vertex,
    fn_allocator *pfn_allocator_edge
);
