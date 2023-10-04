
/** Transaction Management: 
 *      All functions that interact with the GPU, about Transactions Management/Manipulation
*/

#ifndef TRANSACTION_MANAGEMENT_TRANSACTION_MANAGEMENT_H
#define TRANSACTION_MANAGEMENT_TRANSACTION_MANAGEMENT_H

/** DEFINES **/

// Setup types
#define TRANSACTION_MANAGEMENT_TRANSACTION_MANAGEMENT_NODE_SETUP 0
#define TRANSACTION_MANAGEMENT_TRANSACTION_MANAGEMENT_MINER_SETUP 1

// Number of Transaction Request Confirmation kernel functions
#define TRANSACTION_MANAGEMENT_TRANSACTION_MANAGEMENT_TRANSACTION_REQUEST_CONFIRMATION_KERNEL_FUNCTIONS_COUNT 1

/** INCLUDES **/
#include <CL/cl.h> // OpenCl
#include <cstdint> // uint64_t

#include "transaction.h" // Transactions Imp.

namespace spyke::transaction_management {

    // Arguments of the transaction_request confirmation kernel entry point
    struct Arguments_Transaction_Request_Confirmation_Kernel_Entry_Point {

        // Boolean to sinalize if the constructor succeed and the data is ready to be used
        bool ready; 

        // Locker to sinalize when is there data to be proccessed ( bool )
        cl_mem is_data_available;

        // Default contructor to initialize all arguments
        // @param __context The context where the arguments should be created
        // @param __print_error If an error occur during the OpenCl Api calls it will be printed
        Arguments_Transaction_Request_Confirmation_Kernel_Entry_Point( cl_context, bool );

        // Initialize the kernel arguments
        // @param __command_queue Command queue where the buffer write should be enqueued
        // @param __print_error If an error occur during the OpenCl Api calls it will be printed
        bool initialize_arguments( cl_command_queue, bool );
        
    };

    // Holds all GPU related data of Transaction_Management
    struct Transaction_Management_GPU {

        // Gpu context
        cl_context transaction_management_context;

        // Command queue responsible for all related threads that work with the confirmation of Transaction_Request's
        cl_command_queue command_queue_transaction_request_confirmation_threads;
        
    };

    // Hold all propreties for the setup
    struct Transaction_Management_Setup_Properties {

        // Sets the setup type to be used:
        // Node / Miner ...
        uint8_t type;

        // Devices information for the context in transaction management
        cl_device_id* devices; cl_int number_devices;

        // DeviceId used to Command queue creation for transaction_request confirmation threads
        cl_device_id command_queue_transaction_request_confirmation_threads_device[ 1 ];

        // Number of threads used globally for the transaction_request confirmation kernel
        size_t transaction_request_confirmation_threads_global_threads_count;

        // Number of threads used locally for the transaction_request confirmation kernel
        size_t transaction_request_confirmation_threads_local_threads_count;

        // Amount of transactions in transaction pool 
        // Each transaction ( TYPES_TRANSACTION_SIZE bytes )
        uint32_t transactions_transaction_pool;

        // If an error occur during the setup it will be printed
        bool print_error;

    };

    // Global variable that hols all GPU data related Transaction_Management
    extern Transaction_Management_GPU transaction_management_gpu;

    // The list of arguments used in Transaction Request Confirmation Kernel entry point
    extern Arguments_Transaction_Request_Confirmation_Kernel_Entry_Point* 
        arguments_transaction_request_confirmation_kernel_entry_point;

    /** SETUP **/

    // Setup everything related to GPU ( memory/threads ... ), for given setup type
    // All related to transaction management
    bool setup_gpu( Transaction_Management_Setup_Properties ); 

}

#endif