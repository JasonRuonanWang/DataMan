#include<string>
#include<iostream>
#include"json/json.hpp"

using json = nlohmann::json;
using namespace std;

class DataMan{
    public:
        DataMan();
        virtual ~DataMan();
        virtual int put(const void *data,
                string doid,
                string var,
                string dtype,
                vector<uint64_t> putshape,
                vector<uint64_t> varshape,
                vector<uint64_t> offset,
                int tolerance,
                int priority) = 0;

        virtual int get(void *data, json j) = 0;
        virtual void get(json j) = 0;

        virtual void flush() = 0;

        void (*get_callback)(void *data,
                string doid,
                string var,
                string dtype,
                vector<uint64_t> putshape,
                vector<uint64_t> varshape,
                vector<uint64_t> offset) = NULL;


    protected:
        inline unsigned int product(unsigned int *shape){
            unsigned int s = 1;
            if(shape){
                for (unsigned int i=1; i<=shape[0]; i++){
                    s *= shape[i];
                }
            }
            return s;
        }

        inline unsigned int dsize(string dtype){
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
            return 0;
        }

        inline json atoj(unsigned int *array){
            json j;
            if(array){
                if(array[0] > 0){
                    j = {array[1]};
                    for (int i=2; i<=array[0]; i++){
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
            for (int i=0; i<j.size(); i++){
                if (j[i] == a)
                    return true;
            }
            return false;
        }

        inline int closest(int v, json j, bool up){
            int s=100, k=0, t;
            for (int i=0; i<j.size(); i++){
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

        void *cache = NULL;
        vector<uint64_t> cache_shape;
        string getmode = "callback"; // graph, callback
        DataMan *next = NULL;

};


