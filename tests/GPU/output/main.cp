
#include "transaction_management.h"
#include "gpu.h"

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
                128, // Global threads count transaction request confirmation 
                64, // Local threads count transaction request confirmation
                10000, // Transactions in transaction pool
                1, // Print error message
            } 
        );

}