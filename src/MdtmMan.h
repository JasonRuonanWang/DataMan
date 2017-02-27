#ifndef MDTMMAN_H_
#define MDTMMAN_H_

#include"StreamMan.h"
#include <queue>

class MdtmMan : public StreamMan{
    public:
        MdtmMan();
        ~MdtmMan();
        MdtmMan(string local_ip,
                string remote_ip,
                string mode,
                string prefix,
                int num_pipes,
                vector<int> tolerance,
                vector<int> priority);

        virtual int put(const void *p_data, json p_jmsg);
        virtual int get(void *p_data, json &p_jmsg);

        void on_recv(json j);
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
    return shared_ptr<DataMan>(new MdtmMan);
}


#endif
