
/** Helper functions for opencl apis:
 *      
*/

#ifndef OPENCL_OPENCL_H
#define OPENCL_OPENCL_H

/** INCLUDES **/
#include <cstdint>
#include <CL/cl.h>

namespace spyke::opencl {

    // Creates and sets the context with given information
    // @param __context
    // @param __device_ids
    // @param __device_ids_count
    // @param __information
    bool set_context( cl_context*, cl_device_id*, uint64_t, const char* );

    // Creates and sets the command queue with given information
    // @param __command_queue
    // @param __context
    // @param __device_id
    // @param __command_queue_properties
    // @param __information
    bool set_command_queue( cl_command_queue*, cl_context, cl_device_id, uint64_t, const char* );

    // Creates and set the kernel with given information
    // @param __context 
    // @param __kernel 
    // @param __device_ids 
    // @param __device_ids_count 
    // @param __function_file_names 
    // @param __function_file_names_count 
    // @param __entry_point 
    // @return True if the creation of the kernel was succesfully
    bool set_kernel( cl_kernel*, cl_context, cl_device_id*, uint64_t, char**, uint64_t, char*, const char* );

}

#endif