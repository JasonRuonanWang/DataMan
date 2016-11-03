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
                unsigned int *putshape,
                unsigned int *varshape,
                unsigned int *offset,
                int tolerance,
                int priority) = 0;

        virtual int get(void *data, json j) = 0;

        inline unsigned int product(unsigned int *shape){
            unsigned int s = 0;
            if(shape){
                s = 1;
                for (unsigned int i=1; i<=shape[0]; i++){
                    s *= shape[i];
                }
            }
            return s;
        }

        inline unsigned int dsize(string dtype){
            if (dtype == "float")
                return sizeof(float);
            if (dtype == "int")
                return sizeof(int);
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

};


