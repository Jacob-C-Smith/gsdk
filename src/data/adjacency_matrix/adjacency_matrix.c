/** !
 * adjacency matrix implementation
 * 
 * @file src/data/adjacency_matrix/adjacency_matrix.c 
 * 
 * @author Jacob Smith
 */

// header
#include <data/adjacency_matrix.h>

// static data
static const unsigned long long eight_bytes_of_f = 0xffffffffffffffff;

// structure definitions
struct adjacency_matrix_s
{
    enum graph_edge_type_e _type; 

    fn_key_accessor *pfn_key_accessor;
    fn_comparator   *pfn_comparator;

    struct 
    {
        void   **pp_vertices;
        size_t   capacity; 
        size_t   count; 
        size_t   vertex_size; 
    } vertices;

    struct
    {
        void   **pp_edges;
        size_t   capacity;
        size_t   count; 
        size_t   edge_size; 
    } edges;
};

static int vertex_index_get ( adjacency_matrix *p_adjacency_matrix, const void *p_key )
{

    // iterate through each vertex
    for ( size_t i = 0; i < p_adjacency_matrix->vertices.count; i++ )

        // check the i'th vertex
        if 
        ( 
            0 == p_adjacency_matrix->pfn_comparator
            (
                p_adjacency_matrix->pfn_key_accessor
                (
                    p_adjacency_matrix->vertices.pp_vertices[i]
                ), 
                p_key
            )
        )

            // return the index
            return (int)i;

    // error
    return -1;
}

