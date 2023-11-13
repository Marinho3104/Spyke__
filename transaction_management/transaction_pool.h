
#ifndef TRANSACTION_MANAGEMENT_TRANSACTION_POOL_H
#define TRANSACTION_MANAGEMENT_TRANSACTION_POOL_H

namespace spyke::transaction_management::transaction_pool {

    /** GLOBAL VARIABLES **/
    
    // Pool where the proccessed transactions are stored
    extern void* pool;


    /** FUNCTIONS **/

    // Setups up the transacions pool and needed functions
    bool setup();

}

#endif