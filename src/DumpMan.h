#ifndef DUMPMAN_H_
#define DUMPMAN_H_


#include "DataMan.h"

class DumpMan : public DataMan{
    public:
        DumpMan();
        ~DumpMan();

        int put(void *data,
                string doid,
                string var,
                string dtype,
                vector<uint64_t> putshape,
                vector<uint64_t> varshape,
                vector<uint64_t> offset,
                uint64_t timestep,
                int tolerance,
                int priority
                );

        int get(void *data,
                string doid,
                string var,
                string dtype,
                vector<uint64_t> getshape,
                vector<uint64_t> varshape,
                vector<uint64_t> offset,
                uint64_t timestep
               );

        int get(void *data,
                string doid,
                string var,
                string &dtype,
                vector<uint64_t> &varshape,
                uint64_t &timestep);

        void flush();
};

extern "C" shared_ptr<DataMan> getMan(){
    return shared_ptr<DumpMan>(new DumpMan);
}



#endif
