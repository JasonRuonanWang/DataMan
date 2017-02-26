#ifndef COMPRESSMAN_H_
#define COMPRESSMAN_H_


#include "DataMan.h"

class CompressMan : public DataMan{
    public:
        CompressMan();
        ~CompressMan();
        virtual shared_ptr<char> compress(
                void* p_data,
                vector<size_t> p_shape,
                string p_dtype,
                size_t &p_compressed_size
                )=0;
        virtual shared_ptr<char> decompress(
                void* p_data,
                vector<size_t> p_shape,
                string p_dtype,
                size_t p_compressed_size
                )=0;

};



#endif
