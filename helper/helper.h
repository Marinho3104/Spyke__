
/** Helper:
 *      Utilitary functions used globally thtrought out the all code 
*/


#ifndef HELPER_HELPER_H
#define HELPER_HELPER_H

/** INCLUDES **/
#include <iostream>
#include <cstdint>

namespace spyke::helper {

    // Prints a general error message format
    void print_error_message( const char*, const char*, const char* );

    // Checks and handles related open cl api calls errors
    // @param __function_name Function name where the open cl api call was executed
    // @param __open_cl_api_function_name Open cl function that was executed
    // @param __cl_status Open cl api function call status result
    // @param __description If not null, prints additionally, can be null
    // @return True if an error occur during the calls, or False if not
    bool check_handle_open_cl_api_errors( const char*, const char*, int32_t, char* __description = 0 );

    // Sets the content of a given file into a given pointer and the respective length.
    // If an error occur the pointing variable will be set to 0 ( NULL )
    // @param __path File path
    // @param __data Data where the file content should be set to, is allocated by this function
    // @param __size If not null is set to the file content size, can be null
    void get_file_content( char*, void**, size_t* );

}

#endif