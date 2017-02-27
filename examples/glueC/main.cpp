/*
 * callback.cpp
 *
 *  Created on: Feb 27, 2017
 *      Author: wfg
 */

#include <iostream>
#include <vector>

#include "../glueC/CallBack.h"



void get( const void* data, unsigned int size )
{
    float* dataf = (float*) data;
    std::cout << "My data:\n";
    for( unsigned int i = 0; i < size; ++i )
    {
        std::cout << dataf[i] << " ";
    }
    std::cout << "\n";

}


int main( )
{
    std::vector<float> floats = { 1., 2., 3. };

    CallBack callBack("get");
    callBack.RegisterCallBackFunction( get );

    std::cout << "Now calling get on data\n";
    callBack.RunCallBack( floats.data(), floats.size() );
}
