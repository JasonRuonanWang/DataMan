#include"StreamMan.h"

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

        virtual int put(const void *data,
                string doid,
                string var,
                string dtype,
                vector<uint64_t> putshape,
                vector<uint64_t> varshape,
                vector<uint64_t> offset,
                int tolerance,
                int priority);

        virtual int get(void *data, json j);

    private:
        void *zmq_ipc_req = NULL;
        void *zmq_ipc_rep = NULL;
        void zmq_ipc_rep_thread_func();
        bool zmq_ipc_rep_thread_active;
        thread *zmq_ipc_rep_thread;
        int zmq_msg_size = 1024;
        string getmode = "callback";
        json pipe_desc;
        vector<FILE*> pipes;
        vector<string> pipenames;

};







