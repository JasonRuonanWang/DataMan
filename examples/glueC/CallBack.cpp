/*
 * CallBack.cpp
 *
 *  Created on: Feb 27, 2017
 *      Author: wfg
 */

#include "../glueC/CallBack.h"


CallBack::CallBack( const std::string name ):
    m_Name{ name }
{ }


CallBack::~CallBack( )
{ }


void CallBack::RegisterCallBackFunction( std::function<void( const void*, unsigned int )> function )
{
    m_Function = function;
}


void CallBack::RunCallBack( const void* data, unsigned int size )
{
    m_Function( data, size );
}
