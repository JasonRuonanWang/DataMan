#include "DumpMan.h"

DumpMan::DumpMan()
    :DataMan(){
}

DumpMan::~DumpMan(){
}

int DumpMan::get(void *data,
        string doid,
        string var,
        string dtype,
        vector<size_t> getshape,
        vector<size_t> varshape,
        vector<size_t> offset,
        size_t timestep
        ){

    return 0;
}

int DumpMan::get(void *data,
        string doid,
        string var,
        string &dtype,
        vector<size_t> &varshape,
        size_t &timestep){

    return 0;
}

int DumpMan::put(const void *data,
        string doid,
        string var,
        string dtype,
        vector<size_t> putshape,
        vector<size_t> varshape,
        vector<size_t> offset,
        size_t timestep,
        int tolerance,
        int priority
        ){

    cout << "DumpMan::put() doid: " << doid << endl;
    cout << "DumpMan::put() putshape : ";
    for (size_t i=0; i<putshape.size(); i++)
        cout << putshape[i] << " " ;
    cout << endl;
    cout << "DumpMan::put() varshape : ";
    for (size_t i=0; i<varshape.size(); i++)
        cout << varshape[i] << " " ;
    cout << endl;
    cout << "DumpMan::put() offset : ";
    for (size_t i=0; i<offset.size(); i++)
        cout << offset[i] << " " ;
    cout << endl;

    if(dtype == "float")
        for (size_t i=0; i<product(putshape,1); i++) cout << ((float*)data)[i] << " ";
    if(dtype == "double")
        for (size_t i=0; i<product(putshape,1); i++) cout << ((double*)data)[i] << " ";


    cout << endl;
    return 0;
}

void DumpMan::flush(){
}




