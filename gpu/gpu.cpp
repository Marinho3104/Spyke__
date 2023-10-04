
/** INCLUDES **/
#include "gpu.h" // Gpu Impl.
#include "gpu_errors.h" // Gpu Error message

namespace spyke::gpu {

    Gpu_Properties gpu_properties;

}

bool spyke::gpu::initiate_gpu_properties( bool __print_error ) {

    // Catch errors
    cl_int _cl_status = 
        // Get platforms number
        clGetPlatformIDs(
            0,
            0, 
            &gpu_properties.number_platforms
        );

    // Check errors
    if ( _cl_status != CL_SUCCESS && __print_error ) 
        { spyke::gpu::errors::print_gpu_error( "initiate_gpu_properties", "clGetPlatformIDs", _cl_status ); return 0; }
    
    // Allocate memory for all platforms
    gpu_properties.platforms_properties = 
        ( Platform_Properties* ) malloc(sizeof(Platform_Properties) * gpu_properties.number_platforms );

    // Array with all platforms
    cl_platform_id platform_ids[ gpu_properties.number_platforms ];

    // Catch errors
    _cl_status = 
        // Set the platforms ids
        clGetPlatformIDs(
            gpu_properties.number_platforms, 
            platform_ids, 
            NULL
        );
    
    // Check errors
    if ( _cl_status != CL_SUCCESS && __print_error ) 
        { spyke::gpu::errors::print_gpu_error( "initiate_gpu_properties", "clGetPlatformIDs", _cl_status ); return 0; }

    // Set the correct platform data into the correct spot
    for ( int _ = 0; _ < gpu_properties.number_platforms; _++ ) gpu_properties.platforms_properties[ _ ].id = platform_ids[ _ ];

    // Set the devices for each platform
    for ( int _ = 0; _ < gpu_properties.number_platforms; _++ ) {

        // Catch errors
        _cl_status = 
            // Get the number of devices in platform
            clGetDeviceIDs(
                gpu_properties.platforms_properties[ _ ].id,
                CL_DEVICE_TYPE_GPU,
                0,
                0,
                &gpu_properties.platforms_properties[ _ ].number_devices
            );

        // Check for errors
        if ( _cl_status != CL_SUCCESS ) 
            { spyke::gpu::errors::print_gpu_error( "initiate_gpu_properties", "clGetDeviceIDs", _cl_status ); return 0; }

        // Allocate memory for all devices
        gpu_properties.platforms_properties[ _ ].device_ids = 
            ( cl_device_id* ) malloc( sizeof( cl_device_id ) * gpu_properties.platforms_properties[ _ ].number_devices );


        // Catch errors
        _cl_status = 
            // Set devices data into
            clGetDeviceIDs( 
                gpu_properties.platforms_properties[ _ ].id,
                CL_DEVICE_TYPE_GPU, 
                gpu_properties.platforms_properties[ _ ].number_devices, 
                gpu_properties.platforms_properties[ _ ].device_ids, 
                NULL
            );

        // Check for errors
        if ( _cl_status != CL_SUCCESS ) 
            { spyke::gpu::errors::print_gpu_error( "initiate_gpu_properties", "clGetDeviceIDs", _cl_status ); return 0; }



    }

    return 1;

}

cl_uint spyke::gpu::get_available_devices( cl_platform_id __platform_id, bool __print_error ) {

    // Number of devi ces
    cl_uint _number_devices;

    // Catch errors
    cl_int _cl_status = 
        // Gets the number of devices available
        clGetDeviceIDs(
            __platform_id,
            CL_DEVICE_TYPE_GPU,
            0,
            0,
            &_number_devices
        );

    // Check errors
    if ( _cl_status != CL_SUCCESS && __print_error ) 
        spyke::gpu::errors::print_gpu_error( "get_available_devices", "clGetDeviceIDs", _cl_status );
    
    return _cl_status != CL_SUCCESS ? 0 : _number_devices;

}

cl_uint spyke::gpu::get_available_platforms( bool __print_error ) {

    // Number of platforms
    cl_uint _number_platforms;

    // Catch errors
    cl_int _cl_status = 
        // Gets platforms ids
        clGetPlatformIDs(
            0, 
            0, 
            &_number_platforms
        );

    // Check errors
    if ( _cl_status != CL_SUCCESS && __print_error ) 
        spyke::gpu::errors::print_gpu_error( "get_available_platforms", "clGetPlatformIDs", _cl_status );
    
    return _cl_status != CL_SUCCESS ? 0 : _number_platforms;

}


