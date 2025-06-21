#include <graph/graph_adjacency_matrix.h>


int graph_adjacency_matrix_vertex_add ( graph **pp_graph, char _name[], void *p_value )
{

    // initialized data
    graph *p_graph = *pp_graph;
    struct adjacency_matrix_s *p_adjacency_matrix = &p_graph->storage._adjacency_matrix;
    size_t size = p_adjacency_matrix->size + 1;
    size_t offset = 0;
    
    // Grow matrix
    p_graph = realloc(p_graph, sizeof(graph) + ( sizeof(node) * ( size * (size + 1) ) ) );
    p_adjacency_matrix = &p_graph->storage._adjacency_matrix;

    // Heading 
    strncpy(p_adjacency_matrix->_data[size - 1]._unweighted_undirected._name, _name, 64);
    p_adjacency_matrix->_data[size - 1]._unweighted_undirected.p_value = p_value;

    for (size_t i = 1; i < size; i++)
    {
        memcpy(
            &p_adjacency_matrix->_data[i * size],
            &p_adjacency_matrix->_data[i * size - 1],
            sizeof(node) * (size - 1)
        );
    }

    p_graph->storage._adjacency_matrix.size = size;
    
    *pp_graph = p_graph;

    putchar('\n');
    
    //log_info("%s\n", __FUNCTION__);
    
    graph_adjacency_matrix_info(p_graph);
    
    return 1;
}


int graph_adjacency_matrix_info ( graph *p_graph )
{

    size_t size = p_graph->storage._adjacency_matrix.size;
    size_t longest = 0;

    for (size_t i = 0; i < size; i++)
    {
        size_t len = strlen(p_graph->storage._adjacency_matrix._data[i]._unweighted_undirected._name);
        
        longest = ( len > longest ) ? len : longest; 
    }
    
    for (size_t i = 0; i <= longest; i++)
    {
        putchar(' ');
        fflush(stdout);
    }
    
    for (size_t i = 0; i < size; i++)
    {
        printf("%-*s ", longest, p_graph->storage._adjacency_matrix._data[i]._unweighted_undirected._name);
        fflush(stdout);
    }

    putchar('\n');fflush(stdout);
    
    for (size_t i = 0; i <= longest; i++)
    {
        putchar(' ');
        fflush(stdout);
    }
    
    for (size_t i = 0; i <= (size + 1) * longest; i++)
    {
        putchar('-');
        fflush(stdout);
    }

    putchar('\n');fflush(stdout);

    for (size_t i = 0; i < size; i++)
    {
        //printf("%-*s ", longest, p_graph->storage._adjacency_matrix._data[i+size]._unweighted_undirected._name);fflush(stdout);

        for (size_t i = 0; i <= longest; i++)
        {
            putchar(' ');
            fflush(stdout);
        }
        for (size_t j = 0; j < size; j++)
        {


            size_t expr = (i * size ) + j;

            // initialized data
            printf("%-*s ", longest,  p_graph->storage._adjacency_matrix._data[expr]._unweighted_undirected._name);fflush(stdout);
        }

        // Formatting
        putchar('\n');fflush(stdout);
    }
    

    return 1;
}