
/** INCLUDES **/
#include "opencl.h"
#include "helper.h"

bool spyke::opencl::set_context( cl_context* __context, cl_device_id* __device_ids, uint64_t __device_ids_count, const char* __information ) {

    // Catch errors
    cl_int _cl_status;

    // Creates and allocate memory for the context
    *__context = 
        clCreateContext( 
            NULL, 
            __device_ids_count, 
            __device_ids, 
            NULL, 
            NULL, 
            &_cl_status
        );

    return ! spyke::helper::check_handle_open_cl_api_errors( __information, "clCreateContext", _cl_status );

}

bool spyke::opencl::set_command_queue( cl_command_queue* __command_queue, cl_context __context, cl_device_id __device_id, uint64_t __command_queue_properties, const char* __information ) {

    // Catch errors
    cl_int _cl_status;

    *__command_queue = clCreateCommandQueue(
        __context, 
        __device_id, 
        __command_queue_properties, 
        &_cl_status
    );

    return ! spyke::helper::check_handle_open_cl_api_errors( __information, "clCreateCommandQueue", _cl_status );

}

bool spyke::opencl::set_kernel( 
        cl_kernel* __kernel, 
        cl_context __context, 
        cl_device_id* __device_ids, 
        uint64_t __device_ids_count, 
        char** __function_file_names, 
        uint64_t __function_file_names_count,
        char* __entry_point,
        const char* __information
    ) {

        // Catch errors
        cl_int _cl_status;

        // Kernel functions pointers and respective lengths
        const char* _kernel_functions
            [ __function_file_names_count ] = { 0 };
        size_t _kernel_functions_lenghts
            [ __function_file_names_count ] = { 0 };

        // Loops throught each file and sets the function body and lenghts
        for ( size_t _ = 0; _ < __function_file_names_count; _++ ) {

            // Get the file content
            spyke::helper::get_file_content( __function_file_names[ _ ], ( void** ) _kernel_functions + _, _kernel_functions_lenghts + _ );

            // Error occur, probably cause the path to a file is wrong
            if ( ! _kernel_functions[ _ ] ) { 

                spyke::helper::print_error_message( 
                    __information, 
                    "get_file_content", 
                    "Given file path is wrong" 
                ); 

                return 0; 
            
            }

        }

        // Create the program
        cl_program _program = 
            clCreateProgramWithSource(
                __context,
                __function_file_names_count,
                _kernel_functions,
                _kernel_functions_lenghts,
                &_cl_status
            );

        // Check for error
        if ( spyke::helper::check_handle_open_cl_api_errors( __information, "clCreateProgramWithSource", _cl_status ) ) return 0;
        
        // Compile and link all functions
        _cl_status = 
            clBuildProgram(
                _program,
                __device_ids_count,
                __device_ids,
                0, 0, 0
            );

        // Get the build log of previous function to be added as addition data to the check function
        char _build_log[12000];
        cl_int _cl_status_temp = clGetProgramBuildInfo(
            _program, 
            *__device_ids, 
            CL_PROGRAM_BUILD_LOG, 
            sizeof( _build_log ), 
            _build_log, 
            0
        );
    
        // Check for error
        if ( spyke::helper::check_handle_open_cl_api_errors( __information, "clGetProgramBuildInfo", _cl_status_temp ) ) return 0;
    
        // Check for error
        if ( spyke::helper::check_handle_open_cl_api_errors( __information, "clBuildProgram", _cl_status, _build_log ) ) return 0;
        
        // Create and sets the kernel object
        *__kernel = 
            clCreateKernel(
                _program,
                __entry_point,
                &_cl_status
            );

        // Check for error
        return ! spyke::helper::check_handle_open_cl_api_errors( __information, "clCreateKernel", _cl_status, _build_log );
        
}

bool spyke::opencl::set_buffer( cl_mem* __memory, cl_context __context, cl_mem_flags __memory_flags, size_t __memory_size, const char* __information ) {

    // Catch errors
    cl_int _cl_status;

    // Creates and sets the buffer
    *__memory = 
        clCreateBuffer(
            __context,
            __memory_flags,
            __memory_size,
            0,
            &_cl_status
        );
    
    // Check for error
    return ! spyke::helper::check_handle_open_cl_api_errors( __information, "clCreateBuffer", _cl_status );
    
}

bool spyke::opencl::set_kernel_argument( cl_kernel __kernel, uint32_t __kernel_argument_index, size_t __argument_size, void* __argument_value, const char* __information ) {

    // Set the kernel argument
    cl_int _cl_status = 
        clSetKernelArg(
            __kernel,
            __kernel_argument_index,
            __argument_size,
            __argument_value
        );

    // Check for error
    return ! spyke::helper::check_handle_open_cl_api_errors( __information, "spyke::opencl::set_kernel_argument", _cl_status );
  
}

bool spyke::opencl::launch_kernel( cl_command_queue __command_queue, cl_kernel __kernel, size_t __global_size, size_t __local_size, const char* __information ) {

    // Launch the kernel
    cl_int _cl_status = 
        clEnqueueNDRangeKernel(
            __command_queue,
            __kernel,
            1, 0,
            &__global_size,
            &__local_size,
            0, 0, 0 
        );

    // Check for error
    return ! spyke::helper::check_handle_open_cl_api_errors( __information, "clEnqueueNDRangeKernel", _cl_status );
    
}

bool spyke::opencl::launch_kernel( cl_command_queue __command_queue, cl_kernel __kernel, const char* __information ) {

    // Launch the kernel
    cl_int _cl_status = 
        clEnqueueTask(
            __command_queue,
            __kernel,
            0, 0, 0 
        );

    // Check for error
    return ! spyke::helper::check_handle_open_cl_api_errors( __information, "clEnqueueNDRangeKernel", _cl_status );

}
