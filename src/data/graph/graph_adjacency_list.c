#include <graph/graph_adjacency_list.h>

int graph_adjacency_list_vertex_add ( graph *p_graph, char _name[], void *p_value )
{

    struct adjacency_list_s *p_adjacency_list = &p_graph->storage._adjacency_list;

    p_adjacency_list->size++;
    log_info("%s\n", __FUNCTION__);
    
    return 1;
}

int graph_adjacency_list_info ( graph *p_graph )
{

    log_info(" %s \n", __FUNCTION__);

    return 1;
}