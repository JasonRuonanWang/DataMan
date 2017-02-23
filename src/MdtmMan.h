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

        int put(void *data,
                string doid,
                string var,
                string dtype,
                vector<uint64_t> putshape,
                vector<uint64_t> varshape,
                vector<uint64_t> offset,
                uint64_t timestep,
                int tolerance,
                int priority);

        int get(void *data,
                string doid,
                string var,
                string dtype,
                vector<uint64_t> getshape,
                vector<uint64_t> varshape,
                vector<uint64_t> offset,
                uint64_t timestep
               );

        int get(void *data,
                string doid,
                string var,
                string &dtype,
                vector<uint64_t> &varshape,
                uint64_t &timestep);

        void on_recv(json j);

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


