#include "CacheMan.h"

CacheItem::CacheItem(){

}

CacheItem::~CacheItem(){

}

CacheMan::CacheMan(){

}

CacheMan::~CacheMan(){


}
int CacheMan::put(void *data,
        string doid,
        string var,
        string dtype,
        vector<unsigned long> putshape,
        vector<unsigned long> varshape,
        vector<unsigned long> offset,
        int tolerance,
        int priority){

    return 0;
}

int CacheMan::get(void *data, json j){

    return 0;
}
