
/** INCLUDES **/
#include "helper.h"
#include "gpu.h"

#include <iostream>

namespace spyke::gpu {

    Gpu_Data gpu_data;

}

bool spyke::gpu::setup() {

    return 
        // Sets the platforms
        set_platforms() &&
        // Sets device ids 
        set_device_ids();

}

bool spyke::gpu::set_platforms() {

    // Catch errors
    cl_int _cl_status;
    
    // Get platforms count
    _cl_status = 
        clGetPlatformIDs(
            0,
            0, 
            &gpu_data.count_platform_datas
        );

    // Check for error
    if ( spyke::helper::check_handle_open_cl_api_errors( "spyke::gpu::set_platforms", "clGetPlatformIDs", _cl_status ) ) return 0;

    // Temporary stack memory for holding all data related to platform 
    cl_platform_id platform_ids[ gpu_data.count_platform_datas ];

    // Set platform ids
    _cl_status = 
        clGetPlatformIDs(
            gpu_data.count_platform_datas, 
            platform_ids, 
            NULL
        );

    // Check for error
    if ( spyke::helper::check_handle_open_cl_api_errors( "spyke::gpu::set_platforms", "clGetPlatformIDs", _cl_status ) ) return 0;

    // Allocate memory for all available platforms
    gpu_data.platform_datas = 
        ( spyke::gpu::Platform_Data* ) malloc( sizeof( spyke::gpu::Platform_Data ) * gpu_data.count_platform_datas );

    // Loop throught each platform id to make a copy
    for ( uint32_t _ = 0; _ < gpu_data.count_platform_datas; _++ ) 

        // Copies each platform id to target spot
        gpu_data.platform_datas[ _ ].platform_id = platform_ids[ _ ];

    // Every thing work fine
    return 1;

}

bool spyke::gpu::set_device_ids() {

    // Catch errors
    cl_int _cl_status;

    // Loop throught each platform id
    for ( uint32_t _ = 0; _ < gpu_data.count_platform_datas; _++ ) {

        // get count of devices in current platform
        _cl_status = 
            clGetDeviceIDs(
                gpu_data.platform_datas[ _ ].platform_id,
                CL_DEVICE_TYPE_GPU,
                0,
                0,
                &gpu_data.platform_datas[ _ ].device_ids_count
            );

        // Check for error
        if ( spyke::helper::check_handle_open_cl_api_errors( "spyke::gpu::set_device_ids", "clGetDeviceIDs", _cl_status ) ) return 0;

        // allocates memory for all device ids
        gpu_data.platform_datas[ _ ].device_ids = 
            ( cl_device_id* ) malloc( sizeof( cl_device_id ) * gpu_data.platform_datas[ _ ].device_ids_count );

        // Sets the device ids of current platform 
        _cl_status = 
            clGetDeviceIDs( 
                gpu_data.platform_datas[ _ ].platform_id,
                CL_DEVICE_TYPE_GPU, 
                gpu_data.platform_datas[ _ ].device_ids_count, 
                gpu_data.platform_datas[ _ ].device_ids, 
                NULL
            );

        // Check for error
        if ( spyke::helper::check_handle_open_cl_api_errors( "spyke::gpu::set_device_ids", "clGetDeviceIDs", _cl_status ) ) return 0;

    }

    // Every thing work fine
    return 1;

}


