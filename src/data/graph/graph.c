/** !
 * graph implementation
 * 
 * @file src/data/graph/graph.c
 * 
 * @author Jacob Smith
 */

// header file
#include <data/graph.h>

#include <float.h>

#include <data/adjacency_matrix.h>
#include <data/adjacency_list.h>
#include <data/edge_list.h>
#include <data/hash_table.h>
#include <data/stack.h>
#include <data/queue.h>
#include <data/priority_queue.h>

// structure definitions
struct vertex_index_entry_s 
{
    void   *p_vertex;
    size_t  index;
};

struct johnson_dijkstra_node_s 
{
    size_t index;
    double distance;
};

struct mst_edge_s 
{
    void *u;
    void *v;
    double weight;
};

struct union_find_s
{
    size_t *parent;
    size_t *rank;
};

typedef struct mst_edge_s   mst_edge; 
typedef struct union_find_s union_find;

// type definitions
typedef struct vertex_index_entry_s    vertex_index_entry; 
typedef struct johnson_dijkstra_node_s johnson_dijkstra_node; 

// forward declarations
static fn_hash64       graph_hash_pointer;
static fn_comparator   graph_equality_pointer;
static fn_comparator   dijkstra_compare;
static fn_key_accessor dijkstra_result_key_accessor;
static fn_key_accessor vertex_index_key_accessor;
static fn_comparator   johnson_compare;
static fn_comparator   kruskal_edge_compare;
static fn_comparator   prim_edge_compare;

static int    union_find_construct ( union_find **pp_union_find, size_t n );
static size_t union_find_find      ( union_find *p_union_find, size_t i );
static void   union_find_union     ( union_find *p_union_find, size_t i, size_t j );
static int    union_find_destroy   ( union_find **pp_union_find );

/// prototypes
graph _prototypes[GRAPH_QUANTITY] = 
{
    [GRAPH_ADJACENCY_MATRIX] = 
    {
        .p_graph = NULL,
        ._type   = GRAPH_ADJACENCY_MATRIX,

        .pfn_vertex_get     = (fn_graph_vertex_get *)     adjacency_matrix_vertex_get,
        .pfn_vertex_search  = (fn_graph_vertex_search *)  adjacency_matrix_vertex_search,
        .pfn_vertex_add     = (fn_graph_vertex_add *)     adjacency_matrix_vertex_add,               
        .pfn_vertex_remove  = (fn_graph_vertex_remove *)  adjacency_matrix_vertex_remove,                  
        .pfn_vertex_count   = (fn_graph_vertex_count *)   adjacency_matrix_vertex_count,
        .pfn_vertex_foreach = (fn_graph_vertex_foreach *) adjacency_matrix_vertex_foreach,
        .pfn_vertex_degree  = (fn_graph_vertex_degree *)  adjacency_matrix_vertex_degree,
        .pfn_neighbors_get  = (fn_graph_neighbors_get *)  adjacency_matrix_neighbors_get,

        .pfn_edge_search  = (fn_graph_edge_search *)  adjacency_matrix_edge_search,
        .pfn_edge_add     = (fn_graph_edge_add *)     adjacency_matrix_edge_add,             
        .pfn_edge_remove  = (fn_graph_edge_remove *)  adjacency_matrix_edge_remove,                
        .pfn_edge_count   = (fn_graph_edge_count *)   adjacency_matrix_edge_count,               
        .pfn_edge_foreach = (fn_graph_edge_foreach *) adjacency_matrix_edge_foreach,

        .pfn_pack   = (fn_graph_pack *)   adjacency_matrix_pack,         
        .pfn_unpack = (fn_graph_unpack *) adjacency_matrix_unpack,
        .pfn_hash   = (fn_graph_hash *)   adjacency_matrix_hash,

        .pfn_destroy = (fn_graph_destroy *) adjacency_matrix_destroy,         
    },
    [GRAPH_ADJACENCY_LIST] = 
    {
        .p_graph = NULL,
        ._type   = GRAPH_ADJACENCY_LIST,

        .pfn_vertex_get     = (fn_graph_vertex_get *)     adjacency_list_vertex_get,
        .pfn_vertex_search  = (fn_graph_vertex_search *)  adjacency_list_vertex_search,
        .pfn_vertex_add     = (fn_graph_vertex_add *)     adjacency_list_vertex_add,               
        .pfn_vertex_remove  = (fn_graph_vertex_remove *)  adjacency_list_vertex_remove,                  
        .pfn_vertex_count   = (fn_graph_vertex_count *)   adjacency_list_vertex_count,
        .pfn_vertex_foreach = (fn_graph_vertex_foreach *) adjacency_list_vertex_foreach,
        .pfn_vertex_degree  = (fn_graph_vertex_degree *)  adjacency_list_vertex_degree,
        .pfn_neighbors_get  = (fn_graph_neighbors_get *)  adjacency_list_neighbors_get,

        .pfn_edge_search  = (fn_graph_edge_search *)  adjacency_list_edge_search,
        .pfn_edge_add     = (fn_graph_edge_add *)     adjacency_list_edge_add,             
        .pfn_edge_remove  = (fn_graph_edge_remove *)  adjacency_list_edge_remove,                
        .pfn_edge_count   = (fn_graph_edge_count *)   adjacency_list_edge_count,               
        .pfn_edge_foreach = (fn_graph_edge_foreach *) adjacency_list_edge_foreach,

        .pfn_pack   = (fn_graph_pack *)   adjacency_list_pack,         
        .pfn_unpack = (fn_graph_unpack *) adjacency_list_unpack,
        .pfn_hash   = (fn_graph_hash *)   adjacency_list_hash,

        .pfn_destroy = (fn_graph_destroy *) adjacency_list_destroy,          
    },
    [GRAPH_EDGE_LIST] = 
    {
        .p_graph = NULL,
        ._type   = GRAPH_EDGE_LIST,

        .pfn_vertex_get     = (fn_graph_vertex_get *)     edge_list_vertex_get,
        .pfn_vertex_search  = (fn_graph_vertex_search *)  edge_list_vertex_search,
        .pfn_vertex_add     = (fn_graph_vertex_add *)     edge_list_vertex_add,               
        .pfn_vertex_remove  = (fn_graph_vertex_remove *)  edge_list_vertex_remove,                  
        .pfn_vertex_count   = (fn_graph_vertex_count *)   edge_list_vertex_count,
        .pfn_vertex_foreach = (fn_graph_vertex_foreach *) edge_list_vertex_foreach,
        .pfn_vertex_degree  = (fn_graph_vertex_degree *)  edge_list_vertex_degree,
        .pfn_neighbors_get  = (fn_graph_neighbors_get *)  edge_list_neighbors_get,

        .pfn_edge_search  = (fn_graph_edge_search *)  edge_list_edge_search,
        .pfn_edge_add     = (fn_graph_edge_add *)     edge_list_edge_add,             
        .pfn_edge_remove  = (fn_graph_edge_remove *)  edge_list_edge_remove,                
        .pfn_edge_count   = (fn_graph_edge_count *)   edge_list_edge_count,               
        .pfn_edge_foreach = (fn_graph_edge_foreach *) edge_list_edge_foreach,

        .pfn_pack   = (fn_graph_pack *)   edge_list_pack,         
        .pfn_unpack = (fn_graph_unpack *) edge_list_unpack,
        .pfn_hash   = (fn_graph_hash *)   edge_list_hash,

        .pfn_destroy = (fn_graph_destroy *) edge_list_destroy,          
    },
};

