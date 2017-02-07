#include "DataMan.h"
#include "CacheMan.h"
#include <thread>

using namespace std;

class StreamMan : public DataMan{
    public:
        StreamMan();
        StreamMan(string local_address,
                string remote_address);
        ~StreamMan();
        virtual int put(const void *data,
                string doid,
                string var,
                string dtype,
                vector<uint64_t> putshape,
                vector<uint64_t> varshape,
                vector<uint64_t> offset,
                uint64_t timestep,
                int tolerance,
                int priority) = 0;
        virtual void on_recv(json j) = 0;
        void flush();
    protected:
        void *zmq_context = NULL;
        void *zmq_tcp_req = NULL;
        void *zmq_tcp_rep = NULL;
        CacheMan m_cache;
    private:
        void zmq_tcp_rep_thread_func();
        bool zmq_tcp_rep_thread_active;
        thread *zmq_tcp_rep_thread=NULL;
};


