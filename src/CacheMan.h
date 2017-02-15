#include "DataMan.h"

class CacheItem : public DataMan{
    public:
        CacheItem();
        ~CacheItem();

        void init(string doid,
                string var,
                string dtype,
                vector<uint64_t> varshape
                );

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
        void clean(const string mode);
        vector<uint64_t> get_shape();
        string get_dtype();

    private:
        void *m_buffer=NULL;
        string m_doid;
        string m_var;
        string m_dtype;
        uint8_t m_bytes;
        uint64_t m_varsize;
        vector<uint64_t> m_varshape;
        bool m_completed;

        inline vector<uint64_t> apply_offset(const vector<uint64_t> &p, const vector<uint64_t> &o){
            cout << p.size() << "  " << o.size() << endl;
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
        const void *get_buffer(string doid, string var);
        void clean(string doid, string var, string mode);
        void clean_all(string mode);
        vector<string> get_do_list();
        vector<string> get_var_list(string doid);
        vector<uint64_t> get_shape(string doid, string var);
        string get_dtype(string doid, string var);

    private:
        typedef map<string, map<string, CacheItem> > CacheDMap;
        typedef map<string, CacheItem> CacheVMap;
        CacheDMap m_cache;

};


