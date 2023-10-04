

/** INCLUDES **/
#include <iostream>

#include "gpu_errors.h" // gpu_errors Impl.


void spyke::gpu::errors::print_gpu_error( const char* __func_name, const char* __api_func_name, cl_int __error_code ) { 
    std::cout << __func_name << "(): An error occur during " << __api_func_name << "() api call with: " << __error_code << " error." << std::endl; 
}
