#include"StreamMan.h"

class MdtmMan : public StreamMan{
    public:
        MdtmMan();
        ~MdtmMan();
        MdtmMan(string local_ip, string remote_ip, string mode, string prefix = "/tmp/", int num_pipes = 4, int *tolerance = NULL, int *priority = NULL);
        virtual int put(void *data,
                string doid,
                string var,
                string dtype,
                unsigned int *putshape,
                unsigned int *varshape,
                unsigned int *offset);


    private:
        void *zmq_ipc_req = NULL;
        void *zmq_ipc_rep = NULL;
        void zmq_ipc_rep_thread_func();
        bool zmq_ipc_rep_thread_active;
        thread *zmq_ipc_rep_thread;
        int zmq_msg_size = 1024;

};







