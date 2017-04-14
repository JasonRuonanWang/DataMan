#ifndef ZMQMAN_H_
#define ZMQMAN_H_

#include"StreamMan.h"

namespace adios
{
namespace realtime
{

class ZmqMan : public StreamMan{
    public:
        ZmqMan();
        ~ZmqMan();

        virtual int init(json p_jmsg);
        virtual int put(const void *p_data, json p_jmsg);
        virtual int get(void *p_data, json &p_jmsg);
        virtual void transform(const void* p_in, void* p_out, json &p_jmsg){};


        virtual void on_recv(json msg);
        string name(){return "ZmqMan";}


    private:
        void *zmq_data = NULL;


};


extern "C" shared_ptr<DataMan> getMan(){
    return make_shared<ZmqMan>();
}


} // end namespace realtime
} // end namespace adios

#endif
