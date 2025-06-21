#include <graph/graph_edge_list.h>

int graph_edge_list_vertex_add ( graph *p_graph, char _name[], void *p_value )
{

    struct edge_list_s *p_edge_list = &p_graph->storage._edge_list;

    p_edge_list->size++;
    log_info("%s \n", __FUNCTION__);
    
    return 1;
}