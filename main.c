/** !
 * GSDK demo program
 * 
 * @file main.c
 * 
 * @author [whoami]
 */

// standard library
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

// gsdk
#include <gsdk.h>
#include <core/log.h>

// Entry point
int main ( int argc, const char *argv[] )
{

    // Print the GSDK version
    log_info(
        "GSDK version %d.%d.%d\n",
        GSDK_VERSION_MAJOR,
        GSDK_VERSION_MINOR,
        GSDK_VERSION_PATCH
    );

    // Success
    return EXIT_SUCCESS;
}
