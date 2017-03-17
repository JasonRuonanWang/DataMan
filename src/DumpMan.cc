#include "DumpMan.h"

DumpMan::DumpMan()
    :DataMan(){
}

DumpMan::~DumpMan(){
}

int DumpMan::init(json p_jmsg){
    if(p_jmsg["dumping"]!=NULL){
        m_dumping = p_jmsg["dumping"].get<bool>();
    }
    return 0;
}
int DumpMan::get(void *p_data, json &p_jmsg){
    return 0;
}

int DumpMan::put(const void *p_data, json p_jmsg){
    put_begin(p_data, p_jmsg);

    if(!m_dumping){
        return 0;
    }

    string doid = p_jmsg["doid"];
    string var = p_jmsg["var"];
    string dtype = p_jmsg["dtype"];
    vector<size_t> putshape = p_jmsg["putshape"].get<vector<size_t>>();
    vector<size_t> varshape = p_jmsg["varshape"].get<vector<size_t>>();
    vector<size_t> offset = p_jmsg["offset"].get<vector<size_t>>();
    size_t putsize = product(putshape, dsize(dtype));

    void *data = malloc(putsize);

    if(p_jmsg["compression_method"] != nullptr){
        auto method = p_jmsg["compression_method"];
        auto man = get_man(method);
        man->transform(p_data, data, p_jmsg);
    }



    cout << p_jmsg.dump(4) << endl;

    if(dtype == "float")
        for (size_t i=0; i<product(putshape,1); i++) cout << ((float*)data)[i] << " ";
    if(dtype == "double")
        for (size_t i=0; i<product(putshape,1); i++) cout << ((double*)data)[i] << " ";

    cout << endl;

    put_end(data, p_jmsg);

    free(data);

    return 0;
}

void DumpMan::flush(){
}




