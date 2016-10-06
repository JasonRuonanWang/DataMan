#include<stdio.h>
#include<stdlib.h>
#include"MdtmMan.h"
#include"api.h"


DataMan *dman = NULL;

void dataman_init(){
    dman = new MdtmMan("tcp://127.0.0.1:12306", "tcp://127.0.0.1:12307", "sender");
}

void dataman_write(void *data,
        string doid,
        string var,
        string dtype,
        unsigned int *putshape = 0,
        unsigned int *varshape = 0,
        unsigned int *offset = 0)
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



