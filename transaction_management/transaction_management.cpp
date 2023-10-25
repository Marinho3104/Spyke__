
/** INCLUDES **/
#include "transaction_management_defines.h"
#include "transaction_management.h"
#include "opencl.h"
#include "helper.h"

#include <iostream>
#include <string.h>

namespace spyke::transaction_management {

    User_Settings user_settings; Gpu_Variables_Data gpu_variables_data;

}

spyke::transaction_management::Kernel_Argument::Kernel_Argument( size_t __argument_size, void* __argument ) 
    : argument_size( __argument_size ), argument( malloc( argument_size ) ) { if( __argument ) set_kernel_argument_data( __argument ); }

void spyke::transaction_management::Kernel_Argument::set_kernel_argument_data( void* __argument ) {

    memcpy(
        argument,
        __argument,
        argument_size
    );

}

bool spyke::transaction_management::start_kernels() {

    // Loops throught all given platforms
    for ( uint32_t _ = 0; _ < gpu_variables_data.platforms_count; _++ ) {

        /** TRANSACTION REQUEST PROCCESS **/

        // Initiates the kernel arguments needed and executes the kernel
        start_transaction_request_proccess_kernel( _ );

    }

    return 1;

}

bool spyke::transaction_management::start_transaction_request_proccess_kernel( uint32_t __platform_index ) {

    // Global offset used for each device id
    size_t _global_offset = 0;

    // Loops throught out all command queues of Transaction Management
    for ( uint32_t _ = 0; _ < gpu_variables_data.platforms[ __platform_index ].transaction_request_proccess_command_queue_count; _++ ) {

        // Copies the value of the global offset into the kernel argument value
        gpu_variables_data.platforms[ __platform_index ].global_offset_kernel_argument.set_kernel_argument_data(
            &_global_offset
        );

        // Set the kernel argument into the kernel
        if(
            ! spyke::opencl::set_kernel_argument(
                gpu_variables_data.platforms[ __platform_index ].transaction_request_proccess_kernel,
                0,
                gpu_variables_data.platforms[ __platform_index ].global_offset_kernel_argument.argument_size,
                gpu_variables_data.platforms[ __platform_index ].global_offset_kernel_argument.argument,
                "spyke::transaction_management::start_transaction_request_proccess_kernel"
            )
        ) return 0;

        // Launch the kernel
        if(
            ! spyke::opencl::launch_kernel(
                gpu_variables_data.platforms[ __platform_index ].transaction_request_proccess_command_queue[ _ ],
                gpu_variables_data.platforms[ __platform_index ].transaction_request_proccess_kernel,
                user_settings.platforms[ __platform_index ].transaction_request_proccess_threads_count[ _ ],
                sizeof( cl_int ) * 8,
                "spyke::transaction_management::start_transaction_request_proccess_kernel"
            )
        ) return 0;

        // Adds to the offset the number of kernels launched
        _global_offset += 
            user_settings.platforms[ __platform_index ].transaction_request_proccess_threads_count[ _ ];

    }

    return 1;

}


bool spyke::transaction_management::setup() {

    if ( 
        user_settings.type == TRANSACTION_MANAGEMENT_TRANSACTION_MANAGEMENT_DEFINES_H_MINER_SETUP &&  
        ! miner_setup()
    ) return 0;

    return 1;

}

bool spyke::transaction_management::miner_setup() {

    // Allocate memory for all Variables

    // Copies and allocated memory for all Platform
    gpu_variables_data.platforms_count = user_settings.platforms_count;
    gpu_variables_data.platforms = ( Gpu_Variables_Platform_Only* ) malloc( sizeof( Gpu_Variables_Platform_Only ) * gpu_variables_data.platforms_count );

    // Catch errors
    cl_int _cl_status;

    // Loops throught all given platforms
    for ( uint32_t _ = 0; _ < gpu_variables_data.platforms_count; _++ ) {

        // Creates and allocate memory for the context
        if (
            ! spyke::opencl::set_context(
                &gpu_variables_data.platforms[ _ ].context,
                user_settings.platforms[ _ ].device_ids,
                user_settings.platforms[ _ ].device_ids_count,
                "spyke::transaction_management::miner_setup"
            )
        ) return 0;

        // ** Transaction Request Proccess ** //

        // Setup the Transaction Request Proccess Kernel
        // Setup the Acquire Thread Kernel
        if (
            ! spyke::transaction_management::transaction_request_proccess_kernel_setup( _ ) ||
            ! spyke::transaction_management::acquire_thread_kernel_setup( _ )
        ) return 0;

    }

    return 1;

}

