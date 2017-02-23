#include "CompressMan.h"

class ZfpMan : public CompressMan{
    public:
        ZfpMan();
        ~ZfpMan();

        virtual int put(void *p_data,
                string p_doid,
                string p_var,
                string p_dtype,
                vector<uint64_t> p_putshape,
                vector<uint64_t> p_varshape,
                vector<uint64_t> p_offset,
                uint64_t p_timestep,
                int p_tolerance,
                int p_priority);

        virtual int get(void *p_data,
                string p_doid,
                string p_var,
                string p_dtype,
                vector<uint64_t> p_getshape,
                vector<uint64_t> p_varshape,
                vector<uint64_t> p_offset,
                uint64_t p_timestep
                );

        virtual int get(void *p_data,
                string p_doid,
                string p_var,
                string &p_dtype,
                vector<uint64_t> &p_varshape,
                uint64_t &p_timestep
                );

        virtual void flush();
};


extern "C" shared_ptr<DataMan> getMan(){
    return shared_ptr<DataMan>(new ZfpMan);
}





