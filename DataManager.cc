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
        vector<uint64_t> putshape,
        vector<uint64_t> varshape,
        vector<uint64_t> offset,
        int tolerance,
        int priority)
{
    if(!dman)
        dataman_init();
    dman->put(data, doid, var, dtype, putshape, varshape, offset, tolerance, priority);
}


void dataman_flush(){
    dman->flush();
    delete dman;
}

void dataman_write_c_to_cpp(const void *data,
        string doid,
        string var,
        string dtype,
        uint64_t *putshape = NULL,
        uint64_t *varshape = NULL,
        uint64_t *offset = NULL,
        int tolerance=0,
        int priority=100)
{
    printf("DataManager::dataman_write_c_to_cpp()\n");


    vector<uint64_t> putshapev;
    vector<uint64_t> varshapev;
    vector<uint64_t> offsetv;

    for(int i=1; i<=varshape[0]; i++){
        putshapev.push_back(putshape[i]);
        varshapev.push_back(varshape[i]);
        offsetv.push_back(offset[i]);
    }

    dataman_write(data, doid, var, dtype, putshapev, varshapev, offsetv, tolerance, priority);
}

void dataman_terminate(){
    printf("DataManager::dataman_terminate()\n");
    if(dman) delete dman;
}

void dataman_reg_cb(void (*cb)
        (void *data,
         string doid,
         string var,
         string dtype,
         vector<uint64_t> putshape,
         vector<uint64_t> varshape,
         vector<uint64_t> offset)
        )
{
    dman->get_callback = cb;
}

extern "C"
{
    void dataman_init_c(){
        dataman_init();
    }
    void dataman_prepare_c(){
    }
    void dataman_write_c(const void *data,
            const char *doid,
            const char *var,
            const char *datatype,
            uint64_t *putshape,
            uint64_t *varshape,
            uint64_t *offset)
    {
        printf("DataManager::dataman_write_c()\n");
        dataman_write_c_to_cpp(data, doid, var, datatype, putshape, varshape, offset);
    }
    void dataman_flush_c(){
        dataman_flush();
    }
    void dataman_terminate_c(){
        dataman_terminate();
    }
}



