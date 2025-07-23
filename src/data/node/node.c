/** !
 * Node implementation
 * 
 * @file node.c
 * 
 * @author Jacob Smith
 */

// header
#include <node/node.h>

// Data
static bool initialized = false;

// function definitions
void node_init ( void ) 
{

    // State check
    if ( initialized == true ) return;

    // Initialize the sync library
    sync_init();
    
    // Initialize the log library
    log_init();

    // Initialize the array library
    array_init();

    // Initialize the dict library
    dict_init();

    // Initialize the json library
    json_init();

    // Set the initialized flag
    initialized = true;

    // Done
    return; 
}

int node_create ( node **pp_node )
{

    // argument check
    if ( pp_node == (void *) 0 ) goto no_node;

    // initialized data
    node *p_node = default_allocator(0, sizeof(node));

    // error check
    if ( p_node == (void *) 0 ) goto no_mem;

    // Initialize memory
    memset(p_node, 0, sizeof(node));

    // return a pointer to the caller
    *pp_node = p_node;

    // success
    return 1;

    // error handling
    {

        // argument errors
        {
            no_node:
                #ifndef NDEBUG
                    log_error("[node] Null pointer provided for parameter \"pp_node\" in call to function \"%s\"\n", __FUNCTION__);
                #endif
                
                // error
                return 0;
        }

        // standard library errors
        {
            no_mem:
                #ifndef NDEBUG
                    log_error("[Standard Library] Failed to allocate memory in call to function \"%s\"\n", __FUNCTION__);
                #endif
                
                // error
                return 0;
        }
    }
}

int node_graph_create ( node_graph **pp_node_graph )
{

    // argument check
    if ( pp_node_graph == (void *) 0 ) goto no_node_graph;

    // initialized data
    node_graph *p_node_graph = default_allocator(0, sizeof(node_graph));

    // error check
    if ( p_node_graph == (void *) 0 ) goto no_mem;

    // Initialize memory
    memset(p_node_graph, 0, sizeof(node_graph));

    // return a pointer to the caller
    *pp_node_graph = p_node_graph;

    // success
    return 1;

    // error handling
    {

        // argument errors
        {
            no_node_graph:
                #ifndef NDEBUG
                    log_error("[node] Null pointer provided for parameter \"pp_node_graph\" in call to function \"%s\"\n", __FUNCTION__);
                #endif
                
                // error
                return 0;
        }

        // standard library errors
        {
            no_mem:
                #ifndef NDEBUG
                    log_error("[Standard Library] Failed to allocate memory in call to function \"%s\"\n", __FUNCTION__);
                #endif
                
                // error
                return 0;
        }
    }
}

