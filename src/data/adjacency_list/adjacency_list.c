#include <data/graph.h>

int adjacency_list_construct ( adjacency_list **pp_adjacency_list, size_t size )
{
    // argument check
    if ( pp_adjacency_list == NULL ) goto no_adjacency_list;

    // initialized data
    adjacency_list *p_adjacency_list = realloc(0, sizeof(adjacency_list));

    // error check
    if ( p_adjacency_list == NULL ) goto no_mem;

    // initialize the adjacency list structure
    p_adjacency_list->size = 0; // Start with 0 vertices
    p_adjacency_list->edge_count = 0; // Start with 0 edges
    
    // Initialize with no vertices allocated - let vertex_add handle allocation
    // The size parameter is ignored for now, we'll allocate as needed
    p_adjacency_list->p_vertices = NULL;
    
    // Return the constructed adjacency list
    *pp_adjacency_list = p_adjacency_list;

    // Return success
    return 1;

    // error handling
    {

        // argument errors
        {
            no_adjacency_list:
                #ifndef NDEBUG
                    printf("[graph] [adjacency list] Null pointer provided for parameter \"pp_adjacency_list\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }

        // standard library errors
        {
            no_mem:
                #ifndef NDEBUG
                    printf("[Standard Library] Call to function \"realloc\" returned an erroneous value in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
    }
}

int adjacency_list_vertex_add(graph *p_graph, void *p_value)
{
    adjacency_list *p_list = &p_graph->storage._adjacency_list;

    // Argument check
    if (p_list == NULL || p_value == NULL) return 0;

    // Check if vertex already exists
    for (size_t i = 0; i < p_list->size; i++) {
        if (p_list->p_vertices[i].p_vertex == p_value) {
            // Vertex already exists
            return 1;
        }
    }
    
    // We need to reallocate to accommodate the new vertex
    // Since we don't have a capacity field, we'll reallocate every time
    // This is not optimal, but it's safe and correct
    size_t new_size = p_list->size + 1;
    
    void *new_vertices = realloc(p_list->p_vertices, new_size * sizeof(*p_list->p_vertices));
    if (new_vertices == NULL) return 0;
    
    p_list->p_vertices = new_vertices;
    
    // Add the new vertex
    p_list->p_vertices[p_list->size].p_vertex = p_value;
    p_list->p_vertices[p_list->size].p_head = NULL;
    p_list->size = new_size;
    
    // Add the new vertex
    p_list->p_vertices[p_list->size].p_vertex = p_value;
    p_list->p_vertices[p_list->size].p_head = NULL;
    p_list->size++;
    
    return 1;
}

int adjacency_list_vertex_remove(adjacency_list *p_list, void *p_value)
{
    // Find the vertex to remove
    size_t vertex_index = SIZE_MAX;
    for (size_t i = 0; i < p_list->size; i++) {
        if (p_list->p_vertices[i].p_vertex == p_value) {
            vertex_index = i;
            break;
        }
    }
    
    // Vertex not found
    if (vertex_index == SIZE_MAX) return 0;
    
    // Free the adjacency list for this vertex
    struct adjacency_list_node_s *current = p_list->p_vertices[vertex_index].p_head;
    while (current != NULL) {
        struct adjacency_list_node_s *next = current->next;
        free(current);
        current = next;
        p_list->edge_count--;
    }
    
    // Remove edges from other vertices that point to this vertex
    for (size_t i = 0; i < p_list->size; i++) {
        if (i == vertex_index) continue;
        
        struct adjacency_list_node_s **current_ptr = &p_list->p_vertices[i].p_head;
        while (*current_ptr != NULL) {
            if ((*current_ptr)->p_vertex == p_value) {
                struct adjacency_list_node_s *to_remove = *current_ptr;
                *current_ptr = (*current_ptr)->next;
                free(to_remove);
                p_list->edge_count--;
            } else {
                current_ptr = &(*current_ptr)->next;
            }
        }
    }
    
    // Shift remaining vertices down
    for (size_t i = vertex_index; i < p_list->size - 1; i++) {
        p_list->p_vertices[i] = p_list->p_vertices[i + 1];
    }
    
    p_list->size--;
    
    return 1;
}

int adjacency_list_vertex_search(adjacency_list *p_list, void *p_value, void **pp_value)
{
    // Search for the vertex
    for (size_t i = 0; i < p_list->size; i++) {
        if (p_list->p_vertices[i].p_vertex == p_value) {
            *pp_value = p_list->p_vertices[i].p_vertex;
            return 1;
        }
    }
    
    // Vertex not found
    return 0;
}

int adjacency_list_vertex_count(adjacency_list *p_list, size_t *p_count)
{
    // Return the vertex count
    *p_count = p_list->size;
    
    return 1;
}

int adjacency_list_vertex_info(adjacency_list *p_list, void *p_value, char _name[], size_t name_size, void **pp_value)
{
    // Argument check
    if (p_list == NULL || p_value == NULL) return 0;

    // Find the vertex
    for (size_t i = 0; i < p_list->size; i++) {
        if (p_list->p_vertices[i].p_vertex == p_value) {
            // Found the vertex
            if (_name != NULL && name_size > 0) {
                // Format vertex name/info
                snprintf(_name, name_size, "Vertex_%zu", i);
            }
            if (pp_value != NULL) {
                *pp_value = p_list->p_vertices[i].p_vertex;
            }
            return 1;
        }
    }
    
    // Vertex not found
    return 0;
}

int adjacency_list_edge_add(adjacency_list *p_list, void *p_a_value, void *p_b_value, double weight)
{
    // Argument check
    if (p_list == NULL || p_a_value == NULL || p_b_value == NULL) {
        #ifndef NDEBUG
            printf("[graph] [adjacency list] Null pointer provided in call to function \"%s\"\n", __FUNCTION__);
        #endif
        return 0;
    }

    // Find vertex A (source vertex)
    size_t vertex_a_index = SIZE_MAX;
    for (size_t i = 0; i < p_list->size; i++) {
        if (p_list->p_vertices[i].p_vertex == p_a_value) {
            vertex_a_index = i;
            break;
        }
    }
    
    // Check if vertex A exists
    if (vertex_a_index == SIZE_MAX) {
        #ifndef NDEBUG
            printf("[graph] [adjacency list] Source vertex not found in call to function \"%s\"\n", __FUNCTION__);
        #endif
        return 0;
    }

    // Find vertex B (destination vertex) to verify it exists
    bool vertex_b_exists = false;
    for (size_t i = 0; i < p_list->size; i++) {
        if (p_list->p_vertices[i].p_vertex == p_b_value) {
            vertex_b_exists = true;
            break;
        }
    }
    
    if (!vertex_b_exists) {
        #ifndef NDEBUG
            printf("[graph] [adjacency list] Destination vertex not found in call to function \"%s\"\n", __FUNCTION__);
        #endif
        return 0;
    }

    // Check if edge already exists
    struct adjacency_list_node_s *current = p_list->p_vertices[vertex_a_index].p_head;
    while (current != NULL) {
        if (current->p_vertex == p_b_value) {
            // Edge already exists, update weight
            current->edge._weighted_directed.weight = weight;
            current->edge._weighted_directed.p_value = p_b_value;
            return 1;
        }
        current = current->next;
    }

    // Create new adjacency list node
    struct adjacency_list_node_s *p_new_node = malloc(sizeof(struct adjacency_list_node_s));
    if (p_new_node == NULL) {
        #ifndef NDEBUG
            printf("[Standard Library] Call to function \"malloc\" returned an erroneous value in call to function \"%s\"\n", __FUNCTION__);
        #endif
        return 0;
    }

    // Initialize the new node
    p_new_node->p_vertex = p_b_value;
    p_new_node->edge._weighted_directed.weight = weight;
    p_new_node->edge._weighted_directed.p_value = p_b_value;
    p_new_node->next = p_list->p_vertices[vertex_a_index].p_head;
    
    // Insert at the beginning of the adjacency list
    p_list->p_vertices[vertex_a_index].p_head = p_new_node;
    
    // Increment edge count
    p_list->edge_count++;

    return 1;
}

int adjacency_list_print(adjacency_list *p_list, void (*print_func)(void *))
{
    // Argument check
    if (p_list == NULL) {
        #ifndef NDEBUG
            printf("[graph] [adjacency list] Null pointer provided for parameter \"p_list\" in call to function \"%s\"\n", __FUNCTION__);
        #endif
        return 0;
    }

    printf("adjacency list @ %p\n\t- vertices: %zu\n\t- edges: %zu\n", (void *)p_list, p_list->size, p_list->edge_count);

    // Print each vertex and its adjacency list
    for (size_t i = 0; i < p_list->size; i++) {
        printf("\t[%zu] ", i),
        print_func(p_list->p_vertices[i].p_vertex),
        putchar(' ');
        
        struct adjacency_list_node_s *current = p_list->p_vertices[i].p_head;
        if (current == NULL) {
            printf("-> NULL");
        } else {
            while (current != NULL) {
                printf("-> (");
                print_func(current->p_vertex);
                printf(", %.2f)", current->edge._weighted_directed.weight);
                current = current->next;
                if (current != NULL) {
                    printf(" ");
                }
            }
        }
        printf("\n");
    }

    return 1;
}