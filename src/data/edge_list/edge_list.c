/** !
 * edge list implementation
 * 
 * @file src/data/edge_list/edge_list.c 
 * 
 * @author Jacob Smith
 */

// header
#include <data/edge_list.h>

// structure definitions
struct edge_list_edge_s
{
    void *p_from;
    void *p_to;
    void *p_data;
};

struct edge_list_s
{
    enum graph_edge_type_e _type; 

    fn_key_accessor *pfn_key_accessor;
    fn_comparator   *pfn_comparator;

    size_t vertex_size;
    size_t edge_size;

    struct
    {
        void   **pp_vertices;
        size_t   count;
        size_t   capacity;
    } vertices;

    struct
    {
        struct edge_list_edge_s **pp_edges;
        size_t                    count;
        size_t                    capacity;
    } edges;
};

// type definitions
typedef struct edge_list_edge_s edge_list_edge; 

// function declarations
static int vertex_index_get ( edge_list *p_edge_list, const void *p_key, void **pp_vertex )
{

    // iterate through each vertex
    for ( size_t i = 0; i < p_edge_list->vertices.count; i++ )
    {

        // initialized data
        void *p_vertex = p_edge_list->vertices.pp_vertices[i];

        // check the i'th vertex
        if 
        ( 
            0 == p_edge_list->pfn_comparator
            (
                p_edge_list->pfn_key_accessor(p_vertex), 
                p_key
            ) 
        )
        {

            // return the vertex to the caller
            if ( pp_vertex ) 
                *pp_vertex = p_vertex;

            // return the index
            return (int)i;
        }
    }

    // error
    return -1;
}

