

/** INCLUDES **/
#include <iostream>

#include "transaction_management.h"
#include "transaction_pool.h"
#include "gpu_errors.h" // Gpu error message

namespace spyke::transaction_management::transaction_pool {

    cl_mem transaction_pool;

}

bool spyke::transaction_management::transaction_pool::allocate_memory_pool( uint64_t __c, bool __print_error ) {

    // Error catch
    cl_int _cl_status;

    // Try to create a buffer to the kernel
    transaction_pool = clCreateBuffer( 
        transaction_management_gpu.transaction_management_context, 
        CL_MEM_READ_WRITE, 
        __c * TYPES_TRANSACTION_SIZE, 
        0, 
        &_cl_status
    );

    // Check for errors
    if ( _cl_status != CL_SUCCESS && __print_error ) 
        spyke::gpu::errors::print_gpu_error( "allocate_memory_pool", "clCreateBuffer", _cl_status );

    return _cl_status == CL_SUCCESS;

}
