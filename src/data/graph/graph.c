/** !
 * graph implementation
 * 
 * @file src/data/graph/graph.c
 * 
 * @author Jacob Smith
 */

// header file
#include <data/graph.h>

#include <data/adjacency_matrix.h>
#include <data/adjacency_list.h>
#include <data/hash_table.h>
#include <data/stack.h>
#include <data/queue.h>

// forward declarations
static fn_hash64     graph_hash_pointer;
static fn_comparator graph_equality_pointer;

/// prototypes
graph _prototypes[GRAPH_QUANTITY] = 
{
    [GRAPH_ADJACENCY_MATRIX] = 
    {
        .p_graph = NULL,
        ._type   = GRAPH_ADJACENCY_MATRIX,

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

        .pfn_vertex_search  = (fn_graph_vertex_search *)  adjacency_list_vertex_search,
        .pfn_vertex_add     = (fn_graph_vertex_add *)     adjacency_list_vertex_add,               
        .pfn_vertex_remove  = (fn_graph_vertex_remove *)  adjacency_list_vertex_remove,                  
        .pfn_vertex_count   = (fn_graph_vertex_count *)   adjacency_list_vertex_count,
        .pfn_vertex_foreach = (fn_graph_vertex_foreach *) adjacency_list_vertex_foreach,
        .pfn_vertex_degree  = (fn_graph_vertex_degree *)  adjacency_list_vertex_degree,
        .pfn_neighbors_get  = (fn_graph_neighbors_get *)  adjacency_list_neighbors_get,

        .pfn_edge_search    = (fn_graph_edge_search *)  adjacency_list_edge_search,
        .pfn_edge_add       = (fn_graph_edge_add *)     adjacency_list_edge_add,             
        .pfn_edge_remove    = (fn_graph_edge_remove *)  adjacency_list_edge_remove,                
        .pfn_edge_count     = (fn_graph_edge_count *)   adjacency_list_edge_count,               
        .pfn_edge_foreach   = (fn_graph_edge_foreach *) adjacency_list_edge_foreach,

        .pfn_pack           = (fn_graph_pack *)   adjacency_list_pack,         
        .pfn_unpack         = (fn_graph_unpack *) adjacency_list_unpack,
        .pfn_hash           = (fn_graph_hash *)   adjacency_list_hash,

        .pfn_destroy        = (fn_graph_destroy *) adjacency_list_destroy,          
    },
    [GRAPH_EDGE_LIST] = 
    {
        .p_graph = NULL,
        ._type   = GRAPH_EDGE_LIST,

        .pfn_vertex_search  = NULL,
        .pfn_vertex_add     = NULL,               
        .pfn_vertex_remove  = NULL,                  
        .pfn_vertex_count   = NULL,
        .pfn_vertex_foreach = NULL,
        .pfn_vertex_degree  = NULL,
        .pfn_neighbors_get  = NULL,

        .pfn_edge_search    = NULL,
        .pfn_edge_add       = NULL,             
        .pfn_edge_remove    = NULL,                
        .pfn_edge_count     = NULL,               
        .pfn_edge_foreach   = NULL,

        .pfn_pack           = NULL,         
        .pfn_unpack         = NULL,
        .pfn_hash           = NULL,

        .pfn_destroy        = NULL,          
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
    graph *p_graph = NULL;
    void *p_concrete_graph = NULL;
    int   result = 0;

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
