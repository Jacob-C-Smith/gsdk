/** !
 * adjacency list implementation
 * 
 * @file src/data/adjacency_list/adjacency_list.c 
 * 
 * @author Jacob Smith
 */

// header
#include <data/adjacency_list.h>

// structure definitions
struct vertex_entry_s
{
    void                *p_vertex;
    struct edge_entry_s **pp_edges;
    size_t               edge_count;
    size_t               edge_capacity;
};

struct edge_entry_s
{
    void *p_to;
    void *p_data;
};

struct adjacency_list_s
{
    enum graph_edge_type_e _type; 

    fn_key_accessor *pfn_key_accessor;
    fn_comparator   *pfn_comparator;

    size_t vertex_size;
    size_t edge_size;

    struct vertex_entry_s **pp_vertices;
    size_t                  vertex_count;
    size_t                  vertex_capacity;
    size_t                  edge_count;
};

// type definitions
typedef struct vertex_entry_s vertex_entry;
typedef struct edge_entry_s   edge_entry;

// function definitions
static int vertex_index_get ( adjacency_list *p_adjacency_list, const void *p_key, vertex_entry **pp_entry )
{

    // iterate through each vertex
    for ( size_t i = 0; i < p_adjacency_list->vertex_count; i++ )
    {

        // initialized data
        vertex_entry *p_entry = p_adjacency_list->pp_vertices[i];

        // check the i'th vertex
        if 
        ( 
            0 == p_adjacency_list->pfn_comparator
            (
                p_adjacency_list->pfn_key_accessor(p_entry->p_vertex),
                p_key
            )
        )
        {

            // return a pointer to the caller
            if ( pp_entry ) 
                *pp_entry = p_entry;

            // success
            return (int)i;
        }
    }

    // error
    return -1;
}

