

/** INCLUDES **/
#include <iostream>
#include <string.h>

#include "transaction_management.h"
#include "transaction_pool.h"
#include "gpu_errors.h"
#include "helper.h"

namespace spyke::transaction_management {

    Transaction_Management_GPU transaction_management_gpu;

    /** Transaction Request Confirmation kernel **/

    Arguments_Transaction_Request_Confirmation_Kernel_Entry_Point* arguments_transaction_request_confirmation_kernel_entry_point = 
        ( Arguments_Transaction_Request_Confirmation_Kernel_Entry_Point* ) 
            malloc( sizeof( Arguments_Transaction_Request_Confirmation_Kernel_Entry_Point ) );

    // Paths to transaction_request confirmation kernel functions
    char* transaction_request_confirmation_kernel_functions_path
        [ TRANSACTION_MANAGEMENT_TRANSACTION_MANAGEMENT_TRANSACTION_REQUEST_CONFIRMATION_KERNEL_FUNCTIONS_COUNT ] = 
            {

                "./transaction_request_confirmation_kernel.spyke"

            },
        *transaction_request_confirmation_entry_point_kernel_function = 
            "transaction_request_confirmation_kernel";
    
}


spyke::transaction_management::Arguments_Transaction_Request_Confirmation_Kernel_Entry_Point::Arguments_Transaction_Request_Confirmation_Kernel_Entry_Point( cl_context __context, bool __print_error ) {

    // Set the ready var to 0, so if an error occur during the constructor execution it can be detected by others
    ready = 0;

    // Status 
    cl_int _cl_status;

    // Allocate memory for is_data_available
    is_data_available = clCreateBuffer(__context, CL_MEM_READ_WRITE, sizeof( bool ), NULL, &_cl_status);
    
    // Check for errors
    if ( _cl_status != CL_SUCCESS && __print_error ) 
        { spyke::gpu::errors::print_gpu_error( "Arguments_Transaction_Request_Confirmation_Kernel_Entry_Point", "clCreateBuffer", _cl_status ); return; }

    // Everything is fine and the data is ready to be used
    ready = 1;

}

bool spyke::transaction_management::Arguments_Transaction_Request_Confirmation_Kernel_Entry_Point::initialize_arguments( cl_command_queue __command_queue, bool __print_error ) {

    // Status 
    cl_int _cl_status;

    // Write into is_data_available
    bool _is_data_available = 1;
    _cl_status = clEnqueueWriteBuffer(
        __command_queue,
        is_data_available,
        CL_TRUE,
        0,
        sizeof( bool ),
        &_is_data_available,
        0,
        0,
        0
    );

    // Check for errors
    if ( _cl_status != CL_SUCCESS && __print_error ) 
        { spyke::gpu::errors::print_gpu_error( "initialize_arguments", "clEnqueueWriteBuffer", _cl_status ); return 0; }

    return 1;

}


