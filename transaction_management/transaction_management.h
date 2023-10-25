

#ifndef TRANSACTION_MANAGEMENT_TRANSACTION_MANAGEMENT_H
#define TRANSACTION_MANAGEMENT_TRANSACTION_MANAGEMENT_H

/** INCLUDES **/
#include <cstdint>
#include <CL/cl.h>

namespace spyke::transaction_management {

    /** STRUCTS **/

    // Holds the information of a specific Platform
    struct User_Settings_Platform_Only {

        // Device ids used in the platform
        cl_device_id* device_ids; uint32_t device_ids_count;

        // Device ids used in Transaction Request Proccess Kernels
        cl_device_id* device_ids_transaction_request_proccess; uint32_t device_ids_transaction_request_proccess_count;


    };

    // Holds all user settings for the Transaction Management Setup
    struct User_Settings {

        // What type of setup will be executed
        uint8_t type;

        // Platforms and respective data involve in each 
        User_Settings_Platform_Only* platforms; uint32_t platforms_count;

    };

    // Holds gpu data about each platform
    struct Gpu_Variables_Platform_Only {

        // Context of Platform
        cl_context context;

        // Transaction Request Proccess Commands Queues
        cl_command_queue* transaction_request_proccess_command_queue;

        // Transaction Request Proccess Kernel object
        cl_kernel transaction_request_proccess_kernel;

    };

    // Holds all gpu variables data about the Transaction Management 
    // ** DONT TOUCH BY USER **
    struct Gpu_Variables_Data {

        // Platforms used in Transaction Managemenet
        Gpu_Variables_Platform_Only* platforms; uint32_t platforms_count;

    };

    /** GLOBAL VARIABLES **/

    // User settings for the Transaction Management Setup
    extern User_Settings user_settings;

    // Gpu Variables used in Transaction Management
    extern Gpu_Variables_Data gpu_variables_data;

    /** FUNCTIONS **/

    // Setups up all needed functions, variable etc for the execution of Transaction Management
    bool setup();

    // Performs the setup for Miner
    bool miner_setup();

    // Create and sets the arguments and variables needed for the Transaction Request Proccess kernel
    bool transaction_request_proccess_kernel_setup( uint32_t );

}

#endif