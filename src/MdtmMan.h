#ifndef MDTMMAN_H_
#define MDTMMAN_H_

#include"StreamMan.h"
#include <queue>

class MdtmMan : public StreamMan{
    public:
        MdtmMan();
        ~MdtmMan();

        virtual int init(json p_jmsg);
        virtual int put(const void *p_data, json p_jmsg);
        virtual int get(void *p_data, json &p_jmsg);
        virtual void transform(const void* p_in, void* p_out, json &p_jmsg){};

        void on_recv(json msg);
        string name(){return "MdtmMan";}

    private:
        void *zmq_ipc_req = NULL;
        int zmq_msg_size = 1024;
        string getmode = "callback";
        json pipe_desc;
        vector<int> pipes;
        vector<string> pipenames;
        queue<json> jqueue;
        queue<void*> bqueue;
        queue<int> iqueue;

};

extern "C" shared_ptr<DataMan> getMan(){
    return make_shared<MdtmMan>();
}


#endif