bool spyke::transaction_management::setup_gpu( Transaction_Management_Setup_Properties __setup_properties ) {

    // Status 
    cl_int _cl_status;

    // Create the context for transaction management
    transaction_management_gpu.transaction_management_context = clCreateContext( 
        0,
        __setup_properties.number_devices,
        __setup_properties.devices,
        0,
        0,
        &_cl_status
    );

    // Check for errors
    if ( _cl_status != CL_SUCCESS && __setup_properties.print_error ) 
        { spyke::gpu::errors::print_gpu_error( "setup_gpu", "clCreateContext", _cl_status ); return 0; }

    // Mutual allocations

    // Transaction pool allocation
    if ( 
        ! spyke::transaction_management::transaction_pool::allocate_memory_pool(
            __setup_properties.transactions_transaction_pool,
            __setup_properties.print_error
        )
    ) return 0;

    // Miner 
    if (
        __setup_properties.type == TRANSACTION_MANAGEMENT_TRANSACTION_MANAGEMENT_MINER_SETUP
    ) {

        // Transaction Request //

        // Allocate the Transaction Request Kernel entry point arguments
        new ( arguments_transaction_request_confirmation_kernel_entry_point )
            Arguments_Transaction_Request_Confirmation_Kernel_Entry_Point(
                transaction_management_gpu.transaction_management_context,
                __setup_properties.print_error
            );

        // Check for errors
        if ( ! arguments_transaction_request_confirmation_kernel_entry_point->ready && __setup_properties.print_error ) 
            { std::cout << "An error occur during the allocation of the transaction request confirmation kernel arguments" << std::endl; return 0; }

        // Create a command queue for transaction request threads
        transaction_management_gpu.command_queue_transaction_request_confirmation_threads = 
            clCreateCommandQueue(
                transaction_management_gpu.transaction_management_context, 
                __setup_properties.command_queue_transaction_request_confirmation_threads_device[ 0 ], 
                0, 
                &_cl_status
            );

        // Check for errors
        if ( _cl_status != CL_SUCCESS && __setup_properties.print_error ) 
            { spyke::gpu::errors::print_gpu_error( "setup_gpu", "clCreateCommandQueue", _cl_status ); return 0; }

        // Initialize the tranaction request kernel function entry point arguments 
        if ( 
            ! arguments_transaction_request_confirmation_kernel_entry_point->initialize_arguments(
                transaction_management_gpu.command_queue_transaction_request_confirmation_threads,
                __setup_properties.print_error
            ) && __setup_properties.print_error 
        ) { std::cout << "An error occur during the intialization of the transaction request confirmation kernel arguments" << std::endl; return 0; }

        // Functions bodies
        char* transaction_request_confirmation_functions
            [ TRANSACTION_MANAGEMENT_TRANSACTION_MANAGEMENT_TRANSACTION_REQUEST_CONFIRMATION_KERNEL_FUNCTIONS_COUNT ];
        size_t transaction_request_confirmation_functions_lengths
            [ TRANSACTION_MANAGEMENT_TRANSACTION_MANAGEMENT_TRANSACTION_REQUEST_CONFIRMATION_KERNEL_FUNCTIONS_COUNT ];
        
        // Write the functions into the array
        for ( int _ = 0; _ < TRANSACTION_MANAGEMENT_TRANSACTION_MANAGEMENT_TRANSACTION_REQUEST_CONFIRMATION_KERNEL_FUNCTIONS_COUNT; _++ ) {

            transaction_request_confirmation_functions[ _ ] = 
                ( char* ) spyke::helper::get_file_content( 
                    transaction_request_confirmation_kernel_functions_path[ _ ],  
                    transaction_request_confirmation_functions_lengths + _
                );

        }

        // Create a program with transaction_request confirmation thread fucntions
        cl_program transaction_request_confirmation_program = 
            clCreateProgramWithSource(
                transaction_management_gpu.transaction_management_context,
                TRANSACTION_MANAGEMENT_TRANSACTION_MANAGEMENT_TRANSACTION_REQUEST_CONFIRMATION_KERNEL_FUNCTIONS_COUNT,
                ( const char** ) transaction_request_confirmation_functions,
                transaction_request_confirmation_functions_lengths, 
                &_cl_status
            );
        
        // Check for errors
        if ( _cl_status != CL_SUCCESS && __setup_properties.print_error ) 
            { spyke::gpu::errors::print_gpu_error( "setup_gpu", "clCreateProgramWithSource", _cl_status ); return 0; }

        // Compile and link the function 
        _cl_status = 
            clBuildProgram(
                transaction_request_confirmation_program, 
                1, 
                __setup_properties.command_queue_transaction_request_confirmation_threads_device, 
                NULL, 
                NULL, 
                NULL
            );
        
        // Check for errors
        if ( _cl_status != CL_SUCCESS && __setup_properties.print_error ) { 

                spyke::gpu::errors::print_gpu_error( "setup_gpu", "clBuildProgram", _cl_status );

                // Get the build log
                char buildLog[4096];
                size_t logSize;
                clGetProgramBuildInfo(transaction_request_confirmation_program, *__setup_properties.command_queue_transaction_request_confirmation_threads_device, CL_PROGRAM_BUILD_LOG, sizeof(buildLog), buildLog, &logSize);

                // Print the build log
                printf("Build Log:\n%s\n", buildLog);

                return 0;
            }

        // Create the kernel
        cl_kernel kernel = 
            clCreateKernel(
                transaction_request_confirmation_program, 
                transaction_request_confirmation_entry_point_kernel_function, 
                &_cl_status
            );

        // Check for errors
        if ( _cl_status != CL_SUCCESS && __setup_properties.print_error ) 
            { spyke::gpu::errors::print_gpu_error( "setup_gpu", "clCreateKernel", _cl_status ); return 0; }

        // Set the arguments for the kernel
        _cl_status = 
            clSetKernelArg(
                kernel, 
                0, 
                sizeof(cl_mem), 
                &arguments_transaction_request_confirmation_kernel_entry_point->is_data_available
            );

        // Check for errors
        if ( _cl_status != CL_SUCCESS && __setup_properties.print_error ) 
            { spyke::gpu::errors::print_gpu_error( "setup_gpu", "clSetKernelArg", _cl_status ); return 0; }

        // Launch the kernel in command queue
        _cl_status = clEnqueueNDRangeKernel(
            transaction_management_gpu.command_queue_transaction_request_confirmation_threads,
            kernel,
            1,
            0,
            &__setup_properties.transaction_request_confirmation_threads_global_threads_count,
            &__setup_properties.transaction_request_confirmation_threads_local_threads_count,
            0,
            0,
            0
        );

        // Check for errors
        if ( _cl_status != CL_SUCCESS && __setup_properties.print_error ) 
            { spyke::gpu::errors::print_gpu_error( "setup_gpu", "clEnqueueNDRangeKernel", _cl_status ); return 0; }

    }

    return 1;

}
