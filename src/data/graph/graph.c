/** !
 * graph implementation
 * 
 * @file src/data/graph/graph.c
 * 
 * @author Jacob Smith
 */

// header file
#include <data/graph.h>

/// prototypes
graph _prototypes[GRAPH_QUANTITY] = 
{
    [GRAPH_ADJACENCY_MATRIX] = 
    {
        .p_graph = NULL,
        ._type   = GRAPH_ADJACENCY_MATRIX,

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
    [GRAPH_ADJACENCY_LIST] = 
    {
        .p_graph = NULL,
        ._type   = GRAPH_ADJACENCY_LIST,

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
            break;
        case GRAPH_ADJACENCY_LIST:
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
