#ifndef DATAMAN_H_
#define DATAMAN_H_

#include<string>
#include<iostream>
#include<cstdint> //uintX_t
#include<vector>
#include<memory> //shared_ptr
#include<functional> //std::function
#include<dlfcn.h>
#include"json.hpp"

using json = nlohmann::json;
using namespace std;

class DataMan{
    public:
        DataMan(){}
        virtual ~DataMan(){}
        int put(const void *p_data,
                string p_doid,
                string p_var,
                string p_dtype,
                vector<size_t> p_putshape,
                vector<size_t> p_varshape,
                vector<size_t> p_offset,
                size_t p_timestep,
                int p_tolerance=0,
                int p_priority=100
                ){
            json msg;
            msg["doid"] = p_doid;
            msg["var"] = p_var;
            msg["dtype"] = p_dtype;
            msg["putshape"] = p_putshape;
            msg["putbytes"] = product(p_putshape, dsize(p_dtype));
            msg["varshape"] = p_varshape;
            msg["varbytes"] = product(p_varshape, dsize(p_dtype));
            msg["offset"] = p_offset;
            msg["timestep"] = p_timestep;
            msg["tolerance"] = p_tolerance;
            msg["priority"] = p_priority;
            return put(p_data, msg);
        }

        virtual int put(const void *p_data, json p_jmsg) = 0;

        int get(void *p_data,
                string p_doid,
                string p_var,
                string p_dtype,
                vector<size_t> p_getshape,
                vector<size_t> p_varshape,
                vector<size_t> p_offset,
                size_t p_timestep
                ){
            json msg;
            msg["doid"] = p_doid;
            msg["var"] = p_var;
            msg["dtype"] = p_dtype;
            msg["getshape"] = p_getshape;
            msg["varshape"] = p_varshape;
            msg["offset"] = p_offset;
            msg["timestep"] = p_timestep;
            return get(p_data, msg);
        }

        int get(void *p_data,
                string p_doid,
                string p_var,
                string &p_dtype,
                vector<size_t> &p_varshape,
                size_t &p_timestep
                ){
            json msg;
            msg["doid"] = p_doid;
            msg["var"] = p_var;
            return get(p_data, msg);
        }

        virtual int get(void *p_data, json &p_jmsg) = 0;
        virtual int init(json p_jmsg) = 0;
        virtual void flush() = 0;
        virtual string name() = 0;
        void reg_callback( std::function<void( const void*, string, string, string, vector<size_t> )> cb ){
            if(m_next.size()==0){
                get_callback = cb;
            }
            else{
                for (int i=0; i<m_next.size(); i++){
                    m_next[i]->reg_callback(cb);
                }
            }
        }

        void dump(const void *p_data, json p_jmsg){
            vector<size_t> p_varshape = p_jmsg["varshape"].get<vector<size_t>>();
            string dtype = p_jmsg["dtype"];
            size_t length = p_jmsg["dumplength"].get<size_t>();
            size_t s=0;
            for (size_t i=0; i<product(p_varshape,1); i++){
                s++;
                cout << ((float*)p_data)[i] << " ";
                if(s == length){
                    cout << endl;
                    s=0;
                }
            }
            cout << endl;
        }

        void add_next(shared_ptr<DataMan> p_next){
            m_next.push_back(p_next);
        }

    protected:

        inline void logging(string p_msg, string p_man = ""){
            if(p_man=="") p_man = name();
            cout << "[";
            cout << p_man;
            cout << "]";
            cout << " ";
            cout << p_msg;
            cout << endl;
        }

        inline bool check_json(json p_jmsg, vector<string> p_strings, string p_man){
            for (auto i : p_strings){
                if (p_jmsg[i] == nullptr){
                    if(p_man != ""){
                        logging("DataMan", "JSON key " + i + " not found! Used in " + p_man);
                    }
                    return false;
                }
            }
            return true;
        }

        virtual int flush_next(){
            for(size_t i=0; i<m_next.size(); i++){
                m_next[i]->flush();
            }
            return 0;
        }

        virtual int put_next(const void *p_data, json p_jmsg){
            for(size_t i=0; i<m_next.size(); i++){
                m_next[i]->put(p_data, p_jmsg);
            }
            return 0;
        }

