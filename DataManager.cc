#include<stdio.h>
#include<stdlib.h>
#include"MdtmMan.h"
#include"DataManager.h"


DataMan *dman = NULL;

void dataman_init(string local_address,
        string remote_address,
        string mode,
        string prefix,
        int num_pipes,
        int *tolerance,
        int *priority){
    dman = new MdtmMan(local_address, remote_address, mode, prefix, num_pipes, tolerance, priority);
}

void dataman_write(void *data,
        string doid,
        string var,
        string dtype,
        unsigned int *putshape = NULL,
        unsigned int *varshape = NULL,
        unsigned int *offset = NULL)
{
    if(!dman)
        dataman_init();
    dman->put(data, doid, var, dtype, putshape, varshape, offset);
}


void dataman_open(){
}


void dataman_term(){
    delete dman;
}



