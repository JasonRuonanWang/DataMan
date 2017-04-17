#ifndef TEMPORALMAN_H_
#define TEMPORALMAN_H_

#include "CompressMan.h"

namespace adios
{
namespace realtime
{

class TemporalMan : public CompressMan{
    public:
        TemporalMan();
        ~TemporalMan();
        virtual int init(json p_jmsg);
        virtual int put(const void *p_data, json p_jmsg);
        virtual int get(void *p_data, json &p_jmsg);
        virtual void flush();
        virtual void transform(const void* p_in, void* p_out, json &p_jmsg);
        string name(){return "TemporalMan";}
};

extern "C" shared_ptr<DataManBase> getMan(){
    return make_shared<TemporalMan>();
}

} // end namespace realtime
} // end namespace adios

#endif