int adjacency_list_construct
( 
    adjacency_list **pp_adjacency_list,
    enum graph_edge_type_e _type,

    size_t vertex_size,
    size_t edge_size,

    fn_key_accessor *pfn_key_accessor,
    fn_comparator   *pfn_comparator
)
{

    // argument check
    if ( NULL == pp_adjacency_list ) goto no_adjacency_list;

    // initialized data
    adjacency_list *p_adjacency_list = NULL;

    // allocate memory for an adjacency list
    p_adjacency_list = default_allocator(NULL, sizeof(adjacency_list));
    if ( NULL == p_adjacency_list ) goto no_mem;

    // populate the struct
    *p_adjacency_list = (adjacency_list)
    {
        ._type = _type,

        .pfn_key_accessor = pfn_key_accessor,
        .pfn_comparator   = pfn_comparator,

        .vertex_size = vertex_size,
        .edge_size   = edge_size,

        .pp_vertices     = NULL,
        .vertex_count    = 0,
        .vertex_capacity = 8,
        .edge_count      = 0,
    };

    // construct the vertex array
    p_adjacency_list->pp_vertices = default_allocator(NULL, sizeof(vertex_entry *) * p_adjacency_list->vertex_capacity);
    if ( NULL == p_adjacency_list->pp_vertices ) goto no_mem;

    // return a pointer to the caller
    *pp_adjacency_list = p_adjacency_list;

    // success
    return 1;

    // error handling
    {

        // argument errors
        {
            no_adjacency_list:
                #ifndef NDEBUG
                    log_error("[adjacency list] Null pointer provided for parameter \"pp_adjacency_list\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
        
        // standard library errors
        {
            no_mem:
                #ifndef NDEBUG
                    log_error("[adjacency list] Failed to allocate memory in call to function \"%s\"\n", __FUNCTION__);
                #endif
                
                // error
                return 0;
        }
    }
}

int adjacency_list_vertex_get 
(
    adjacency_list  *p_adjacency_list,
    void           **p_p_vertices
)
{

    // argument check
    if ( NULL == p_adjacency_list ) goto no_adjacency_list;
    if ( NULL ==     p_p_vertices ) goto no_vertices;

    // iterate through each vertex
    for ( size_t i = 0; i < p_adjacency_list->vertex_count; i++ )
        p_p_vertices[i] = p_adjacency_list->pp_vertices[i]->p_vertex;

    // success
    return 1;

    // error handling
    {
        
        // argument check
        {
            no_adjacency_list:
                #ifndef NDEBUG
                    log_error("[adjacency list] Null pointer provided for parameter \"p_adjacency_list\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
            
            no_vertices:
                #ifndef NDEBUG
                    log_error("[adjacency list] Null pointer provided for parameter \"p_p_vertices\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
    }
}

int adjacency_list_vertex_search 
( 
    adjacency_list *p_adjacency_list, 
    const void *p_key, 
    void **pp_vertex
)
{
    
    // argument check
    if ( NULL == p_adjacency_list ) goto no_adjacency_list;
    if ( NULL ==            p_key ) goto no_key;
    
    // initialized data
    vertex_entry *p_entry = NULL;

    // find the vertex
    if ( -1 == vertex_index_get(p_adjacency_list, p_key, &p_entry) ) goto vertex_not_found;

    // return the vertex to the caller
    if ( pp_vertex ) 
        *pp_vertex = p_entry->p_vertex;

    // success
    return 1;

    // error handling
    {

        // argument errors
        {
            no_adjacency_list:
                #ifndef NDEBUG
                    log_error("[adjacency list] Null pointer provided for parameter \"p_adjacency_list\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;

            no_key:
                #ifndef NDEBUG
                    log_error("[adjacency list] Null pointer provided for parameter \"p_key\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
        
        // adjacency list errors
        {
            vertex_not_found:
                #ifndef NDEBUG
                    log_error("[adjacency list] Failed to find vertex in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
    }
}

size_t adjacency_list_vertex_count ( adjacency_list *p_adjacency_list )
{

    // argument check
    if ( NULL == p_adjacency_list ) goto no_adjacency_list;

    // success
    return p_adjacency_list->vertex_count;

    // error handling
    {

        // argument errors
        {
            no_adjacency_list:
                #ifndef NDEBUG
                    log_error("[adjacency list] Null pointer provided for parameter \"p_adjacency_list\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
    }
}

int adjacency_list_edge_search 
( 
    adjacency_list *p_adjacency_list, 
    const void *p_from, 
    const void *p_to, 
    void **pp_edge
)
{
    
    // argument check
    if ( NULL == p_adjacency_list ) goto no_adjacency_list;
    if ( NULL ==           p_from ) goto no_from;
    if ( NULL ==             p_to ) goto no_to;

    // initialized data
    vertex_entry *p_entry = NULL;

    // find the source vertex
    if ( -1 == vertex_index_get(p_adjacency_list, p_from, &p_entry) ) goto vertex_not_found;

    // search the edges
    for ( size_t i = 0; i < p_entry->edge_count; i++ )
    {

        // initialized data
        edge_entry *p_edge = p_entry->pp_edges[i];
        
        // check the i'th edge
        if 
        ( 
            0 == p_adjacency_list->pfn_comparator
            (
                p_adjacency_list->pfn_key_accessor(p_edge->p_to),
                p_to
            )
        )
        {

            // return a pointer to the caller
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
            no_adjacency_list:
                #ifndef NDEBUG
                    log_error("[adjacency list] Null pointer provided for parameter \"p_adjacency_list\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;

            no_from:
                #ifndef NDEBUG
                    log_error("[adjacency list] Null pointer provided for parameter \"p_from\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;

            no_to:
                #ifndef NDEBUG
                    log_error("[adjacency list] Null pointer provided for parameter \"p_to\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }

        // adjacency list errors
        {
            vertex_not_found:
                #ifndef NDEBUG
                    log_error("[adjacency list] Failed to find vertex in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
    }
}

size_t adjacency_list_edge_count ( adjacency_list *p_adjacency_list )
{

    // argument check
    if ( NULL == p_adjacency_list ) goto no_adjacency_list;

    // success
    return p_adjacency_list->edge_count;

    // error handling
    {

        // argument errors
        {
            no_adjacency_list:
                #ifndef NDEBUG
                    log_error("[adjacency list] Null pointer provided for parameter \"p_adjacency_list\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
    }
}

int adjacency_list_neighbors_get 
( 
    adjacency_list *p_adjacency_list, 
    const void *p_key, 
    size_t *p_count, 
    void ***ppp_neighbors
)
{

    // argument check
    if ( NULL == p_adjacency_list ) goto no_adjacency_list;
    if ( NULL ==            p_key ) goto no_key;

    // initialized data
    vertex_entry *p_entry = NULL;
    size_t count = 0;
    void **pp_neighbors = NULL;

    // find the vertex
    if ( -1 == vertex_index_get(p_adjacency_list, p_key, &p_entry) ) goto vertex_not_found;

    // count the neighbors
    count = p_entry->edge_count;

    // return the neighbors to the caller
    if ( ppp_neighbors )
    {

        // if there are neighbors ...
        if ( count > 0 )
        {

            // ... allocate memory for neighbors ...
            pp_neighbors = default_allocator(NULL, sizeof(void *) * count);
            if ( NULL == pp_neighbors ) goto no_mem;

            // .. and store neighbors
            for ( size_t i = 0; i < count; i++ )
                pp_neighbors[i] = p_entry->pp_edges[i]->p_to;
        }

        // return a pointer to the caller
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
            no_adjacency_list:
                #ifndef NDEBUG
                    log_error("[adjacency list] Null pointer provided for parameter \"p_adjacency_list\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;

            no_key:
                #ifndef NDEBUG
                    log_error("[adjacency list] Null pointer provided for parameter \"p_key\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
        
        // standard library errors
        {
            no_mem:
                #ifndef NDEBUG
                    log_error("[adjacency list] Failed to allocate memory in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }

        // adjacency list errors
        {
            vertex_not_found:
                #ifndef NDEBUG
                    log_error("[adjacency list] Failed to find vertex in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
    }
}

int adjacency_list_vertex_degree
( 
    adjacency_list *p_adjacency_list, 
    const void *p_key, 
    size_t *p_in_degree, 
    size_t *p_out_degree 
)
{

    // argument check
    if ( NULL == p_adjacency_list ) goto no_adjacency_list;
    if ( NULL ==            p_key ) goto no_key;

    // initialized data
    vertex_entry *p_target_entry = NULL;
    size_t        in_degree      = 0;
    size_t        out_degree     = 0;

    // find the vertex
    if ( -1 == vertex_index_get(p_adjacency_list, p_key, &p_target_entry) ) goto vertex_not_found;

    // store the out degree
    out_degree = p_target_entry->edge_count;

    // iterate through each vertex
    for ( size_t i = 0; i < p_adjacency_list->vertex_count; i++ )
    {

        // initialized data
        vertex_entry *p_entry = p_adjacency_list->pp_vertices[i];
        
        // iterate through each edge
        for ( size_t j = 0; j < p_entry->edge_count; j++ )
        {

            // initialized data
            edge_entry *p_edge = p_entry->pp_edges[j];

            // increment the in degree
            if ( p_edge->p_to == p_target_entry->p_vertex )
                in_degree++;
        }
    }

    // return the input degree
    if ( p_in_degree ) 
        *p_in_degree = in_degree;
    
    // return the output degree
    if ( p_out_degree ) 
        *p_out_degree = out_degree;

    // success
    return 1;

    // error handling
    {

        // argument errors
        {
            no_adjacency_list:
                #ifndef NDEBUG
                    log_error("[adjacency list] Null pointer provided for parameter \"p_adjacency_list\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;

            no_key:
                #ifndef NDEBUG
                    log_error("[adjacency list] Null pointer provided for parameter \"p_key\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
        
        // adjacency list errors
        {
            vertex_not_found:
                #ifndef NDEBUG
                    log_error("[adjacency list] Failed to find vertex in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
    }
}

int adjacency_list_vertex_add 
( 
    adjacency_list *p_adjacency_list, 
    void             *p_vertex
)
{

    // argument check
    if ( NULL == p_adjacency_list ) goto no_adjacency_list;
    if ( NULL ==         p_vertex ) goto no_vertex;

    // initialized data
    vertex_entry *p_entry = NULL;

    // check if the vertex already exists
    if ( -1 != vertex_index_get(p_adjacency_list, p_adjacency_list->pfn_key_accessor(p_vertex), NULL) ) goto vertex_already_exists;

    // resize the vertex array
    if ( p_adjacency_list->vertex_count == p_adjacency_list->vertex_capacity )
    {
        p_adjacency_list->vertex_capacity *= 2;
        p_adjacency_list->pp_vertices = default_allocator(p_adjacency_list->pp_vertices, sizeof(vertex_entry *) * p_adjacency_list->vertex_capacity);
        if ( NULL == p_adjacency_list->pp_vertices ) goto no_mem;
    }

    // allocate a vertex entry
    p_entry = default_allocator(NULL, sizeof(vertex_entry));
    if ( NULL == p_entry ) goto no_mem;

    // populate the struct
    *p_entry = (vertex_entry) 
    {
        .p_vertex      = p_vertex,
        .edge_count    = 0,
        .edge_capacity = 8,
        .pp_edges      = default_allocator(NULL, sizeof(edge_entry *) * 8),
    };
    if ( NULL == p_entry->pp_edges ) goto no_mem_edges;

    // add to the vertices array
    p_adjacency_list->pp_vertices[p_adjacency_list->vertex_count++] = p_entry;

    // success
    return 1;

    // error handling
    {

        // argument errors
        {
            no_adjacency_list:
                #ifndef NDEBUG
                    log_error("[adjacency list] Null pointer provided for parameter \"p_adjacency_list\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;

            no_vertex:
                #ifndef NDEBUG
                    log_error("[adjacency list] Null pointer provided for parameter \"p_vertex\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }

        // standard library errors
        {
            no_mem_edges:
                if ( p_entry ) default_allocator(p_entry, 0);
            no_mem:
                #ifndef NDEBUG
                    log_error("[adjacency list] Failed to allocate memory in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
        
        // adjacency list errors
        {
            vertex_already_exists:
                #ifndef NDEBUG
                    log_error("[adjacency list] Vertex already exists in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
    }
}

int adjacency_list_vertex_remove
( 
    adjacency_list *p_adjacency_list, 
    const void *p_key, 
    void **pp_vertex, 
    fn_allocator *pfn_allocator_vertex,
    fn_allocator *pfn_allocator_edge
)
{

    // argument check
    if ( NULL == p_adjacency_list ) goto no_adjacency_list;
    if ( NULL ==            p_key ) goto no_key;

    // initialized data
    vertex_entry *p_entry = NULL;
    int idx = -1;

    // store the vertex index
    idx = vertex_index_get(p_adjacency_list, p_key, &p_entry);
    if ( idx == -1 ) goto vertex_not_found;

    // remove outgoing edges
    for ( size_t i = 0; i < p_entry->edge_count; i++ )
    {

        // initialized data
        edge_entry *p_edge = p_entry->pp_edges[i];

        // release the edge data
        if ( pfn_allocator_edge ) 
            pfn_allocator_edge(p_edge->p_data, 0);
        
        // release the edge
        default_allocator(p_edge, 0);

        // decrement the edge counter
        p_adjacency_list->edge_count--;
    }

    // release the edges
    if ( p_entry->pp_edges ) 
        default_allocator(p_entry->pp_edges, 0);

    // remove incoming edges 
    for ( size_t i = 0; i < p_adjacency_list->vertex_count; i++ )
    {

        // initialized data
        vertex_entry *p_other = p_adjacency_list->pp_vertices[i];
        
        // skip
        if ( (int)i == idx ) continue;
        
        // walk through each edge
        for ( int j = (int)p_other->edge_count - 1; j >= 0; j-- )
        {

            // initialized data
            edge_entry *p_edge = p_other->pp_edges[j];

            // check the edge
            if ( p_edge->p_to == p_entry->p_vertex )
            {

                // release the edge data
                if ( pfn_allocator_edge ) 
                    pfn_allocator_edge(p_edge->p_data, 0);

                // release the edge
                default_allocator(p_edge, 0);
                
                // shift edges
                if ( (size_t)j < p_other->edge_count - 1 )
                    memmove
                    (
                        &p_other->pp_edges[j], 
                        &p_other->pp_edges[j+1], 
                        sizeof(edge_entry *) * (p_other->edge_count - j - 1)
                    );
                
                // decrement the edge counters
                p_other->edge_count--,
                p_adjacency_list->edge_count--;
            }
        }
    }

    // release the vertex
    if ( pfn_allocator_vertex ) 
        p_entry->p_vertex = pfn_allocator_vertex(p_entry->p_vertex, 0);

    // return a pointer to the vertex
    if ( pp_vertex ) 
        *pp_vertex = p_entry->p_vertex;

    // remove the vertex from the array
    if ( (size_t)idx < p_adjacency_list->vertex_count - 1 )
        memmove
        (
            &p_adjacency_list->pp_vertices[idx], 
            &p_adjacency_list->pp_vertices[idx+1], 
            sizeof(vertex_entry *) * (p_adjacency_list->vertex_count - idx - 1)
        );
    
    // decrement the vertex counter
    p_adjacency_list->vertex_count--;

    // release the vertex
    p_entry = default_allocator(p_entry, 0);

    // success
    return 1;

    // error handling
    {

        // argument errors
        {
            no_adjacency_list:
                #ifndef NDEBUG
                    log_error("[adjacency list] Null pointer provided for parameter \"p_adjacency_list\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;

            no_key:
                #ifndef NDEBUG
                    log_error("[adjacency list] Null pointer provided for parameter \"p_key\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }

        // adjacency list errors
        {
            vertex_not_found:
                #ifndef NDEBUG
                    log_error("[adjacency list] Failed to find vertex in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
    }
}

int adjacency_list_edge_add 
( 
    adjacency_list *p_adjacency_list, 
    const void       *p_from, 
    const void       *p_to, 
    void             *p_edge_data
)
{

    // argument check
    if ( NULL == p_adjacency_list ) goto no_adjacency_list;
    if ( NULL ==           p_from ) goto no_from;
    if ( NULL ==             p_to ) goto no_to;
    if ( NULL ==      p_edge_data ) goto no_edge_data;

    // initialized data
    edge_entry   *p_edge       = NULL;
    vertex_entry *p_from_entry = NULL;
    vertex_entry *p_to_entry   = NULL;

    // find both vertices
    if ( -1 == vertex_index_get(p_adjacency_list, p_from, &p_from_entry) ) goto vertex_not_found;
    if ( -1 == vertex_index_get(p_adjacency_list, p_to  , &p_to_entry  ) ) goto vertex_not_found;

    // resize the edge array 
    if ( p_from_entry->edge_count == p_from_entry->edge_capacity )
    {

        // double the capacity
        p_from_entry->edge_capacity *= 2;

        // resize
        p_from_entry->pp_edges = default_allocator(p_from_entry->pp_edges, sizeof(edge_entry *) * p_from_entry->edge_capacity);
        if ( NULL == p_from_entry->pp_edges ) goto no_mem;
    }

    // allocate edge entry
    p_edge = default_allocator(NULL, sizeof(edge_entry));
    if ( NULL == p_edge ) goto no_mem;

    // store the destination vertex
    p_edge->p_to = p_to_entry->p_vertex;

    // store the edge data
    p_edge->p_data = p_edge_data;

    // add to outgoing edge
    p_from_entry->pp_edges[p_from_entry->edge_count++] = p_edge;

    // increment edge counter
    p_adjacency_list->edge_count++;

    // if undirected, add mirrored edge
    if ( !(p_adjacency_list->_type & GRAPH_DIRECTED) )
    {

        // initialized data
        edge_entry *p_mirror = NULL;

        // resize the edge array 
        if ( p_to_entry->edge_count == p_to_entry->edge_capacity )
        {

            // double the capacity
            p_to_entry->edge_capacity *= 2;

            // resize
            p_to_entry->pp_edges = default_allocator(p_to_entry->pp_edges, sizeof(edge_entry *) * p_to_entry->edge_capacity);
            if ( NULL == p_to_entry->pp_edges ) goto no_mem;
        }

        // allocate memory for the mirror edge
        p_mirror = default_allocator(NULL, sizeof(edge_entry));
        if ( NULL == p_mirror ) goto no_mem;

        // store the destination vertex
        p_mirror->p_to = p_from_entry->p_vertex;

        // store the edge data
        p_mirror->p_data = p_edge_data;

        // store the mirror edge
        p_to_entry->pp_edges[p_to_entry->edge_count++] = p_mirror;

        // increment the edge counter
        p_adjacency_list->edge_count++;
    }

    // success
    return 1;

    // error handling
    {

        // argument errors
        {
            no_adjacency_list:
                #ifndef NDEBUG
                    log_error("[adjacency list] Null pointer provided for parameter \"p_adjacency_list\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;

            no_from:
                #ifndef NDEBUG
                    log_error("[adjacency list] Null pointer provided for parameter \"p_from\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;

            no_to:
                #ifndef NDEBUG
                    log_error("[adjacency list] Null pointer provided for parameter \"p_to\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;

            no_edge_data:
                #ifndef NDEBUG
                    log_error("[adjacency list] Null pointer provided for parameter \"p_edge_data\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
        
        // standard library errors
        {
            no_mem:
                #ifndef NDEBUG
                    log_error("[interfaces] Failed to allocate memory in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }

        // adjacency list errors
        {
            vertex_not_found:
                #ifndef NDEBUG
                    log_error("[adjacency list] Failed to find vertex in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
    }
}

int adjacency_list_edge_remove 
( 
    adjacency_list *p_adjacency_list, 
    const void *p_from, 
    const void *p_to, 
    void **pp_edge,
    fn_allocator *pfn_allocator_edge
)
{

    // argument check
    if ( NULL == p_adjacency_list ) goto no_adjacency_list;
    if ( NULL == p_from           ) goto no_from;
    if ( NULL == p_to             ) goto no_to;

    // initialized data
    void         *p_data       = NULL;
    vertex_entry *p_from_entry = NULL;
    vertex_entry *p_to_entry   = NULL;

    // find both vertices
    if ( -1 == vertex_index_get(p_adjacency_list, p_from, &p_from_entry) ) goto vertex_not_found;
    if ( -1 == vertex_index_get(p_adjacency_list, p_to  , &p_to_entry  ) ) goto vertex_not_found;

    // remove from the source entry
    for ( size_t i = p_from_entry->edge_count; i > 0; i-- )
    {

        // initialized data
        edge_entry *p_edge = p_from_entry->pp_edges[i-1];

        // check the destination vertex
        if ( p_edge->p_to == p_to_entry->p_vertex )
        {

            // store the edge data
            p_data = p_edge->p_data;

            // release the edge
            default_allocator(p_edge, 0);
            
            // shift edges
            if ( i - 1 < p_from_entry->edge_count - 1 )
                memmove
                (
                    &p_from_entry->pp_edges[i-1], 
                    &p_from_entry->pp_edges[i], 
                    sizeof(edge_entry *) * (p_from_entry->edge_count - (i - 1) - 1)
                );
            
            // decrement the source edge counter
            p_from_entry->edge_count--;

            // decrement the edge counter
            p_adjacency_list->edge_count--;

            // done
            break;
        }
    }

    // if undirected, remove mirror
    if ( !(p_adjacency_list->_type & GRAPH_DIRECTED) )
    {

        // remove from the destination entry
        for ( size_t i = p_to_entry->edge_count; i > 0; i-- )
        {

            // initialized data
            edge_entry *p_edge = p_to_entry->pp_edges[i-1];

            // check the destination vertex
            if ( p_edge->p_to == p_from_entry->p_vertex )
            {

                // release the edge
                default_allocator(p_edge, 0);
                
                // shift edges
                if ( i - 1 < p_to_entry->edge_count - 1 )
                    memmove(
                        &p_to_entry->pp_edges[i-1], 
                        &p_to_entry->pp_edges[i], 
                        sizeof(edge_entry *) * (p_to_entry->edge_count - (i - 1) - 1)
                    );

                // decrement the destination edge counter
                p_to_entry->edge_count--;

                // decrement the edge counter
                p_adjacency_list->edge_count--;

                // done
                break;
            }
        }
    }

    // release the edge 
    if ( pfn_allocator_edge ) 
        p_data = pfn_allocator_edge(p_data, 0);
    
    // return the edge
    if ( pp_edge ) 
        *pp_edge = p_data;

    // success
    return 1;

    // error handling
    {

        // argument errors
        {
            no_adjacency_list:
                #ifndef NDEBUG
                    log_error("[adjacency list] Null pointer provided for parameter \"p_adjacency_list\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;

            no_from:
                #ifndef NDEBUG
                    log_error("[adjacency list] Null pointer provided for parameter \"p_from\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;

            no_to:
                #ifndef NDEBUG
                    log_error("[adjacency list] Null pointer provided for parameter \"p_to\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }

        // adjacency list errors
        {
            vertex_not_found:
                #ifndef NDEBUG
                    log_error("[adjacency list] Failed to find vertex in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
    }
}

int adjacency_list_vertex_foreach
(
    adjacency_list *p_adjacency_list,
    fn_foreach       *pfn_foreach
)
{

    // argument check
    if ( NULL == p_adjacency_list ) goto no_adjacency_list;
    if ( NULL ==      pfn_foreach ) goto no_foreach;

    // iterate over each vertex
    for ( size_t i = 0; i < p_adjacency_list->vertex_count; i++ )
        pfn_foreach(p_adjacency_list->pp_vertices[i]->p_vertex);

    // success
    return 1;

    // error handling
    {

        // argument errors
        {
            no_adjacency_list:
                #ifndef NDEBUG
                    log_error("[adjacency list] Null pointer provided for parameter \"p_adjacency_list\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;

            no_foreach:
                #ifndef NDEBUG
                    log_error("[adjacency list] Null pointer provided for parameter \"pfn_foreach\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
    }
}

int adjacency_list_edge_foreach
(
    adjacency_list *p_adjacency_list,
    fn_foreach     *pfn_foreach
)
{

    // argument check
    if ( NULL == p_adjacency_list ) goto no_adjacency_list;
    if ( NULL ==      pfn_foreach ) goto no_foreach;

    // iterate over each vertex ...
    for ( size_t i = 0; i < p_adjacency_list->vertex_count; i++ )
    {

        // initialized data
        vertex_entry *p_entry = p_adjacency_list->pp_vertices[i];
        
        // ... and each edge
        for ( size_t j = 0; j < p_entry->edge_count; j++ )
            pfn_foreach(p_entry->pp_edges[j]->p_data);
    }

    // success
    return 1;

    // error handling
    {

        // argument errors
        {
            no_adjacency_list:
                #ifndef NDEBUG
                    log_error("[adjacency list] Null pointer provided for parameter \"p_adjacency_list\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;

            no_foreach:
                #ifndef NDEBUG
                    log_error("[adjacency list] Null pointer provided for parameter \"pfn_foreach\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
    }
}

int adjacency_list_pack
(
    void *p_buffer, 
    adjacency_list *p_adjacency_list,

    fn_pack *pfn_vertex,
    fn_pack *pfn_edge
)
{

    // argument check
    if ( NULL ==           p_buffer ) goto no_buffer;
    if ( NULL == p_adjacency_list ) goto no_adjacency_list;
    if ( NULL ==         pfn_vertex ) goto no_vertex_pack;
    if ( NULL ==           pfn_edge ) goto no_edge_pack;

    // initialized data 
    char *p = p_buffer;

    // pack the metadata
    p += pack_pack(p, "%i32%2i64", 
        p_adjacency_list->_type,
        p_adjacency_list->vertex_size,
        p_adjacency_list->edge_size
    );

    // pack the vertex quantity
    p += pack_pack(p, "%i64", p_adjacency_list->vertex_count);

    // pack each vertex
    for ( size_t i = 0; i < p_adjacency_list->vertex_count; i++ )
    {

        // initialized data
        vertex_entry *p_entry = p_adjacency_list->pp_vertices[i];

        // pack vertex data
        p += pfn_vertex(p, p_entry->p_vertex);

        // pack edge count for this vertex
        p += pack_pack(p, "%i64", p_entry->edge_count);

        // pack each edge
        for ( size_t j = 0; j < p_entry->edge_count; j++ )
        {

            // initialized data
            edge_entry *p_edge = p_entry->pp_edges[j];
            int target_idx = -1;

            // find the index of the target vertex
            for ( size_t k = 0; k < p_adjacency_list->vertex_count; k++ )
                if ( p_adjacency_list->pp_vertices[k]->p_vertex == p_edge->p_to )
                {
                    target_idx = (int)k;
                    break;
                }

            // pack the index
            p += pack_pack(p, "%i64", (size_t)target_idx);

            // pack the edge
            p += pfn_edge(p, p_edge->p_data);
        }
    }
    
    // success
    return (int)(p - (char *)p_buffer);

    // error handling
    {

        // argument errors
        {
            no_buffer:
                #ifndef NDEBUG
                    log_error("[adjacency list] Null pointer provided for parameter \"p_buffer\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;

            no_adjacency_list:
                #ifndef NDEBUG
                    log_error("[adjacency list] Null pointer provided for parameter \"p_adjacency_list\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;

            no_vertex_pack:
                #ifndef NDEBUG
                    log_error("[adjacency list] Null pointer provided for parameter \"pfn_vertex\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;

            no_edge_pack:
                #ifndef NDEBUG
                    log_error("[adjacency list] Null pointer provided for parameter \"pfn_edge\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
    }
}

int adjacency_list_unpack
(
    adjacency_list **pp_adjacency_list,
    void *p_buffer, 

    fn_unpack *pfn_vertex,
    fn_unpack *pfn_edge,

    fn_key_accessor *pfn_key_accessor,
    fn_comparator   *pfn_comparator
)
{

    // argument check
    if ( NULL == pp_adjacency_list ) goto no_adjacency_list;
    if ( NULL ==          p_buffer ) goto no_buffer;
    if ( NULL ==        pfn_vertex ) goto no_vertex_unpack;
    if ( NULL ==          pfn_edge ) goto no_edge_unpack;
    if ( NULL ==  pfn_key_accessor ) goto no_key_accessor;
    if ( NULL ==    pfn_comparator ) goto no_comparator;

    // initialized data 
    char *p = p_buffer;
    adjacency_list *p_adjacency_list = NULL;
    enum graph_edge_type_e _type = 0;
    size_t vertex_size = 0;
    size_t edge_size   = 0;
    size_t vertex_count = 0;

    // unpack the metadata
    p += pack_unpack(p, "%i32%2i64", 
        &_type,
        &vertex_size,
        &edge_size
    );

    // construct the adjacency list
    if ( 0 == adjacency_list_construct
    (
        &p_adjacency_list,
        _type,

        vertex_size,
        edge_size,

        pfn_key_accessor,
        pfn_comparator
    ) ) goto failed_to_construct;

    // unpack the vertex quantity
    p += pack_unpack(p, "%i64", &vertex_count);

    // unpack vertices
    for ( size_t i = 0; i < vertex_count; i++ )
    {

        // initialized data
        void *p_vertex = NULL;
        size_t edges_for_vertex = 0;

        // unpack the vertex
        p += pfn_vertex(&p_vertex, p);

        // add the vertex
        if ( 0 == adjacency_list_vertex_add(p_adjacency_list, p_vertex) ) goto failed_to_add_vertex;

        // skip edges for now
        p += pack_unpack(p, "%i64", &edges_for_vertex);

        // iterate through each edge
        for ( size_t j = 0; j < edges_for_vertex; j++ )
        {

            // initialized data
            void *p_dummy_edge = NULL;

            // skip target index
            p += sizeof(size_t),

            // unpack the edge
            p += pfn_edge(&p_dummy_edge, p);
        }
    }

    // reset p
    p = (char *)p_buffer + sizeof(enum graph_edge_type_e) + sizeof(size_t) * 2 + sizeof(size_t);

    // unpack edges
    for ( size_t i = 0; i < vertex_count; i++ )
    {
        
        // initialized data
        void         *p_vertex = NULL;
        size_t        edges_for_vertex = 0;
        vertex_entry *p_from_entry = NULL;

        // unpack the vertex
        p += pfn_vertex(&p_vertex, p);

        // unpack the quantity of edges
        p += pack_unpack(p, "%i64", &edges_for_vertex);

        // store the source vertex
        p_from_entry = p_adjacency_list->pp_vertices[i];

        // iterate through each edge
        for ( size_t j = 0; j < edges_for_vertex; j++ )
        {
            
            // initialized data
            size_t        target_idx  = 0;
            void         *p_edge_data = NULL;
            vertex_entry *p_to_entry  = NULL;

            // unpack the index
            p += pack_unpack(p, "%i64", &target_idx);

            // unpack the edge
            p += pfn_edge(&p_edge_data, p);

            // store the destination vertex
            p_to_entry = p_adjacency_list->pp_vertices[target_idx];

            // add the edge
            if 
            ( 
                0 == adjacency_list_edge_add
                (
                    p_adjacency_list, 
                    pfn_key_accessor(p_from_entry->p_vertex),
                    pfn_key_accessor(p_to_entry->p_vertex),
                    p_edge_data
                ) 
            ) 
                goto failed_to_add_edge;
        }
    }

    // return a pointer to the caller
    *pp_adjacency_list = p_adjacency_list;

    // success
    return (int)(p - (char *)p_buffer);

    // error handling
    {

        // argument errors
        {
            no_adjacency_list:
                #ifndef NDEBUG
                    log_error("[adjacency list] Null pointer provided for parameter \"pp_adjacency_list\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;

            no_buffer:
                #ifndef NDEBUG
                    log_error("[adjacency list] Null pointer provided for parameter \"p_buffer\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;

            no_vertex_unpack:
                #ifndef NDEBUG
                    log_error("[adjacency list] Null pointer provided for parameter \"pfn_vertex\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;

            no_edge_unpack:
                #ifndef NDEBUG
                    log_error("[adjacency list] Null pointer provided for parameter \"pfn_edge\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;

            no_key_accessor:
                #ifndef NDEBUG
                    log_error("[adjacency list] Null pointer provided for parameter \"pfn_key_accessor\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;

            no_comparator:
                #ifndef NDEBUG
                    log_error("[adjacency list] Null pointer provided for parameter \"pfn_comparator\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }

        // adjacency list errors
        {
            failed_to_construct:
                #ifndef NDEBUG
                    log_error("[adjacency list] Failed to construct adjacency list in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;

            failed_to_add_vertex:
                #ifndef NDEBUG
                    log_error("[adjacency list] Failed to add vertex in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;

            failed_to_add_edge:
                #ifndef NDEBUG
                    log_error("[adjacency list] Failed to add edge in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
    }
}

hash64 adjacency_list_hash
(
    adjacency_list *p_adjacency_list,
    fn_hash64 *pfn_hash64
)
{

    // argument check
    if ( NULL == p_adjacency_list ) goto no_adjacency_list;

    // initialized data
    hash64     result   = 0;
    fn_hash64 *pfn_hash = (pfn_hash64) ? pfn_hash64 : default_hash;

    // iterate over each vertex ...
    for ( size_t i = 0; i < p_adjacency_list->vertex_count; i++ )
    {
        
        // initialized data
        vertex_entry *p_entry = p_adjacency_list->pp_vertices[i];

        // ... hash the vertex ...
        result ^= pfn_hash(p_entry->p_vertex, 8);
        
        // ... and each edge
        for ( size_t j = 0; j < p_entry->edge_count; j++ )
            result ^= pfn_hash(p_entry->pp_edges[j]->p_data, 8);
    }

    // success
    return result;

    // error handling
    {

        // argument errors
        {
            no_adjacency_list:
                #ifndef NDEBUG
                    log_error("[adjacency list] Null pointer provided for parameter \"p_adjacency_list\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
    }
}

int adjacency_list_destroy
(
    adjacency_list **pp_adjacency_list,
    
    fn_allocator *pfn_allocator_vertex,
    fn_allocator *pfn_allocator_edge
)
{

    // argument check
    if ( NULL == pp_adjacency_list ) goto no_adjacency_list;

    // initialized data
    adjacency_list *p_adjacency_list = *pp_adjacency_list;

    // no more pointer for caller
    *pp_adjacency_list = NULL;

    // iterate over each vertex ...
    for ( size_t i = 0; i < p_adjacency_list->vertex_count; i++ )
    {
        
        // initialized data
        vertex_entry *p_entry = p_adjacency_list->pp_vertices[i];
        
        // ... and each edge
        for ( size_t j = 0; j < p_entry->edge_count; j++ )
        {
            
            // initialized data
            edge_entry *p_edge = p_entry->pp_edges[j];

            // release the edge data
            if ( pfn_allocator_edge ) 
                p_edge->p_data = pfn_allocator_edge(p_edge->p_data, 0);

            // release the edge entry
            p_edge = default_allocator(p_edge, 0);
        }

        // release the edge array
        if ( p_entry->pp_edges ) 
            p_entry->pp_edges = default_allocator(p_entry->pp_edges, 0);

        // release the vertex data
        if ( pfn_allocator_vertex ) 
            p_entry->p_vertex = pfn_allocator_vertex(p_entry->p_vertex, 0);

        // release the vertex entry
        p_entry = default_allocator(p_entry, 0);
    }

    // release the vertex array
    if ( p_adjacency_list->pp_vertices ) 
        p_adjacency_list->pp_vertices = default_allocator(p_adjacency_list->pp_vertices, 0);

    // release the adjacency list
    p_adjacency_list = default_allocator(p_adjacency_list, 0);

    // success
    return 1;

    // error handling
    {

        // argument errors
        {
            no_adjacency_list:
                #ifndef NDEBUG
                    log_error("[adjacency list] Null pointer provided for parameter \"pp_adjacency_list\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
    }
}
