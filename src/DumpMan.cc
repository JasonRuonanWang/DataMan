#include "DumpMan.h"

DumpMan::DumpMan()
    :DataMan(){
}

DumpMan::~DumpMan(){
}

int DumpMan::get(void *p_data, json &p_jmsg){
    return 0;
}

int DumpMan::put(const void *p_data, json p_jmsg){

    string doid = p_jmsg["doid"];
    string var = p_jmsg["var"];
    string dtype = p_jmsg["dtype"];
    vector<size_t> putshape = p_jmsg["putshape"].get<vector<size_t>>();
    vector<size_t> varshape = p_jmsg["varshape"].get<vector<size_t>>();
    vector<size_t> offset = p_jmsg["offset"].get<vector<size_t>>();

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
        for (size_t i=0; i<product(putshape,1); i++) cout << ((float*)p_data)[i] << " ";
    if(dtype == "double")
        for (size_t i=0; i<product(putshape,1); i++) cout << ((double*)p_data)[i] << " ";


    cout << endl;
    return 0;
}

void DumpMan::flush(){
}




