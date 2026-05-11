# [gsdk](../../../README.md) > [data](../data.md) > graph
## Model relations using verticies and edges

 > 1 [Example](#example)
 >
 > 2 [Tester](#tester)
 >
 > 3 [Definitions](#definitions)
 >
 >> 3.1 [Enumeration definitions](#enumeration-definitions)
 >> 
 >> 3.2 [Type definitions](#type-definitions)
 >>
 >> 3.3 [Function declarations](#function-declarations)

 ## Example
 To run the example program, execute this command
 ```
 $ ./build/examples/graph_example
 ```
 [Source](../../../src/examples/graph_example.c)
 
 ## Tester
 TODO

 ## Definitions
 ### Enumeration definitions
 ```c
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
 ```

 ### Type definitions
 ```c
// type definitions
typedef struct graph_s graph;
typedef struct graph_sssp_result_s graph_sssp_result;

typedef double(fn_weight_accessor)(const void *p_edge);
typedef int (fn_graph_traversal)(graph *p_graph, const void *p_start_key, fn_foreach *pfn_foreach);
typedef int (fn_graph_sssp)(graph *p_graph, const void *p_start_key, fn_weight_accessor *pfn_weight, graph_sssp_result **pp_results);
typedef int (fn_graph_apsp)(graph *p_graph, fn_weight_accessor *pfn_weight, double ***ppp_matrix);
typedef int (fn_graph_mst)(graph *p_graph, fn_weight_accessor *pfn_weight, graph **pp_mst);

typedef size_t(fn_graph_vertex_count)(void *p_graph);
typedef int(fn_graph_vertex_get)(void *p_graph, void **p_p_vertices);
typedef int(fn_graph_vertex_search)(void *p_graph, const void *p_key, void **pp_vertex);
typedef int(fn_graph_vertex_add)(void *p_graph, const void *p_vertex);
typedef int(fn_graph_vertex_remove)(void *p_graph, const void *p_key, void **pp_vertex, fn_allocator *pfn_allocator_vertex, fn_allocator *pfn_allocator_edge);
typedef int(fn_graph_vertex_foreach)(void *p_graph, fn_foreach *pfn_foreach);
typedef int(fn_graph_vertex_degree)(void *p_graph, const void *p_key, size_t *p_in_degree, size_t *p_out_degree);
typedef int(fn_graph_neighbors_get)(void *p_graph, const void *p_key, size_t *p_count, void ***ppp_neighbors);

typedef size_t(fn_graph_edge_count)(void *p_graph);
typedef int(fn_graph_edge_search)(void *p_graph, const void *p_from, const void *p_to, void **pp_edge);
typedef int(fn_graph_edge_add)(void *p_graph, const void *p_from, const void *p_to, const void *p_edge);
typedef int(fn_graph_edge_remove)(void *p_graph, const void *p_from, const void *p_to, void **pp_edge, fn_allocator *pfn_allocator_edge);
typedef int(fn_graph_edge_foreach)(void *p_graph, fn_foreach *pfn_foreach);

typedef int(fn_graph_pack)(void *p_buffer, void *p_graph, fn_pack *pfn_vertex, fn_pack *pfn_edge);
typedef int(fn_graph_unpack)(void **pp_graph, void *p_buffer, fn_unpack *pfn_vertex, fn_unpack *pfn_edge, fn_key_accessor *pfn_key_accessor, fn_comparator *pfn_comparator);
typedef hash64(fn_graph_hash)(void *p_graph, fn_hash64 *pfn_hash64);

typedef int(fn_graph_destroy)(void **const pp_graph, fn_allocator *pfn_allocator_vertex, fn_allocator *pfn_allocator_edge);
 ```

 ### Function declarations
 ```c 
// function declarations
/// constructors
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

/// traversal
fn_graph_traversal graph_algorithm_bfs;
fn_graph_traversal graph_algorithm_dfs;

/// single source shortest path
fn_graph_sssp graph_algorithm_sssp_dijkstra;
fn_graph_sssp graph_algorithm_sssp_bellman_ford;

/// all pairs shortest path
fn_graph_apsp graph_algorithm_apsp_floyd_warshall;
fn_graph_apsp graph_algorithm_apsp_johnson;

/// minimum spanning tree
fn_graph_mst graph_algorithm_mst_kruskal;
fn_graph_mst graph_algorithm_mst_prim;

/// accessors
int graph_vertex_get 
( 
    graph  *p_graph, 
    void  **p_p_vertices 
);

int graph_vertex_search 
( 
    graph       *p_graph, 
    const void  *p_key, 
    void       **pp_vertex
);

size_t graph_vertex_count ( graph *p_graph );

int graph_edge_search 
( 
    graph       *p_graph, 
    const void  *p_from, 
    const void  *p_to, 
    void       **pp_edge 
);

size_t graph_edge_count ( graph *p_graph );

int graph_vertex_degree
(
    graph      *p_graph,
    const void *p_key,
    size_t     *p_in_degree,
    size_t     *p_out_degree
);

int graph_neighbors_get
(
    graph        *p_graph,
    const void   *p_key,
    size_t       *p_count,
    void       ***ppp_neighbors
);

/// mutators
int graph_vertex_add 
( 
    graph *const  p_graph, 
    const void   *p_vertex 
);

int graph_vertex_remove 
( 
    graph *const   p_graph, 
    const void    *p_key, 
    void         **pp_vertex, 
    fn_allocator  *pfn_allocator_vertex, 
    fn_allocator  *pfn_allocator_edge
);

int graph_edge_add 
( 
    graph      *p_graph, 
    const void *p_from, 
    const void *p_to, 
    void       *p_edge
);

int graph_edge_remove 
( 
    graph         *p_graph, 
    const void    *p_from, 
    const void    *p_to, 
    void         **pp_edge,
    fn_allocator  *pfn_allocator_edge
);

/// iterator
int graph_vertex_foreach
(
    graph      *p_graph,
    fn_foreach *pfn_foreach
);

int graph_edge_foreach
(
    graph      *p_graph,
    fn_foreach *pfn_foreach
);

/// reflection
int graph_pack
(
    void  *p_buffer, 
    graph *p_graph,

    fn_pack *pfn_vertex,
    fn_pack *pfn_edge
);

int graph_unpack
(
    graph **pp_graph,
    void   *p_buffer, 

    fn_unpack *pfn_vertex,
    fn_unpack *pfn_edge,

    fn_key_accessor *pfn_key_accessor,
    fn_comparator   *pfn_comparator
);

hash64 graph_hash ( graph *const p_graph, fn_hash64 *pfn_hash64 );

/// destructors
int graph_destroy
(
    graph **pp_graph,
    
    fn_allocator *pfn_allocator_vertex,
    fn_allocator *pfn_allocator_edge
);
 ```
