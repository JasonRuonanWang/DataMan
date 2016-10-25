#include <sys/stat.h>
#include <unistd.h>
#include "ZmqMan.h"
#include "zmq.h"

ZmqMan::ZmqMan(string local_address,
        string remote_address,
        string mode,
        string prefix,
        int num_pipes,
        int *tolerance,
        int *priority)
    :StreamMan(local_address, remote_address)
{

}

ZmqMan::~ZmqMan(){
}

int ZmqMan::put(void *data,
        string doid,
        string var,
        string dtype,
        unsigned int *putshape,
        unsigned int *varshape,
        unsigned int *offset,
        int tolerance,
        int priority)
{
    return 0;
}

int ZmqMan::get(void *data, json j){
    return 0;
}

void ZmqMan::zmq_ipc_rep_thread_func(){
}


