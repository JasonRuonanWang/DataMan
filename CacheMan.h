#include "DataMan.h"

class CacheItem{
    public:
        CacheItem();
        ~CacheItem();

    private:
        void *buffers;
        string vars;
        vector<uint64_t> varshapes;
        bool completed;

};

class CacheMan : public DataMan{

    public:
        CacheMan();
        ~CacheMan();

        virtual int put(void *data,
                string doid,
                string var,
                string dtype,
                vector<unsigned long> putshape,
                vector<unsigned long> varshape,
                vector<unsigned long> offset,
                int tolerance,
                int priority);

        virtual int get(void *data, json j);

    private:
        vector<CacheItem> cache;


};



