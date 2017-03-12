#ifndef CACHEMAN_H_
#define CACHEMAN_H_


#include "DataMan.h"

class CacheItem : public DataMan{
    public:
        CacheItem();
        ~CacheItem();

        void init(string doid,
                string var,
                string dtype,
                vector<size_t> varshape
                );

        virtual int init(json p_jmsg);
        virtual int put(const void *p_data, json p_jmsg, int p_flag);
        virtual int get(void *p_data, json &p_jmsg);

        void flush();
        string name(){return "CacheItem";}
        const void *get_buffer();
        void clean(const string mode);
        vector<size_t> get_shape();
        string get_dtype();


    private:
        void *m_buffer=NULL;
        string m_doid;
        string m_var;
        string m_dtype;
        uint8_t m_bytes;
        size_t m_varsize;
        vector<size_t> m_varshape;
        bool m_completed;

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

class CacheMan : public DataMan{

    public:
        CacheMan();
        ~CacheMan();

        virtual int init(json p_jmsg);
        virtual int put(const void *p_data, json p_jmsg, int p_flag);
        virtual int get(void *p_data, json &p_jmsg);

        void flush();
        string name(){return "CacheMan";}
        const void *get_buffer(string doid, string var);
        void clean(string doid, string var, string mode);
        void clean_all(string mode);
        vector<string> get_do_list();
        vector<string> get_var_list(string doid);
        vector<size_t> get_shape(string doid, string var);
        string get_dtype(string doid, string var);


    private:
        typedef map<string, map<string, CacheItem> > CacheDMap;
        typedef map<string, CacheItem> CacheVMap;
        CacheDMap m_cache;

};


#endif