int node_graph_construct ( node_graph **pp_node_graph, const json_value *const p_value )
{

    // argument check
    if ( pp_node_graph ==        (void *) 0 ) goto no_node_graph;
    if ( p_value       ==        (void *) 0 ) goto no_value;
    if ( p_value->type != JSON_VALUE_OBJECT ) goto wrong_type;

    // initialized data
    node_graph *p_node_graph = (void *) 0;

    // Allocate a node graph
    if ( node_graph_create(&p_node_graph) == 0 ) goto failed_to_allocate_node_graph;

    // Parse the json object into a node graph
    {

        // initialized data
        dict *p_dict = p_value->object;
        json_value *p_nodes       = dict_get(p_dict, "nodes"),
                   *p_connections = dict_get(p_dict, "connections");

        // Missing properties
        if ( p_nodes == (void *) 0 ) goto missing_nodes_value;

        // Type check
        if ( p_nodes->type != JSON_VALUE_OBJECT ) goto wrong_nodes_type;

        // Parse the nodes
        {

            // initialized data
            dict *p_dict = p_nodes->object;
            size_t node_quantity = dict_keys(p_dict, 0);
            const char **pp_keys = (void *) 0; 

            // error check
            if ( node_quantity == (void *) 0 ) goto no_nodes;

            // Grow the allocation
            p_node_graph = default_allocator(p_node_graph, sizeof(node_graph) + (node_quantity * sizeof(node *)));

            // Store the node quantity
            p_node_graph->node_quantity = node_quantity;

            // Allocate memory for keys
            pp_keys = default_allocator(0, node_quantity * sizeof(node *));

            // Construct a lookup for nodes
            if ( dict_construct(&p_node_graph->p_nodes, node_quantity, 0) == 0 ) goto failed_to_construct_dict;

            // Get the keys
            if ( dict_keys(p_dict, pp_keys) == 0 ) goto failed_to_get_keys;

            // Construct each node
            for (size_t i = 0; i < node_quantity; i++)
            {
                
                // initialized data
                node *p_node = (void *) 0;
                const char *const p_key = pp_keys[i];
                json_value *p_node_value = dict_get(p_dict, p_key);

                // Construct the node
                if ( node_construct(&p_node, p_key, p_node_value, 0) == 0 ) goto failed_to_construct_node;

                // Store the node in the node graph
                p_node_graph->_p_nodes[i] = p_node;

                // Add the node to the dictionary
                dict_add(p_node_graph->p_nodes, p_node->_name, p_node);
            }
            
            // Release memory
            pp_keys = default_allocator(pp_keys, 0);
        }
        
        // Parse the connections
        {
            
            // initialized data
            array *p_array = p_connections->list;
            size_t connection_quantity = array_size(p_array);

            // Construct each connection
            for (size_t i = 0; i < connection_quantity; i++)
            {
                
                // initialized data
                json_value *p_connection = (void *) 0;
                
                // Store the connection
                array_index(p_array, i, (void **)&p_connection);

                // error check
                if ( p_connection->type != JSON_VALUE_ARRAY ) goto wrong_connection_type;

                // Parse the connection
                {

                    // initialized data
                    array *p_array = p_connection->list;
                    json_value *p_in  = (void *) 0,
                               *p_out = (void *) 0;

                    // error check
                    if ( array_size(p_array) > 2 ) goto too_many_connections;
                    if ( array_size(p_array) < 2 ) goto too_few_connections;

                    // Get the input and output
                    array_index(p_array, 0, (void **) &p_in);
                    array_index(p_array, 1, (void **) &p_out);

                    // error check
                    if ( p_in        == 0 ) goto no_input;
                    if ( p_out       == 0 ) goto no_output;

                    // Type check
                    if ( p_in->type  != JSON_VALUE_STRING ) goto input_wrong_type;
                    if ( p_out->type != JSON_VALUE_STRING ) goto output_wrong_type;
                    
                    // Parse the input and output
                    {
                        
                        // initialized data
                        node *p_node_in  = (void *) 0,
                             *p_node_out = (void *) 0;
                        char *in_node_text = p_in->string,
                             *in_connection_text = (void *) 0,
                             *out_node_text = p_out->string,
                             *out_connection_text = (void *) 0;
                        size_t j = 0, k = 0;

                        // Extract the input connection
                        in_connection_text = strchr(in_node_text, ':');

                        // Extract the output connection
                        out_connection_text = strchr(out_node_text, ':');

                        // error check
                        if ( in_connection_text  == (void *) 0 ) goto input_malformed;
                        if ( out_connection_text == (void *) 0 ) goto output_malformed;

                        // Split the string
                        *in_connection_text = '\0', *out_connection_text = '\0';

                        // Increment the cursor
                        in_connection_text++, out_connection_text++;

                        // Store the node
                        p_node_in  = dict_get(p_node_graph->p_nodes, in_node_text);
                        p_node_out = dict_get(p_node_graph->p_nodes, out_node_text);

                        // Make the connection from the input
                        {

                            // Iterate through each output in the input node
                            for (j = 0; j < p_node_in->out_quantity; j++)

                                // Find the corresponding connection
                                if ( strcmp(p_node_in->out[j]._name, in_connection_text) == 0 ) break;
                            
                            // Store the output node
                            p_node_in->out[j].p_out = p_node_out;

                            // Iterate through each input in the output node
                            for (k = 0; k < p_node_out->in_quantity; k++)

                                // Find the corresponding connection
                                if ( strcmp(p_node_out->in[k]._name, out_connection_text) == 0 ) break;
                            
                            // Store the index of the input connection
                            p_node_in->out[j].in_index = k;
                        }

                        // Make the connection to the output
                        {

                            // Iterate through each input in the output node
                            for (j = 0; j < p_node_out->in_quantity; j++)

                                // Find the corresponding connection
                                if ( strcmp(p_node_out->in[j]._name, out_connection_text) == 0 ) break;
                            
                            // Store the output node
                            p_node_out->in[j].p_in = p_node_in;

                            // Iterate through each output in the input node
                            for (k = 0; k < p_node_in->out_quantity; k++)

                                // Find the corresponding connection
                                if ( strcmp(p_node_in->out[k]._name, in_connection_text) == 0 ) break;
                            
                            // Store the index of the output connection
                            p_node_out->in[j].out_index = k;
                        }
                    }
                }
            }
        }
    }

    // return a pointer to the caller
    *pp_node_graph = p_node_graph;

    // success
    return 1;

    no_nodes:
    failed_to_construct_dict:
    failed_to_get_keys:
    missing_nodes_value:
    failed_to_construct_node:
    wrong_nodes_type:
    no_connections:
    wrong_connection_type:
    too_many_connections:
    too_few_connections:
    no_input:
    no_output:
    input_wrong_type:
    output_wrong_type:
    input_malformed:
    output_malformed:

        // error
        return 0;

    // error handling
    {
        
        // argument errors
        {
            no_node_graph:
                #ifndef NDEBUG
                    log_error("[node] Null pointer provided for parameter \"pp_node_graph\" in call to function \"%s\"\n", __FUNCTION__);
                #endif
                
                // error
                return 0;

            no_value:
                #ifndef NDEBUG
                    log_error("[node] Null pointer provided for parameter \"p_value\" in call to function \"%s\"\n", __FUNCTION__);
                #endif
                
                // error
                return 0;

            wrong_type:
                #ifndef NDEBUG
                    log_error("[node] Parameter \"p_value\" must be of type [ object ] in call to function \"%s\"\n", __FUNCTION__);
                #endif
                
                // error
                return 0;
        }
 
        // Node errors
        {
            failed_to_allocate_node_graph:
                #ifndef NDEBUG
                    log_error("[node] Failed to allocate node graph in call to function \"%s\"\n", __FUNCTION__);
                #endif
                
                // error
                return 0;
        }
    }
}

