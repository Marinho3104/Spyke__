
/** INCLUDES **/
#include "gpu_defines.h"
#include "gpu.h"

#include <iostream>


bool spyke::gpu::setup( bool __debug ) {

    return 
        // Sets the platforms
        set_platforms( __debug ) &&
        // Sets device ids 
        set_device_ids( __debug );

}

bool spyke::gpu::set_platforms( bool __debug ) {

    // Catch errors
    cl_int _cl_status;
    
    // Get platforms count
    _cl_status = 
        clGetPlatformIDs(
            0,
            0, 
            &gpu_data.count_platform_datas
        );

}

bool spyke::gpu::set_device_ids( bool __debug ) {



}

bool spyke::gpu::check_handle_errors( const char* __function_name, const char* __open_cl_api_function_name, int32_t __cl_status ) {

    // Check for errors
    if ( __cl_status != CL_SUCCESS ) {

        // If is define the macro for printing the error
        #ifdef GPU_GPU_DEFINES_H_PRINT_ERROR_INFORMATION

            std::cout << "Error" << std::endl;

        #endif

        return 0;

    }

    return 1;

}
