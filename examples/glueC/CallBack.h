/*
 * CallBack.h
 *
 *  Created on: Feb 27, 2017
 *      Author: wfg
 */

#ifndef CALLBACK_H_
#define CALLBACK_H_

#include <string>
#include <functional>


class CallBack
{

public:

    const std::string m_Name;

    CallBack( const std::string name );

    ~CallBack( );

    /**
     * For C++ applications
     */
    void RegisterCallBackFunction( std::function<void( const void*, unsigned int )> function );

    /**
     * For C applications
     */
    //void RegisterCallBackFunction( void( *function )( const void*, unsigned int ) );


    void RunCallBack( const void* data, unsigned int size );



private:

    std::function< void( const void*, unsigned int ) > m_Function;




};



#endif /* CALLBACK_H_ */
