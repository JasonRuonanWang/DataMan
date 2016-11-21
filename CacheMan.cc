#include "CacheMan.h"


CacheItem::CacheItem(string d,
        string v,
        uint8_t b,
        vector<uint64_t> s){
    doid = d;
    var = v;
    bytes = b;
    shape = s;

    uint64_t size = accumulate(shape.begin(), shape.end(), bytes, multiplies<uint64_t>());
    buffer = malloc(size);
}

CacheItem::~CacheItem(){
    if(buffer) free(buffer);
}

int CacheItem::put(const void *data,
        string doid,
        string var,
        string dtype,
        vector<uint64_t> putshape,
        vector<uint64_t> varshape,
        vector<uint64_t> offset,
        int tolerance,
        int priority){
    return 0;
}

int CacheItem::get(void *data, json j){

    return 0;
}

void CacheItem::get(json j){

}

void CacheItem::flush(){

}

CacheMan::CacheMan(){

}

CacheMan::~CacheMan(){

}

int CacheMan::put(void *data,
        string doid,
        string var,
        string dtype,
        vector<uint64_t> putshape,
        vector<uint64_t> varshape,
        vector<uint64_t> offset,
        int tolerance,
        int priority){

    return 0;
}

int CacheMan::get(void *data, json j){

    return 0;
}