// function definitions
int graph_construct 
(
    graph **const pp_graph,

    enum graph_storage_type_e _storage_type,
    enum graph_edge_type_e    _edge_type,

    unsigned long long vertex_size,
    unsigned long long edge_size,

    fn_key_accessor *pfn_key_accessor,
    fn_comparator   *pfn_comparator
)
{

    // argument check
    if ( NULL == pp_graph ) goto no_graph;

    // initialized data
    graph *p_graph          = NULL;
    void  *p_concrete_graph = NULL;
    int    result           = 0;

    // allocate memory for a graph
    p_graph = default_allocator(0, sizeof(graph));
    if ( NULL == p_graph ) goto failed_to_allocate_graph;

    // strategy
    switch (_storage_type)
    {
        case GRAPH_ADJACENCY_MATRIX:
            result = adjacency_matrix_construct((adjacency_matrix **)&p_concrete_graph, _edge_type, vertex_size, edge_size, pfn_key_accessor, pfn_comparator);
            break;
        case GRAPH_ADJACENCY_LIST:
            result = adjacency_list_construct((adjacency_list **)&p_concrete_graph, _edge_type, vertex_size, edge_size, pfn_key_accessor, pfn_comparator);
            break;
        case GRAPH_EDGE_LIST:
            result = edge_list_construct((edge_list **)&p_concrete_graph, _edge_type, vertex_size, edge_size, pfn_key_accessor, pfn_comparator);
            break;
        default: goto invalid_graph_type;
    }

    // error check
    if ( 0 == result ) goto failed_to_construct_concrete_graph;

    // clone the prototype
    memcpy(p_graph, &_prototypes[_storage_type], sizeof(graph));

    // store the concrete graph
    p_graph->p_graph = p_concrete_graph;

    // store the edge type
    p_graph->_edge_type = _edge_type;

    // store the vertex size
    p_graph->vertex_size = vertex_size;

    // store the edge size
    p_graph->edge_size = edge_size;

    // store the key accessor
    p_graph->pfn_key_accessor = pfn_key_accessor;

    // store the comparator
    p_graph->pfn_comparator = pfn_comparator;

    // return a pointer to the caller
    *pp_graph = p_graph;

    // success
    return 1;

    // error handling
    {

        // argument errors
        {
            no_graph:
                #ifndef NDEBUG
                    printf("[graph] Null pointer provided for parameter \"pp_graph\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }

        // graph errors
        {
            failed_to_allocate_graph:
                #ifndef NDEBUG
                    printf("[graph] Failed to allocate graph in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;

            invalid_graph_type:
                #ifndef NDEBUG
                    printf("[graph] Invalid graph type in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // release the graph
                p_graph = default_allocator(p_graph, 0);

                // error
                return 0;

            failed_to_construct_concrete_graph:
                #ifndef NDEBUG
                    printf("[graph] Failed to construct concrete graph in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // release the graph
                p_graph = default_allocator(p_graph, 0);

                // error
                return 0;
        }
    }
}

int graph_algorithm_bfs
( 
    graph      *p_graph, 
    const void *p_start_key, 
    fn_foreach *pfn_foreach
)
{

    // argument check
    if ( NULL ==     p_graph ) goto no_graph;
    if ( NULL == p_start_key ) goto no_start_key;
    if ( NULL == pfn_foreach ) goto no_foreach;
    
    // initialized data
    queue      *p_queue        = NULL;
    hash_table *p_visited      = NULL;
    void       *p_start_vertex = NULL;
    size_t      vertex_count   = graph_vertex_count(p_graph);

    // search for the starting vertex
    if ( 0 == graph_vertex_search(p_graph, p_start_key, &p_start_vertex) ) goto start_vertex_not_found;

    // construct a queue
    if ( 0 == queue_construct(&p_queue) ) goto failed_to_construct_queue;

    // construct a hash table to track visited vertices
    if ( 0 == hash_table_construct(&p_visited, vertex_count ? vertex_count * 2 + 1 : 8, COLLISION_RESOLUTION_DEFAULT, graph_equality_pointer, NULL, graph_hash_pointer) ) goto failed_to_construct_hash_table;

    // enqueue the start vertex
    if ( 0 == queue_enqueue(p_queue, p_start_vertex) ) goto failed_to_enqueue;

    // traverse the graph
    while ( false == queue_empty(p_queue) )
    {
        
        // initialized data
        void    *p_vertex         = NULL;
        void    *p_key            = NULL;
        void   **pp_neighbors     = NULL;
        void    *p_visited_vertex = NULL;
        size_t   neighbor_count   = 0;

        // dequeue a vertex
        if ( 0 == queue_dequeue(p_queue, &p_vertex) ) goto failed_to_dequeue;

        // check if the vertex has been visited
        if ( 1 == hash_table_search(p_visited, p_vertex, &p_visited_vertex) ) continue;

        // mark the vertex as visited
        if ( 0 == hash_table_insert(p_visited, p_vertex) ) goto failed_to_insert_visited;

        // call the foreach function
        pfn_foreach(p_vertex);

        // get the key for the vertex
        p_key = p_graph->pfn_key_accessor(p_vertex);

        // get the neighbors of the vertex
        if ( 0 == graph_neighbors_get(p_graph, p_key, &neighbor_count, &pp_neighbors) ) continue;

        // enqueue the neighbors
        for ( size_t i = 0; i < neighbor_count; i++ )
        {
            
            // if the neighbor has not been visited ...
            if ( 0 == hash_table_search(p_visited, pp_neighbors[i], &p_visited_vertex) )

                // ... enqueue the neighbor 
                if ( 0 == queue_enqueue(p_queue, pp_neighbors[i]) ) 
                {
                    
                    // release the neighbors
                    if ( pp_neighbors ) pp_neighbors = default_allocator(pp_neighbors, 0);

                    // error
                    goto failed_to_enqueue;
                }
        }

        // release the neighbors
        if ( pp_neighbors ) pp_neighbors = default_allocator(pp_neighbors, 0);
    }

    // destroy the queue
    queue_destroy(&p_queue);

    // destroy the hash table
    hash_table_destroy(&p_visited, NULL);

    // success
    return 1;

    // error handling
    {

        // argument errors
        {
            no_graph:
                #ifndef NDEBUG
                    printf("[graph] Null pointer provided for parameter \"p_graph\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
            
            no_start_key:
                #ifndef NDEBUG
                    printf("[graph] Null pointer provided for parameter \"p_start_key\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
            
            no_foreach:
                #ifndef NDEBUG
                    printf("[graph] Null pointer provided for parameter \"pfn_foreach\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }

        // graph errors
        {
            start_vertex_not_found:
                #ifndef NDEBUG
                    printf("[graph] Failed to find starting vertex in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }

        // queue errors
        {
            failed_to_construct_queue:
                #ifndef NDEBUG
                    printf("[graph] Failed to construct queue in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;

            failed_to_enqueue:
                #ifndef NDEBUG
                    printf("[graph] Failed to enqueue vertex in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // destroy the queue and hash table
                if ( p_queue )   queue_destroy(&p_queue);
                if ( p_visited ) hash_table_destroy(&p_visited, NULL);

                // error
                return 0;

            failed_to_dequeue:
                #ifndef NDEBUG
                    printf("[graph] Failed to dequeue vertex in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // destroy the queue and hash table
                if ( p_queue )   queue_destroy(&p_queue);
                if ( p_visited ) hash_table_destroy(&p_visited, NULL);

                // error
                return 0;
        }

        // hash table errors
        {
            failed_to_construct_hash_table:
                #ifndef NDEBUG
                    printf("[graph] Failed to construct hash table in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // destroy the queue
                if ( p_queue ) queue_destroy(&p_queue);

                // error
                return 0;

            failed_to_insert_visited:
                #ifndef NDEBUG
                    printf("[graph] Failed to insert visited vertex into hash table in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // destroy the queue and hash table
                if ( p_queue )   queue_destroy(&p_queue);
                if ( p_visited ) hash_table_destroy(&p_visited, NULL);

                // error
                return 0;
        }
    }
}

int graph_algorithm_dfs 
( 
    graph      *p_graph, 
    const void *p_start_key, 
    fn_foreach *pfn_foreach
)
{

    // argument check
    if ( NULL ==     p_graph ) goto no_graph;
    if ( NULL == p_start_key ) goto no_start_key;
    if ( NULL == pfn_foreach ) goto no_foreach;
    
    // initialized data
    stack      *p_stack        = NULL;
    hash_table *p_visited      = NULL;
    void       *p_start_vertex = NULL;
    size_t      vertex_count   = graph_vertex_count(p_graph);

    // search for the starting vertex
    if ( 0 == graph_vertex_search(p_graph, p_start_key, &p_start_vertex) ) goto start_vertex_not_found;

    // construct a stack
    if ( 0 == stack_construct(&p_stack, vertex_count ? vertex_count : 8) ) goto failed_to_construct_stack;

    // construct a hash table to track visited vertices
    if ( 0 == hash_table_construct(&p_visited, vertex_count ? vertex_count * 2 + 1 : 8, COLLISION_RESOLUTION_DEFAULT, graph_equality_pointer, NULL, graph_hash_pointer) ) goto failed_to_construct_hash_table;

    // push the start vertex
    if ( 0 == stack_push(p_stack, p_start_vertex) ) goto failed_to_push;

    // traverse the graph
    while ( false == stack_is_empty(p_stack) )
    {
        
        // initialized data
        void    *p_vertex         = NULL;
        void    *p_key            = NULL;
        void   **pp_neighbors     = NULL;
        void    *p_visited_vertex = NULL;
        size_t   neighbor_count   = 0;

        // pop a vertex
        if ( 0 == stack_pop(p_stack, &p_vertex) ) continue;

        // check if the vertex has been visited
        if ( 1 == hash_table_search(p_visited, p_vertex, &p_visited_vertex) ) continue;

        // mark the vertex as visited
        if ( 0 == hash_table_insert(p_visited, p_vertex) ) goto failed_to_insert_visited;

        // call the foreach function
        pfn_foreach(p_vertex);

        // get the key for the vertex
        p_key = p_graph->pfn_key_accessor(p_vertex);

        // get the neighbors of the vertex
        if ( 0 == graph_neighbors_get(p_graph, p_key, &neighbor_count, &pp_neighbors) ) continue;

        // push the neighbors
        for ( size_t i = 0; i < neighbor_count; i++ )
        {
            
            // if the neighbor has not been visited ...
            if ( 0 == hash_table_search(p_visited, pp_neighbors[i], &p_visited_vertex) )

                // ... push the neighbor
                if ( 0 == stack_push(p_stack, pp_neighbors[i]) ) 
                {
                    
                    // release the neighbors
                    if ( pp_neighbors ) default_allocator(pp_neighbors, 0);

                    // error
                    goto failed_to_push;
                }
        }

        // release the neighbors
        if ( pp_neighbors ) default_allocator(pp_neighbors, 0);
    }

    // destroy the stack 
    stack_destroy(&p_stack);

    // destroy the hash table
    hash_table_destroy(&p_visited, NULL);

    // success
    return 1;

    // error handling
    {

        // argument errors
        {
            no_graph:
                #ifndef NDEBUG
                    printf("[graph] Null pointer provided for parameter \"p_graph\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
            
            no_start_key:
                #ifndef NDEBUG
                    printf("[graph] Null pointer provided for parameter \"p_start_key\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
            
            no_foreach:
                #ifndef NDEBUG
                    printf("[graph] Null pointer provided for parameter \"pfn_foreach\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }

        // graph errors
        {
            start_vertex_not_found:
                #ifndef NDEBUG
                    printf("[graph] Failed to find starting vertex in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }

        // stack errors
        {
            failed_to_construct_stack:
                #ifndef NDEBUG
                    printf("[graph] Failed to construct stack in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;

            failed_to_push:
                #ifndef NDEBUG
                    printf("[graph] Failed to push vertex in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // destroy the stack and hash table
                if ( p_stack )   stack_destroy(&p_stack);
                if ( p_visited ) hash_table_destroy(&p_visited, NULL);

                // error
                return 0;
        }

        // hash table errors
        {
            failed_to_construct_hash_table:
                #ifndef NDEBUG
                    printf("[graph] Failed to construct hash table in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // destroy the stack
                if ( p_stack ) stack_destroy(&p_stack);

                // error
                return 0;

            failed_to_insert_visited:
                #ifndef NDEBUG
                    printf("[graph] Failed to insert visited vertex into hash table in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // destroy the stack and hash table
                if ( p_stack )   stack_destroy(&p_stack);
                if ( p_visited ) hash_table_destroy(&p_visited, NULL);

                // error
                return 0;
        }
    }
}

int graph_algorithm_sssp_dijkstra 
( 
    graph              *p_graph, 
    const void         *p_start_key, 
    fn_weight_accessor *pfn_weight, 
    graph_sssp_result **pp_results 
)
{

    // argument check
    if ( NULL ==     p_graph ) goto no_graph;
    if ( NULL == p_start_key ) goto no_start_key;
    if ( NULL ==  pfn_weight ) goto no_weight_accessor;
    if ( NULL ==  pp_results ) goto no_results;

    // initialized data
    priority_queue     *p_priority_queue = NULL;
    hash_table         *p_result_map     = NULL;
    hash_table         *p_visited        = NULL;
    graph_sssp_result  *p_results        = NULL;
    void               *p_start_vertex   = NULL;
    void              **p_p_vertices     = NULL;
    graph_sssp_result  *p_start_res      = NULL;
    size_t              vertex_count     = -1;
    size_t              edge_count       = -1;

    // type check
    if ( !(p_graph->_edge_type & GRAPH_WEIGHTED) ) goto no_weighted_graph;
    
    // store the quantity of vertices
    vertex_count = graph_vertex_count(p_graph);

    // store the quantity of edges
    edge_count = graph_edge_count(p_graph);

    // search for the starting vertex
    if ( 0 == graph_vertex_search(p_graph, p_start_key, &p_start_vertex) ) goto start_vertex_not_found;

    // allocate memory for results
    p_results = default_allocator(NULL, sizeof(graph_sssp_result) * vertex_count);
    if ( NULL == p_results ) goto no_mem;

    // allocate memory for vertices
    p_p_vertices = default_allocator(NULL, sizeof(void *) * vertex_count);
    if ( NULL == p_p_vertices ) goto no_mem;

    // get the vertices
    if ( 0 == graph_vertex_get(p_graph, p_p_vertices) ) goto failed_to_get_vertices;
    
    // initialize results
    for (size_t i = 0; i < vertex_count; i++)
        p_results[i] = (graph_sssp_result)
        {
            .p_vertex   = p_p_vertices[i],
            .distance   = DBL_MAX,
            .p_previous = NULL,
        };

    // release vertices
    p_p_vertices = default_allocator(p_p_vertices, 0);
    
    // construct a hash table to map vertices to their results
    if ( 0 == hash_table_construct(&p_result_map, vertex_count * 2 + 1, COLLISION_RESOLUTION_DEFAULT, graph_equality_pointer, dijkstra_result_key_accessor, graph_hash_pointer) ) goto failed_to_construct_hash_table;

    // populate the result map and find the start result
    for ( size_t i = 0; i < vertex_count; i++ )
    {
        hash_table_insert(p_result_map, &p_results[i]);
        if ( p_results[i].p_vertex == p_start_vertex )
            p_start_res = &p_results[i];
    }

    // set the start distance to 0
    if ( p_start_res ) p_start_res->distance = 0.0;
    else goto start_vertex_not_found;

    // construct a priority queue
    if ( 0 == priority_queue_construct(&p_priority_queue, edge_count + 1, dijkstra_compare) ) goto failed_to_construct_pq;

    // construct a hash table to track visited (finalized) vertices
    if ( 0 == hash_table_construct(&p_visited, vertex_count * 2 + 1, COLLISION_RESOLUTION_DEFAULT, graph_equality_pointer, NULL, graph_hash_pointer) ) goto failed_to_construct_hash_table;

    // enqueue the start vertex result
    priority_queue_enqueue(p_priority_queue, p_start_res);

    // traverse the graph
    while ( false == priority_queue_empty(p_priority_queue) )
    {
        
        // initialized data
        graph_sssp_result  *u_res          = NULL;
        void               *u              = NULL;
        void               *u_key          = NULL;
        size_t              neighbor_count = 0;
        void              **pp_neighbors   = NULL;
        void               *p_dummy        = NULL;

        // dequeue the vertex with the smallest distance
        if ( 0 == priority_queue_dequeue(p_priority_queue, (void **)&u_res) ) break;

        // store u
        u = u_res->p_vertex;

        // check if the vertex has already been finalized
        if ( 1 == hash_table_search(p_visited, u, &p_dummy) ) continue;

        // mark the vertex as finalized
        hash_table_insert(p_visited, u);

        // get the key for the vertex
        u_key = p_graph->pfn_key_accessor(u);

        // get the neighbors of the vertex
        if ( 0 == graph_neighbors_get(p_graph, u_key, &neighbor_count, &pp_neighbors) ) continue;

        // for each neighbor ...
        for ( size_t i = 0; i < neighbor_count; i++ )
        {
            
            // initialized data
            void              *v      = pp_neighbors[i];
            void              *v_key  = p_graph->pfn_key_accessor(v);
            void              *p_edge = NULL;
            graph_sssp_result *v_res  = NULL;
            double             weight = 0.0;
            double             alt    = 0.0;

            // skip
            if ( 1 == hash_table_search(p_visited, v, &p_dummy) ) continue;

            // find the edge between u and v
            if ( 0 == graph_edge_search(p_graph, u_key, v_key, &p_edge) ) continue;

            // get the weight of the edge
            weight = pfn_weight(p_edge);

            // find the result entry for the neighbor
            if ( 0 == hash_table_search(p_result_map, v, (void **)&v_res) ) continue;

            // calculate the alternative distance
            alt = u_res->distance + weight;

            // if the alternative distance is smaller ...
            if ( alt < v_res->distance )
            {
                
                // ... update the neighbor's distance and previous vertex
                v_res->distance   = alt;
                v_res->p_previous = u;

                // enqueue the neighbor
                priority_queue_enqueue(p_priority_queue, v_res);
            }
        }

        // release the neighbors
        if ( pp_neighbors ) default_allocator(pp_neighbors, 0);
    }

    // destroy the priority queue and hash tables
    priority_queue_destroy(&p_priority_queue);
    hash_table_destroy(&p_result_map, NULL);
    hash_table_destroy(&p_visited, NULL);

    // return the results to the caller
    *pp_results = p_results;

    // success
    return 1;

    // error handling
    {

        // argument errors
        {
            no_graph:
                #ifndef NDEBUG
                    printf("[graph] Null pointer provided for parameter \"p_graph\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;

            no_start_key:
                #ifndef NDEBUG
                    printf("[graph] Null pointer provided for parameter \"p_start_key\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;

            no_weight_accessor:
                #ifndef NDEBUG
                    printf("[graph] Null pointer provided for parameter \"pfn_weight\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;

            no_results:
                #ifndef NDEBUG
                    printf("[graph] Null pointer provided for parameter \"pp_results\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }

        // graph errors
        {
            no_weighted_graph:
                #ifndef NDEBUG
                    printf("[graph] Graph is not weighted in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;

            start_vertex_not_found:
                #ifndef NDEBUG
                    printf("[graph] Failed to find starting vertex in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
                
            failed_to_get_vertices:
                #ifndef NDEBUG
                    printf("[graph] Failed to get vertices in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }

        // standard library errors
        {
            no_mem:
                #ifndef NDEBUG
                    printf("[standard library] Failed to allocate memory in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }

        // hash table errors
        {
            failed_to_construct_hash_table:
                #ifndef NDEBUG
                    printf("[graph] Failed to construct hash table in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // destroy the priority queue and hash tables
                if ( p_priority_queue ) priority_queue_destroy(&p_priority_queue);
                if ( p_result_map )     hash_table_destroy(&p_result_map, NULL);
                if ( p_visited )        hash_table_destroy(&p_visited, NULL);
                if ( p_results )        default_allocator(p_results, 0);

                // error
                return 0;
        }

        // priority queue errors
        {
            failed_to_construct_pq:
                #ifndef NDEBUG
                    printf("[graph] Failed to construct priority queue in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // destroy the priority queue and hash tables
                if ( p_priority_queue ) priority_queue_destroy(&p_priority_queue);
                if ( p_result_map )     hash_table_destroy(&p_result_map, NULL);
                if ( p_visited )        hash_table_destroy(&p_visited, NULL);
                if ( p_results )        default_allocator(p_results, 0);

                // error
                return 0;
        }
    }
}

int graph_algorithm_sssp_bellman_ford 
( 
    graph              *p_graph, 
    const void         *p_start_key, 
    fn_weight_accessor *pfn_weight, 
    graph_sssp_result **pp_results 
)
{

    // argument check
    if ( NULL == p_graph     ) goto no_graph;
    if ( NULL == p_start_key ) goto no_start_key;
    if ( NULL == pfn_weight  ) goto no_weight_accessor;
    if ( NULL == pp_results  ) goto no_results;

    // initialized data
    hash_table         *p_result_map   = NULL;
    graph_sssp_result  *p_results      = NULL;
    void               *p_start_vertex = NULL;
    graph_sssp_result  *p_start_res    = NULL;
    void              **p_p_vertices   = NULL;
    size_t              vertex_count   = -1;

    // type check
    if ( !(p_graph->_edge_type & GRAPH_WEIGHTED) ) goto no_weighted_graph;

    // store the quantity of vertices
    vertex_count = graph_vertex_count(p_graph);

    // search for the starting vertex
    if ( 0 == graph_vertex_search(p_graph, p_start_key, &p_start_vertex) ) goto start_vertex_not_found;

    // allocate memory for results
    p_results = default_allocator(NULL, sizeof(graph_sssp_result) * vertex_count);
    if ( NULL == p_results ) goto no_mem;

    // allocate memory for vertices
    p_p_vertices = default_allocator(NULL, sizeof(void *) * vertex_count);
    if ( NULL == p_p_vertices ) goto no_mem;

    // get the vertices
    if ( 0 == graph_vertex_get(p_graph, p_p_vertices) ) goto failed_to_get_vertices;
    
    // initialize results
    for (size_t i = 0; i < vertex_count; i++)
        p_results[i] = (graph_sssp_result)
        {
            .p_vertex   = p_p_vertices[i],
            .distance   = DBL_MAX,
            .p_previous = NULL,
        };
    
    // release vertices
    p_p_vertices = default_allocator(p_p_vertices, 0);

    // construct a hash table to map vertices to their results
    if ( 0 == hash_table_construct(&p_result_map, vertex_count * 2 + 1, COLLISION_RESOLUTION_DEFAULT, graph_equality_pointer, dijkstra_result_key_accessor, graph_hash_pointer) ) goto failed_to_construct_hash_table;

    // populate the result map and find the start result
    for ( size_t i = 0; i < vertex_count; i++ )
    {
        hash_table_insert(p_result_map, &p_results[i]);
        if ( p_results[i].p_vertex == p_start_vertex )
            p_start_res = &p_results[i];
    }

    // set the start distance to 0
    if ( p_start_res ) p_start_res->distance = 0.0;
    else goto start_vertex_not_found;

    // relax edges |V| - 1 times
    for ( size_t i = 0; i < vertex_count - 1; i++ )
    {
        
        // iterate over all edges
        for ( size_t j = 0; j < vertex_count; j++ )
        {
            
            // initialized data
            graph_sssp_result  *u_res          = &p_results[j];
            void               *u              = u_res->p_vertex;
            void               *u_key          = p_graph->pfn_key_accessor(u);
            size_t              neighbor_count = 0;
            void              **pp_neighbors   = NULL;

            // skip if the distance is still infinity
            if ( DBL_MAX == u_res->distance ) continue;

            // get the neighbors of the vertex
            if ( 0 == graph_neighbors_get(p_graph, u_key, &neighbor_count, &pp_neighbors) ) continue;

            // for each neighbor ...
            for ( size_t k = 0; k < neighbor_count; k++ )
            {
                
                // initialized data
                void              *v      = pp_neighbors[k];
                void              *v_key  = p_graph->pfn_key_accessor(v);
                void              *p_edge = NULL;
                graph_sssp_result *v_res  = NULL;
                double             weight = 0.0;

                // find the edge between u and v
                if ( 0 == graph_edge_search(p_graph, u_key, v_key, &p_edge) ) continue;

                // store the weight of the edge
                weight = pfn_weight(p_edge);

                // find the result entry for the neighbor
                if ( 0 == hash_table_search(p_result_map, v, (void **)&v_res) ) continue;

                // relax the edge
                if ( u_res->distance + weight < v_res->distance )
                {
                    v_res->distance   = u_res->distance + weight;
                    v_res->p_previous = u;
                }
            }

            // release the neighbors
            if ( pp_neighbors ) default_allocator(pp_neighbors, 0);
        }
    }

    // check for negative weight cycles
    for ( size_t j = 0; j < vertex_count; j++ )
    {
        
        // initialized data
        graph_sssp_result  *u_res          = &p_results[j];
        void               *u              = u_res->p_vertex;
        void               *u_key          = p_graph->pfn_key_accessor(u);
        size_t              neighbor_count = 0;
        void              **pp_neighbors   = NULL;

        // skip if the distance is still infinity
        if ( u_res->distance == DBL_MAX ) continue;

        // get the neighbors of the vertex
        if ( 0 == graph_neighbors_get(p_graph, u_key, &neighbor_count, &pp_neighbors) ) continue;

        // for each neighbor ...
        for ( size_t k = 0; k < neighbor_count; k++ )
        {
            
            // initialized data
            void              *v      = pp_neighbors[k];
            void              *v_key  = p_graph->pfn_key_accessor(v);
            void              *p_edge = NULL;
            graph_sssp_result *v_res  = NULL;
            double             weight = 0.0;

            // find the edge between u and v
            if ( 0 == graph_edge_search(p_graph, u_key, v_key, &p_edge) ) continue;

            // store the weight of the edge
            weight = pfn_weight(p_edge);

            // find the result entry for the neighbor
            if ( 0 == hash_table_search(p_result_map, v, (void **)&v_res) ) continue;

            // check for negative weight cycle
            if ( u_res->distance + weight < v_res->distance )
            {
                
                // release the neighbors
                if ( pp_neighbors ) default_allocator(pp_neighbors, 0);

                // negative cycle detected
                goto negative_cycle;
            }
        }

        // release the neighbors
        if ( pp_neighbors ) default_allocator(pp_neighbors, 0);
    }

    // destroy the hash table
    hash_table_destroy(&p_result_map, NULL);

    // return the results to the caller
    *pp_results = p_results;

    // success
    return 1;

    // error handling
    {

        // argument errors
        {
            no_graph:
                #ifndef NDEBUG
                    printf("[graph] Null pointer provided for parameter \"p_graph\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;

            no_start_key:
                #ifndef NDEBUG
                    printf("[graph] Null pointer provided for parameter \"p_start_key\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;

            no_weight_accessor:
                #ifndef NDEBUG
                    printf("[graph] Null pointer provided for parameter \"pfn_weight\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;

            no_results:
                #ifndef NDEBUG
                    printf("[graph] Null pointer provided for parameter \"pp_results\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }

        // graph errors
        {
            start_vertex_not_found:
                #ifndef NDEBUG
                    printf("[graph] Failed to find starting vertex in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;

            no_weighted_graph:
                #ifndef NDEBUG
                    printf("[graph] Graph is not weighted in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;

            negative_cycle:
                #ifndef NDEBUG
                    printf("[graph] Negative weight cycle detected in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // destroy the hash table
                if ( p_result_map ) hash_table_destroy(&p_result_map, NULL);
                if ( p_results )    default_allocator(p_results, 0);

                // error
                return 0;

            failed_to_get_vertices:
                #ifndef NDEBUG
                    printf("[graph] Failed to get vertices in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }

        // standard library errors
        {
            no_mem:
                #ifndef NDEBUG
                    printf("[standard library] Failed to allocate memory in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }

        // hash table errors
        {
            failed_to_construct_hash_table:
                #ifndef NDEBUG
                    printf("[graph] Failed to construct hash table in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // destroy the hash table
                if ( p_result_map ) hash_table_destroy(&p_result_map, NULL);
                if ( p_results )    default_allocator(p_results, 0);

                // error
                return 0;
        }
    }
}

int graph_algorithm_apsp_floyd_warshall
( 
    graph                *p_graph, 
    fn_weight_accessor   *pfn_weight, 
    double             ***ppp_matrix 
)
{

    // argument check
    if ( NULL ==    p_graph ) goto no_graph;
    if ( NULL == pfn_weight ) goto no_weight_accessor;
    if ( NULL == ppp_matrix ) goto no_matrix;

    // initialized data
    size_t               vertex_count = -1;
    void               **pp_vertices  = NULL;
    double             **pp_matrix    = NULL;
    hash_table          *p_index_map  = NULL;
    vertex_index_entry  *p_entries    = NULL;

    // type check
    if ( !(p_graph->_edge_type & GRAPH_WEIGHTED) ) goto no_weighted_graph;
    
    // store the quantity of vertices
    vertex_count = graph_vertex_count(p_graph);

    // allocate memory for vertices
    pp_vertices = default_allocator(NULL, sizeof(void *) * vertex_count);
    if ( NULL == pp_vertices ) goto no_mem;

    // collect all vertices
    if ( 0 == graph_vertex_get(p_graph, pp_vertices) ) goto failed_to_get_vertices;

    // allocate memory for the matrix
    pp_matrix = default_allocator(NULL, sizeof(double *) * vertex_count);
    if ( NULL == pp_matrix ) goto no_mem;

    // allocate rows
    for ( size_t i = 0; i < vertex_count; i++ )
    {

        // allocate a row
        pp_matrix[i] = default_allocator(NULL, sizeof(double) * vertex_count);
        if ( NULL == pp_matrix[i] ) goto no_mem;

        // initialize the matrix
        for ( size_t j = 0; j < vertex_count; j++ )
            pp_matrix[i][j] = (i == j) ? 0.0 : DBL_MAX;
    }

    // allocate memory for entries
    p_entries = default_allocator(NULL, sizeof(vertex_index_entry) * vertex_count);
    if ( NULL == p_entries ) goto no_mem;

    // construct a hash table to map vertices to indices
    if ( 0 == hash_table_construct(&p_index_map, vertex_count * 2 + 1, COLLISION_RESOLUTION_DEFAULT, graph_equality_pointer, vertex_index_key_accessor, graph_hash_pointer) ) goto failed_to_construct_hash_table;

    // populate the index map
    for ( size_t i = 0; i < vertex_count; i++ )

        // store the vertex
        p_entries[i].p_vertex = pp_vertices[i],

        // store the index
        p_entries[i].index = i,

        // populate the index map
        hash_table_insert(p_index_map, &p_entries[i]);
    

    // populate the matrix with edge weights
    for ( size_t i = 0; i < vertex_count; i++ )
    {
        
        // initialized data
        void    *u              = pp_vertices[i];
        void    *u_key          = p_graph->pfn_key_accessor(u);
        size_t   neighbor_count = 0;
        void   **pp_neighbors   = NULL;

        // get the neighboring vertices
        if ( 0 == graph_neighbors_get(p_graph, u_key, &neighbor_count, &pp_neighbors) ) continue;

        // for each neighbor ...
        for ( size_t k = 0; k < neighbor_count; k++ )
        {
            
            // initialized data
            void               *v      = pp_neighbors[k];
            void               *v_key  = p_graph->pfn_key_accessor(v);
            void               *p_edge = NULL;
            vertex_index_entry *v_ent  = NULL;

            // find the edge between u and v
            if ( 0 == graph_edge_search(p_graph, u_key, v_key, &p_edge) ) continue;

            // find the index of the neighbor
            if ( 0 == hash_table_search(p_index_map, v, (void **)&v_ent) ) continue;

            // store the weight in the matrix
            pp_matrix[i][v_ent->index] = pfn_weight(p_edge);
        }

        // release the neighbors
        if ( pp_neighbors ) default_allocator(pp_neighbors, 0);
    }

    // run Floyd-Warshall
    for ( size_t k = 0; k < vertex_count; k++ )
        for ( size_t i = 0; i < vertex_count; i++ )
            for ( size_t j = 0; j < vertex_count; j++ )
                if ( pp_matrix[i][k] != DBL_MAX && pp_matrix[k][j] != DBL_MAX )
                    if ( pp_matrix[i][k] + pp_matrix[k][j] < pp_matrix[i][j] )
                        pp_matrix[i][j] = pp_matrix[i][k] + pp_matrix[k][j];

    // cleanup
    hash_table_destroy(&p_index_map, NULL);
    default_allocator(p_entries, 0);
    default_allocator(pp_vertices, 0);

    // return the matrix to the caller
    *ppp_matrix = pp_matrix;

    // success
    return 1;

    // error handling
    {

        // argument errors
        {
            no_graph:
                #ifndef NDEBUG
                    printf("[graph] Null pointer provided for parameter \"p_graph\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;

            no_weight_accessor:
                #ifndef NDEBUG
                    printf("[graph] Null pointer provided for parameter \"pfn_weight\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;

            no_matrix:
                #ifndef NDEBUG
                    printf("[graph] Null pointer provided for parameter \"ppp_matrix\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }

        // graph errors
        {
            no_weighted_graph:
                #ifndef NDEBUG
                    printf("[graph] Graph is not weighted in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;

            failed_to_get_vertices:
                #ifndef NDEBUG
                    printf("[graph] Failed to get vertices in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // cleanup
                if ( pp_vertices ) default_allocator(pp_vertices, 0);

                // error
                return 0;
        }

        // standard library errors
        {
            no_mem:
                #ifndef NDEBUG
                    printf("[standard library] Failed to allocate memory in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }

        // hash table errors
        {
            failed_to_construct_hash_table:
                #ifndef NDEBUG
                    printf("[graph] Failed to construct hash table in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // cleanup
                if ( p_entries )   default_allocator(p_entries, 0);
                if ( pp_vertices ) default_allocator(pp_vertices, 0);
                if ( pp_matrix ) 
                {
                    for ( size_t i = 0; i < vertex_count; i++ )
                        if ( pp_matrix[i] ) default_allocator(pp_matrix[i], 0);
                    default_allocator(pp_matrix, 0);
                }

                // error
                return 0;
        }
    }
}

int graph_algorithm_apsp_johnson 
( 
    graph              *p_graph, 
    fn_weight_accessor *pfn_weight, 
    double           ***ppp_matrix 
)
{

    // argument check
    if ( NULL ==    p_graph ) goto no_graph;
    if ( NULL == pfn_weight ) goto no_weight_accessor;
    if ( NULL == ppp_matrix ) goto no_matrix;

    // initialized data
    size_t               vertex_count = -1;
    size_t               edge_count   = -1;
    void               **pp_vertices  = NULL;
    double             **pp_matrix    = NULL;
    hash_table          *p_index_map  = NULL;
    vertex_index_entry  *p_entries    = NULL;
    double              *h            = NULL;

    // type check
    if ( !(p_graph->_edge_type & GRAPH_WEIGHTED) ) goto no_weighted_graph;
    
    // store the quantity of vertices
    vertex_count = graph_vertex_count(p_graph);

    // store the quantity of edges
    edge_count = graph_edge_count(p_graph);

    // allocate memory for vertices
    pp_vertices = default_allocator(NULL, sizeof(void *) * vertex_count);
    if ( NULL == pp_vertices ) goto no_mem;

    // collect all vertices
    if ( 0 == graph_vertex_get(p_graph, pp_vertices) ) goto failed_to_get_vertices;

    // allocate memory for entries
    p_entries = default_allocator(NULL, sizeof(vertex_index_entry) * vertex_count);
    if ( NULL == p_entries ) goto no_mem;

    // construct a hash table to map vertices to indices
    if ( 0 == hash_table_construct(&p_index_map, vertex_count * 2 + 1, LINEAR_PROBE, graph_equality_pointer, vertex_index_key_accessor, graph_hash_pointer) ) goto failed_to_construct_hash_table;

    // iterate through each vertex
    for ( size_t i = 0; i < vertex_count; i++ )

        // store the vertex
        p_entries[i].p_vertex = pp_vertices[i],

        // store the index
        p_entries[i].index = i,

        // populate the index map
        hash_table_insert(p_index_map, &p_entries[i]);
    
    // Step 1: Bellman-Ford from virtual source
    h = default_allocator(NULL, sizeof(double) * vertex_count);
    if ( NULL == h ) goto no_mem;

    // initialize distances
    for ( size_t i = 0; i < vertex_count; i++ ) 
        h[i] = 0.0;

    // bellman-ford
    for ( size_t i = 0; i < vertex_count - 1; i++ )
        for ( size_t j = 0; j < vertex_count; j++ )
        {

            // initialized data
            void    *u              = pp_vertices[j];
            void    *u_key          = p_graph->pfn_key_accessor(u);
            size_t   neighbor_count = 0;
            void   **pp_neighbors   = NULL;

            // skip
            if ( 0 == graph_neighbors_get(p_graph, u_key, &neighbor_count, &pp_neighbors) ) continue;

            // iterate through each edge
            for ( size_t k = 0; k < neighbor_count; k++ )
            {

                // initialized data
                void               *v      = pp_neighbors[k];
                void               *v_key  = p_graph->pfn_key_accessor(v);
                void               *p_edge = NULL;
                vertex_index_entry *v_ent  = NULL;

                // skip
                if ( 0 == graph_edge_search(p_graph, u_key, v_key, &p_edge) ) continue;

                // skip
                if ( 0 == hash_table_search(p_index_map, v, (void **)&v_ent) ) continue;

                // relax
                if ( h[j] + pfn_weight(p_edge) < h[v_ent->index] )
                    h[v_ent->index] = h[j] + pfn_weight(p_edge);
            }

            // release neighbors
            if ( pp_neighbors ) 
                pp_neighbors = default_allocator(pp_neighbors, 0);
        }
    
    // check for negative cycles
    for ( size_t j = 0; j < vertex_count; j++ )
    {

        // initialized data
        void    *u              = pp_vertices[j];
        void    *u_key          = p_graph->pfn_key_accessor(u);
        size_t   neighbor_count = 0;
        void   **pp_neighbors   = NULL;

        // skip
        if ( 0 == graph_neighbors_get(p_graph, u_key, &neighbor_count, &pp_neighbors) ) continue;

        // iterate through each edge
        for ( size_t k = 0; k < neighbor_count; k++ )
        {

            // initialized data
            void               *v      = pp_neighbors[k];
            void               *v_key  = p_graph->pfn_key_accessor(v);
            void               *p_edge = NULL;
            vertex_index_entry *v_ent  = NULL;

            // skip
            if ( 0 == graph_edge_search(p_graph, u_key, v_key, &p_edge) ) continue;

            // skip
            if ( 0 == hash_table_search(p_index_map, v, (void **)&v_ent) ) continue;

            // negative cycle?
            if ( h[j] + pfn_weight(p_edge) < h[v_ent->index] )
            {

                // release neighbors
                if ( pp_neighbors ) 
                    pp_neighbors = default_allocator(pp_neighbors, 0);
                
                // bail
                goto negative_cycle;
            }
        }

        // release neighbors
        if ( pp_neighbors ) 
            pp_neighbors = default_allocator(pp_neighbors, 0);
    }

    // allocate a distances matrix
    pp_matrix = default_allocator(NULL, sizeof(double *) * vertex_count);
    if ( NULL == pp_matrix ) goto no_mem;

    // iterate through each vertex
    for ( size_t i = 0; i < vertex_count; i++ )
    {

        // initialized data
        priority_queue        *p_priority_queue         = NULL;
        johnson_dijkstra_node *p_start_node = NULL;
        hash_table            *p_visited    = NULL;

        // allocate a row
        pp_matrix[i] = default_allocator(NULL, sizeof(double) * vertex_count);
        if ( NULL == pp_matrix[i] ) goto no_mem;

        // initialize the distances
        for ( size_t j = 0; j < vertex_count; j++ ) 
            pp_matrix[i][j] = DBL_MAX;

        // 0's along the diagonal
        pp_matrix[i][i] = 0.0;

        // construct a priority queue
        if ( 0 == priority_queue_construct(&p_priority_queue, edge_count + 1, johnson_compare) ) goto failed_to_construct_pq;

        // allocate the starting node
        p_start_node = default_allocator(NULL, sizeof(johnson_dijkstra_node));

        // store the index
        p_start_node->index = i;

        // store the distance
        p_start_node->distance = 0.0;

        // enqueue the starting node
        priority_queue_enqueue(p_priority_queue, p_start_node);

        // construct a hash table to track visited vertices
        if ( 0 == hash_table_construct(&p_visited, vertex_count * 2 + 1, LINEAR_PROBE, graph_equality_pointer, NULL, graph_hash_pointer) ) 
        {

            // destroy the priority queue
            priority_queue_destroy(&p_priority_queue);

            // bail
            goto failed_to_construct_hash_table;
        }

        // run dijkstra
        while ( false == priority_queue_empty(p_priority_queue) )
        {

            // initialized data
            johnson_dijkstra_node  *u_node         = NULL;
            size_t                  u_idx          = -1; 
            void                   *u              = NULL;
            void                   *p_dummy        = NULL;
            void                   *u_key          = NULL;
            size_t                  neighbor_count = 0;
            void                  **pp_neighbors   = NULL;

            // dequeue u
            priority_queue_dequeue(p_priority_queue, (void **)&u_node);

            // store the index of u
            u_idx = u_node->index;

            // store u
            u = pp_vertices[u_idx];

            // u has been visited ...
            if ( 1 == hash_table_search(p_visited, u, &p_dummy) )
            {

                // skip
                default_allocator(u_node, 0);
                continue;
            }

            // mark u as visited
            hash_table_insert(p_visited, u);

            // store u's key
            u_key = p_graph->pfn_key_accessor(u);

            // get u's neighbors
            if ( 1 == graph_neighbors_get(p_graph, u_key, &neighbor_count, &pp_neighbors) )
            {

                // iterate through each neighbor
                for ( size_t k = 0; k < neighbor_count; k++ )
                {

                    // initialized data
                    void               *v          = pp_neighbors[k];
                    void               *v_key      = p_graph->pfn_key_accessor(v);
                    void               *p_edge     = NULL;
                    vertex_index_entry *v_ent      = NULL;
                    size_t              v_idx      = -1;
                    double              weight     = -1;
                    double              hat_weight = -1;

                    // skip
                    if ( 0 == graph_edge_search(p_graph, u_key, v_key, &p_edge) ) continue;

                    // skip
                    if ( 0 == hash_table_search(p_index_map, v, (void **)&v_ent) ) continue;
                    
                    // store the index of v 
                    v_idx = v_ent->index;

                    // store the weight of the edge
                    weight = pfn_weight(p_edge);

                    // compute the new weight
                    hat_weight = weight + h[u_idx] - h[v_idx];

                    if ( pp_matrix[i][u_idx] + hat_weight < pp_matrix[i][v_idx] )
                    {

                        // initialized data
                        johnson_dijkstra_node *v_node = NULL;

                        // update distances
                        pp_matrix[i][v_idx] = pp_matrix[i][u_idx] + hat_weight;

                        // allocate a node 
                        v_node = default_allocator(NULL, sizeof(johnson_dijkstra_node));

                        // store the index
                        v_node->index = v_idx;

                        // store the distance
                        v_node->distance = pp_matrix[i][v_idx];

                        // enqueue the node
                        priority_queue_enqueue(p_priority_queue, v_node);
                    }
                }

                // release neighbors
                if ( pp_neighbors ) 
                    pp_neighbors = default_allocator(pp_neighbors, 0);
            }

            // release u
            u_node = default_allocator(u_node, 0);
        }

        // finalize distances for row i
        for ( size_t j = 0; j < vertex_count; j++ )
            if ( pp_matrix[i][j] != DBL_MAX )
                pp_matrix[i][j] = pp_matrix[i][j] - h[i] + h[j];

        // release the priority queue
        priority_queue_destroy(&p_priority_queue);

        // release the visited map
        hash_table_destroy(&p_visited, NULL);
    }

    // release the index map
    hash_table_destroy(&p_index_map, NULL);

    // release entries
    p_entries = default_allocator(p_entries, 0);

    // release the vertex list
    pp_vertices = default_allocator(pp_vertices, 0);

    // release the distances
    h = default_allocator(h, 0);

    // return the matrix to the caller
    *ppp_matrix = pp_matrix;

    // success
    return 1;

    // error handling
    {

        // argument errors
        {
            no_graph:
                #ifndef NDEBUG
                    printf("[graph] Null pointer provided for parameter \"p_graph\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;

            no_weight_accessor:
                #ifndef NDEBUG
                    printf("[graph] Null pointer provided for parameter \"pfn_weight\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;

            no_matrix:
                #ifndef NDEBUG
                    printf("[graph] Null pointer provided for parameter \"ppp_matrix\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }

        // standard library errors
        {
            no_mem:
                #ifndef NDEBUG
                    printf("[standard library] Failed to allocate memory in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }

        // graph errors
        {
            no_weighted_graph:
                #ifndef NDEBUG
                    printf("[graph] Graph is not weighted in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;

            failed_to_get_vertices:
                #ifndef NDEBUG
                    printf("[graph] Failed to get vertices in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // cleanup
                if ( pp_vertices ) default_allocator(pp_vertices, 0);

                // error
                return 0;

            negative_cycle:
                #ifndef NDEBUG
                    printf("[graph] Negative weight cycle detected in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // cleanup
                if ( p_entries )   default_allocator(p_entries, 0);
                if ( pp_vertices ) default_allocator(pp_vertices, 0);
                if ( h )           default_allocator(h, 0);
                if ( p_index_map ) hash_table_destroy(&p_index_map, NULL);
                if ( pp_matrix ) 
                {
                    for ( size_t i = 0; i < vertex_count; i++ )
                        if ( pp_matrix[i] ) default_allocator(pp_matrix[i], 0);
                    default_allocator(pp_matrix, 0);
                }

                // error
                return 0;
        }

        // priority queue errors
        {
            failed_to_construct_pq:
                #ifndef NDEBUG
                    printf("[graph] Failed to construct priority queue in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // cleanup
                if ( p_entries )   default_allocator(p_entries, 0);
                if ( pp_vertices ) default_allocator(pp_vertices, 0);
                if ( h )           default_allocator(h, 0);
                if ( p_index_map ) hash_table_destroy(&p_index_map, NULL);
                if ( pp_matrix ) 
                {
                    for ( size_t i = 0; i < vertex_count; i++ )
                        if ( pp_matrix[i] ) default_allocator(pp_matrix[i], 0);
                    default_allocator(pp_matrix, 0);
                }

                // error
                return 0;
        }

        // hash table errors
        {
            failed_to_construct_hash_table:
                #ifndef NDEBUG
                    printf("[graph] Failed to construct hash table in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // cleanup
                if ( p_entries )   default_allocator(p_entries, 0);
                if ( pp_vertices ) default_allocator(pp_vertices, 0);
                if ( h )           default_allocator(h, 0);
                if ( p_index_map ) hash_table_destroy(&p_index_map, NULL);
                if ( pp_matrix ) 
                {
                    for ( size_t i = 0; i < vertex_count; i++ )
                        if ( pp_matrix[i] ) default_allocator(pp_matrix[i], 0);
                    default_allocator(pp_matrix, 0);
                }

                // error
                return 0;
        }
    }
}

int graph_algorithm_mst_kruskal 
( 
    graph              *p_graph, 
    fn_weight_accessor *pfn_weight, 
    graph             **pp_mst 
)
{

    // argument check
    if ( NULL ==    p_graph ) goto no_graph;
    if ( NULL == pfn_weight ) goto no_weight_accessor;
    if ( NULL ==     pp_mst ) goto no_mst;

    // initialized data
    graph               *p_mst           = NULL;
    size_t               vertex_count    = graph_vertex_count(p_graph);
    size_t               edge_count      = graph_edge_count(p_graph);
    void               **pp_vertices     = NULL;
    mst_edge            *p_edges         = NULL;
    size_t               collected_edges = 0;
    union_find          *p_union_find    = NULL;
    hash_table          *p_index_map     = NULL;
    vertex_index_entry  *p_entries       = NULL;

    // type check
    if ( !(p_graph->_edge_type & GRAPH_WEIGHTED) ) goto no_weighted_graph;

    // construct the MST graph
    if ( 0 == graph_construct(&p_mst, p_graph->_type, p_graph->_edge_type, p_graph->vertex_size, p_graph->edge_size, p_graph->pfn_key_accessor, p_graph->pfn_comparator) ) goto failed_to_construct_mst;

    // if the graph is empty ...
    if ( 0 == vertex_count )
    {

        // ... return an empty graph
        *pp_mst = p_mst;

        // success
        return 1;
    }

    // allocate memory for vertices
    pp_vertices = default_allocator(NULL, sizeof(void *) * vertex_count);
    if ( NULL == pp_vertices ) goto no_mem;

    // collect all vertices
    if ( 0 == graph_vertex_get(p_graph, pp_vertices) ) goto failed_to_get_vertices;

    // populate the mst graph
    for ( size_t i = 0; i < vertex_count; i++ )
        graph_vertex_add(p_mst, pp_vertices[i]);

    // allocate memory for entries
    p_entries = default_allocator(NULL, sizeof(vertex_index_entry) * vertex_count);
    if ( NULL == p_entries ) goto no_mem;

    // construct a hash table to map vertices to indices
    if ( 0 == hash_table_construct(&p_index_map, vertex_count * 2 + 1, COLLISION_RESOLUTION_DEFAULT, graph_equality_pointer, vertex_index_key_accessor, graph_hash_pointer) ) goto failed_to_construct_hash_table;

    // populate the index map
    for ( size_t i = 0; i < vertex_count; i++ )
        p_entries[i].p_vertex = pp_vertices[i],
        p_entries[i].index = i,
        hash_table_insert(p_index_map, &p_entries[i]);

    // allocate memory for edges
    p_edges = default_allocator(NULL, sizeof(mst_edge) * edge_count * 2);
    if ( NULL == p_edges ) goto no_mem;

    // iterate through each vertex
    for ( size_t i = 0; i < vertex_count; i++ )
    {

        // initialized data
        void    *u              = pp_vertices[i];
        void    *u_key          = p_graph->pfn_key_accessor(u);
        size_t   neighbor_count = 0;
        void   **pp_neighbors   = NULL;

        // get neighbors
        if ( 1 == graph_neighbors_get(p_graph, u_key, &neighbor_count, &pp_neighbors) )
        {

            // iterate through each neighbor
            for ( size_t j = 0; j < neighbor_count; j++ )
            {

                // initialized data
                void *v      = pp_neighbors[j];
                void *v_key  = p_graph->pfn_key_accessor(v);
                void *p_edge = NULL;

                // undirected optimization
                if ( !(p_graph->_edge_type & GRAPH_DIRECTED) )
                    if ( p_graph->pfn_comparator(u_key, v_key) < 0 ) 
                        continue;

                // edge?
                if ( 1 == graph_edge_search(p_graph, u_key, v_key, &p_edge) )

                    // populate the edge struct
                    p_edges[collected_edges] = (mst_edge)
                    {
                        .u      = u,
                        .v      = v,
                        .weight = pfn_weight(p_edge)
                    },

                    // increment edge counter
                    collected_edges++;
            }
            
            // release neighbors
            if ( pp_neighbors ) 
                default_allocator(pp_neighbors, 0);
        }
    }

    // sort the edges by weight
    qsort(p_edges, collected_edges, sizeof(mst_edge), kruskal_edge_compare);

    // construct a union find
    union_find_construct(&p_union_find, vertex_count);

    // iterate over sorted edges
    for ( size_t i = 0; i < collected_edges; i++ )
    {

        // initialized data
        vertex_index_entry *u_ent = NULL;
        vertex_index_entry *v_ent = NULL;

        // lookup u
        hash_table_search(p_index_map, p_edges[i].u, (void **)&u_ent);

        // lookup v
        hash_table_search(p_index_map, p_edges[i].v, (void **)&v_ent);

        // different trees?
        if ( union_find_find(p_union_find, u_ent->index) != union_find_find(p_union_find, v_ent->index) )
        {

            // initialized data
            void *u_key  = p_graph->pfn_key_accessor(p_edges[i].u);
            void *v_key  = p_graph->pfn_key_accessor(p_edges[i].v);
            void *p_edge = NULL;

            // store the edge
            graph_edge_search(p_graph, u_key, v_key, &p_edge);

            // add it to the mst
            graph_edge_add(p_mst, u_key, v_key, p_edge);

            // merge u and v
            union_find_union(p_union_find, u_ent->index, v_ent->index);
        }
    }

    // release the union find
    union_find_destroy(&p_union_find);

    // release the index map
    hash_table_destroy(&p_index_map, NULL);

    // release vertex index entries
    default_allocator(p_entries, 0);

    // release vertex list
    default_allocator(pp_vertices, 0);

    // release edge list
    default_allocator(p_edges, 0);

    // return an mst to the caller
    *pp_mst = p_mst;

    // success
    return 1;

    // error handling
    {

        // argument errors
        {
            no_graph:
                #ifndef NDEBUG
                    printf("[graph] Null pointer provided for parameter \"p_graph\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;

            no_weight_accessor:
                #ifndef NDEBUG
                    printf("[graph] Null pointer provided for parameter \"pfn_weight\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;

            no_mst:
                #ifndef NDEBUG
                    printf("[graph] Null pointer provided for parameter \"pp_mst\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }

        // graph errors
        {
            no_weighted_graph:
                #ifndef NDEBUG
                    printf("[graph] Graph is not weighted in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;

            failed_to_construct_mst:
                #ifndef NDEBUG
                    printf("[graph] Failed to construct MST graph in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;

            failed_to_get_vertices:
                #ifndef NDEBUG
                    printf("[graph] Failed to get vertices in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }

        // standard library errors
        {
            no_mem:
                #ifndef NDEBUG
                    printf("[standard library] Failed to allocate memory in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // cleanup
                if ( p_mst )       graph_destroy(&p_mst, NULL, NULL);
                if ( pp_vertices ) default_allocator(pp_vertices, 0);
                if ( p_entries )   default_allocator(p_entries, 0);
                if ( p_index_map ) hash_table_destroy(&p_index_map, NULL);
                if ( p_edges )     default_allocator(p_edges, 0);

                // error
                return 0;
        }

        // hash table errors
        {
            failed_to_construct_hash_table:
                #ifndef NDEBUG
                    printf("[graph] Failed to construct hash table in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // cleanup
                if ( p_mst )       graph_destroy(&p_mst, NULL, NULL);
                if ( pp_vertices ) default_allocator(pp_vertices, 0);
                if ( p_entries )   default_allocator(p_entries, 0);

                // error
                return 0;
        }
    }
}

int graph_algorithm_mst_prim
( 
    graph              *p_graph, 
    fn_weight_accessor *pfn_weight, 
    graph             **pp_mst 
)
{

    // argument check
    if ( NULL ==    p_graph ) goto no_graph;
    if ( NULL == pfn_weight ) goto no_weight_accessor;
    if ( NULL ==     pp_mst ) goto no_mst;

    // initialized data
    priority_queue  *p_priority_queue = NULL;
    hash_table      *p_visited        = NULL;
    graph           *p_mst            = NULL;
    size_t           vertex_count     = graph_vertex_count(p_graph);
    size_t           edge_count       = graph_edge_count(p_graph);
    void           **pp_vertices      = NULL;
    void            *p_start_vertex   = NULL;
    void            *u_key            = NULL;  
    size_t           neighbor_count   = 0;
    void           **pp_neighbors     = NULL;
    void            *p_mst_edge       = NULL;

    // type check
    if ( !(p_graph->_edge_type & GRAPH_WEIGHTED) ) goto no_weighted_graph;

    // construct the mst graph
    if ( 0 == graph_construct(&p_mst, p_graph->_type, p_graph->_edge_type, p_graph->vertex_size, p_graph->edge_size, p_graph->pfn_key_accessor, p_graph->pfn_comparator) ) goto failed_to_construct_mst;

    // if the graph is empty ...
    if ( 0 == vertex_count )
    {

        // ... return an empty graph
        *pp_mst = p_mst;

        // success
        return 1;
    }

    // allocate memory for vertices
    pp_vertices = default_allocator(NULL, sizeof(void *) * vertex_count);
    if ( NULL == pp_vertices ) goto no_mem;

    // collect all vertices
    if ( 0 == graph_vertex_get(p_graph, pp_vertices) ) goto failed_to_get_vertices;

    // populate the mst graph
    for ( size_t i = 0; i < vertex_count; i++ )
        graph_vertex_add(p_mst, pp_vertices[i]);

    // pick a starting vertex
    p_start_vertex = pp_vertices[0];

    // construct a priority queue
    if ( 0 == priority_queue_construct(&p_priority_queue, edge_count + 1, prim_edge_compare) ) goto failed_to_construct_pq;

    // construct a hash table to track visited vertices
    if ( 0 == hash_table_construct(&p_visited, vertex_count * 2 + 1, COLLISION_RESOLUTION_DEFAULT, graph_equality_pointer, NULL, graph_hash_pointer) ) goto failed_to_construct_hash_table;

    // mark the start vertex as visited
    hash_table_insert(p_visited, p_start_vertex);

    // store the key of the starting vertex
    u_key = p_graph->pfn_key_accessor(p_start_vertex);

    // get neighbors of the starting vertex
    if ( 1 == graph_neighbors_get(p_graph, u_key, &neighbor_count, &pp_neighbors) )
    {

        // iterate through each neighbor
        for ( size_t i = 0; i < neighbor_count; i++ )
        {

            // initialized data
            void     *v          = pp_neighbors[i];
            void     *v_key      = p_graph->pfn_key_accessor(v);
            void     *p_edge     = NULL;
            mst_edge *p_mst_edge = default_allocator(NULL, sizeof(mst_edge));

            // skip
            if ( 0 == graph_edge_search(p_graph, u_key, v_key, &p_edge) ) 
            {
                p_mst_edge = default_allocator(p_mst_edge, 0);
                continue;
            }

            // populate the mst edge
            *p_mst_edge = (mst_edge)
            {
                .u      = p_start_vertex,
                .v      = v,
                .weight = pfn_weight(p_edge)
            };

            // enqueue the mst edge
            priority_queue_enqueue(p_priority_queue, p_mst_edge);
        }

        // release neighbors
        if ( pp_neighbors ) 
            pp_neighbors = default_allocator(pp_neighbors, 0);
    }

    // traverse the graph
    while ( false == priority_queue_empty(p_priority_queue) )
    {
        
        // initialized data
        mst_edge *p_mst_edge = NULL;
        void     *v          = NULL;
        void     *v_dummy    = NULL;

        // dequeue the edge with the smallest weight
        if ( 0 == priority_queue_dequeue(p_priority_queue, (void **)&p_mst_edge) ) break;

        // store v
        v = p_mst_edge->v;

        // skip
        if ( 1 == hash_table_search(p_visited, v, &v_dummy) )
        {
            p_mst_edge = default_allocator(p_mst_edge, 0);
            continue;
        }

        // mark the vertex as visited
        hash_table_insert(p_visited, v);

        // add the edge to the MST graph
        {

            // initialized data
            void *u_key  = p_graph->pfn_key_accessor(p_mst_edge->u);
            void *v_key  = p_graph->pfn_key_accessor(v);
            void *p_edge = NULL;

            // store the edge
            graph_edge_search(p_graph, u_key, v_key, &p_edge);

            // add it to the mst
            graph_edge_add(p_mst, u_key, v_key, p_edge);
        }

        // add neighbors to the priority queue
        {

            // initialized data
            void    *v_key          = p_graph->pfn_key_accessor(v);
            size_t   neighbor_count = 0;
            void   **pp_neighbors   = NULL;

            // store neighboring vertices
            if ( 1 == graph_neighbors_get(p_graph, v_key, &neighbor_count, &pp_neighbors) )
            {

                // iterate through each neighbor
                for ( size_t i = 0; i < neighbor_count; i++ )
                {

                    // initialized data
                    void *w      = pp_neighbors[i];
                    void *w_key  = p_graph->pfn_key_accessor(w);
                    void *p_edge = NULL;
                    
                    // skip
                    if ( 1 == hash_table_search(p_visited, w, &v_dummy) ) continue;

                    // store the edge
                    if ( 1 == graph_edge_search(p_graph, v_key, w_key, &p_edge) )
                    {

                        // initialized data
                        mst_edge *p_new_me = default_allocator(NULL, sizeof(mst_edge));

                        // populate the new mst edge
                        *p_new_me = (mst_edge) 
                        {   
                            .u      = v,
                            .v      = w,
                            .weight = pfn_weight(p_edge),
                        };

                        // enqueue the new mst edge
                        priority_queue_enqueue(p_priority_queue, p_new_me);
                    }
                }

                // release the neighbors
                if ( pp_neighbors ) 
                    default_allocator(pp_neighbors, 0);
            }
        }

        // release the mst edge
        default_allocator(p_mst_edge, 0);
    }

    // drain the priority queue
    while ( false == priority_queue_empty(p_priority_queue) )
        if ( priority_queue_dequeue(p_priority_queue, &p_mst_edge) )
            p_mst_edge = default_allocator(p_mst_edge, 0);

    // release the priority queue 
    priority_queue_destroy(&p_priority_queue);

    // release the hash table
    hash_table_destroy(&p_visited, NULL);

    // release the vertices
    pp_vertices = default_allocator(pp_vertices, 0);

    // return a pointer to the caller
    *pp_mst = p_mst;

    // success
    return 1;

    // error handling
    {

        // argument errors
        {
            no_graph:
                #ifndef NDEBUG
                    printf("[graph] Null pointer provided for parameter \"p_graph\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;

            no_weight_accessor:
                #ifndef NDEBUG
                    printf("[graph] Null pointer provided for parameter \"pfn_weight\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;

            no_mst:
                #ifndef NDEBUG
                    printf("[graph] Null pointer provided for parameter \"pp_mst\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }

        // graph errors
        {
            no_weighted_graph:
                #ifndef NDEBUG
                    printf("[graph] Graph is not weighted in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;

            failed_to_construct_mst:
                #ifndef NDEBUG
                    printf("[graph] Failed to construct MST graph in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;

            failed_to_get_vertices:
                #ifndef NDEBUG
                    printf("[graph] Failed to get vertices in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }

        // standard library errors
        {
            no_mem:
                #ifndef NDEBUG
                    printf("[standard library] Failed to allocate memory in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // cleanup
                if ( p_mst )       graph_destroy(&p_mst, NULL, NULL);
                if ( pp_vertices ) default_allocator(pp_vertices, 0);

                // error
                return 0;
        }

        // priority queue errors
        {
            failed_to_construct_pq:
                #ifndef NDEBUG
                    printf("[graph] Failed to construct priority queue in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // cleanup
                if ( p_mst )       graph_destroy(&p_mst, NULL, NULL);
                if ( pp_vertices ) default_allocator(pp_vertices, 0);

                // error
                return 0;
        }

        // hash table errors
        {
            failed_to_construct_hash_table:
                #ifndef NDEBUG
                    printf("[graph] Failed to construct hash table in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // cleanup
                if ( p_mst )       graph_destroy(&p_mst, NULL, NULL);
                if ( pp_vertices ) default_allocator(pp_vertices, 0);
                if ( p_priority_queue ) 
                {
                    void *p_mst_edge = NULL;
                    while ( false == priority_queue_empty(p_priority_queue) )
                        if ( priority_queue_dequeue(p_priority_queue, &p_mst_edge) )
                            default_allocator(p_mst_edge, 0);
                    priority_queue_destroy(&p_priority_queue);
                }

                // error
                return 0;
        }
    }
}

int graph_vertex_get 
( 
    graph  *p_graph, 
    void  **p_p_vertices 
)
{

    // argument check
    if ( NULL == p_graph ) goto no_graph;
    
    // done
    return p_graph->pfn_vertex_get(p_graph->p_graph, p_p_vertices);

    // error handling
    {

        // argument errors
        {
            no_graph:
                #ifndef NDEBUG
                    printf("[graph] Null pointer provided for parameter \"p_graph\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
    }
}

int graph_vertex_search 
( 
    graph *p_graph, 
    const void *p_key, 
    void **pp_vertex
)
{

    // argument check
    if ( NULL == p_graph ) goto no_graph;
    
    // done
    return p_graph->pfn_vertex_search(p_graph->p_graph, p_key, pp_vertex);

    // error handling
    {

        // argument errors
        {
            no_graph:
                #ifndef NDEBUG
                    printf("[graph] Null pointer provided for parameter \"p_graph\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
    }
}

size_t graph_vertex_count ( graph *p_graph )
{

    // argument check
    if ( NULL == p_graph ) goto no_graph;
    
    // done
    return p_graph->pfn_vertex_count(p_graph->p_graph);

    // error handling
    {

        // argument errors
        {
            no_graph:
                #ifndef NDEBUG
                    printf("[graph] Null pointer provided for parameter \"p_graph\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
    }
}

int graph_edge_search 
( 
    graph *p_graph, 
    const void *p_from, 
    const void *p_to, 
    void **pp_edge 
)
{

    // argument check
    if ( NULL == p_graph ) goto no_graph;
    
    // done
    return p_graph->pfn_edge_search(p_graph->p_graph, p_from, p_to, pp_edge);

    // error handling
    {

        // argument errors
        {
            no_graph:
                #ifndef NDEBUG
                    printf("[graph] Null pointer provided for parameter \"p_graph\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
    }
}

size_t graph_edge_count ( graph *p_graph )
{

    // argument check
    if ( NULL == p_graph ) goto no_graph;
    
    // done
    return p_graph->pfn_edge_count(p_graph->p_graph);

    // error handling
    {

        // argument errors
        {
            no_graph:
                #ifndef NDEBUG
                    printf("[graph] Null pointer provided for parameter \"p_graph\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
    }
}

int graph_vertex_degree
(
    graph *p_graph,
    const void *p_key,
    size_t *p_in_degree,
    size_t *p_out_degree
)
{

    // argument check
    if ( NULL == p_graph ) goto no_graph;
    
    // done
    return p_graph->pfn_vertex_degree(p_graph->p_graph, p_key, p_in_degree, p_out_degree);

    // error handling
    {

        // argument errors
        {
            no_graph:
                #ifndef NDEBUG
                    printf("[graph] Null pointer provided for parameter \"p_graph\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
    }
}

int graph_neighbors_get
(
    graph *p_graph,
    const void *p_key,
    size_t *p_count,
    void ***ppp_neighbors
)
{

    // argument check
    if ( NULL == p_graph ) goto no_graph;
    
    // done
    return p_graph->pfn_neighbors_get(p_graph->p_graph, p_key, p_count, ppp_neighbors);

    // error handling
    {

        // argument errors
        {
            no_graph:
                #ifndef NDEBUG
                    printf("[graph] Null pointer provided for parameter \"p_graph\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
    }
}

int graph_vertex_add ( graph *const p_graph, const void *p_vertex )
{

    // argument check
    if ( NULL == p_graph ) goto no_graph;
    
    // done
    return p_graph->pfn_vertex_add(p_graph->p_graph, p_vertex);

    // error handling
    {

        // argument errors
        {
            no_graph:
                #ifndef NDEBUG
                    printf("[graph] Null pointer provided for parameter \"p_graph\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
    }
}

int graph_vertex_remove 
( 
    graph *const p_graph, 
    const void *p_key, 
    void **pp_vertex, 
    fn_allocator *pfn_allocator_vertex, 
    fn_allocator *pfn_allocator_edge
)
{

    // argument check
    if ( NULL == p_graph ) goto no_graph;
    
    // done
    return p_graph->pfn_vertex_remove(p_graph->p_graph, p_key, pp_vertex, pfn_allocator_vertex, pfn_allocator_edge);

    // error handling
    {

        // argument errors
        {
            no_graph:
                #ifndef NDEBUG
                    printf("[graph] Null pointer provided for parameter \"p_graph\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
    }
}

int graph_edge_add 
( 
    graph      *p_graph, 
    const void *p_from, 
    const void *p_to, 
    void       *p_edge
)
{

    // argument check
    if ( NULL == p_graph ) goto no_graph;
    
    // done
    return p_graph->pfn_edge_add(p_graph->p_graph, p_from, p_to, p_edge);

    // error handling
    {

        // argument errors
        {
            no_graph:
                #ifndef NDEBUG
                    printf("[graph] Null pointer provided for parameter \"p_graph\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
    }
}

int graph_edge_remove 
( 
    graph      *p_graph, 
    const void *p_from, 
    const void *p_to, 
    void       **pp_edge,
    fn_allocator *pfn_allocator_edge
)
{

    // argument check
    if ( NULL == p_graph ) goto no_graph;
    
    // done
    return p_graph->pfn_edge_remove(p_graph->p_graph, p_from, p_to, pp_edge, pfn_allocator_edge);

    // error handling
    {

        // argument errors
        {
            no_graph:
                #ifndef NDEBUG
                    printf("[graph] Null pointer provided for parameter \"p_graph\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
    }
}

int graph_vertex_foreach
(
    graph      *p_graph,
    fn_foreach *pfn_foreach
)
{

    // argument check
    if ( NULL == p_graph ) goto no_graph;
    
    // done
    return p_graph->pfn_vertex_foreach(p_graph->p_graph, pfn_foreach);

    // error handling
    {

        // argument errors
        {
            no_graph:
                #ifndef NDEBUG
                    printf("[graph] Null pointer provided for parameter \"p_graph\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
    }
}

int graph_edge_foreach
(
    graph      *p_graph,
    fn_foreach *pfn_foreach
)
{

    // argument check
    if ( NULL == p_graph ) goto no_graph;
    
    // done
    return p_graph->pfn_edge_foreach(p_graph->p_graph, pfn_foreach);

    // error handling
    {

        // argument errors
        {
            no_graph:
                #ifndef NDEBUG
                    printf("[graph] Null pointer provided for parameter \"p_graph\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
    }
}

int graph_pack
(
    void *p_buffer, 
    graph *p_graph,

    fn_pack *pfn_vertex,
    fn_pack *pfn_edge
)
{

    // argument check
    if ( NULL ==  p_graph ) goto no_graph;
    if ( NULL == p_buffer ) goto no_buffer;

    // initialized data
    char *p = p_buffer;

    // pack the storage type
    p += pack_pack(p, "%i32", p_graph->_type);

    // done
    p += p_graph->pfn_pack(p, p_graph->p_graph, pfn_vertex, pfn_edge);

    // success
    return p - (char *)p_buffer;

    // error handling
    {

        // argument errors
        {
            no_graph:
                #ifndef NDEBUG
                    printf("[graph] Null pointer provided for parameter \"p_graph\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;

            no_buffer:
                #ifndef NDEBUG
                    printf("[graph] Null pointer provided for parameter \"p_buffer\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
    }
}

int graph_unpack
(
    graph **pp_graph,
    void *p_buffer, 

    fn_unpack *pfn_vertex,
    fn_unpack *pfn_edge,

    fn_key_accessor *pfn_key_accessor,
    fn_comparator   *pfn_comparator
)
{
    
    // argument check
    if ( NULL == pp_graph ) goto no_graph;
    if ( NULL == p_buffer ) goto no_buffer;

    // initialized data
    char *p = p_buffer;
    enum graph_storage_type_e _storage_type = 0;
    graph *p_graph = NULL;
    void *p_concrete_graph = NULL;

    // unpack the storage type
    p += pack_unpack(p, "%i32", &_storage_type);

    // validate the storage type
    if ( _storage_type >= GRAPH_QUANTITY ) goto invalid_storage_type;

    // allocate a graph 
    p_graph = default_allocator(NULL, sizeof(graph));
    if ( NULL == p_graph ) goto no_mem;

    // clone the prototype
    memcpy(p_graph, &_prototypes[_storage_type], sizeof(graph));

    // unpack the concrete graph
    p += p_graph->pfn_unpack(&p_concrete_graph, p, pfn_vertex, pfn_edge, pfn_key_accessor, pfn_comparator);

    // set the concrete graph
    p_graph->p_graph = p_concrete_graph;

    // populate wrapper fields from the concrete graph
    {

        // initialized data
        enum graph_edge_type_e _edge_type = 0;
        size_t vertex_size = 0;
        size_t edge_size = 0;
        char *p_peek = (char *)p_buffer + sizeof(enum graph_storage_type_e);

        // unpack metadata
        pack_unpack(p_peek, "%i32%2i64", &_edge_type, &vertex_size, &edge_size);

        // store the metadata in the wrapper
        p_graph->_edge_type = _edge_type;
        p_graph->vertex_size = vertex_size;
        p_graph->edge_size = edge_size;
        p_graph->pfn_key_accessor = pfn_key_accessor;
        p_graph->pfn_comparator = pfn_comparator;
    }

    // return the graph to the caller
    *pp_graph = p_graph;

    // success
    return p - (char *)p_buffer;

    // error handling
    {

        // argument errors
        {
            no_graph:
                #ifndef NDEBUG
                    printf("[graph] Null pointer provided for parameter \"pp_graph\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;

            no_buffer:
                #ifndef NDEBUG
                    printf("[graph] Null pointer provided for parameter \"p_buffer\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }

        // graph errors
        {
            invalid_storage_type:
                #ifndef NDEBUG
                    printf("[graph] Invalid storage type in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }

        // standard library errors
        {
            no_mem:
                #ifndef NDEBUG
                    printf("[standard library] Failed to allocate memory in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
    }
}

hash64 graph_hash ( graph *const p_graph, fn_hash64 *pfn_hash64 )
{

    // argument check
    if ( NULL == p_graph ) goto no_graph;
    
    // done
    return p_graph->pfn_hash(p_graph->p_graph, pfn_hash64);

    // error handling
    {

        // argument errors
        {
            no_graph:
                #ifndef NDEBUG
                    printf("[graph] Null pointer provided for parameter \"p_graph\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
    }
}

int graph_destroy
(
    graph **pp_graph,
    
    fn_allocator *pfn_allocator_vertex,
    fn_allocator *pfn_allocator_edge
)
{

    // argument check
    if ( NULL == pp_graph || NULL == *pp_graph ) goto no_graph;

    // initialized data
    graph *p_graph = *pp_graph;

    // destroy the concrete graph
    p_graph->pfn_destroy(&p_graph->p_graph, pfn_allocator_vertex, pfn_allocator_edge);

    // release the graph
    default_allocator(p_graph, 0);

    // set the pointer to NULL
    *pp_graph = NULL;

    // success
    return 1;

    // error handling
    {

        // argument errors
        {
            no_graph:
                #ifndef NDEBUG
                    printf("[graph] Null pointer provided for parameter \"pp_graph\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
    }
}

static hash64 graph_hash_pointer ( const void *const k, unsigned long long l )
{

    // unused
    (void) l;

    // done
    return (hash64) k;
}

static int graph_equality_pointer ( const void *p_a, const void *p_b )
{

    // done
    return p_a == p_b ? 0 : 1;
}

static int dijkstra_compare ( const void *p_a, const void *p_b )
{

    // initialized data
    const graph_sssp_result *p_res_a = p_a;
    const graph_sssp_result *p_res_b = p_b;

    // compare the distances
    if ( p_res_a->distance < p_res_b->distance ) return 1;
    if ( p_res_a->distance > p_res_b->distance ) return -1;

    // equal
    return 0;
}

static void *dijkstra_result_key_accessor ( const void *p_result )
{

    // initialized data
    graph_sssp_result *p_sssp_result = (graph_sssp_result *)p_result;

    // done
    return p_sssp_result->p_vertex;
}

static void *vertex_index_key_accessor ( const void *p_entry )
{

    // initialized data
    vertex_index_entry *p_vertex_index = (vertex_index_entry *)p_entry;

    // done
    return p_vertex_index->p_vertex;
}

static int johnson_compare ( const void *p_a, const void *p_b )
{

    // initialized data
    const struct johnson_dijkstra_node_s *p_node_a = p_a;
    const struct johnson_dijkstra_node_s *p_node_b = p_b;

    // compare the distances
    if ( p_node_a->distance < p_node_b->distance ) return 1;
    if ( p_node_a->distance > p_node_b->distance ) return -1;

    // equal
    return 0;
}

static int kruskal_edge_compare ( const void *p_a, const void *p_b )
{

    // initialized data 
    const mst_edge *p_edge_a = p_a;
    const mst_edge *p_edge_b = p_b;

    // compare the distances
    if ( p_edge_a->weight < p_edge_b->weight ) return -1;
    if ( p_edge_a->weight > p_edge_b->weight ) return  1;

    // equal
    return 0;
}

static int prim_edge_compare ( const void *p_a, const void *p_b )
{

    // initialized data 
    const mst_edge *p_edge_a = p_a;
    const mst_edge *p_edge_b = p_b;

    // compare the distances
    if ( p_edge_a->weight < p_edge_b->weight ) return  1;
    if ( p_edge_a->weight > p_edge_b->weight ) return -1;

    // equal
    return 0;
}

static int union_find_construct ( union_find **pp_union_find, size_t n )
{

    // initialized data
    union_find *p_union_find = default_allocator(NULL, sizeof(union_find));

    // allocate memory for parents
    p_union_find->parent = default_allocator(NULL, sizeof(size_t) * n);

    // allocate memory for ranks
    p_union_find->rank = default_allocator(NULL, sizeof(size_t) * n);

    // iterate through each element
    for ( size_t i = 0; i < n; i++ ) 
        p_union_find->parent[i] = i,
        p_union_find->rank[i] = 0;

    // return a pointer to the caller 
    *pp_union_find = p_union_find;

    // success
    return 1;
}

static size_t union_find_find ( union_find *p_union_find, size_t i )
{

    // base case 
    if ( p_union_find->parent[i] == i ) return i;

    // recurse
    return p_union_find->parent[i] = union_find_find(p_union_find, p_union_find->parent[i]);
}

static void union_find_union ( union_find *p_union_find, size_t i, size_t j )
{

    // initialized data
    size_t root_i = union_find_find(p_union_find, i);
    size_t root_j = union_find_find(p_union_find, j);
    
    if ( root_i != root_j ) 
    {
        if ( p_union_find->rank[root_i] < p_union_find->rank[root_j] )
            p_union_find->parent[root_i] = root_j;
        else if ( p_union_find->rank[root_i] > p_union_find->rank[root_j] )
            p_union_find->parent[root_j] = root_i;
        else 
            p_union_find->parent[root_i] = root_j,
            p_union_find->rank[root_j]++;
    }
}

static int union_find_destroy ( union_find **pp_union_find )
{

    // initialized data
    union_find *p_union_find = *pp_union_find;

    // no more pointer for caller
    *pp_union_find = NULL;

    // release the parents
    p_union_find->parent = default_allocator(p_union_find->parent, 0);
    
    // release the ranks
    p_union_find->rank = default_allocator(p_union_find->rank, 0);

    // release the struct
    default_allocator(p_union_find, 0);

    // success
    return 1;
}
