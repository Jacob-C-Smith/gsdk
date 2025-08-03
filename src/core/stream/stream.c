/** ! 
 * Stream implementation
 * 
 * @file src/core/stream.c
 * 
 * @author Jacob Smith
 */

// standard library
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <limits.h>
#include <time.h>

// core
#include <core/log.h>
#include <core/stream.h>

// function definitions
int stream_from_path ( stream **pp_stream, const char *p_path )
{

    // argument check
    if ( NULL == p_stream ) goto no_stream;
    if ( NULL == p_path )   goto no_path;

    // initialized data
    stream
}

int stream_from_file ( stream **p_stream, FILE *p_file );

int stream_from_directory ( stream **p_stream, DIR *p_directory );

int stream_from_memory ( stream **p_stream, void *p_buffer, size_t size );

int stream_from_tcp_socket ( stream **p_stream, socket_tcp *p_socket );

int stream_from_pipe ( stream **p_stream, int p_pipe );

int stream_destroy ( stream **pp_stream );