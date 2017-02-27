/*
 * glue.h
 *
 *  Created on: Feb 27, 2017
 *      Author: wfg
 */

#ifndef GLUE_H_
#define GLUE_H_


#ifdef __cplusplus
extern "C"
{
#endif

typedef struct CallBack CallBack;

CallBack* callback_init( const char* name );

void callback_register( CallBack*, void( *function )( const void*, unsigned int ) );

void callback_run( CallBack*, const void* data, unsigned int size );

void callback_delete( CallBack* callback );

#ifdef __cplusplus
} //end extern C
#endif



#endif /* GLUE_H_ */
