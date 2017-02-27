/*
 * glue.cpp
 *
 *  Created on: Feb 27, 2017
 *      Author: wfg
 */



#include "../glueC/glue.h"

#include "../glueC/CallBack.h"



#ifdef __cplusplus
extern "C"
{
#endif

CallBack* callback_init( const char* name )
{
    return new CallBack( name );
}


void callback_register( CallBack* callback, void( *function )( const void*, unsigned int ) )
{
    callback->RegisterCallBackFunction( function );
}

void callback_run( CallBack* callback, const void* data, unsigned int size )
{
    callback->RunCallBack( data, size );
}


void callback_delete( CallBack* callback )
{
    delete callback;
}


#ifdef __cplusplus
} //end extern C
#endif
