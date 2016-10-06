#include"DataMan.h"
#include<thread>

using namespace std;

class StreamMan : public DataMan{
    public:
        StreamMan(string local_ip, string remote_ip);
        ~StreamMan();
        virtual int put(void *data,
                string doid,
                string var,
                string dtype,
                unsigned int *putshape,
                unsigned int *varshape,
                unsigned int *offset) = 0;
    protected:
        void *zmq_context = NULL;
    private:
        void *zmq_tcp_req = NULL;
        void *zmq_tcp_rep = NULL;
        void zmq_tcp_rep_thread_func();
        bool zmq_tcp_rep_thread_active;
        thread *zmq_tcp_rep_thread;

};




