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

    bool delete_tolerance=false;
    bool delete_priority=false;

    if (!tolerance){
        tolerance = new int[num_pipes];
        for (int i=0; i<num_pipes; i++){
            tolerance[i]=0;
        }
        delete_tolerance=true;
    }
    if (!priority){
        priority = new int[num_pipes];
        for (int i=0; i<num_pipes; i++){
            priority[i]=50;
        }
        priority[0]=100;
        delete_priority=true;
    }

    dman = new MdtmMan(local_address, remote_address, mode, prefix, num_pipes, tolerance, priority);

    if(delete_tolerance) delete tolerance;
    if(delete_priority) delete priority;
}

void dataman_write(void *data,
        string doid,
        string var,
        string dtype,
        unsigned int *putshape = NULL,
        unsigned int *varshape = NULL,
        unsigned int *offset = NULL,
        int tolerance=0,
        int priority=100)
{
    if(!dman)
        dataman_init();
    dman->put(data, doid, var, dtype, putshape, varshape, offset, tolerance, priority);
}


void dataman_open(){
}


void dataman_term(){
    delete dman;
}



