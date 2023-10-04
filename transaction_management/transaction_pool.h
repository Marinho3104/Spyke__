
/** Transaction Pool: 
 *      Holds all functions relationated to the Pool transaction
*/

#ifndef TRANSACTION_MANAGEMENT_TRANSACTION_POOL_H
#define TRANSACTION_MANAGEMENT_TRANSACTION_POOL_H

/** INCLUDES **/
#include <CL/cl.h> // OpenCl

#include "transaction.h" // Transactions Impl.

namespace spyke::transaction_management::transaction_pool {

    // Transaction pool 
    extern cl_mem transaction_pool;

    // Allocates memory for the memory pool
    // @param __c Amount of transaction to allocate ( __c * TYPES_TRANSACTION_SIZE )
    // @param __print_error If an error occur during the OpenCl Api calls it will be printed
    bool allocate_memory_pool( uint64_t, bool );

}

#endif