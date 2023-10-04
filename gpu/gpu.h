
/** Gpu related functions: 
 *      Holds all variables and functions related to GPU only
*/

#ifndef GPU_GPU_H
#define GPU_GPU_H

/** INCLUDES **/
#include <CL/cl.h> // OpenCl

namespace spyke::gpu {

    // Holds all properties about a specific platform
    struct Platform_Properties {

        // Platform id
        cl_platform_id id;

        // Devices information
        cl_device_id* device_ids; cl_uint number_devices;

    };
    
    // Holds all data related to the GPU's 
    struct Gpu_Properties {

        // Platforms information
        Platform_Properties* platforms_properties; cl_uint number_platforms;

    };

    extern Gpu_Properties gpu_properties;

    // Initiate GPU properties, is mandatory to call this function before any gpu action
    // @param __print_error If an error occur during the OpenCl Api calls it will be printed
    bool initiate_gpu_properties( bool __print_error = 0 );

    // Returns the number of devices available for a specific platform ( GPU's only )
    // @param __device_id Platform id where the devices will be count 
    // @param __print_error If an error occur during the OpenCl Api calls it will be printed
    cl_uint get_available_devices( cl_platform_id, bool __print_error = 0 );

    // Returns the current number of available platforms ( versions of OpenCl available to use ( OpenCl-nvidia/OpenCl-Amd ... ) )
    // @param __print_error If an error occur during the OpenCl Api calls it will be printed
    cl_uint get_available_platforms( bool __print_error = 0 );

}

#endif