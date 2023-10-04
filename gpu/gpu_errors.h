

#ifndef GPU_GPU_ERRORS_H
#define GPU_GPU_ERRORS_H

/** INCLUDES **/
#include <CL/cl.h> // OpenCl

namespace spyke::gpu::errors {

    // Prints a formatted error message for GPU/OpenCl api calls errors
    // @param __func_name function's name where the error occur
    // @param __api_func_name function's name where the error occur
    // @param __error_code error code
    void print_gpu_error( const char*, const char*, cl_int );

}

#endif