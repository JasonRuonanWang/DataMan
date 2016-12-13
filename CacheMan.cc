#include "CacheMan.h"

CacheItem::CacheItem(string p_doid,
        string p_var,
        uint8_t p_bytes,
        vector<uint64_t> p_shape){
    m_doid = p_doid;
    m_var = p_var;
    m_bytes = p_bytes;
    m_shape = p_shape;

    m_varsize = product(p_shape, p_bytes);
    m_buffer = malloc(m_varsize);
}

CacheItem::~CacheItem(){
    if(m_buffer) free(m_buffer);
}

int CacheItem::get(void *data,
        string doid,
        string var,
        string &dtype,
        vector<uint64_t> &varshape,
        uint64_t &timestep
        ){

    return 0;
}

int CacheItem::get(void *data,
        string doid,
        string var,
        string &dtype,
        vector<uint64_t> &getshape,
        vector<uint64_t> &varshape,
        vector<uint64_t> &offset,
        uint64_t &timestep,
        int &tolerance,
        int &priority){
    return 0;
}

int CacheItem::put(const void *p_data,
        string p_doid,
        string p_var,
        string p_dtype,
        vector<uint64_t> p_putshape,
        vector<uint64_t> p_varshape,
        vector<uint64_t> p_offset,
        uint64_t timestep,
        int p_tolerance,
        int p_priority){

    uint64_t putsize = product(p_putshape);
    uint64_t chunksize = p_putshape.back();
    for(uint64_t i=0; i<putsize; i+=chunksize){
        vector<uint64_t> p = one2multi(p_putshape, i);
        p = offset(p, p_offset);
        uint64_t ig = multi2one(p_varshape, p);
        memcpy(m_buffer, p_data, chunksize * m_bytes);
    }

    return 0;
}

void CacheItem::flush(){
}
void CacheItem::clean(string mode){
    if(mode == "zero"){
        for(int i=0; i<m_varsize; i++){
            ((float*)m_buffer)[i]=0;
        }
        return;
    }
    if(mode == "nan"){
        for(int i=0; i<m_varsize; i++){
            ((float*)m_buffer)[i]=numeric_limits<float>::quiet_NaN();
        }
        return;
    }
}

const void *CacheItem::get_buffer(){
    return m_buffer;
}

CacheMan::CacheMan(){

}

CacheMan::~CacheMan(){

}

int CacheMan::put(const void *p_data,
        string p_doid,
        string p_var,
        string p_dtype,
        vector<uint64_t> p_putshape,
        vector<uint64_t> p_varshape,
        vector<uint64_t> p_offset,
        uint64_t p_timestep,
        int p_tolerance,
        int p_priority){

    if(cache.count(p_var) == 0){
        cache.insert(pair<string,CacheItem>(p_var, CacheItem(p_doid, p_var, dsize(p_dtype), p_varshape)));
    }
    cache[p_var].put(p_data, p_doid, p_var, p_dtype, p_putshape, p_varshape, p_offset, p_timestep, p_tolerance, p_priority);

    return 0;
}

int CacheMan::get(void *data,
        string doid,
        string var,
        string &dtype,
        vector<uint64_t> &varshape,
        uint64_t &timestep){

    return 0;
}

int CacheMan::get(void *data,
        string doid,
        string var,
        string &dtype,
        vector<uint64_t> &getshape,
        vector<uint64_t> &varshape,
        vector<uint64_t> &offset,
        uint64_t &timestep,
        int &tolerance,
        int &priority){

    return 0;
}

void CacheMan::flush(){
}

const void *CacheMan::get_buffer(string var){
    return cache[var].get_buffer();
}

void CacheMan::clean(string var, string mode){
    cache[var].clean(mode);
}
