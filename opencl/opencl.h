

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

    // Creates and sets a buffer with given information
    // @param __memory
    // @param __context
    // @param __memory_flags
    // @param __memory_size
    // @param __information
    bool set_buffer( cl_mem*, cl_context, cl_mem_flags, size_t, const char* );

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

    // Sets a kernel argument into a given kernel
    // @param __kernel
    // @param __kernel_argument_index
    // @param __argument_size
    // @param __argument_data
    bool set_kernel_argument( cl_kernel, uint32_t, size_t, void*, const char* );

    // Launch kernel
    // @param __command_queue
    // @param __kernel
    // @param __global_work_size
    // @param __local_work_size
    // @param __information
    bool launch_kernel( cl_command_queue, cl_kernel, size_t, size_t, const char* );

    // Launch kernel
    // @param __command_queue
    // @param __kernel
    // @param __information
    bool launch_kernel( cl_command_queue, cl_kernel, const char* );

    // Enqueues a read to a gpu memory buffer and reads the value into a given variable
    // @param __command_queue
    // @param __buffer
    // @param __block
    // @param __offset
    // @param __size
    // @param __to
    // @param __information
    bool read_buffer( cl_command_queue, cl_mem, cl_bool, size_t, size_t, void*, const char* );


    // Enqueues a read to a gpu memory buffer and reads the value into a given variable
    // @param __command_queue
    // @param __buffer
    // @param __block
    // @param __offset
    // @param __size
    // @param __data
    // @param __information
    bool write_buffer( cl_command_queue, cl_mem, cl_bool, size_t, size_t, void*, const char* );

}

#endif