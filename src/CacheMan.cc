#include "CacheMan.h"


void CacheItem::init(string p_doid,
        string p_var,
        string p_dtype,
        vector<uint64_t> p_varshape){
    m_doid = p_doid;
    m_var = p_var;
    m_dtype = p_dtype;
    m_varshape = p_varshape;
    m_bytes = dsize(m_dtype);
    m_varsize = product(m_varshape);
    m_buffer = malloc(m_varsize * m_bytes);
}

CacheItem::CacheItem()
:DataMan(){}

CacheItem::~CacheItem(){
    if(m_buffer) free(m_buffer);
}

int CacheItem::get(void *p_data,
        string p_doid,
        string p_var,
        string p_dtype,
        vector<uint64_t> p_getshape,
        vector<uint64_t> p_varshape,
        vector<uint64_t> p_offset,
        uint64_t p_timestep
        ){
    return 0;
}

int CacheItem::get(void *p_data,
        string p_doid,
        string p_var,
        string &p_dtype,
        vector<uint64_t> &p_varshape,
        uint64_t &p_timestep
        ){
    return 0;
}

int CacheItem::put(void *p_data,
        string p_doid,
        string p_var,
        string p_dtype,
        vector<uint64_t> p_putshape,
        vector<uint64_t> p_varshape,
        vector<uint64_t> p_offset,
        uint64_t p_timestep,
        int p_tolerance,
        int p_priority){
    if(!m_buffer) init(p_doid, p_var, p_dtype, p_varshape);
    uint64_t putsize = product(p_putshape);
    uint64_t chunksize = p_putshape.back();
    for(uint64_t i=0; i<putsize; i+=chunksize){
        vector<uint64_t> p = one2multi(p_putshape, i);
        p = apply_offset(p, p_offset);
        uint64_t ig = multi2one(p_varshape, p);
        memcpy((char*)m_buffer + ig * m_bytes, (char*)p_data + i * m_bytes, chunksize * m_bytes);
    }

    return 0;
}

vector<uint64_t> CacheItem::get_shape(){
    return m_varshape;
}

string CacheItem::get_dtype(){
    return m_dtype;
}

void CacheItem::flush(){
}

void CacheItem::clean(const string mode){
    if(mode == "zero"){
        memset(m_buffer, 0, m_varsize * m_bytes);
        return;
    }
    if(mode == "nan"){
        memset(m_buffer, numeric_limits<int>::quiet_NaN(), m_varsize * m_bytes);
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

int CacheMan::put(void *p_data,
        string p_doid,
        string p_var,
        string p_dtype,
        vector<uint64_t> p_putshape,
        vector<uint64_t> p_varshape,
        vector<uint64_t> p_offset,
        uint64_t p_timestep,
        int p_tolerance,
        int p_priority){
    m_cache[p_doid][p_var].put(p_data, p_doid, p_var, p_dtype, p_putshape, p_varshape, p_offset, p_timestep, p_tolerance, p_priority);
    return 0;
}

int CacheMan::get(void *p_data,
        string p_doid,
        string p_var,
        string p_dtype,
        vector<uint64_t> p_getshape,
        vector<uint64_t> p_varshape,
        vector<uint64_t> p_offset,
        uint64_t p_timestep
        ){
    return 0;
}

int CacheMan::get(void *p_data,
        string p_doid,
        string p_var,
        string &p_dtype,
        vector<uint64_t> &p_varshape,
        uint64_t &p_timestep
        ){
    return 0;
}

void CacheMan::flush(){
    dump(m_cache["aaa"]["data"].get_buffer(), "aaa", "data", "float",  m_cache["aaa"]["data"].get_shape(), 10);
}

const void *CacheMan::get_buffer(string doid, string var){
    return m_cache[doid][var].get_buffer();
}

void CacheMan::clean(string doid, string var, string mode){
    m_cache[doid][var].clean(mode);
}

void CacheMan::clean_all(string mode){
    for(CacheDMap::iterator i = m_cache.begin(); i != m_cache.end(); ++i){
        for(CacheVMap::iterator j = m_cache[i->first].begin(); j != m_cache[i->first].end(); ++j){
            j->second.clean(mode);
        }
    }
}

vector<string> CacheMan::get_do_list(){
    vector<string> do_list;
    for(CacheDMap::const_iterator it = m_cache.begin(); it != m_cache.end(); ++it)
        do_list.push_back(it->first);
    return do_list;
}

vector<string> CacheMan::get_var_list(string doid){
    vector<string> var_list;
    for(CacheVMap::const_iterator it = m_cache[doid].begin(); it != m_cache[doid].end(); ++it)
        var_list.push_back(it->first);
    return var_list;
}

vector<uint64_t> CacheMan::get_shape(string doid, string var){
    return m_cache[doid][var].get_shape();
}

string CacheMan::get_dtype(string doid, string var){
    return m_cache[doid][var].get_dtype();
}
