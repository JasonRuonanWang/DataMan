#include"StreamMan.h"

class ZmqMan : public StreamMan{
    public:
        ZmqMan();
        ~ZmqMan();
        ZmqMan(string local_ip,
                string remote_ip,
                string mode,
                string prefix,
                int num_pipes,
                int *tolerance,
                int *priority);

        virtual int put(void *data,
                string doid,
                string var,
                string dtype,
                vector<unsigned long> putshape,
                vector<unsigned long> varshape,
                vector<unsigned long> offset,
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

};







