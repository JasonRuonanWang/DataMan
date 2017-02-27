#ifndef ZFPMAN_H_
#define ZFPMAN_H_

#include "CompressMan.h"

class ZfpMan : public CompressMan{
    public:
        ZfpMan();
        ~ZfpMan();


        virtual int put(const void *p_data, json p_jmsg);

        virtual int get(void *p_data,
                string p_doid,
                string p_var,
                string p_dtype,
                vector<size_t> p_getshape,
                vector<size_t> p_varshape,
                vector<size_t> p_offset,
                size_t p_timestep
                );

        virtual int get(void *p_data,
                string p_doid,
                string p_var,
                string &p_dtype,
                vector<size_t> &p_varshape,
                size_t &p_timestep
                );

        virtual void flush();

        void* compress(void* p_data, json &p_jmsg);
        void* decompress(void* p_data, json p_jmsg);

        string name(){return "ZfpMan";}

};

extern "C" shared_ptr<DataMan> getMan(){
    return shared_ptr<DataMan>(new ZfpMan);
}

#endif