        inline size_t product(size_t *shape){
            size_t s = 1;
            if(shape){
                for (size_t i=1; i<=shape[0]; i++){
                    s *= shape[i];
                }
            }
            return s;
        }

        inline size_t product(vector<size_t> shape, size_t size=1){
            return accumulate(shape.begin(), shape.end(), size, multiplies<size_t>());
        }

        inline uint8_t dsize(string dtype){
            if (dtype == "char")
                return sizeof(char);
            if (dtype == "short")
                return sizeof(short);
            if (dtype == "int")
                return sizeof(int);
            if (dtype == "long")
                return sizeof(long);
            if (dtype == "unsigned char")
                return sizeof(unsigned char);
            if (dtype == "unsigned short")
                return sizeof(unsigned short);
            if (dtype == "unsigned int")
                return sizeof(unsigned int);
            if (dtype == "unsigned long")
                return sizeof(unsigned long);
            if (dtype == "float")
                return sizeof(float);
            if (dtype == "double")
                return sizeof(double);
            if (dtype == "long double")
                return sizeof(long double);
            if (dtype == "complex<float>")
                return sizeof(float)*2;
            if (dtype == "complex<double>")
                return sizeof(double)*2;

            if (dtype == "int8_t")
                return sizeof(int8_t);
            if (dtype == "uint8_t")
                return sizeof(uint8_t);
            if (dtype == "int16_t")
                return sizeof(int16_t);
            if (dtype == "uint16_t")
                return sizeof(uint16_t);
            if (dtype == "int32_t")
                return sizeof(int32_t);
            if (dtype == "uint32_t")
                return sizeof(uint32_t);
            if (dtype == "int64_t")
                return sizeof(int64_t);
            if (dtype == "uint64_t")
                return sizeof(uint64_t);
            return 0;
        }

        inline json atoj(unsigned int *array){
            json j;
            if(array){
                if(array[0] > 0){
                    j = {array[1]};
                    for (unsigned int i=2; i<=array[0]; i++){
                        j.insert(j.end(), array[i]);
                    }
                }
            }
            return j;
        }

        inline string rmquote(string in){
            return in.substr(1,in.length()-2);
        }

        inline bool isin(string a, json j){
            for (unsigned int i=0; i<j.size(); i++){
                if (j[i] == a)
                    return true;
            }
            return false;
        }

        inline int closest(int v, json j, bool up){
            int s=100, k=0, t;
            for (unsigned int i=0; i<j.size(); i++){
                if(up)
                    t = j[i].get<int>() - v;
                else
                    t = v - j[i].get<int>();
                if(t >= 0 && t < s){
                    s = t;
                    k = i;
                }
            }
            return k;
        }

        inline void check_shape(json &p_jmsg){
            vector<size_t> putshape = p_jmsg["putshape"].get<vector<size_t>>();
            vector<size_t> varshape = p_jmsg["varshape"].get<vector<size_t>>();
            vector<size_t> offset = p_jmsg["offset"].get<vector<size_t>>();
            if(putshape.size()>0){
                if(putshape.size()>varshape.size()){
                    varshape.resize(putshape.size());
                    for (size_t i=0; i<putshape.size(); i++){
                        varshape[i] = putshape[i];
                    }
                    p_jmsg["varshape"] = varshape;
                }
                if(putshape.size()>offset.size()){
                    offset.resize(putshape.size());
                    for (size_t i=0; i<putshape.size(); i++){
                        offset[i] = 0;
                    }
                    p_jmsg["offset"] = offset;
                }
            }
        }

        inline shared_ptr<DataMan> get_man(string method){
            string soname = "lib" + method + "man.so";
            void *so = NULL;
            so = dlopen(soname.c_str(), RTLD_NOW);
            if(so){
                shared_ptr<DataMan> (*func)() = NULL;
                func = (shared_ptr<DataMan>(*)()) dlsym(so,"getMan");
                if(func){
                    return func();
                }
                else{
                    logging("getMan() not found in " + soname);
                }
            }
            else{
                logging("Dynamic library " + soname + " not found in LD_LIBRARY_PATH");
            }
            return nullptr;
        }

        std::function<void(const void*, string, string, string, vector<size_t> )> get_callback;
        vector<shared_ptr<DataMan>> m_next;
};

#endif
