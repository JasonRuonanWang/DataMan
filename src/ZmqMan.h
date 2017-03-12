#ifndef ZMQMAN_H_
#define ZMQMAN_H_

#include"StreamMan.h"

class ZmqMan : public StreamMan{
    public:
        ZmqMan();
        ~ZmqMan();

        virtual int init(json p_jmsg);
        virtual int put(const void *p_data, json p_jmsg, int p_flag);
        virtual int get(void *p_data, json &p_jmsg);


        virtual void on_recv(json j);
        string name(){return "ZmqMan";}


    private:
        void *zmq_data = NULL;


};


extern "C" shared_ptr<DataMan> getMan(){
    return shared_ptr<DataMan>(new ZmqMan);
}



#endif
