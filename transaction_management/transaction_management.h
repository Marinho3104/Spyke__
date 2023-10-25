

#ifndef TRANSACTION_MANAGEMENT_TRANSACTION_MANAGEMENT_H
#define TRANSACTION_MANAGEMENT_TRANSACTION_MANAGEMENT_H

/** INCLUDES **/
#include <cstdint>
#include <CL/cl.h>

namespace spyke::transaction_management {

    /** STRUCTS **/

    // Holds information about a specific Kernel argument
    struct Kernel_Argument {

        // Variable allocated size
        size_t argument_size;

        // Variable data
        void* argument;

        // Constructor
        Kernel_Argument( size_t, void* );

        // Sets the argument value
        void set_kernel_argument_data( void* );

    };

    // Holds the information of a specific Platform
    struct User_Settings_Platform_Only {

        // Device ids used in the platform
        cl_device_id* device_ids; uint32_t device_ids_count;

        // Device ids used in Transaction Request Proccess Kernels
        cl_device_id* device_ids_transaction_request_proccess; uint32_t device_ids_transaction_request_proccess_count;

        // Number of threads used in each device id for the Transaction Request Proccess Kernel
        size_t* transaction_request_proccess_threads_count;

        // Device id used in Acquire Thread Kernel
        cl_device_id device_id_acquire_thread;

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
        cl_command_queue* transaction_request_proccess_command_queue; uint32_t transaction_request_proccess_command_queue_count;

        // Transaction Request Proccess Kernel object
        cl_kernel transaction_request_proccess_kernel;

        // Transaction Request Proccess Kernel total amount of threads used
        size_t transaction_request_proccess_total_amount_of_threads;
       

        // Acquire Thread Command queue
        cl_command_queue acquire_thread_command_queue;

        // Acquire Thread Kernel object
        cl_kernel acquire_thread_kernel;


        /** KERNEL ARGUMENTS **/

        // __global_offset - Used in transaction_request_proccess_kernel
        // To sinalize the offset that a given kernel should use in order to maintain the global id always unique 
        // in different device ids
        Kernel_Argument global_offset_kernel_argument;

        // __have_work - Used in transaction_request_proccess_kernel, acquire_thread_kernel
        // To sinalize a specific thread tht have new Transaction Request to proccess, for each thread 
        // is used a single bit meaning that from the total amount of threads that will run in a context/platform
        // we divide it by 32 to have the amount of variables needed to be allocated to serve every thread in ( int - 4 bytes - 32 bits ) 
        Kernel_Argument have_work_kernel_argument;

        // __variables_count - Used in acquire_thread_kernel
        // Used to make the thread know how much variable is there in total in the __have_work int array
        Kernel_Argument have_work_variables_count_kernel_argument;

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

    // Execute all kernel functions
    bool start_kernels();

    // Execute the Transaction Request Proccess kernel
    bool start_transaction_request_proccess_kernel( uint32_t );


    // Setups up all needed functions, variable etc for the execution of Transaction Management
    bool setup();

    // Performs the setup for Miner
    bool miner_setup();

    // Create and sets the arguments and variables needed for the Transaction Request Proccess kernel
    bool transaction_request_proccess_kernel_setup( uint32_t );

    // Create and sets the arguments and variables needed for the Acquire thread kernel
    bool acquire_thread_kernel_setup( uint32_t );


    // Adds a new transaction request into the active kernels for proccessing
    // @param __data Transaction Request data
    bool add_new_transaction_request( void* );

}

#endif