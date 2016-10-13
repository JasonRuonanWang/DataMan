#include"DataMan.h"
#include<thread>

using namespace std;

class StreamMan : public DataMan{
    public:
        StreamMan(string local_address,
                string remote_address);
        ~StreamMan();
        virtual int put(void *data,
                string doid,
                string var,
                string dtype,
                unsigned int *putshape,
                unsigned int *varshape,
                unsigned int *offset,
                int priority) = 0;
        virtual int get(void *data, json j) = 0;
    protected:
        void *zmq_context = NULL;
        void *zmq_tcp_req = NULL;
        void *zmq_tcp_rep = NULL;
    private:
        void zmq_tcp_rep_thread_func();
        bool zmq_tcp_rep_thread_active;
        thread *zmq_tcp_rep_thread;

};




