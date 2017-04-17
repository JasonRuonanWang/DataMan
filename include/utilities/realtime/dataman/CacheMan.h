#ifndef CACHEMAN_H_
#define CACHEMAN_H_


#include "DataMan.h"

using namespace std;

namespace adios
{
namespace realtime
{

class CacheItem : public DataManBase{
    public:
        CacheItem() = default;
        virtual ~CacheItem() = default;

        void init(string doid,
                string var,
                string dtype,
                vector<size_t> varshape
                );

        virtual int init(json p_jmsg);
        virtual int put(const void *p_data, json p_jmsg);
        virtual int get(void *p_data, json &p_jmsg);
        virtual void transform(const void* p_in, void* p_out, json &p_jmsg){};

        void flush();
        string name(){return "CacheItem";}
        string type(){return "Cache";}
        const void *get_buffer();
        void clean(const string mode);
        void remove(size_t timestep);
        vector<size_t> get_shape();
        string get_dtype();


    private:
        map<size_t, vector<char>> m_buffer;
        string m_doid;
        string m_var;
        string m_dtype;
        size_t m_bytes;
        size_t m_varsize;
        size_t m_varbytes;
        vector<size_t> m_varshape;
        bool m_completed;
        size_t m_timestep=0;

        inline vector<size_t> apply_offset(const vector<size_t> &p, const vector<size_t> &o){
            vector<size_t> g;
            for(int i=0; i<p.size(); i++){
                g.push_back(p[i]+o[i]);
            }
            return g;
        }

        inline size_t multi2one(const vector<size_t> &v, const vector<size_t> &p){
            size_t index=0;
            for (int i=1; i<v.size(); i++){
                index += accumulate(v.begin() + i, v.end(), p[i-1], multiplies<size_t>());
            }
            index += p.back();
            return index;
        }

        inline vector<size_t> one2multi(const vector<size_t> &v, size_t p){
            vector<size_t> index(v.size());
            for (int i=1; i<v.size(); i++){
                size_t s = accumulate(v.begin() + i, v.end(), 1, multiplies<size_t>());
                index[i-1] = p / s;
                p -= index[i-1] * s;
            }
            index.back()=p;
            return index;
        }
};

class CacheMan : public DataManBase{

    public:
        CacheMan() = default;
        virtual ~CacheMan() = default;

        virtual int init(json p_jmsg);
        virtual int put(const void *p_data, json p_jmsg);
        virtual int get(void *p_data, json &p_jmsg);
        virtual void transform(const void* p_in, void* p_out, json &p_jmsg){};

        void flush();
        string name(){return "CacheMan";}
        string type(){return "Cache";}
        const void *get_buffer(string doid, string var);
        void clean(string doid, string var, string mode);
        void clean_all(string mode);
        void remove(string doid, string var, size_t timestep);
        void remove_all(size_t timestep);
        vector<string> get_do_list();
        vector<string> get_var_list(string doid);
        vector<size_t> get_shape(string doid, string var);
        string get_dtype(string doid, string var);

    private:
        typedef map<string, CacheItem> CacheVarMap;
        typedef map<string, CacheVarMap> CacheDoMap;
        CacheDoMap m_cache;

};

} // end namespace realtime
} // end namespace adios

#endif
