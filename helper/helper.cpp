
/** INCLUDES **/
#include <iostream>

#include "helper.h"

void* spyke::helper::get_file_content( char* __path, uint64_t* __size ) {

    FILE* _file = 
        fopen( __path, "rb" );

    if ( ! _file ) return 0;

    fseek( _file, 0, SEEK_END );

    uint32_t _file_size = 
        ftell( _file );

    if ( __size ) *__size = _file_size;

    fseek( _file, 0, SEEK_SET );

    void* _data = malloc( _file_size );
    
    fread(
        _data,
        _file_size,
        1, _file
    );

    fclose( _file );

    return _data;

}
