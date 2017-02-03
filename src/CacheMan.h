#include "DataMan.h"

class CacheItem : public DataMan{
    public:
        CacheItem(){
            cout << "CacheItem::CacheItem() is called. This should never happen!" << endl;
        }
        CacheItem(string p_doid,
                string p_var,
                uint8_t p_bytes,
                vector<uint64_t> p_shape);
        ~CacheItem();

        int put(const void *data,
                string doid,
                string var,
                string dtype,
                vector<uint64_t> putshape,
                vector<uint64_t> varshape,
                vector<uint64_t> offset,
                uint64_t timestep,
                int tolerance,
                int priority);

        int get(void *data,
                string doid,
                string var,
                string &dtype,
                vector<uint64_t> &getshape,
                vector<uint64_t> &varshape,
                vector<uint64_t> &offset,
                uint64_t &timestep,
                int &tolerance,
                int &priority);

        int get(void *data,
                string doid,
                string var,
                string &dtype,
                vector<uint64_t> &varshape,
                uint64_t &timestep);

        void flush();
        const void *get_buffer();
        void clean(string mode);

    private:
        void *m_buffer=NULL;
        string m_doid;
        string m_var;
        uint8_t m_bytes;
        uint64_t m_varsize;
        vector<uint64_t> m_shape;
        bool m_completed;

        inline vector<uint64_t> offset(const vector<uint64_t> &p, const vector<uint64_t> &o){
            vector<uint64_t> g;
            for(int i=0; i<p.size(); i++){
                g.push_back(p[i]+o[i]);
            }
            return g;
        }

        inline uint64_t multi2one(const vector<uint64_t> &v, const vector<uint64_t> &p){
            uint64_t index=0;
            for (int i=1; i<v.size(); i++){
                index += accumulate(v.begin() + i, v.end(), p[i-1], multiplies<uint64_t>());
            }
            index += p.back();
            return index;
        }

        inline vector<uint64_t> one2multi(const vector<uint64_t> &v, uint64_t p){
            vector<uint64_t> index(v.size());
            for (int i=1; i<v.size(); i++){
                uint64_t s = accumulate(v.begin() + i, v.end(), 1, multiplies<uint64_t>());
                index[i-1] = p / s;
                p -= index[i-1] * s;
            }
            index.back()=p;
            return index;
        }
};

class CacheMan : public DataMan{

    public:
        CacheMan();
        ~CacheMan();

        int put(const void *data,
                string doid,
                string var,
                string dtype,
                vector<uint64_t> putshape,
                vector<uint64_t> varshape,
                vector<uint64_t> offset,
                uint64_t timestep,
                int tolerance,
                int priority);

        int get(void *data,
                string doid,
                string var,
                string &dtype,
                vector<uint64_t> &getshape,
                vector<uint64_t> &varshape,
                vector<uint64_t> &offset,
                uint64_t &timestep,
                int &tolerance,
                int &priority);

        int get(void *data,
                string doid,
                string var,
                string &dtype,
                vector<uint64_t> &varshape,
                uint64_t &timestep
                );

        void flush();
        const void *get_buffer(string var);
        void clean(string var, string mode);

    private:
        map<string, CacheItem*> cache;

};


