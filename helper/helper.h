

#ifndef HELPER_HELPER_H
#define HELPER_HELPER_H

/** INCLUDES **/
#include <cstdint>

namespace spyke::helper {

    // Returns the content of a file or null if file dont exists
    // @param __path Path to the file
    // @param __size If not null will be set to the size of the file
    void* get_file_content( char*, uint64_t* );

}

#endif