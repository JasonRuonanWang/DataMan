#include "DataMan.h"

class DumpMan : public DataMan{
    public:
        DumpMan();
        ~DumpMan();

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
};

std::unique_ptr<DumpMan> getDumpMan(){
    std::unique_ptr<DumpMan> p(new DumpMan);
    return std::move(p);
}


