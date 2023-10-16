
#include "transaction_management.h"
#include "gpu.h"

#include <iostream>
#include <unistd.h>
#include <thread>

int main() {

    spyke::gpu::initiate_gpu_properties( 1 );

    spyke::transaction_management::setup_gpu(
        ( spyke::transaction_management::Transaction_Management_Setup_Properties ) 
            { 
                TRANSACTION_MANAGEMENT_TRANSACTION_MANAGEMENT_MINER_SETUP, // Setup Type
                spyke::gpu::gpu_properties.platforms_properties[ 0 ].device_ids, // Devices to be used in transaction management
                spyke::gpu::gpu_properties.platforms_properties[ 0 ].number_devices, // Number of devices to be used
                {
                    spyke::gpu::gpu_properties.platforms_properties[ 0 ].device_ids[ 0 ]
                }, // Device used to create the command queue for transaction_request confirmation threads  
                32, // Global threads count transaction request confirmation 
                1, // Transactions in transaction pool
                1, // Print error message
            } 
        );

    std::cout << "Waiting 2 seconds ... " << std::endl;
    sleep( 2 );

    std::cout << "Done" << std::endl;

    spyke::transaction_management::arguments_transaction_request_confirmation_kernel_entry_point->set_new_transaction_request( 0 );

    std::cout << "Done" << std::endl;

    while( 1 );

}