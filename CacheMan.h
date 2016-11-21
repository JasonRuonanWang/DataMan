#include "DataMan.h"


class CacheItem : public DataMan{
    public:
        CacheItem(string d,
                string v,
                uint8_t b,
                vector<uint64_t> s);
        ~CacheItem();

        int put(const void *data,
                string doid,
                string var,
                string dtype,
                vector<uint64_t> putshape,
                vector<uint64_t> varshape,
                vector<uint64_t> offset,
                int tolerance,
                int priority);

        int get(void *data, json j);
        void get(json j);
        void flush();

    private:
        void *buffer=NULL;
        string doid;
        string var;
        uint8_t bytes;
        vector<uint64_t> shape;
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
                vector<uint64_t> putshape,
                vector<uint64_t> varshape,
                vector<uint64_t> offset,
                int tolerance,
                int priority);

        virtual int get(void *data, json j);

    private:
        map<string, CacheItem> cache;

};



