/*
 * main.c
 *
 *  Created on: Feb 27, 2017
 *      Author: wfg
 */

#include <stdio.h> //printf
#include <stdlib.h> //malloc, free

#include "../glueC/glue.h"



void get( const void* data, unsigned int size )
{
    float* dataf = (float*) data;
    printf("%s\n","My data:");
    unsigned int i;
    for( i = 0; i < size; ++i )
    {
        printf( "%f ", dataf[i] );
    }
    printf("\n");

}


int main( )
{
    unsigned int size = 3;
    float* floats;
    floats = (float*) malloc( size * sizeof( float ) );
    floats[0] = 1;
    floats[1] = 2;
    floats[2] = 3;

    CallBack* callback = callback_init( "getFunction" );
    callback_register( callback, get );
    callback_run( callback, floats, size );

    callback_delete( callback );

    free( floats );
}


