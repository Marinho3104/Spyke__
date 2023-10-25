

/** INCLUDES **/
#include "gpu_defines.h" 
#include "helper.h"

#include <iostream>
#include <CL/cl.h>

void spyke::helper::print_error_message( const char* __parent_function_name, const char* __function_name, const char* __information ) {

    // If is define the macro for printing the error
    #ifdef GPU_GPU_DEFINES_H_PRINT_ERROR_INFORMATION

        std::cout << "An error occur during execution of " << __parent_function_name << "() !" << std::endl;
        if ( __function_name ) std::cout << "\t During the execution of " << __function_name << "():" << std::endl;
        std::cout << "\t\t" << __information << std::endl;

    #endif

}

bool spyke::helper::check_handle_open_cl_api_errors( const char* __function_name, const char* __open_cl_api_function_name, int32_t __cl_status, char* __description ) {

    // Check for errors
    if ( __cl_status != CL_SUCCESS ) {

        // If is define the macro for printing the error
        #ifdef GPU_GPU_DEFINES_H_PRINT_ERROR_INFORMATION

            // Prin error message
            std::cout << __function_name << "(): An occur during " << __open_cl_api_function_name << "() api call with error: " << __cl_status << std::endl;

            // If additional data is set print it
            if ( __description ) std::cout << __description << std::endl;

        #endif

        return 1;

    }

    return 0;

}

void spyke::helper::get_file_content( char* __path, void** __data, size_t* __size ) {

    FILE* _file = 
        fopen( __path, "rb" );

    if ( ! _file ) { *__data = 0;  return; }

    fseek( _file, 0, SEEK_END );

    uint32_t _file_size = 
        ftell( _file );

    if ( __size ) *__size = _file_size;

    fseek( _file, 0, SEEK_SET );

    *__data = malloc( _file_size );
    
    fread(
        *__data,
        _file_size,
        1, _file
    );

    fclose( _file );

}
