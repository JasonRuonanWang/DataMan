#ifndef DATAMAN_H_INCLUDED
#define DATAMAN_H_INCLUDED

#include<string>
#include<iostream>
#include"json.hpp"

using json = nlohmann::json;
using namespace std;

class DataMan{
    public:
        DataMan(){}
        virtual ~DataMan(){}
        virtual int put(void *p_data,
                string p_doid,
                string p_var,
                string p_dtype,
                vector<uint64_t> p_putshape,
                vector<uint64_t> p_varshape,
                vector<uint64_t> p_offset,
                uint64_t p_timestep,
                int p_tolerance,
                int p_priority
                ){
            for(unsigned int i=0; i<m_next.size(); i++){
                m_next[i]->put(
                        p_data,
                        p_doid,
                        p_var,
                        p_dtype,
                        p_putshape,
                        p_varshape,
                        p_offset,
                        p_timestep,
                        p_tolerance,
                        p_priority
                        );
            }
            return 0;
        }

        virtual int get(void *p_data,
                string p_doid,
                string p_var,
                string p_dtype,
                vector<uint64_t> p_getshape,
                vector<uint64_t> p_varshape,
                vector<uint64_t> p_offset,
                uint64_t p_timestep
                ) = 0;

        virtual int get(void *p_data,
                string p_doid,
                string p_var,
                string &p_dtype,
                vector<uint64_t> &p_varshape,
                uint64_t &p_timestep
                ) = 0;

        virtual void flush() = 0;

        void reg_callback(void (*cb)
            (const void *data,
             string doid,
             string var,
             string dtype,
             vector<uint64_t> varshape)
            ){
            get_callback = cb;
        }

        void dump(const void *p_data,
                string p_doid,
                string p_var,
                string p_dtype,
                vector<uint64_t> p_varshape,
                int p_length
                ){
            int s=0;
            for (uint64_t i=0; i<product(p_varshape,1); i++){
                s++;
                cout << ((float*)p_data)[i] << " ";
                if(s == p_length){
                    cout << endl;
                    s=0;
                }
            }
        }

        void add_next(shared_ptr<DataMan> p_next){
            m_next.push_back(p_next);
        }

    protected:
        void (*get_callback)(const void *data,
                string doid,
                string var,
                string dtype,
                vector<uint64_t> varshape) = NULL;

        inline uint64_t product(unsigned int *shape){
            unsigned int s = 1;
            if(shape){
                for (unsigned int i=1; i<=shape[0]; i++){
                    s *= shape[i];
                }
            }
            return s;
        }

        inline uint64_t product(vector<uint64_t> shape, uint64_t size=1){
            return accumulate(shape.begin(), shape.end(), size, multiplies<uint64_t>());
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


        string m_getmode = "callback"; // graph, callback
        vector<shared_ptr<DataMan>> m_next;
};



#endif
