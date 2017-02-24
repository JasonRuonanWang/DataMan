#include<stdio.h>
#include<stdlib.h>
#include<dlfcn.h>
#include"DataManager.h"

DataManager::DataManager()
    :DataMan()
{
}
DataManager::~DataManager()
{
}

int DataManager::put(
        const void *p_data,
        string p_doid,
        string p_var,
        string p_dtype,
        vector<size_t> p_putshape,
        vector<size_t> p_varshape,
        vector<size_t> p_offset,
        size_t p_timestep,
        int p_tolerance,
        int p_priority
        ){
    cout << "DataManager::put() varshape : ";
    for (size_t i=0; i<p_putshape.size(); i++)
        cout << p_putshape[i] << " " ;
    cout << endl;
    put_next(
            p_data,
            p_doid,
            p_var,
            p_dtype,
            p_putshape,
            p_varshape,
            p_offset,
            p_timestep,
            p_tolerance,
            p_priority
            );
    return 0;
}

void DataManager::add_file(string p_method){
}

void DataManager::add_stream(
        string p_local_ip,
        string p_remote_ip,
        int p_local_port,
        int p_remote_port,
        int p_num_channels,
        string p_method,
        vector<int> p_tolerance,
        vector<int> p_priority
        ){

    m_local_ip = p_local_ip;
    m_remote_ip = p_remote_ip;
    m_local_port = p_local_port;
    m_remote_port = p_remote_port;
    m_num_channels = p_num_channels;
    m_tolerance = p_tolerance;
    m_priority = p_priority;

    if (m_tolerance.size() < m_num_channels){
        for (int i=0; i<m_num_channels; i++){
            m_tolerance.push_back(0);
        }
    }
    if (m_priority.size() < m_num_channels){
        for (int i=0; i<m_num_channels; i++){
            m_priority.push_back(100/(i+1));
        }
    }

    string soname = "lib" + p_method + "man.so";
    void *so = NULL;
    so = dlopen(soname.c_str(),RTLD_NOW);
    if(so){
        shared_ptr<DataMan> (*get_man)() = NULL;
        get_man = (shared_ptr<DataMan>(*)()) dlsym(so,"getMan");
        if(get_man){
            add_next(get_man());
        }
        else{
            cout << "getMan() not found in " << soname << endl;
        }
    }
    else{
        cout << soname << " not found!" << endl;
    }
}

void DataManager::flush(){
}


int DataManager::get(void *p_data,
        string p_doid,
        string p_var,
        string p_dtype,
        vector<size_t> p_getshape,
        vector<size_t> p_varshape,
        vector<size_t> p_offset,
        size_t p_timestep
        ){
    return 0;
}

int DataManager::get(void *p_data,
        string p_doid,
        string p_var,
        string &p_dtype,
        vector<size_t> &p_varshape,
        size_t &p_timestep
        ){
    return 0;
}



