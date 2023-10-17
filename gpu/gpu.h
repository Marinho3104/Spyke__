
/** GPU Related:
 *      Holds all data related to most abtract part of GPU like platforms, devices, etc ... 
**/

#ifndef GPU_GPU_H
#define GPU_GPU_H

/** INCLUDES **/
#include <CL/cl.h>
#include <cstdint>

namespace spyke::gpu {

    // Holds all platform information
    struct Platform_Data {

        // Platform Id
        cl_platform_id platform_id;
        
        // Count of all platform device ids
        uint32_t device_ids_count;

        // All platform device ids
        cl_device_id* device_ids; 

    };

    // Holds all arguments related to most abtract part of GPU like platforms, devices, etc ...
    struct Gpu_Data {

        // Count of all available platforms
        uint32_t count_platform_datas;

        // All available platforms
        Platform_Data* platform_datas;

    }; 

    // Global variable holding GPU most abstract data
    extern Gpu_Data gpu_data;

    // Initial setup to get all GPU abstract data and set it into the global variable
    // @param __debug If true prints error messages
    // @return True if all operations work successfully or Fale if not
    bool setup();

    // Sets all available platforms
    // @param __debug If true prints error messages
    // @return True if all operations work successfully or Fale if not
    bool set_platforms();

    // Sets ids for all available platforms
    // @param __debug If true prints error messages
    // @return True if all operations work successfully or Fale if not
    bool set_device_ids();

    // Checks and handles related open cl api calls errors
    // @param __function_name Function name where the open cl api call was executed
    // @param __open_cl_api_function_name Open cl function that was executed
    // @param __cl_status Open cl api function call status result
    // @return True if an error occur during the calls, or False if not
    bool check_handle_errors( const char*, const char*, int32_t );

}

#endif

