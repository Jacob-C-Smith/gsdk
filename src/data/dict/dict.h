/** !
 * Include header for dictionary library
 * 
 * @file dict/dict.h 
 * 
 * @author Jacob Smith
 */

// header guard
#pragma once

#undef NDEBUG

// standard library
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

// core
#include <core/hash.h>

// reflection
#include <core/pack.h>

// forward declarations
struct dict_item_s;
struct dict_s;

// type definitions
/** !
 *  @brief The type definition of a dictionary struct
 */
typedef struct dict_s dict;

// initializer
/** !
 * This gets called at runtime before main. 
 * 
 * @param void
 * 
 * @return void
 */
void dict_init ( void ) __attribute__((constructor));

// constructors
/** !
 *  Construct a dictionary with a specific number of hash table entries
 *
 * @param pp_dict           return
 * @param size              number of hash table entries. 
 * @param pfn_hash_function pointer to a hash function, or 0 for default
 *
 * @sa dict_create
 * @sa dict_destroy
 *
 * @return 1 on success, 0 on error
 */
int dict_construct ( dict **const pp_dict, size_t size, fn_hash64 pfn_hash_function );

/** !
 *  Construct a dictionary from an array of strings
 *
 * @param pp_dict return
 * @param keys    pointer to null terminated array of strings
 * @param size    number of hash table entries. 
 *
 * @sa dict_create
 * @sa dict_destroy
 *
 * @return 1 on success, 0 on error
 */
int dict_from_keys ( dict **const pp_dict, const char **const keys, size_t size );

// accessors
/** !
 *  Get a property's value
 *
 * @param p_dict dictionary
 * @param key    the name of the property
 *
 * @sa dict_values
 * @sa dict_keys
 *
 * @return pointer to specified property's value on success, null pointer on error
 */
const void *dict_get ( dict *const p_dict, const char *const key );

/** !
 *  Get a dictionarys' values, or the number of properties in the dictionary
 *
 * @param p_dict dictionary
 * @param values return -OR- null pointer
 *
 * @sa dict_get
 * @sa dict_keys
 *
 * @return 1 on success, 0 on error, if values != null, else number of properties in dictionary
 */
size_t dict_values ( dict *const p_dict, void **const values );

/** !
 *  Get a dictionarys' keys, or the number of properties in the dictionary
 *
 * @param p_dict dictionary
 * @param keys   return -OR- null pointer
 *
 * @sa dict_get
 * @sa dict_values
 *
 * @return 1 on success, 0 on error, if keys != null, else number of properties in dictionary
 */
size_t dict_keys ( dict *const p_dict, const char **const keys );

// mutators
/** !
 *  Add a property to a dictionary. 
 *
 * @param p_dict  dictionary
 * @param key     the name of the property
 * @param p_value the value of the property
 *
 * @sa dict_pop
 *
 * @return 1 on success, 0 on error
 */
int dict_add ( dict *const p_dict, const char *const key,  void * const p_value );

/** !
 *  Remove a property from a dictionary. 
 *
 * @param p_dict   dictionary
 * @param key      the name of the property
 * @param pp_value return
 *
 * @sa dict_add
 *
 * @return 1 on success, 0 on error
 */
int dict_pop ( dict *const p_dict, const char *const key, const void **const pp_value );

// iterators
/** !
 *  Call a function on each value in a dictionary
 * 
 * @param p_dict dictionary
 * @param function the function to call. 
 * 
 * @return 1 on success, 0 on error
*/
int dict_foreach ( dict *const p_dict, void (*function)(const void *const, size_t i) );

// Shallow copy
/** !
 *  Make a shallow copy of a dictionary
 *
 * @param p_dict  source dictionary
 * @param pp_dict return
 *
 * @sa dict_clear
 *
 * @return 1 on success, 0 on error
 */
int dict_copy ( dict *const p_dict, dict **const pp_dict );

// reflection
/** !
 * Pack a dict into a buffer
 * 
 * @param p_dict       the dict
 * @param p_buffer     the buffer
 * @param pfn_elemenet pointer to pack function IF not null ELSE default
 * 
 * @return 1 on success, 0 on error
 */
int dict_pack ( dict *p_dict, void *p_buffer, fn_pack *pfn_element );

/** !
 * Unpack a buffer into a dict
 * 
 * @param pp_dict      result
 * @param p_buffer     the buffer
 * @param pfn_elemenet pointer to unpack function IF not null ELSE default
 * 
 * @return 1 on success, 0 on error
 */
int dict_unpack ( dict **pp_dict, void *p_buffer, fn_unpack *pfn_element );

// Clear all items
/** !
 *  Remove all properties from a dictionary
 *
 * @param p_dict dictionary
 *
 * @sa dict_copy
 *
 * @return 1 on success, 0 on error
 */
int dict_clear ( dict *const p_dict );

/** !
 *  Remove all properties from a dictionary, and deallocate values with free_func
 *
 * @param p_dict       dictionary
 * @param free_fun_ptr pointer to deallocator function 
 * 
 * @sa dict_clear
 * @sa dict_copy
 *
 * @return 1 on success, 0 on error
 */
int dict_free_clear ( dict *const p_dict, void (*const free_func)(const void *const) );

// destructors
/** !
 *  Destroy and deallocate a dictionary
 *
 * @param pp_dict dictionary
 *
 * @sa dict_create
 *
 * @return 1 on success, 0 on error
 */
int dict_destroy ( dict **const pp_dict, fn_allocator *pfn_allocator );
