#include <core/certificate.h>

#include <errno.h>
#include <core/log.h>

size_t load_file ( const char *path, void *buffer, bool binary_mode );

/** !
 * 
 * 
 * 
 * 
 */
int key_pair_parse ( certificate **pp_certificate, const char *p_path )
{

    // Unused
    (void) pp_certificate;
    // argument check
    // if ( NULL == pp_certificate );

    // initialized data
    size_t  len    = load_file(p_path, 0, 0);
    char   *p_data = malloc(len);

    // Load the file
    load_file(p_path, p_data, 0);

    // success
    return 1;
}

size_t load_file ( const char *path, void *buffer, bool binary_mode )
{

    // argument checking 
    if ( path == 0 ) goto no_path;

    // initialized data
    size_t  ret = 0;
    FILE   *f   = fopen(path, (binary_mode) ? "rb" : "r");
    
    // Check if file is valid
    if ( f == NULL ) goto invalid_file;

    // Find file size and prep for read
    fseek(f, 0, SEEK_END);
    ret = (size_t) ftell(f);
    fseek(f, 0, SEEK_SET);
    
    // Read to data
    if ( buffer ) 
        ret = fread(buffer, 1, ret, f);

    // The file is no longer needed
    fclose(f);
    
    // success
    return ret;

    // error handling
    {

        // argument errors
        {
            no_path:
                #ifndef NDEBUG
                    printf("[json] Null path provided to function \"%s\n", __FUNCTION__);
                #endif

            // error
            return 0;
        }

        // File errors
        {
            invalid_file:
                #ifndef NDEBUG
                    printf("[Standard library] Failed to load file \"%s\". %s\n", path, strerror(errno));
                #endif

            // error
            return 0;
        }
    }
}
