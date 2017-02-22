#include "DumpMan.h"

DumpMan::DumpMan(){
}

DumpMan::~DumpMan(){
}

int DumpMan::get(void *data,
        string doid,
        string var,
        string dtype,
        vector<uint64_t> getshape,
        vector<uint64_t> varshape,
        vector<uint64_t> offset,
        uint64_t timestep
        ){

    return 0;
}

int DumpMan::get(void *data,
        string doid,
        string var,
        string &dtype,
        vector<uint64_t> &varshape,
        uint64_t &timestep){

    return 0;
}

int DumpMan::put(void *data,
        string doid,
        string var,
        string dtype,
        vector<uint64_t> putshape,
        vector<uint64_t> varshape,
        vector<uint64_t> offset,
        uint64_t timestep,
        int tolerance,
        int priority
        ){
    for (int i=0; i<product(varshape,1); i++)
        cout << ((float*)data)[i] << " ";
    cout << endl;
    return 0;
}

void DumpMan::flush(){
}




