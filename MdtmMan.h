#include"StreamMan.h"

class MdtmMan : public StreamMan{
    public:
        MdtmMan();
        ~MdtmMan();
        MdtmMan(string local_ip, string remote_ip, string mode, string prefix = "/tmp/", int num_pipes = 4, int *tolerance = NULL, int *priority = NULL);
        virtual int put(void *data, string doid, string var, int *varshape, int *offset, int *putshape);

    private:
        void *zmq_ipc_req = NULL;

};







