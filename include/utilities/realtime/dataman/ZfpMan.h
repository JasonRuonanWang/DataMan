#ifndef ZFPMAN_H_
#define ZFPMAN_H_

#include "CompressMan.h"

namespace adios
{
namespace realtime
{

class ZfpMan : public CompressMan{
    public:
        ZfpMan();
        ~ZfpMan();
        virtual int init(json p_jmsg);
        virtual int put(const void *p_data, json p_jmsg);
        virtual int get(void *p_data, json &p_jmsg);
        virtual void flush();
        void* compress(void* p_data, json &p_jmsg);
        void* decompress(void* p_data, json p_jmsg);
        virtual void transform(const void* p_in, void* p_out, json &p_jmsg);
        string name(){return "ZfpMan";}
};

extern "C" shared_ptr<DataManBase> getMan(){
    return make_shared<ZfpMan>();
}

} // end namespace realtime
} // end namespace adios

#endif