bool spyke::transaction_management::transaction_request_proccess_kernel_setup( uint32_t __platform_index ) {

    // Allocate memory for all command queues
    gpu_variables_data.platforms[ __platform_index ].transaction_request_proccess_command_queue_count = 
        user_settings.platforms[ __platform_index ].device_ids_transaction_request_proccess_count;
    gpu_variables_data.platforms[ __platform_index ].transaction_request_proccess_command_queue = 
        ( cl_command_queue* ) malloc( sizeof( cl_command_queue ) * user_settings.platforms[ __platform_index ].device_ids_transaction_request_proccess_count );

    // Creates the command queue for each device id
    for ( uint32_t _ = 0; _ < user_settings.platforms[ __platform_index ].device_ids_transaction_request_proccess_count; _++ )

        if ( 
            ! spyke::opencl::set_command_queue(
                gpu_variables_data.platforms[ __platform_index ].transaction_request_proccess_command_queue + _,
                gpu_variables_data.platforms[ __platform_index ].context,
                user_settings.platforms[ __platform_index ].device_ids_transaction_request_proccess[ _ ],
                CL_QUEUE_OUT_OF_ORDER_EXEC_MODE_ENABLE,
                "spyke::transaction_management::transaction_request_proccess_kernel_setup"
            ) 
        ) return 0;

    // Need to create a temporary variable cause g++ is a bitch
    char* _file_names[] = TRANSACTION_MANAGEMENT_TRANSACTION_MANAGEMENT_DEFINES_H_TRANSACTION_REQUEST_PROCCESS_KERNEL_FUNCTION_FILE_NAMES;

    // creates and sets the kernel
    if (
        ! spyke::opencl::set_kernel(
            &gpu_variables_data.platforms[ __platform_index ].transaction_request_proccess_kernel,
            gpu_variables_data.platforms[ __platform_index ].context,
            user_settings.platforms[ __platform_index ].device_ids_transaction_request_proccess,
            user_settings.platforms[ __platform_index ].device_ids_transaction_request_proccess_count,
            _file_names,
            TRANSACTION_MANAGEMENT_TRANSACTION_MANAGEMENT_DEFINES_H_TRANSACTION_REQUEST_PROCCESS_KERNEL_FUNCTION_FILE_NAMES_COUNT,
            TRANSACTION_MANAGEMENT_TRANSACTION_MANAGEMENT_DEFINES_H_TRANSACTION_REQUEST_PROCCESS_KERNEL_ENTRY_POINT_FUNCTION_NAME,
            "spyke::transaction_management::transaction_request_proccess_kernel_setup"
        )
    ) return 0;

    // Sets the total amount of threads that will be used by the Transaction Request Proccess Kernels
    gpu_variables_data.platforms[ __platform_index ].transaction_request_proccess_total_amount_of_threads = 0;

    for ( size_t _ = 0; _ < gpu_variables_data.platforms[ __platform_index ].transaction_request_proccess_command_queue_count; _++ )

        gpu_variables_data.platforms[ __platform_index ].transaction_request_proccess_total_amount_of_threads += 
            user_settings.platforms[ __platform_index ].transaction_request_proccess_threads_count[ _ ];

    // Kernel Arguments

    // __global_offset kernel argument
    // There is no need to initialize the value already cause it will change for each instance of device id
    // to maintain the global id unique
    gpu_variables_data.platforms[ __platform_index ].global_offset_kernel_argument = 
        Kernel_Argument( sizeof( cl_long ), 0 );

    cl_mem _temp_have_work_kernel_argument;

    if (
        ! spyke::opencl::set_buffer(
            &_temp_have_work_kernel_argument,
            gpu_variables_data.platforms[ __platform_index ].context,
            CL_MEM_READ_WRITE,
            gpu_variables_data.platforms[ __platform_index ].transaction_request_proccess_total_amount_of_threads / sizeof( cl_int ),
            "spyke::transaction_management::transaction_request_proccess_kernel_setup"
        )
    ) return 0;
        
    // __have_work kernel argument
    gpu_variables_data.platforms[ __platform_index ].have_work_kernel_argument = 
        Kernel_Argument( 
            sizeof( cl_mem ),
            &_temp_have_work_kernel_argument
        );

    // Set the argument into the kernel arguments
    if (
        ! spyke::opencl::set_kernel_argument(
            gpu_variables_data.platforms[ __platform_index ].transaction_request_proccess_kernel,
            1,
            gpu_variables_data.platforms[ __platform_index ].have_work_kernel_argument.argument_size,
            gpu_variables_data.platforms[ __platform_index ].have_work_kernel_argument.argument,
            "spyke::transaction_management::transaction_request_proccess_kernel_setup"
        )
    ) return 0;

    return 1;

}

