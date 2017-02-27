#ifndef COMPRESSMAN_H_
#define COMPRESSMAN_H_


#include "DataMan.h"

class CompressMan : public DataMan{
    public:
        CompressMan();
        ~CompressMan();
        virtual void* compress(
                void* p_data,
                vector<size_t> p_shape,
                string p_dtype,
                size_t &p_compressed_size
                )=0;
        virtual void* decompress(
                void* p_data,
                vector<size_t> p_shape,
                string p_dtype,
                size_t p_compressed_size
                )=0;

};



#endif
