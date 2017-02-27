#ifndef COMPRESSMAN_H_
#define COMPRESSMAN_H_


#include "DataMan.h"

class CompressMan : public DataMan{
    public:
        CompressMan();
        ~CompressMan();
        virtual void* compress(void* p_data, json &p_jmsg) = 0;
        virtual void* decompress(void* p_data, json p_jmsg) = 0;
};



#endif
