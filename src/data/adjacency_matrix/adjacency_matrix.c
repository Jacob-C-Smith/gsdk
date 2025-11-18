#include <data/adjacency_matrix.h>

int adjacency_matrix_construct ( adjacency_matrix **pp_adjacency_matrix, size_t size )
{
    // argument check
    if ( pp_adjacency_matrix == NULL ) goto no_adjacency_matrix;

    // initialized data
    adjacency_matrix *p_adjacency_matrix = default_allocator(0, sizeof(adjacency_matrix));

    // error check
    if ( p_adjacency_matrix == NULL ) goto no_mem;

    // initialize the adjacency_matrix structure
    p_adjacency_matrix->size = 0; // Start with 0 vertices
    p_adjacency_matrix->edge_count = 0; // Start with 0 edges
    
    // Initialize with no vertices allocated - let vertex_add handle allocation
    // The size parameter is ignored for now, we'll allocate as needed
    p_adjacency_matrix->p_vertices = NULL;
    
    // return the constructed adjacency_matrix
    *pp_adjacency_matrix = p_adjacency_matrix;

    // return success
    return 1;

    // error handling
    {

        // argument errors
        {
            no_adjacency_matrix:
                #ifndef NDEBUG
                    printf("[graph] [adjacency matrix] Null pointer provided for parameter \"pp_adjacency_matrix\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }

        // standard library errors
        {
            no_mem:
                #ifndef NDEBUG
                    log_error("[standard library] Call to function \"default_allocator\" returned an erroneous value in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
    }
}

int adjacency_matrix_vertex_add(adjacency_matrix *p_matrix, void *p_value)
{
    // Argument check
    if (p_matrix == NULL || p_value == NULL) return 0;

    // Check if vertex already exists
    for (size_t i = 0; i < p_matrix->size; i++) {
        if (p_matrix->p_vertices[i] == p_value) {
            // Vertex already exists
            return 1;
        }
    }
    
    // We need to default_allocatorate to accommodate the new vertex
    size_t new_size = p_matrix->size + 1;
    
    void *new_vertices = default_allocator(p_matrix->p_vertices, new_size * sizeof(void *));
    if (new_vertices == NULL) return 0;
    
    p_matrix->p_vertices = new_vertices;
    p_matrix->p_vertices[p_matrix->size] = p_value;

    double **new_matrix = default_allocator(p_matrix->matrix, new_size * sizeof(double *));
    if (new_matrix == NULL) return 0;

    for(size_t i = 0; i < new_size; i++)
    {
        new_matrix[i] = default_allocator(new_matrix[i], new_size * sizeof(double));
        if(new_matrix[i] == NULL) return 0;
    }

    p_matrix->matrix = new_matrix;

    for(size_t i = 0; i < new_size; i++)
    {
        for(size_t j = 0; j < new_size; j++)
        {
            if(i >= p_matrix->size || j >= p_matrix->size)
            {
                p_matrix->matrix[i][j] = 0.0;
            }
        }
    }

    p_matrix->size = new_size;
    
    return 1;
}

int adjacency_matrix_edge_add(adjacency_matrix *p_matrix, void *p_a_value, void *p_b_value, double weight)
{
    // Argument check
    if (p_matrix == NULL || p_a_value == NULL || p_b_value == NULL) return 0;

    size_t a_idx = -1, b_idx = -1;

    for(size_t i = 0; i < p_matrix->size; i++)
    {
        if(p_matrix->p_vertices[i] == p_a_value) a_idx = i;
        if(p_matrix->p_vertices[i] == p_b_value) b_idx = i;
    }

    if(a_idx == -1 || b_idx == -1) return 0;

    p_matrix->matrix[a_idx][b_idx] = weight;
    p_matrix->edge_count++;

    return 1;
}

int adjacency_matrix_print(adjacency_matrix *p_matrix, void (*print_func)(void *))
{
    // Argument check
    if (p_matrix == NULL) {
        #ifndef NDEBUG
            printf("[graph] [adjacency matrix] Null pointer provided for parameter \"p_matrix\" in call to function \"%s\"\n", __FUNCTION__);
        #endif
        return 0;
    }

    printf("adjacency matrix @ %p\n\t- vertices: %zu\n\t- edges: %zu\n", (void *)p_matrix, p_matrix->size, p_matrix->edge_count);

    for (size_t i = 0; i < p_matrix->size; i++) {
        print_func(p_matrix->p_vertices[i]);
        printf(" ");
    }
    printf("\n");

    for (size_t i = 0; i < p_matrix->size; i++) {
        for (size_t j = 0; j < p_matrix->size; j++) {
            printf("%.2f ", p_matrix->matrix[i][j]);
        }
        printf("\n");
    }

    return 1;
}