int node_construct ( node **pp_node, const char *const p_name, const json_value *const p_value, fn_node_data_constructor *pfn_node_data_constructor )
{

    // argument check
    if ( pp_node       ==        (void *) 0 ) goto no_node;
    if ( p_name        ==        (void *) 0 ) goto no_name;
    if ( p_value       ==        (void *) 0 ) goto no_value;
    if ( p_value->type != JSON_VALUE_OBJECT ) goto wrong_type;

    // initialized data
    node *p_node = (void *) 0;

    // Allocate a node
    if ( node_create(&p_node) == 0 ) goto failed_to_create_node;

    // Construct a node from a json value
    {

        // initialized data
        dict *p_dict = p_value->object;
        json_value *p_out  = dict_get(p_dict, "out"),
                   *p_in   = dict_get(p_dict, "in"),
                   *p_data = dict_get(p_dict, "data");
        
        // Set the name
        {
            size_t len = strlen(p_name);

            // Copy the string
            strncpy(p_node->_name, p_name, len);
        }

        // Set the out
        if ( p_out ) 
        {

            // initialized data
            array *p_array = p_out->list;
            size_t len = array_size(p_array);

            p_node->out_quantity = len;

            for (size_t i = 0; i < len; i++)
            {
                
                // initialized data
                json_value *i_value = (void *) 0;

                array_index(p_array, i, (void **)&i_value);

                strncpy(p_node->out[i]._name, i_value->string, 63);
            }
        }

        // Set the in
        if ( p_in ) 
        {

            // initialized data
            array *p_array = p_in->list;
            size_t len = array_size(p_array);

            p_node->in_quantity = len;

            for (size_t i = 0; i < len; i++)
            {
                
                // initialized data
                json_value *i_value = (void *) 0;

                array_index(p_array, i, (void **)&i_value);

                strncpy(p_node->in[i]._name, i_value->string, 63);
            }
        }

        // Store the node data
        p_node->value = p_data;
    }

    // return a pointer to the caller
    *pp_node = p_node;

    // success
    return 1;

    // error handling
    {

        // argument check
        {
            no_node:
                #ifndef NDEBUG
                    log_error("[node] Null pointer provided for parameter \"pp_node\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;

            no_name:
                #ifndef NDEBUG
                    log_error("[node] Null pointer provided for parameter \"p_name\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;

            no_value:
                #ifndef NDEBUG
                    log_error("[node] Null pointer provided for parameter \"p_value\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
            
            wrong_type:
                #ifndef NDEBUG
                    log_error("[node] Parameter \"p_value\" must be of type [ object ] in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }

        // Node errors
        {
            failed_to_create_node:
                #ifndef NDEBUG
                    log_error("[node] Failed to allocate memory in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
    }
}

int node_graph_print ( const node_graph *const p_node_graph )
{

    // argument check
    if ( p_node_graph == (void *) 0 ) goto no_node_graph;

    // Print the node graph
    log_info("=== node graph @ %p ===\n", p_node_graph);
    printf(" - nodes: \n");

    // Print each node 
    for (size_t i = 0; i < p_node_graph->node_quantity; i++)
    {

        // Print the name of the node
        printf("      - %s:\n", p_node_graph->_p_nodes[i]->_name);
        if ( p_node_graph->_p_nodes[i]->value )
        {
            printf("        - data: ");
            json_value_print(p_node_graph->_p_nodes[i]->value);
            putchar('\n');
        }
        
        if ( p_node_graph->_p_nodes[i]->out_quantity == 0 ) goto no_outputs;

        // Print the outputs
        log_info("        - out:\n");

        // Iterate through each output
        for (size_t j = 0; j < p_node_graph->_p_nodes[i]->out_quantity; j++)
        {
            
            log_info("           %s", p_node_graph->_p_nodes[i]->out[j]._name);
            // Print the output and input
            printf(" >>> ");
            log_error("%s:%s\n",
                //p_node_graph->_p_nodes[i]->_name,
                //p_node_graph->_p_nodes[i]->out[j]._name//,
                p_node_graph->_p_nodes[i]->out[j].p_out->_name,
                p_node_graph->_p_nodes[i]->out[j].p_out->in[p_node_graph->_p_nodes[i]->out[j].in_index]._name
            );
        }   
        
        no_outputs:

        // State check
        if ( p_node_graph->_p_nodes[i]->in_quantity == 0 ) goto no_inputs;

        // Print the inputs
        log_error("        - in:\n");

        // Iterate through each node
        for (size_t j = 0; j < p_node_graph->_p_nodes[i]->in_quantity; j++)
        {

            // Print the input and output
            log_error("           %s", p_node_graph->_p_nodes[i]->in[j]._name);

            printf(" <<< ");
            log_info("%s:%s\n",
                p_node_graph->_p_nodes[i]->in[j].p_in->_name,
                p_node_graph->_p_nodes[i]->in[j].p_in->out[p_node_graph->_p_nodes[i]->in[j].out_index]._name,
                p_node_graph->_p_nodes[i]->_name,
                p_node_graph->_p_nodes[i]->in[j]._name
            );
        }
        no_inputs:;
    }

    // success
    return 1;

    // error handling
    {

        // argument errors
        {
            no_node_graph:
                #ifndef NDEBUG
                    log_error("[node] Null pointer provided for parameter \"p_node_graph\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
    }
}
