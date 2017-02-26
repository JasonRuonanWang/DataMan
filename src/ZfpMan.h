#ifndef ZFPMAN_H_
#define ZFPMAN_H_


#include "CompressMan.h"

class ZfpMan : public CompressMan{
    public:
        ZfpMan();
        ~ZfpMan();

        virtual int put(const void *p_data,
                string p_doid,
                string p_var,
                string p_dtype,
                vector<size_t> p_putshape,
                vector<size_t> p_varshape,
                vector<size_t> p_offset,
                size_t p_timestep,
                int p_tolerance,
                int p_priority);

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

        shared_ptr<char> compress(
                void* p_data,
                vector<size_t> p_shape,
                string p_dtype,
                size_t &p_compressed_size
                );
        shared_ptr<char> decompress(
                void* p_data,
                vector<size_t> p_shape,
                string p_dtype,
                size_t p_compressed_size
                );

        string name(){return "ZfpMan";}

        void set_rate(int p_rate){m_rate = p_rate;}
        void set_precision(int p_precision){m_precision = p_precision;}
        void set_accuracy(int p_accuracy){m_accuracy = p_accuracy;}

    private:
        int m_rate;
        int m_precision;
        int m_accuracy;
};


extern "C" shared_ptr<DataMan> getMan(){
    return shared_ptr<DataMan>(new ZfpMan);
}



#endif
