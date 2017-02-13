#include "DataMan.h"
#include "CacheMan.h"
#include <thread>

using namespace std;

class StreamMan : public DataMan{
    public:
        StreamMan();

        StreamMan(string local_address, string remote_address);

        ~StreamMan();

        void init(string local_address, string remote_address);

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

        int put(const void *data,
                string doid,
                string var,
                string dtype,
                vector<uint64_t> putshape,
                vector<uint64_t> varshape,
                vector<uint64_t> offset,
                uint64_t timestep,
                int tolerance,
                int priority,
                json msg);

        virtual void on_recv(json j) = 0;

        void flush();

    protected:
        void *zmq_context = NULL;
        CacheMan m_cache;
    private:
        void *zmq_meta_req = NULL;
        void *zmq_meta_rep = NULL;
        void zmq_meta_rep_thread_func();
        bool zmq_meta_rep_thread_active;
        thread *zmq_meta_rep_thread=NULL;
};

