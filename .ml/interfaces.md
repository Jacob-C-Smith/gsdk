fn_allocator
   - Description: A function pointer for custom memory allocation. It takes a pointer and a size, and returns a new pointer to the allocated memory.
   - Usage:
       - array: Used in array_map to optionally release elements after applying a mapping function.


  fn_comparator
   - Description: A function pointer used for comparing two elements, typically for sorting. It returns an integer less than, equal to, or greater than zero if the first
     argument is considered to be respectively less than, equal to, or greater than the second.
   - Usage:
       - array: Used in array_sort to sort the elements of an array.


  fn_equality
   - Description: A function pointer that checks for equality between two elements.
   - Usage:
       - cache: Used in cache_find and cache_remove to find elements by key.
       - hash_table: Used in hash_table_search to find elements by key.


  fn_key_accessor
   - Description: A function pointer that retrieves a key from a given value.
   - Usage:
       - cache: Used in conjunction with fn_equality to get the key from an element for comparison in cache_find and cache_remove.
       - hash_table: Used in hash_table_insert to get the key from a property.
       - tree: Used in binary_tree_search and binary_tree_insert to extract the key from a value for comparison.


  fn_hash64
   - Description: A function pointer that computes a 64-bit hash for a given key.
   - Usage:
       - dict: Used in dict_add and dict_get to determine the bucket for a key-value pair.
       - hash_table: Used in hash_table_insert and hash_table_search to compute the hash of a key.


  fn_fori
   - Description: A function pointer for iterating over a collection with an index.
   - Usage:
       - array: Used in array_fori to apply a function to each element with its index.
       - cache: Used in cache_fori to apply a function to each element with its index.


  fn_foreach
   - Description: A function pointer for iterating over a collection.
   - Usage:
       - cache: Used in cache_for_each to apply a function to each element.
       - dict: Used in dict_foreach to apply a function to each value in the dictionary.
       - set: Used in set_foreach_i to apply a function to each element.
       - tuple: Used in tuple_foreach to apply a function to each element.


  fn_map
   - Description: A function pointer that applies a transformation to an element and returns the transformed element.
   - Usage:
       - array: Used in array_map to apply a function to each element.
       - cache: Used in cache_map to apply a function to each element.


  fn_pack
   - Description: A function pointer for serializing a value into a buffer.
   - Usage:
       - array: Used in array_pack to serialize array elements.
       - bitmap: Used in bitmap_pack to serialize the bitmap.
       - cache: Used in cache_pack to serialize cache elements.
       - dict: Used in dict_pack to serialize dictionary elements.
       - tuple: Used in tuple_pack to serialize tuple elements.


  fn_unpack
   - Description: A function pointer for deserializing a value from a buffer.
   - Usage:
       - array: Used in array_unpack to deserialize array elements.
       - bitmap: Used in bitmap_unpack to deserialize the bitmap.
       - cache: Used in cache_unpack to deserialize cache elements.
       - dict: Used in dict_unpack to deserialize dictionary elements.
       - tuple: Used in tuple_unpack to deserialize tuple elements.