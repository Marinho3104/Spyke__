
/** INCLUDES **/
#include "transaction_management_defines.h"
#include "transaction_management.h"
#include "opencl.h"
#include "helper.h"

#include <iostream>

namespace spyke::transaction_management {

    User_Settings user_settings; Gpu_Variables_Data gpu_variables_data;

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

        std::cout << "l" << std::endl;
        // Creates and allocate memory for the context
        if (
            ! spyke::opencl::set_context(
                &gpu_variables_data.platforms[ _ ].context,
                user_settings.platforms[ _ ].device_ids,
                user_settings.platforms[ _ ].device_ids_count,
                "spyke::transaction_management::miner_setup"
            )
        ) return 0;
        std::cout << "l" << std::endl;

        // ** Transaction Request Proccess ** //

        // Allocate memory for all command queues
        gpu_variables_data.platforms[ _ ].transaction_request_proccess_command_queue = 
            ( cl_command_queue* ) malloc( sizeof( cl_command_queue ) * user_settings.platforms[ _ ].device_ids_transaction_request_proccess_count );
        std::cout << "l" << std::endl;

        // Creates the command queue for each device id
        for ( uint32_t __ = 0; __ < user_settings.platforms[ _ ].device_ids_transaction_request_proccess_count; __++ )

            if ( 
                !spyke::opencl::set_command_queue(
                    gpu_variables_data.platforms[ _ ].transaction_request_proccess_command_queue + _,
                    gpu_variables_data.platforms[ _ ].context,
                    user_settings.platforms[ _ ].device_ids_transaction_request_proccess[ __ ],
                    CL_QUEUE_OUT_OF_ORDER_EXEC_MODE_ENABLE,
                    "spyke::transaction_management::miner_setup"
                ) 
            ) return 0;

        // Setup the Transaction Request Proccess Kernel
        if (
            ! spyke::transaction_management::transaction_request_proccess_kernel_setup( _ )
        ) return 0;

        std::cout << "l" << std::endl;

    }

    return 1;

}

bool spyke::transaction_management::transaction_request_proccess_kernel_setup( uint32_t __platform_index ) {

    // Need to create a temporary variable cause g++ is a bitch
    char* _file_names[] = TRANSACTION_MANAGEMENT_TRANSACTION_MANAGEMENT_DEFINES_H_TRANSACTION_PROCCESS_KERNEL_FUNCTION_FILE_NAMES;

    if (
        ! spyke::opencl::set_kernel(
            &gpu_variables_data.platforms[ __platform_index ].transaction_request_proccess_kernel,
            gpu_variables_data.platforms[ __platform_index ].context,
            user_settings.platforms[ __platform_index ].device_ids_transaction_request_proccess,
            user_settings.platforms[ __platform_index ].device_ids_transaction_request_proccess_count,
            _file_names,
            TRANSACTION_MANAGEMENT_TRANSACTION_MANAGEMENT_DEFINES_H_TRANSACTION_PROCCESS_KERNEL_FUNCTION_FILE_NAMES_COUNT,
            TRANSACTION_MANAGEMENT_TRANSACTION_MANAGEMENT_DEFINES_H_TRANSACTION_PROCCESS_KERNEL_ENTRY_POINT_FUNCTION_NAME,
            "spyke::transaction_management::transaction_request_proccess_kernel_setup"
        )
    ) return 0;

    return 1;

}