int adjacency_matrix_construct
( 
    adjacency_matrix       **pp_adjacency_matrix,
    enum graph_edge_type_e   _type,

    size_t vertex_size,
    size_t edge_size,

    fn_key_accessor *pfn_key_accessor,
    fn_comparator   *pfn_comparator
)
{

    // argument check
    if ( NULL == pp_adjacency_matrix ) goto no_adjacency_matrix;

    // initialized data
    adjacency_matrix *p_adjacency_matrix = NULL;

    // allocate memory for an adjacency matrix
    p_adjacency_matrix = default_allocator(NULL, sizeof(adjacency_matrix));
    if ( NULL == p_adjacency_matrix ) goto no_mem;

    // populate the struct
    *p_adjacency_matrix = (adjacency_matrix)
    {
        ._type = _type,

        .pfn_key_accessor = pfn_key_accessor,
        .pfn_comparator   = pfn_comparator,

        .vertices =
        {
            .pp_vertices = NULL,
            .capacity    = 0,
            .count       = 0,
            .vertex_size = vertex_size,
        },
        .edges =
        {
            .pp_edges  = NULL,
            .capacity  = 0,
            .count     = 0,
            .edge_size = edge_size,
        },
    };

    // return a pointer to the caller
    *pp_adjacency_matrix = p_adjacency_matrix;

    // success
    return 1;

    // error handling
    {

        // argument errors
        {
            no_adjacency_matrix:
                #ifndef NDEBUG
                    log_error("[adjacency matrix] Null pointer provided for parameter \"pp_adjacency_matrix\" in call to function \"%s\"\n", __FUNCTION__);
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
    }
}

int adjacency_matrix_vertex_get 
(
    adjacency_matrix  *p_adjacency_matrix,
    void             **p_p_vertices
)
{

    // argument check
    if ( NULL ==  p_adjacency_matrix ) goto no_adjacency_matrix;
    if ( NULL == p_p_vertices ) goto no_vertices;

    // iterate through each vertex
    for ( size_t i = 0; i < p_adjacency_matrix->vertices.count; i++ )
        p_p_vertices[i] = p_adjacency_matrix->vertices.pp_vertices[i];

    // success
    return 1;

    // error handling
    {
        
        // argument check
        {
            no_adjacency_matrix:
                #ifndef NDEBUG
                    log_error("[adjacency matrix] Null pointer provided for parameter \"p_adjacency_matrix\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
            
            no_vertices:
                #ifndef NDEBUG
                    log_error("[adjacency matrix] Null pointer provided for parameter \"p_p_vertices\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
    }
}

int adjacency_matrix_vertex_search 
( 
    adjacency_matrix *p_adjacency_matrix, 
    const void *p_key, 
    void **pp_vertex
)
{
    
    // argument check
    if ( NULL == p_adjacency_matrix ) goto no_adjacency_matrix;
    
    // initialized data
    int idx = -1;

    // store the index of the requested vertex
    idx = vertex_index_get(p_adjacency_matrix, p_key);
    if ( idx == -1 ) goto vertex_not_found;

    // return a pointer to the caller
    if ( pp_vertex ) 
        *pp_vertex = p_adjacency_matrix->vertices.pp_vertices[idx];

    // success
    return 1;

    // error handling
    {

        // argument errors
        {
            no_adjacency_matrix:
                #ifndef NDEBUG
                    log_error("[adjacency matrix] Null pointer provided for parameter \"p_adjacency_matrix\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
        
        // adjacency matrix errors
        {
            vertex_not_found:
                #ifndef NDEBUG
                    log_error("[adjacency matrix] Failed to find vertex in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
    }
}

size_t adjacency_matrix_vertex_count ( adjacency_matrix *p_adjacency_matrix )
{
    
    // argument check
    if ( NULL == p_adjacency_matrix ) goto no_adjacency_matrix;
    
    // success
    return p_adjacency_matrix->vertices.count;

    // error handling
    {

        // argument errors
        {
            no_adjacency_matrix:
                #ifndef NDEBUG
                    log_error("[adjacency matrix] Null pointer provided for parameter \"p_adjacency_matrix\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
    }
}

int adjacency_matrix_edge_search 
( 
    adjacency_matrix  *p_adjacency_matrix, 
    const void        *p_from, 
    const void        *p_to, 
    void             **pp_edge
)
{
    
    // argument check
    if ( NULL == p_adjacency_matrix ) goto no_adjacency_matrix;
    if ( NULL ==             p_from ) goto no_from;
    if ( NULL ==               p_to ) goto no_to;

    // initialized data
    int row = -1; 
    int col = -1; 

    // find the index of the source vertex
    row = vertex_index_get(p_adjacency_matrix, p_from);
    if ( row == -1 ) goto vertex_not_found;
    
    // find the index of the destination vertex
    col = vertex_index_get(p_adjacency_matrix, p_to);
    if ( col == -1 ) goto vertex_not_found;

    // return a pointer to the caller
    if ( pp_edge ) 
        *pp_edge = p_adjacency_matrix->edges.pp_edges[row * p_adjacency_matrix->vertices.capacity + col];

    // success
    return (p_adjacency_matrix->edges.pp_edges[row * p_adjacency_matrix->vertices.capacity + col] != NULL);

    // error handling
    {

        // argument errors
        {
            no_adjacency_matrix:
                #ifndef NDEBUG
                    log_error("[adjacency matrix] Null pointer provided for parameter \"p_adjacency_matrix\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;

            no_from:
                #ifndef NDEBUG
                    log_error("[adjacency matrix] Null pointer provided for parameter \"p_from\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;

            no_to:
                #ifndef NDEBUG
                    log_error("[adjacency matrix] Null pointer provided for parameter \"p_to\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
        
        // adjacency matrix errors
        {
            vertex_not_found:
                #ifndef NDEBUG
                    log_error("[adjacency matrix] Failed to find vertex in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
    }
}

size_t adjacency_matrix_edge_count ( adjacency_matrix *p_adjacency_matrix )
{
    
    // argument check
    if ( NULL == p_adjacency_matrix ) goto no_adjacency_matrix;
    
    // success
    return p_adjacency_matrix->edges.count;

    // error handling
    {

        // argument errors
        {
            no_adjacency_matrix:
                #ifndef NDEBUG
                    log_error("[adjacency matrix] Null pointer provided for parameter \"p_adjacency_matrix\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
    }
}

int adjacency_matrix_neighbors_get 
( 
    adjacency_matrix   *p_adjacency_matrix, 
    const void         *p_key, 
    size_t             *p_count, 
    void             ***ppp_neighbors
)
{

    // argument check
    if ( NULL == p_adjacency_matrix ) goto no_adjacency_matrix;
    if ( NULL ==              p_key ) goto no_key;

    // initialized data
    void   **pp_neighbors = NULL;
    size_t   count        = 0;
    int      idx          = -1;

    // find the index of the vertex
    idx = vertex_index_get(p_adjacency_matrix, p_key);
    if ( idx == -1 ) goto vertex_not_found;

    // count the neighbors
    for ( size_t i = 0; i < p_adjacency_matrix->vertices.count; i++ )
        if ( p_adjacency_matrix->edges.pp_edges[idx * p_adjacency_matrix->vertices.capacity + i] )
            count++;

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
            for ( size_t i = 0, j = 0; i < p_adjacency_matrix->vertices.count; i++ )
                if ( p_adjacency_matrix->edges.pp_edges[idx * p_adjacency_matrix->vertices.capacity + i] )
                    pp_neighbors[j++] = p_adjacency_matrix->vertices.pp_vertices[i];
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
            no_adjacency_matrix:
                #ifndef NDEBUG
                    log_error("[adjacency matrix] Null pointer provided for parameter \"p_adjacency_matrix\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;

            no_key:
                #ifndef NDEBUG
                    log_error("[adjacency matrix] Null pointer provided for parameter \"p_key\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }

        // adjacency matrix errors
        {
            vertex_not_found:
                #ifndef NDEBUG
                    log_error("[adjacency matrix] Failed to find vertex in call to function \"%s\"\n", __FUNCTION__);
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
    }
}

int adjacency_matrix_vertex_degree
( 
    adjacency_matrix *p_adjacency_matrix, 
    const void       *p_key, 
    size_t           *p_in_degree, 
    size_t           *p_out_degree 
)
{

    // argument check
    if ( NULL == p_adjacency_matrix ) goto no_adjacency_matrix;
    if ( NULL ==              p_key ) goto no_key;

    // initialized data
    size_t in_degree  = 0;
    size_t out_degree = 0;
    int    idx        = -1;

    // find the index of the vertex
    idx = vertex_index_get(p_adjacency_matrix, p_key);
    if ( idx == -1 ) goto vertex_not_found;

    // count the degrees
    for ( size_t i = 0; i < p_adjacency_matrix->vertices.count; i++ )
    {

        // out degree
        if ( p_adjacency_matrix->edges.pp_edges[idx * p_adjacency_matrix->vertices.capacity + i] )
            out_degree++;

        // in degree
        if ( p_adjacency_matrix->edges.pp_edges[i * p_adjacency_matrix->vertices.capacity + idx] )
            in_degree++;
    }

    // return the quantity of incoming edges to the caller
    if ( p_in_degree ) 
        *p_in_degree = in_degree;

    // return the quantity of outgoing edges to the caller
    if ( p_out_degree ) 
        *p_out_degree = out_degree;

    // success
    return 1;

    // error handling
    {

        // argument errors
        {
            no_adjacency_matrix:
                #ifndef NDEBUG
                    log_error("[adjacency matrix] Null pointer provided for parameter \"p_adjacency_matrix\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;

            no_key:
                #ifndef NDEBUG
                    log_error("[adjacency matrix] Null pointer provided for parameter \"p_key\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }

        // adjacency matrix errors
        {
            vertex_not_found:
                #ifndef NDEBUG
                    log_error("[adjacency matrix] Failed to find vertex in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
    }
}

int adjacency_matrix_vertex_add 
( 
    adjacency_matrix *p_adjacency_matrix, 
    void             *p_vertex
)
{

    // argument check
    if ( NULL == p_adjacency_matrix ) goto no_adjacency_matrix;
    if ( NULL ==           p_vertex ) goto no_vertex;

    // resize?
    if ( p_adjacency_matrix->vertices.count >= p_adjacency_matrix->vertices.capacity )
    {
        
        // initialized data
        void   **pp_edges = NULL;
        size_t   capacity = (p_adjacency_matrix->vertices.capacity == 0) ? 8 : p_adjacency_matrix->vertices.capacity * 2;
        
        // resize the vertex array
        p_adjacency_matrix->vertices.pp_vertices = default_allocator(p_adjacency_matrix->vertices.pp_vertices, sizeof(void *) * capacity);
        if ( NULL == p_adjacency_matrix->vertices.pp_vertices ) goto no_mem;
        
        // allocate memory for edges
        pp_edges = default_allocator(NULL, sizeof(void *) * (capacity * capacity));
        if ( NULL == pp_edges ) goto no_mem;

        // zero the memory
        memset(pp_edges, 0, sizeof(void *) * (capacity * capacity));
        
        // update edges
        if ( p_adjacency_matrix->edges.pp_edges )
        {

            // shift edges
            for ( size_t i = 0; i < p_adjacency_matrix->vertices.count; i++ )
                memcpy
                (
                    &pp_edges[i * capacity], 
                    &p_adjacency_matrix->edges.pp_edges[i * p_adjacency_matrix->vertices.capacity], 
                    sizeof(void *) * p_adjacency_matrix->vertices.count
                );
            
            // release the old edges
            default_allocator(p_adjacency_matrix->edges.pp_edges, 0);
        }
        
        // store the new edges
        p_adjacency_matrix->edges.pp_edges = pp_edges;

        // update the capacity
        p_adjacency_matrix->vertices.capacity = capacity;
        p_adjacency_matrix->edges.capacity    = capacity;
    }

    // insert the vertex
    p_adjacency_matrix->vertices.pp_vertices[p_adjacency_matrix->vertices.count] = p_vertex;

    // increment the vertex counter
    p_adjacency_matrix->vertices.count++;

    // success
    return 1;

    // error handling
    {

        // argument errors
        {
            no_adjacency_matrix:
                #ifndef NDEBUG
                    log_error("[adjacency matrix] Null pointer provided for parameter \"p_adjacency_matrix\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;

            no_vertex:
                #ifndef NDEBUG
                    log_error("[adjacency matrix] Null pointer provided for parameter \"p_vertex\" in call to function \"%s\"\n", __FUNCTION__);
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
    }
}

int adjacency_matrix_vertex_remove
( 
    adjacency_matrix  *p_adjacency_matrix, 
    const void        *p_key, 
    void             **pp_vertex, 
    fn_allocator      *pfn_allocator_vertex,
    fn_allocator      *pfn_allocator_edge
)
{

    // argument check
    if ( NULL == p_adjacency_matrix ) goto no_adjacency_matrix;
    if ( NULL ==              p_key ) goto no_key;

    // initialized data
    void *p_vertex = NULL;
    int   idx      = -1;

    // store the index of the vertex
    idx = vertex_index_get(p_adjacency_matrix, p_key);
    if ( idx == -1 ) goto vertex_not_found;

    // remove edges
    for ( size_t i = 0; i < p_adjacency_matrix->vertices.count; i++ )
    {

        // initialized data
        void *p_edge = p_adjacency_matrix->edges.pp_edges[idx * p_adjacency_matrix->vertices.capacity + i];

        // if an edge exists
        if ( p_edge )
        {

            // release the edge
            if ( pfn_allocator_edge ) 
                pfn_allocator_edge(p_edge, 0);

            // remove the edge
            p_adjacency_matrix->edges.pp_edges[idx * p_adjacency_matrix->vertices.capacity + i] = NULL,
            p_adjacency_matrix->edges.count--;

            // if undirected, remove the mirrored edge
            if ( !(p_adjacency_matrix->_type & GRAPH_DIRECTED) )
                p_adjacency_matrix->edges.pp_edges[i * p_adjacency_matrix->vertices.capacity + idx] = NULL,
                p_adjacency_matrix->edges.count--;
        }

        // if directed, check for incoming edges
        if ( p_adjacency_matrix->_type & GRAPH_DIRECTED )
        {

            // skip
            if ( i == (size_t)idx ) continue;

            // store the i'th edge
            p_edge = p_adjacency_matrix->edges.pp_edges[i * p_adjacency_matrix->vertices.capacity + idx];

            // if an edge exists
            if ( p_edge )
            {

                // release the edge
                if ( pfn_allocator_edge ) 
                    pfn_allocator_edge(p_edge, 0);

                // remove the edge
                p_adjacency_matrix->edges.pp_edges[i * p_adjacency_matrix->vertices.capacity + idx] = NULL,
                p_adjacency_matrix->edges.count--;
            }
        }
    }

    // store the vertex
    p_vertex = p_adjacency_matrix->vertices.pp_vertices[idx];

    // inner vertex
    if ( (size_t)idx < p_adjacency_matrix->vertices.count - 1 )
    {

        // update the vertex list
        p_adjacency_matrix->vertices.pp_vertices[idx] = p_adjacency_matrix->vertices.pp_vertices[p_adjacency_matrix->vertices.count - 1];

        // repair the vertex list
        memcpy(&p_adjacency_matrix->edges.pp_edges[idx * p_adjacency_matrix->vertices.capacity], &p_adjacency_matrix->edges.pp_edges[(p_adjacency_matrix->vertices.count - 1) * p_adjacency_matrix->vertices.capacity], sizeof(void *) * p_adjacency_matrix->vertices.capacity);

        // repair the edges
        for ( size_t i = 0; i < p_adjacency_matrix->vertices.count; i++ )
            p_adjacency_matrix->edges.pp_edges[i * p_adjacency_matrix->vertices.capacity + idx] = p_adjacency_matrix->edges.pp_edges[i * p_adjacency_matrix->vertices.capacity + (p_adjacency_matrix->vertices.count - 1)];
        
        // clear the last row and column
        memset(&p_adjacency_matrix->edges.pp_edges[(p_adjacency_matrix->vertices.count - 1) * p_adjacency_matrix->vertices.capacity], 0, sizeof(void *) * p_adjacency_matrix->vertices.capacity);
        for ( size_t i = 0; i < p_adjacency_matrix->vertices.count; i++ )
            p_adjacency_matrix->edges.pp_edges[i * p_adjacency_matrix->vertices.capacity + (p_adjacency_matrix->vertices.count - 1)] = NULL;
    }

    // last vertex
    else
    {

        // clear the last row and column
        memset(&p_adjacency_matrix->edges.pp_edges[idx * p_adjacency_matrix->vertices.capacity], 0, sizeof(void *) * p_adjacency_matrix->vertices.capacity);
        for ( size_t i = 0; i < p_adjacency_matrix->vertices.count; i++ )
            p_adjacency_matrix->edges.pp_edges[i * p_adjacency_matrix->vertices.capacity + idx] = NULL;
    }

    // decrement the vertex counter
    p_adjacency_matrix->vertices.count--;

    // release the vertex
    if ( pfn_allocator_vertex ) 
        p_vertex = pfn_allocator_vertex(p_vertex, 0);

    // return a pointer to the caller
    if ( pp_vertex ) 
        *pp_vertex = p_vertex;

    // success
    return 1;

    // error handling
    {

        // argument errors
        {
            no_adjacency_matrix:
                #ifndef NDEBUG
                    log_error("[adjacency matrix] Null pointer provided for parameter \"p_adjacency_matrix\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;

            no_key:
                #ifndef NDEBUG
                    log_error("[adjacency matrix] Null pointer provided for parameter \"p_key\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }

        // adjacency matrix errors
        {
            vertex_not_found:
                #ifndef NDEBUG
                    log_error("[adjacency matrix] Failed to find vertex in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
    }
}

int adjacency_matrix_edge_add 
( 
    adjacency_matrix *p_adjacency_matrix, 
    const void       *p_from, 
    const void       *p_to, 
    void             *p_edge
)
{

    // argument check
    if ( NULL == p_adjacency_matrix ) goto no_adjacency_matrix;
    if ( NULL ==             p_from ) goto no_from;
    if ( NULL ==               p_to ) goto no_to;
    if ( NULL ==             p_edge ) goto no_edge;

    // initialized data
    int row = -1; 
    int col = -1; 

    // find the index of the source vertex
    row = vertex_index_get(p_adjacency_matrix, p_from);
    if ( row == -1 ) goto vertex_not_found;
    
    // find the index of the destination vertex
    col = vertex_index_get(p_adjacency_matrix, p_to);
    if ( col == -1 ) goto vertex_not_found;

    // store the edge
    p_adjacency_matrix->edges.pp_edges[row * p_adjacency_matrix->vertices.capacity + col] = p_edge,
    p_adjacency_matrix->edges.count++;

    // if not directed, mirror the edge
    if ( !(p_adjacency_matrix->_type & GRAPH_DIRECTED) )
        p_adjacency_matrix->edges.pp_edges[col * p_adjacency_matrix->vertices.capacity + row] = p_edge,
        p_adjacency_matrix->edges.count++;

    // success
    return 1;

    // error handling
    {

        // argument errors
        {
            no_adjacency_matrix:
                #ifndef NDEBUG
                    log_error("[adjacency matrix] Null pointer provided for parameter \"p_adjacency_matrix\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;

            no_to:
                #ifndef NDEBUG
                    log_error("[adjacency matrix] Null pointer provided for parameter \"p_to\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;

            no_from:
                #ifndef NDEBUG
                    log_error("[adjacency matrix] Null pointer provided for parameter \"p_from\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;

            no_edge:
                #ifndef NDEBUG
                    log_error("[adjacency matrix] Null pointer provided for parameter \"p_edge\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
        
        // adjacency matrix errors
        {
            vertex_not_found:
                #ifndef NDEBUG
                    log_error("[adjacency matrix] Failed to find vertex in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
    }
}

int adjacency_matrix_edge_remove 
( 
    adjacency_matrix *p_adjacency_matrix, 
    const void *p_from, 
    const void *p_to, 
    void **pp_edge,
    fn_allocator *pfn_allocator_edge
)
{

    // argument check
    if ( NULL == p_adjacency_matrix ) goto no_adjacency_matrix;
    if ( NULL ==             p_from ) goto no_from;
    if ( NULL ==               p_to ) goto no_to;

    // initialized data
    void *p_edge = NULL;
    int   row    = -1; 
    int   col    = -1; 

    // find the index of the source vertex
    row = vertex_index_get(p_adjacency_matrix, p_from);
    if ( row == -1 ) goto vertex_not_found;
    
    // find the index of the destination vertex
    col = vertex_index_get(p_adjacency_matrix, p_to);
    if ( col == -1 ) goto vertex_not_found;

    // remove the edge
    p_edge = p_adjacency_matrix->edges.pp_edges[row * p_adjacency_matrix->vertices.capacity + col],
    p_adjacency_matrix->edges.pp_edges[row * p_adjacency_matrix->vertices.capacity + col] = NULL,
    p_adjacency_matrix->edges.count--;

    // if not directed, remove the extra edge
    if ( !(p_adjacency_matrix->_type & GRAPH_DIRECTED) )
        p_adjacency_matrix->edges.pp_edges[col * p_adjacency_matrix->vertices.capacity + row] = NULL,
        p_adjacency_matrix->edges.count--;

    // release the edge
    if ( pfn_allocator_edge )
        p_edge = pfn_allocator_edge(p_edge, 0);

    // return a pointer to the caller
    if ( pp_edge ) 
        *pp_edge = p_edge;

    // success
    return 1;

    // error handling
    {

        // argument errors
        {
            no_adjacency_matrix:
                #ifndef NDEBUG
                    log_error("[adjacency matrix] Null pointer provided for parameter \"p_adjacency_matrix\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;

            no_to:
                #ifndef NDEBUG
                    log_error("[adjacency matrix] Null pointer provided for parameter \"p_to\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;

            no_from:
                #ifndef NDEBUG
                    log_error("[adjacency matrix] Null pointer provided for parameter \"p_from\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
        
        // adjacency matrix errors
        {
            vertex_not_found:
                #ifndef NDEBUG
                    log_error("[adjacency matrix] Failed to find vertex in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
    }
}

int adjacency_matrix_vertex_foreach
(
    adjacency_matrix *p_adjacency_matrix,
    fn_foreach       *pfn_foreach
)
{

    // argument check
    if ( NULL == p_adjacency_matrix ) goto no_adjacency_matrix;
    if ( NULL ==        pfn_foreach ) goto no_foreach;

    // iterate over each vertex
    for ( size_t i = 0; i < p_adjacency_matrix->vertices.count; i++ )

        // call the foreach function
        pfn_foreach(p_adjacency_matrix->vertices.pp_vertices[i]);

    // success
    return 1;

    // error handling
    {

        // argument errors
        {
            no_adjacency_matrix:
                #ifndef NDEBUG
                    log_error("[adjacency matrix] Null pointer provided for parameter \"p_adjacency_matrix\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;

            no_foreach:
                #ifndef NDEBUG
                    log_error("[adjacency matrix] Null pointer provided for parameter \"pfn_foreach\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
    }
}

int adjacency_matrix_edge_foreach
(
    adjacency_matrix *p_adjacency_matrix,
    fn_foreach       *pfn_foreach
)
{

    // argument check
    if ( NULL == p_adjacency_matrix ) goto no_adjacency_matrix;
    if ( NULL ==        pfn_foreach ) goto no_foreach;

    // iterate over each edge
    for ( size_t i = 0; i < p_adjacency_matrix->vertices.count; i++ )
        for ( size_t j = 0; j < p_adjacency_matrix->vertices.count; j++ )
        {

            // undirected optimization
            if ( !(p_adjacency_matrix->_type & GRAPH_DIRECTED) && j < i ) continue;

            // call the foreach function
            if ( p_adjacency_matrix->edges.pp_edges[i * p_adjacency_matrix->vertices.capacity + j] )
                pfn_foreach(p_adjacency_matrix->edges.pp_edges[i * p_adjacency_matrix->vertices.capacity + j]);
        }

    // success
    return 1;

    // error handling
    {

        // argument errors
        {
            no_adjacency_matrix:
                #ifndef NDEBUG
                    log_error("[adjacency matrix] Null pointer provided for parameter \"p_adjacency_matrix\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;

            no_foreach:
                #ifndef NDEBUG
                    log_error("[adjacency matrix] Null pointer provided for parameter \"pfn_foreach\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
    }
}

int adjacency_matrix_pack
(
    void *p_buffer, 
    adjacency_matrix *p_adjacency_matrix,

    fn_pack *pfn_vertex,
    fn_pack *pfn_edge
)
{

    // argument check
    if ( NULL ==           p_buffer ) goto no_buffer;
    if ( NULL == p_adjacency_matrix ) goto no_adjacency_matrix;
    if ( NULL ==         pfn_vertex ) goto no_vertex_pack;
    if ( NULL ==           pfn_edge ) goto no_edge_pack;

    // initialized data 
    char *p = p_buffer;

    // pack the metadata
    p += pack_pack(p, "%i32%2i64", 
        p_adjacency_matrix->_type,
        p_adjacency_matrix->vertices.vertex_size,
        p_adjacency_matrix->edges.edge_size
    );

    // pack the vertex quantity
    p += pack_pack(p, "%i64", p_adjacency_matrix->vertices.count);

    // pack the vertices
    for ( size_t i = 0; i < p_adjacency_matrix->vertices.count; i++ )
        p += pfn_vertex(p, p_adjacency_matrix->vertices.pp_vertices[i]);

    // pack the adjacency matrix
    for ( size_t i = 0; i < p_adjacency_matrix->vertices.count; i++ )
        for ( size_t j = 0; j < p_adjacency_matrix->vertices.count; j++ )
            if ( p_adjacency_matrix->edges.pp_edges[i * p_adjacency_matrix->vertices.capacity + j] )
                p += pfn_edge(p, p_adjacency_matrix->edges.pp_edges[i * p_adjacency_matrix->vertices.capacity + j]);
            else
                p += pack_pack(p, "%i64", eight_bytes_of_f);
    
    // success
    return p - (char *)p_buffer;

    // error handling
    {
        
        // argument errors
        {
            no_buffer:
                #ifndef NDEBUG
                    log_error("[adjacency matrix] Null pointer provided for \"p_buffer\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;

            no_adjacency_matrix:
                #ifndef NDEBUG
                    log_error("[adjacency matrix] Null pointer provided for \"p_adjacency_matrix\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
            
            no_vertex_pack:
                #ifndef NDEBUG
                    log_error("[adjacency matrix] Null pointer provided for \"pfn_vertex\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
            
            no_edge_pack:
                #ifndef NDEBUG
                    log_error("[adjacency matrix] Null pointer provided for \"pfn_edge\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
    }
}

int adjacency_matrix_unpack
(
    adjacency_matrix **pp_adjacency_matrix,
    void *p_buffer, 

    fn_unpack *pfn_vertex,
    fn_unpack *pfn_edge,

    fn_key_accessor *pfn_key_accessor,
    fn_comparator   *pfn_comparator
)
{

    // argument check
    if ( NULL == pp_adjacency_matrix ) goto no_adjacency_matrix;
    if ( NULL ==            p_buffer ) goto no_buffer;
    if ( NULL ==          pfn_vertex ) goto no_vertex_unpack;
    if ( NULL ==            pfn_edge ) goto no_edge_unpack;
    if ( NULL ==    pfn_key_accessor ) goto no_key_accessor;
    if ( NULL ==      pfn_comparator ) goto no_comparator;

    // initialized data 
    char                   *p                  = p_buffer;
    adjacency_matrix       *p_adjacency_matrix = NULL;
    enum graph_edge_type_e  _type              = 0;
    size_t                  vertex_size        = 0;
    size_t                  edge_size          = 0;
    size_t                  vertex_count       = 0;

    // unpack the metadata
    p += pack_unpack(p, "%i32%2i64", 
        &_type,
        &vertex_size,
        &edge_size
    );

    // construct the adjacency matrix
    adjacency_matrix_construct
    (
        &p_adjacency_matrix,
        _type,

        vertex_size,
        edge_size,

        pfn_key_accessor,
        pfn_comparator
    );

    // unpack the vertex quantity
    p += pack_unpack(p, "%i64", &vertex_count);

    // unpack the vertices
    for ( size_t i = 0; i < vertex_count; i++ )
    {

        // initialized data
        void *p_vertex = NULL;

        // unpack the vertex
        p += pfn_vertex(&p_vertex, p);

        // add the vertex
        adjacency_matrix_vertex_add(p_adjacency_matrix, p_vertex);
    }

    // unpack the adjacency matrix
    for ( size_t i = 0; i < vertex_count; i++ )
        for ( size_t j = 0; j < vertex_count; j++ )
        {

            // initialized data
            void *p_edge = NULL;
            unsigned long long sentinel = 0;

            // skip
            pack_unpack(p, "%i64", &sentinel);
            if ( sentinel == eight_bytes_of_f ) 
            {
                p += sizeof(sentinel);
                continue;
            }
            
            // unpack the edge
            p += pfn_edge(&p_edge, p),
            p_adjacency_matrix->edges.pp_edges[i * p_adjacency_matrix->vertices.capacity + j] = p_edge,
            p_adjacency_matrix->edges.count++;
        }

    // return a pointer to the caller
    *pp_adjacency_matrix = p_adjacency_matrix;

    // success
    return p - (char *)p_buffer;

    // error handling
    {
        
        // argument errors
        {
            no_adjacency_matrix:
                #ifndef NDEBUG
                    log_error("[adjacency matrix] Null pointer provided for \"pp_adjacency_matrix\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;

            no_buffer:
                #ifndef NDEBUG
                    log_error("[adjacency matrix] Null pointer provided for \"p_buffer\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;

            no_vertex_unpack:
                #ifndef NDEBUG
                    log_error("[adjacency matrix] Null pointer provided for \"pfn_vertex\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
            
            no_edge_unpack:
                #ifndef NDEBUG
                    log_error("[adjacency matrix] Null pointer provided for \"pfn_edge\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
            
            no_key_accessor:
                #ifndef NDEBUG
                    log_error("[adjacency matrix] Null pointer provided for \"pfn_key_accessor\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;

            no_comparator:
                #ifndef NDEBUG
                    log_error("[adjacency matrix] Null pointer provided for \"pfn_comparator\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
    }
}

hash64 adjacency_matrix_hash
(
    adjacency_matrix *p_adjacency_matrix,
    fn_hash64 *pfn_hash64
)
{

    // argument check
    if ( NULL == p_adjacency_matrix ) goto no_adjacency_matrix;

    // initialized data
    hash64     result   = 0;
    fn_hash64 *pfn_hash = (pfn_hash64) ? pfn_hash64 : default_hash;

    // hash the vertices
    for ( size_t i = 0; i < p_adjacency_matrix->vertices.count; i++ )
        result ^= pfn_hash(p_adjacency_matrix->vertices.pp_vertices[i], 8);

    // hash the edges
    for ( size_t i = 0; i < p_adjacency_matrix->vertices.count; i++ )
        for ( size_t j = 0; j < p_adjacency_matrix->vertices.count; j++ )
            if ( p_adjacency_matrix->edges.pp_edges[i * p_adjacency_matrix->vertices.capacity + j] )
                result ^= pfn_hash(p_adjacency_matrix->edges.pp_edges[i * p_adjacency_matrix->vertices.capacity + j], 8);

    // success
    return result;

    // error handling
    {

        // argument errors
        {
            no_adjacency_matrix:
                #ifndef NDEBUG
                    log_error("[adjacency matrix] Null pointer provided for parameter \"p_adjacency_matrix\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
    }
}

int adjacency_matrix_destroy
(
    adjacency_matrix **pp_adjacency_matrix,
    
    fn_allocator *pfn_allocator_vertex,
    fn_allocator *pfn_allocator_edge
)
{
    
    // argument check
    if ( NULL == pp_adjacency_matrix ) goto no_adjacency_matrix;

    // initialized data
    adjacency_matrix *p_adjacency_matrix = *pp_adjacency_matrix;

    // fast exit
    if ( NULL == p_adjacency_matrix ) return 1;

    // no more pointer for caller
    *pp_adjacency_matrix = NULL;

    // release each vertex
    if ( pfn_allocator_vertex )
        for ( size_t i = 0; i < p_adjacency_matrix->vertices.count; i++ )
            pfn_allocator_vertex(p_adjacency_matrix->vertices.pp_vertices[i], 0);

    // release vertices
    default_allocator(p_adjacency_matrix->vertices.pp_vertices, 0);

    // release each edge
    if ( pfn_allocator_edge )
        for ( size_t i = 0; i < p_adjacency_matrix->vertices.count; i++ )
            for (size_t j = 0; j < p_adjacency_matrix->vertices.count; j++)
                if ( p_adjacency_matrix->edges.pp_edges[i * p_adjacency_matrix->vertices.capacity + j] )
                    pfn_allocator_edge(p_adjacency_matrix->edges.pp_edges[i * p_adjacency_matrix->vertices.capacity + j], 0);
    
    // release edges
    default_allocator(p_adjacency_matrix->edges.pp_edges, 0);

    // release the adjacency matrix
    default_allocator(p_adjacency_matrix, 0);

    // success
    return 1;

    // error handling
    {

        // argument errors
        {
            no_adjacency_matrix:
                #ifndef NDEBUG
                    printf("[adjacency matrix] Null pointer provided for parameter \"pp_adjacency_matrix\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
    }
}
