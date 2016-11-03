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
        vector<int> tolerance,
        vector<int> priority)
{
    if (tolerance.size() < num_pipes){
        for (int i=0; i<num_pipes; i++){
            tolerance.push_back(0);
        }
    }
    if (priority.size() < num_pipes){
        for (int i=0; i<num_pipes; i++){
            priority.push_back(100/(i+1));
        }
    }
    dman = new MdtmMan(local_address, remote_address, mode, prefix, num_pipes, tolerance, priority);
}

void dataman_write(const void *data,
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

extern "C"
{

    void dataman_init_c(){
        dataman_init();
    }

    void dataman_open_c(){
        dataman_open();
    }

    void dataman_write_c(const void *data, const char *name, const char *var, const char *type){
        dataman_write(data, name, var, type);
    }

}

