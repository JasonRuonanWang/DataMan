#ifndef DUMPMAN_H_
#define DUMPMAN_H_


#include "DataMan.h"

class DumpMan : public DataMan{
    public:
        DumpMan();
        ~DumpMan();

        virtual int put(const void *p_data, json p_jmsg);

        int get(void *data,
                string doid,
                string var,
                string dtype,
                vector<size_t> getshape,
                vector<size_t> varshape,
                vector<size_t> offset,
                size_t timestep
               );

        int get(void *data,
                string doid,
                string var,
                string &dtype,
                vector<size_t> &varshape,
                size_t &timestep);

        void flush();
        string name(){return "DumpMan";}
};

extern "C" shared_ptr<DataMan> getMan(){
    return shared_ptr<DumpMan>(new DumpMan);
}



#endif