int edge_list_construct
( 
    edge_list              **pp_edge_list,
    enum graph_edge_type_e   _type,

    size_t vertex_size,
    size_t edge_size,

    fn_key_accessor *pfn_key_accessor,
    fn_comparator   *pfn_comparator
)
{

    // argument check
    if ( NULL == pp_edge_list ) goto no_edge_list;

    // initialized data
    edge_list *p_edge_list = NULL;

    // allocate memory for an edge list
    p_edge_list = default_allocator(NULL, sizeof(edge_list));
    if ( NULL == p_edge_list ) goto no_mem;

    // populate the struct
    *p_edge_list = (edge_list)
    {
        ._type = _type,

        .pfn_key_accessor = pfn_key_accessor,
        .pfn_comparator   = pfn_comparator,

        .vertex_size = vertex_size,
        .edge_size   = edge_size,

        .vertices = 
        {
            .pp_vertices = default_allocator(NULL, sizeof(void *) * 8),
            .count       = 0,
            .capacity    = 8
        },

        .edges = 
        {
            .pp_edges = default_allocator(NULL, sizeof(edge_list_edge *) * 8),
            .count    = 0,
            .capacity = 8
        }
    };

    // check for allocation errors
    if ( NULL == p_edge_list->vertices.pp_vertices ) goto no_mem;
    if ( NULL ==       p_edge_list->edges.pp_edges ) goto no_mem;

    // return a pointer to the caller
    *pp_edge_list = p_edge_list;

    // success
    return 1;

    // error handling
    {

        // argument errors
        {
            no_edge_list:
                #ifndef NDEBUG
                    log_error("[edge list] Null pointer provided for parameter \"pp_edge_list\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
        
        // standard library errors
        {
            no_mem:
                #ifndef NDEBUG
                    log_error("[edge list] Failed to allocate memory in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // release the edge list
                if ( p_edge_list )
                {
                    if ( p_edge_list->vertices.pp_vertices ) default_allocator(p_edge_list->vertices.pp_vertices, 0);
                    if ( p_edge_list->edges.pp_edges       ) default_allocator(p_edge_list->edges.pp_edges, 0);
                    default_allocator(p_edge_list, 0);
                }

                // error
                return 0;
        }
    }
}

int edge_list_vertex_search 
( 
    edge_list   *p_edge_list, 
    const void  *p_key, 
    void       **pp_vertex
)
{
    
    // argument check
    if ( NULL == p_edge_list ) goto no_edge_list;
    if ( NULL ==       p_key ) goto no_key;
    
    // find the vertex
    if ( -1 == vertex_index_get(p_edge_list, p_key, pp_vertex) ) goto vertex_not_found;

    // success
    return 1;

    // error handling
    {

        // argument errors
        {
            no_edge_list:
                #ifndef NDEBUG
                    log_error("[edge list] Null pointer provided for parameter \"p_edge_list\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;

            no_key:
                #ifndef NDEBUG
                    log_error("[edge list] Null pointer provided for parameter \"p_key\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }

        // edge list errors
        {
            vertex_not_found:
                #ifndef NDEBUG
                    log_error("[edge list] Failed to find vertex in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
    }
}

size_t edge_list_vertex_count ( edge_list *p_edge_list )
{

    // argument check
    if ( NULL == p_edge_list ) goto no_edge_list;

    // success
    return p_edge_list->vertices.count;

    // error handling
    {

        // argument errors
        {
            no_edge_list:
                #ifndef NDEBUG
                    log_error("[edge list] Null pointer provided for parameter \"p_edge_list\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
    }
}

int edge_list_edge_search 
( 
    edge_list *p_edge_list, 
    const void *p_from, 
    const void *p_to, 
    void **pp_edge
)
{
    
    // argument check
    if ( NULL == p_edge_list ) goto no_edge_list;
    if ( NULL ==      p_from ) goto no_from;
    if ( NULL ==        p_to ) goto no_to;

    // initialized data
    void *u = NULL;
    void *v = NULL;

    // find both vertices
    if ( -1 == vertex_index_get(p_edge_list, p_from, &u) ) goto vertex_not_found;
    if ( -1 == vertex_index_get(p_edge_list, p_to,   &v) ) goto vertex_not_found;

    // scan all edges
    for ( size_t i = 0; i < p_edge_list->edges.count; i++ )
    {

        // initialized data
        edge_list_edge *p_edge = p_edge_list->edges.pp_edges[i];
        
        // check the edge
        if ( p_edge->p_from == u && p_edge->p_to == v )
        {

            // return the edge to the caller
            if ( pp_edge ) 
                *pp_edge = p_edge->p_data;

            // success
            return 1;
        }
        
        // if undirected, check mirror
        if ( !(p_edge_list->_type & GRAPH_DIRECTED) )
            if ( p_edge->p_from == v && p_edge->p_to == u )
            {

                // return the edge to the caller
                if ( pp_edge ) 
                    *pp_edge = p_edge->p_data;

                // success
                return 1;
            }
    }

    // error
    return 0;

    // error handling
    {

        // argument errors
        {
            no_edge_list:
                #ifndef NDEBUG
                    log_error("[edge list] Null pointer provided for parameter \"p_edge_list\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;

            no_from:
                #ifndef NDEBUG
                    log_error("[edge list] Null pointer provided for parameter \"p_from\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;

            no_to:
                #ifndef NDEBUG
                    log_error("[edge list] Null pointer provided for parameter \"p_to\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }

        // edge list errors
        {
            vertex_not_found:
                #ifndef NDEBUG
                    log_error("[edge list] Failed to find vertex in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
    }
}

size_t edge_list_edge_count ( edge_list *p_edge_list )
{

    // argument check
    if ( NULL == p_edge_list ) goto no_edge_list;

    // if undirected, each edge is counted twice
    if ( !(p_edge_list->_type & GRAPH_DIRECTED) )
        return p_edge_list->edges.count * 2;

    // success
    return p_edge_list->edges.count;

    // error handling
    {

        // argument errors
        {
            no_edge_list:
                #ifndef NDEBUG
                    log_error("[edge list] Null pointer provided for parameter \"p_edge_list\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
    }
}

int edge_list_neighbors_get 
( 
    edge_list    *p_edge_list, 
    const void   *p_key, 
    size_t       *p_count, 
    void       ***ppp_neighbors
)
{

    // argument check
    if ( NULL == p_edge_list ) goto no_edge_list;
    if ( NULL ==       p_key ) goto no_key;

    // initialized data
    void    *u            = NULL;
    size_t   count        = 0;
    void   **pp_neighbors = NULL;

    // find the vertex
    if ( -1 == vertex_index_get(p_edge_list, p_key, &u) ) goto vertex_not_found;

    // scan all edges to count neighbors
    for ( size_t i = 0; i < p_edge_list->edges.count; i++ )
    {

        // initialized data
        edge_list_edge *p_edge = p_edge_list->edges.pp_edges[i];
        
        // count neighbors
        if ( p_edge->p_from == u ) 
            count++;
        else if ( !(p_edge_list->_type & GRAPH_DIRECTED) && p_edge->p_to == u ) 
            count++;
    }

    // return the neighbors to the caller
    if ( ppp_neighbors )
    {

        // if there are neighbors ...
        if ( count > 0 )
        {

            // ... allocate memory for the neighbors ...
            pp_neighbors = default_allocator(NULL, sizeof(void *) * count);
            if ( NULL == pp_neighbors ) goto no_mem;

            // ... populate the neighbors
            for ( size_t i = 0, j = 0; i < p_edge_list->edges.count; i++ )
            {

                // initialized data
                edge_list_edge *p_edge = p_edge_list->edges.pp_edges[i];
                
                // store the neighbor
                if ( p_edge->p_from == u ) 
                    pp_neighbors[j++] = p_edge->p_to;
                else if ( !(p_edge_list->_type & GRAPH_DIRECTED) && p_edge->p_to == u ) 
                    pp_neighbors[j++] = p_edge->p_from;
            }
        }

        // return the neighbors to the caller
        *ppp_neighbors = pp_neighbors;
    }

    // return the count to the caller
    if ( p_count ) 
        *p_count = count;

    // success
    return 1;

    // error handling
    {

        // argument errors
        {
            no_edge_list:
                #ifndef NDEBUG
                    log_error("[edge list] Null pointer provided for parameter \"p_edge_list\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;

            no_key:
                #ifndef NDEBUG
                    log_error("[edge list] Null pointer provided for parameter \"p_key\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }

        // edge list errors
        {
            vertex_not_found:
                #ifndef NDEBUG
                    log_error("[edge list] Failed to find vertex in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }

        // standard library errors
        {
            no_mem:
                #ifndef NDEBUG
                    log_error("[edge list] Failed to allocate memory in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
    }
}

int edge_list_vertex_degree
( 
    edge_list  *p_edge_list, 
    const void *p_key, 
    size_t     *p_in_degree, 
    size_t     *p_out_degree 
)
{

    // argument check
    if ( NULL == p_edge_list ) goto no_edge_list;
    if ( NULL ==       p_key ) goto no_key;

    // initialized data
    void   *u          = NULL;
    size_t  in_degree  = 0;
    size_t  out_degree = 0;

    // find the vertex
    if ( -1 == vertex_index_get(p_edge_list, p_key, &u) ) goto vertex_not_found;

    // scan all edges
    for ( size_t i = 0; i < p_edge_list->edges.count; i++ )
    {

        // initialized data
        edge_list_edge *p_edge = p_edge_list->edges.pp_edges[i];
        
        // count degrees
        if ( p_edge->p_from == u ) out_degree++;
        if ( p_edge->p_to   == u ) in_degree++;
    }

    // if undirected, in_degree == out_degree
    if ( !(p_edge_list->_type & GRAPH_DIRECTED) )
        in_degree = out_degree;

    // return the quantity of incoming edges
    if ( p_in_degree )  
        *p_in_degree  = in_degree;

    // return the quantity of outgoing edges
    if ( p_out_degree ) 
        *p_out_degree = out_degree;

    // success
    return 1;

    // error handling
    {

        // argument errors
        {
            no_edge_list:
                #ifndef NDEBUG
                    log_error("[edge list] Null pointer provided for parameter \"p_edge_list\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;

            no_key:
                #ifndef NDEBUG
                    log_error("[edge list] Null pointer provided for parameter \"p_key\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }

        // edge list errors
        {
            vertex_not_found:
                #ifndef NDEBUG
                    log_error("[edge list] Failed to find vertex in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
    }
}

int edge_list_vertex_add 
( 
    edge_list *p_edge_list, 
    void      *p_vertex
)
{

    // argument check
    if ( NULL == p_edge_list ) goto no_edge_list;
    if ( NULL ==    p_vertex ) goto no_vertex;

    // check if the vertex already exists
    if ( -1 != vertex_index_get(p_edge_list, p_edge_list->pfn_key_accessor(p_vertex), NULL) ) goto vertex_already_exists;

    // resize?
    if ( p_edge_list->vertices.count == p_edge_list->vertices.capacity )
    {

        // double the capacity
        p_edge_list->vertices.capacity *= 2;

        // resize the vertex array
        p_edge_list->vertices.pp_vertices = default_allocator(p_edge_list->vertices.pp_vertices, sizeof(void *) * p_edge_list->vertices.capacity);
        if ( NULL == p_edge_list->vertices.pp_vertices ) goto no_mem;
    }

    // add to the vertices array
    p_edge_list->vertices.pp_vertices[p_edge_list->vertices.count++] = p_vertex;

    // success
    return 1;

    // error handling
    {

        // argument errors
        {
            no_edge_list:
                #ifndef NDEBUG
                    log_error("[edge list] Null pointer provided for parameter \"p_edge_list\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;

            no_vertex:
                #ifndef NDEBUG
                    log_error("[edge list] Null pointer provided for parameter \"p_vertex\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }

        // edge list errors
        {
            vertex_already_exists:
                #ifndef NDEBUG
                    log_error("[edge list] Vertex already exists in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
        
        // standard library errors
        {
            no_mem:
                #ifndef NDEBUG
                    log_error("[edge list] Failed to allocate memory in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
    }
}

int edge_list_vertex_remove
( 
    edge_list     *p_edge_list, 
    const void    *p_key, 
    void         **pp_vertex, 

    fn_allocator  *pfn_allocator_vertex,
    fn_allocator  *pfn_allocator_edge
)
{

    // argument check
    if ( NULL == p_edge_list ) goto no_edge_list;
    if ( NULL ==       p_key ) goto no_key;

    // initialized data
    void *u   = NULL;
    int   idx = -1;

    // store the index of the vertex
    idx = vertex_index_get(p_edge_list, p_key, &u);
    if ( idx == -1 ) goto vertex_not_found;

    // remove all edges touching this vertex
    for ( int i = (int)p_edge_list->edges.count - 1; i >= 0; i-- )
    {

        // initialized data
        edge_list_edge *p_edge = p_edge_list->edges.pp_edges[i];

        // if the vertex is connected to the edge
        if ( p_edge->p_from == u || p_edge->p_to == u )
        {

            // release the edge data
            if ( pfn_allocator_edge ) 
                p_edge->p_data = pfn_allocator_edge(p_edge->p_data, 0);

            // release the edge entry
            default_allocator(p_edge, 0);

            // shift edges
            if ( (size_t)i < p_edge_list->edges.count - 1 )
                memmove
                (
                    &p_edge_list->edges.pp_edges[i], 
                    &p_edge_list->edges.pp_edges[i+1], 
                    sizeof(edge_list_edge *) * (p_edge_list->edges.count - i - 1)
                );
            
            // decrement the edge counter
            p_edge_list->edges.count--;
        }
    }

    // remove vertex
    if ( (size_t)idx < p_edge_list->vertices.count - 1 )
        memmove
        (
            &p_edge_list->vertices.pp_vertices[idx], 
            &p_edge_list->vertices.pp_vertices[idx+1], 
            sizeof(void *) * (p_edge_list->vertices.count - idx - 1)
        );

    // decrement the vertex counter
    p_edge_list->vertices.count--;

    // release the vertex data
    if ( pfn_allocator_vertex ) 
        u = pfn_allocator_vertex(u, 0);

    // return the vertex to the caller
    if ( pp_vertex ) 
        *pp_vertex = u;

    // success
    return 1;

    // error handling
    {

        // argument errors
        {
            no_edge_list:
                #ifndef NDEBUG
                    log_error("[edge list] Null pointer provided for parameter \"p_edge_list\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;

            no_key:
                #ifndef NDEBUG
                    log_error("[edge list] Null pointer provided for parameter \"p_key\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }

        // edge list errors
        {
            vertex_not_found:
                #ifndef NDEBUG
                    log_error("[edge list] Failed to find vertex in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
    }
}

int edge_list_edge_add 
( 
    edge_list  *p_edge_list, 
    const void *p_from, 
    const void *p_to, 
    void       *p_edge_data
)
{

    // argument check
    if ( NULL == p_edge_list ) goto no_edge_list;
    if ( NULL ==      p_from ) goto no_from;
    if ( NULL ==        p_to ) goto no_to;
    if ( NULL == p_edge_data ) goto no_edge_data;

    // initialized data
    void           *u      = NULL;
    void           *v      = NULL;
    edge_list_edge *p_edge = NULL;

    // find the source vertex
    if ( -1 == vertex_index_get(p_edge_list, p_from, &u) ) goto vertex_not_found;

    // find the destination vertex
    if ( -1 == vertex_index_get(p_edge_list, p_to, &v) ) goto vertex_not_found;

    // resize?
    if ( p_edge_list->edges.count == p_edge_list->edges.capacity )
    {

        // double the capacity
        p_edge_list->edges.capacity *= 2;

        // resize the edge array
        p_edge_list->edges.pp_edges = default_allocator(p_edge_list->edges.pp_edges, sizeof(edge_list_edge *) * p_edge_list->edges.capacity);
        if ( NULL == p_edge_list->edges.pp_edges ) goto no_mem;
    }

    // allocate edge entry
    p_edge = default_allocator(NULL, sizeof(edge_list_edge));
    if ( NULL == p_edge ) goto no_mem;

    // populate the struct
    *p_edge = (edge_list_edge) 
    {
        .p_from = u,
        .p_to   = v,
        .p_data = p_edge_data
    };

    // add to the edges array
    p_edge_list->edges.pp_edges[p_edge_list->edges.count++] = p_edge;

    // success
    return 1;

    // error handling
    {

        // argument errors
        {
            no_edge_list:
                #ifndef NDEBUG
                    log_error("[edge list] Null pointer provided for parameter \"p_edge_list\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;

            no_from:
                #ifndef NDEBUG
                    log_error("[edge list] Null pointer provided for parameter \"p_from\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;

            no_to:
                #ifndef NDEBUG
                    log_error("[edge list] Null pointer provided for parameter \"p_to\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;

            no_edge_data:
                #ifndef NDEBUG
                    log_error("[edge list] Null pointer provided for parameter \"p_edge_data\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }

        // edge list errors
        {
            vertex_not_found:
                #ifndef NDEBUG
                    log_error("[edge list] Failed to find vertex in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
        
        // standard library errors
        {
            no_mem:
                #ifndef NDEBUG
                    log_error("[edge list] Failed to allocate memory in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
    }
}

int edge_list_edge_remove 
( 
    edge_list     *p_edge_list, 
    const void    *p_from, 
    const void    *p_to, 
    void         **pp_edge,

    fn_allocator  *pfn_allocator_edge
)
{

    // argument check
    if ( NULL == p_edge_list ) goto no_edge_list;
    if ( NULL ==      p_from ) goto no_from;
    if ( NULL ==        p_to ) goto no_to;

    // initialized data
    void *u = NULL;
    void *v = NULL;

    // find the source vertex
    if ( -1 == vertex_index_get(p_edge_list, p_from, &u) ) goto vertex_not_found;

    // find the destination vertex
    if ( -1 == vertex_index_get(p_edge_list, p_to, &v) ) goto vertex_not_found;

    // iterate through each edge 
    for ( int i = (int)p_edge_list->edges.count - 1; i >= 0; i-- )
    {

        // initialized data
        edge_list_edge *p_edge = p_edge_list->edges.pp_edges[i];
        
        // check the edge
        bool match = (p_edge->p_from == u && p_edge->p_to == v);
        
        // if undirected, check mirror
        if ( !match && !(p_edge_list->_type & GRAPH_DIRECTED) )
            match = (p_edge->p_from == v && p_edge->p_to == u);

        // match?
        if ( match )
        {

            // store the edge data
            void *p_data = p_edge->p_data;

            // return the edge data to the caller
            if ( pp_edge ) 
                *pp_edge = p_data;

            // release the edge data
            if ( pfn_allocator_edge ) 
                pfn_allocator_edge(p_data, 0);

            // release the edge entry
            default_allocator(p_edge, 0);

            // shift edges
            if ( (size_t)i < p_edge_list->edges.count - 1 )
                memmove
                (
                    &p_edge_list->edges.pp_edges[i], 
                    &p_edge_list->edges.pp_edges[i+1], 
                    sizeof(edge_list_edge *) * (p_edge_list->edges.count - i - 1)
                );
            
            // decrement the edge counter
            p_edge_list->edges.count--;

            // success
            return 1;
        }
    }

    // error
    return 0;

    // error handling
    {

        // argument errors
        {
            no_edge_list:
                #ifndef NDEBUG
                    log_error("[edge list] Null pointer provided for parameter \"p_edge_list\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;

            no_from:
                #ifndef NDEBUG
                    log_error("[edge list] Null pointer provided for parameter \"p_from\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;

            no_to:
                #ifndef NDEBUG
                    log_error("[edge list] Null pointer provided for parameter \"p_to\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }

        // edge list errors
        {
            vertex_not_found:
                #ifndef NDEBUG
                    log_error("[edge list] Failed to find vertex in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
    }
}

int edge_list_vertex_foreach
(
    edge_list  *p_edge_list,
    fn_foreach *pfn_foreach
)
{

    // argument check
    if ( NULL == p_edge_list ) goto no_edge_list;
    if ( NULL == pfn_foreach ) goto no_foreach;

    // iterate through each vertex
    for ( size_t i = 0; i < p_edge_list->vertices.count; i++ )
        pfn_foreach(p_edge_list->vertices.pp_vertices[i]);

    // success
    return 1;

    // error handling
    {

        // argument errors
        {
            no_edge_list:
                #ifndef NDEBUG
                    log_error("[edge list] Null pointer provided for parameter \"p_edge_list\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;

            no_foreach:
                #ifndef NDEBUG
                    log_error("[edge list] Null pointer provided for parameter \"pfn_foreach\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
    }
}

int edge_list_edge_foreach
(
    edge_list  *p_edge_list,
    fn_foreach *pfn_foreach
)
{

    // argument check
    if ( NULL == p_edge_list ) goto no_edge_list;
    if ( NULL == pfn_foreach ) goto no_foreach;

    // iterate through each edge
    for ( size_t i = 0; i < p_edge_list->edges.count; i++ )
        pfn_foreach(p_edge_list->edges.pp_edges[i]->p_data);

    // success
    return 1;

    // error handling
    {

        // argument errors
        {
            no_edge_list:
                #ifndef NDEBUG
                    log_error("[edge list] Null pointer provided for parameter \"p_edge_list\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;

            no_foreach:
                #ifndef NDEBUG
                    log_error("[edge list] Null pointer provided for parameter \"pfn_foreach\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
    }
}

int edge_list_pack
(
    void *p_buffer, 
    edge_list *p_edge_list,

    fn_pack *pfn_vertex,
    fn_pack *pfn_edge
)
{

    // argument check
    if ( NULL ==   p_buffer ) goto no_buffer;
    if ( NULL == p_edge_list ) goto no_edge_list;
    if ( NULL == pfn_vertex ) goto no_vertex_pack;
    if ( NULL ==   pfn_edge ) goto no_edge_pack;

    // initialized data 
    char *p = p_buffer;

    // pack the metadata
    p += pack_pack(p, "%i32%2i64", 
        p_edge_list->_type,
        p_edge_list->vertex_size,
        p_edge_list->edge_size
    );

    // pack the vertex quantity
    p += pack_pack(p, "%i64", p_edge_list->vertices.count);

    // pack the vertices
    for ( size_t i = 0; i < p_edge_list->vertices.count; i++ )
        p += pfn_vertex(p, p_edge_list->vertices.pp_vertices[i]);

    // pack the edge quantity
    p += pack_pack(p, "%i64", p_edge_list->edges.count);

    // pack the edges
    for ( size_t i = 0; i < p_edge_list->edges.count; i++ )
    {

        // initialized data
        edge_list_edge *p_edge   = p_edge_list->edges.pp_edges[i];
        int             from_idx = -1;
        int             to_idx   = -1;

        // find source and destination indices
        for ( size_t k = 0; k < p_edge_list->vertices.count; k++ )
        {
            if ( p_edge_list->vertices.pp_vertices[k] == p_edge->p_from ) from_idx = (int)k;
            if ( p_edge_list->vertices.pp_vertices[k] == p_edge->p_to   ) to_idx   = (int)k;
            if ( from_idx != -1 && to_idx != -1 ) break;
        }

        // pack the indices
        p += pack_pack(p, "%2i64", (size_t)from_idx, (size_t)to_idx);

        // pack the edge data
        p += pfn_edge(p, p_edge->p_data);
    }
    
    // success
    return (int)(p - (char *)p_buffer);

    // error handling
    {

        // argument errors
        {
            no_buffer:
                #ifndef NDEBUG
                    log_error("[edge list] Null pointer provided for parameter \"p_buffer\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;

            no_edge_list:
                #ifndef NDEBUG
                    log_error("[edge list] Null pointer provided for parameter \"p_edge_list\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;

            no_vertex_pack:
                #ifndef NDEBUG
                    log_error("[edge list] Null pointer provided for parameter \"pfn_vertex\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;

            no_edge_pack:
                #ifndef NDEBUG
                    log_error("[edge list] Null pointer provided for parameter \"pfn_edge\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
    }
}

int edge_list_unpack
(
    edge_list **pp_edge_list,
    void *p_buffer, 

    fn_unpack *pfn_vertex,
    fn_unpack *pfn_edge,

    fn_key_accessor *pfn_key_accessor,
    fn_comparator   *pfn_comparator
)
{

    // argument check
    if ( NULL ==     pp_edge_list ) goto no_edge_list;
    if ( NULL ==         p_buffer ) goto no_buffer;
    if ( NULL ==       pfn_vertex ) goto no_vertex_unpack;
    if ( NULL ==         pfn_edge ) goto no_edge_unpack;
    if ( NULL == pfn_key_accessor ) goto no_key_accessor;
    if ( NULL ==   pfn_comparator ) goto no_comparator;

    // initialized data 
    char                   *p            = p_buffer;
    edge_list              *p_edge_list  = NULL;
    enum graph_edge_type_e  _type        = 0;
    size_t                  vertex_size  = 0;
    size_t                  edge_size    = 0;
    size_t                  vertex_count = 0;
    size_t                  edge_count   = 0;

    // unpack the metadata
    p += pack_unpack(p, "%i32%2i64", 
        &_type,
        &vertex_size,
        &edge_size
    );

    // construct the edge list
    if ( 0 == edge_list_construct
    (
        &p_edge_list,
        _type,

        vertex_size,
        edge_size,

        pfn_key_accessor,
        pfn_comparator
    ) ) goto failed_to_construct;

    // unpack the vertex quantity
    p += pack_unpack(p, "%i64", &vertex_count);

    // unpack all vertices
    for ( size_t i = 0; i < vertex_count; i++ )
    {

        // initialized data
        void *p_vertex = NULL;

        // unpack the vertex
        p += pfn_vertex(&p_vertex, p);

        // add the vertex
        edge_list_vertex_add(p_edge_list, p_vertex);
    }

    // unpack the edge quantity
    p += pack_unpack(p, "%i64", &edge_count);

    // unpack edges
    for ( size_t i = 0; i < edge_count; i++ )
    {

        // initialized data
        size_t  from_idx    = 0;
        size_t  to_idx      = 0;
        void   *p_edge_data = NULL;
        void   *u           = NULL;
        void   *v           = NULL;

        // unpack indices
        p += pack_unpack(p, "%2i64", &from_idx, &to_idx);

        // unpack the edge data
        p += pfn_edge(&p_edge_data, p);

        // store the vertices
        u = p_edge_list->vertices.pp_vertices[from_idx];
        v = p_edge_list->vertices.pp_vertices[to_idx];

        // add the edge
        edge_list_edge_add
        (
            p_edge_list, 
            pfn_key_accessor(u),
            pfn_key_accessor(v),
            p_edge_data
        );
    }

    // return a pointer to the caller
    *pp_edge_list = p_edge_list;

    // success
    return (int)(p - (char *)p_buffer);

    // error handling
    {

        // argument errors
        {
            no_edge_list:
                #ifndef NDEBUG
                    log_error("[edge list] Null pointer provided for parameter \"pp_edge_list\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;

            no_buffer:
                #ifndef NDEBUG
                    log_error("[edge list] Null pointer provided for parameter \"p_buffer\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;

            no_vertex_unpack:
                #ifndef NDEBUG
                    log_error("[edge list] Null pointer provided for parameter \"pfn_vertex\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;

            no_edge_unpack:
                #ifndef NDEBUG
                    log_error("[edge list] Null pointer provided for parameter \"pfn_edge\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;

            no_key_accessor:
                #ifndef NDEBUG
                    log_error("[edge list] Null pointer provided for parameter \"pfn_key_accessor\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;

            no_comparator:
                #ifndef NDEBUG
                    log_error("[edge list] Null pointer provided for parameter \"pfn_comparator\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }

        // edge list errors
        {
            failed_to_construct:
                #ifndef NDEBUG
                    log_error("[edge list] Failed to construct edge list in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
    }
}

hash64 edge_list_hash
(
    edge_list *p_edge_list,
    fn_hash64 *pfn_hash64
)
{

    // argument check
    if ( NULL == p_edge_list ) goto no_edge_list;

    // initialized data
    hash64     result = 0;
    fn_hash64 *pfn_hash = (pfn_hash64) ? pfn_hash64 : default_hash;

    // hash the vertices
    for ( size_t i = 0; i < p_edge_list->vertices.count; i++ )
        result ^= pfn_hash(p_edge_list->vertices.pp_vertices[i], 8);
    
    // hash the edges
    for ( size_t i = 0; i < p_edge_list->edges.count; i++ )
        result ^= pfn_hash(p_edge_list->edges.pp_edges[i]->p_data, 8);

    // success
    return result;

    // error handling
    {

        // argument errors
        {
            no_edge_list:
                #ifndef NDEBUG
                    log_error("[edge list] Null pointer provided for parameter \"p_edge_list\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
    }
}

int edge_list_destroy
(
    edge_list **pp_edge_list,
    
    fn_allocator *pfn_allocator_vertex,
    fn_allocator *pfn_allocator_edge
)
{

    // argument check
    if ( NULL == pp_edge_list ) goto no_edge_list;

    // initialized data
    edge_list *p_list = *pp_edge_list;

    // fast fail
    if ( NULL == p_list ) return 1;

    // no more pointer for caller
    *pp_edge_list = NULL;

    // release each edge
    for ( size_t i = 0; i < p_list->edges.count; i++ )
    {

        // initialized data
        edge_list_edge *p_edge = p_list->edges.pp_edges[i];

        // release the edge data
        if ( pfn_allocator_edge ) 
            pfn_allocator_edge(p_edge->p_data, 0);

        // release the edge entry
        default_allocator(p_edge, 0);
    }

    // release the edge array
    if ( p_list->edges.pp_edges ) 
        default_allocator(p_list->edges.pp_edges, 0);
    
    // release each vertex
    if ( pfn_allocator_vertex )
        for ( size_t i = 0; i < p_list->vertices.count; i++ )
            pfn_allocator_vertex(p_list->vertices.pp_vertices[i], 0);

    // release the vertex array
    if ( p_list->vertices.pp_vertices ) 
        default_allocator(p_list->vertices.pp_vertices, 0);

    // release the edge list
    default_allocator(p_list, 0);

    // success
    return 1;

    // error handling
    {

        // argument errors
        {
            no_edge_list:
                #ifndef NDEBUG
                    log_error("[edge list] Null pointer provided for parameter \"pp_edge_list\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }   
    }
}
