#include<stdio.h>
#include<stdlib.h>
#include"MdtmMan.h"
#include"DataManager.h"


DataMan *dman = NULL;

void dataman_init(string local_address ="tcp://127.0.0.1:12306",
        string remote_address="tcp://127.0.0.1:12307",
        string mode="sender",
        string prefix="/tmp/",
        int num_pipes=10,
        vector<int> tolerance=vector<int>(),
        vector<int> priority=vector<int>())
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
        vector<unsigned long> putshape = vector<unsigned long>(),
        vector<unsigned long> varshape = vector<unsigned long>(),
        vector<unsigned long> offset = vector<unsigned long>(),
        int tolerance=0,
        int priority=100)
{
    printf("DataManager::dataman_write()\n");
    if(!dman)
        dataman_init();
    dman->put(data, doid, var, dtype, putshape, varshape, offset, tolerance, priority);
}

void dataman_write_c_to_cpp(const void *data,
        string doid,
        string var,
        string dtype,
        unsigned long *putshape = NULL,
        unsigned long *varshape = NULL,
        unsigned long *offset = NULL,
        int tolerance=0,
        int priority=100)
{
    printf("DataManager::dataman_write_c_to_cpp()\n");
    vector<unsigned long> putshapev;
    vector<unsigned long> varshapev;
    vector<unsigned long> offsetv;
    for(int i=1; i<=varshape[0]; i++){
        putshapev.push_back(putshape[i]);
        varshapev.push_back(varshape[i]);
        offsetv.push_back(offset[i]);
    }

    dataman_write(data, doid, var, dtype, putshapev, varshapev, offsetv, tolerance, priority);
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

    void dataman_write_c(const void *data,
            const char *doid,
            const char *var,
            const char *datatype,
            unsigned long *putshape,
            unsigned long *varshape,
            unsigned long *offset)
    {
        printf("DataManager::dataman_write_c()\n");
        dataman_write_c_to_cpp(data, doid, var, datatype, putshape, varshape, offset);
    }

}