bool spyke::transaction_management::acquire_thread_kernel_setup( uint32_t __platform_index ) {

    if ( 
        ! spyke::opencl::set_command_queue(
            &gpu_variables_data.platforms[ __platform_index ].acquire_thread_command_queue,
            gpu_variables_data.platforms[ __platform_index ].context,
            user_settings.platforms[ __platform_index ].device_id_acquire_thread,
            CL_QUEUE_OUT_OF_ORDER_EXEC_MODE_ENABLE,
            "spyke::transaction_management::acquire_thread_kernel_setup"
        )
    ) return 0;

    // Need to create a temporary variable cause g++ is a bitch
    char* _file_names[] = TRANSACTION_MANAGEMENT_TRANSACTION_MANAGEMENT_DEFINES_H_ACQUIRE_THREAD_KERNEL_FUNCTION_FILE_NAMES;

    // creates and sets the kernel
    if (
        ! spyke::opencl::set_kernel(
            &gpu_variables_data.platforms[ __platform_index ].acquire_thread_kernel,
            gpu_variables_data.platforms[ __platform_index ].context,
            user_settings.platforms[ __platform_index ].device_ids_transaction_request_proccess,
            user_settings.platforms[ __platform_index ].device_ids_transaction_request_proccess_count,
            _file_names,
            TRANSACTION_MANAGEMENT_TRANSACTION_MANAGEMENT_DEFINES_H_ACQUIRE_THREAD_KERNEL_FUNCTION_FILE_NAMES_COUNT,
            TRANSACTION_MANAGEMENT_TRANSACTION_MANAGEMENT_DEFINES_H_ACQUIRE_THREAD_KERNEL_ENTRY_POINT_FUNCTION_NAME,
            "spyke::transaction_management::acquire_thread_kernel_setup"
        )
    ) return 0;

    // Kernel Arguments

    // Same argument as in Transaction Request Proccess - __have_work, so there is no need to allocate it again
    // Set the argument into the kernel arguments
    if (
        ! spyke::opencl::set_kernel_argument(
            gpu_variables_data.platforms[ __platform_index ].acquire_thread_kernel,
            0,
            gpu_variables_data.platforms[ __platform_index ].have_work_kernel_argument.argument_size,
            gpu_variables_data.platforms[ __platform_index ].have_work_kernel_argument.argument,
            "spyke::transaction_management::acquire_thread_kernel_setup"
        )
    ) return 0;

    // __have_work_variables_count kernel argument

    // Temporary variable to hold the value so it can be passed the pointer to the value
    cl_long _temporary_variable_have_work_variables_count = 
        gpu_variables_data.platforms[ __platform_index ].transaction_request_proccess_total_amount_of_threads / sizeof( cl_int );

    gpu_variables_data.platforms[ __platform_index ].have_work_variables_count_kernel_argument = 
        Kernel_Argument( 
            sizeof( cl_long ),
            &_temporary_variable_have_work_variables_count
        );

    // Add the variable into the kernel argument
    if (
        ! spyke::opencl::set_kernel_argument(
            gpu_variables_data.platforms[ __platform_index ].acquire_thread_kernel,
            1,
            gpu_variables_data.platforms[ __platform_index ].have_work_variables_count_kernel_argument.argument_size,
            gpu_variables_data.platforms[ __platform_index ].have_work_variables_count_kernel_argument.argument,
            "spyke::transaction_management::acquire_thread_kernel_setup"
        )
    ) return 0;

    return 1;

}

bool spyke::transaction_management::add_new_transaction_request( void* __data ) {

    // Loops throught each platform
    for ( uint32_t _ = 0; _ < gpu_variables_data.platforms_count; _++ ) {

        // Launch the task to look for a empty thread to start the proccess 
        if (
            ! spyke::opencl::launch_kernel(
                gpu_variables_data.platforms[ _ ].acquire_thread_command_queue,
                gpu_variables_data.platforms[ _ ].acquire_thread_kernel,
                "spyke::transaction_management::add_new_transaction_request"
            )
        ) return 0;

        break;

    }

    return 1;

}

