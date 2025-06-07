#pragma once

#include <pack/pack.h>

typedef struct { float x, y, z; } vec3;

typedef struct { vec3 loc, rot, sca; } transform;

void vector_init ( void ) __attribute__((constructor));

int  vec3_pack   ( void *p_buffer, const void *const p_data );
int  vec3_unpack ( void *p_data, void *p_buffer );
void vec3_print  ( vec3 _vec3 );

int  transform_pack   ( void *p_buffer, const void *const p_data );
int  transform_unpack ( void *p_data, void *p_buffer );
void transform_print  ( transform _transform );

void vector_exit ( void ) __attribute__((destructor));
