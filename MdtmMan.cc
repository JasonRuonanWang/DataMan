#include"MdtmMan.h"
#include"zmq.h"
//#include <sys/types.h>
#include <sys/stat.h>

MdtmMan::MdtmMan(string local_address,
        string remote_address,
        string mode,
        string prefix,
        int num_pipes,
        int *tolerance,
        int *priority)
    :StreamMan(local_address, remote_address)
{
    json msg;
    msg["operation"] = "init";
    msg["mode"] = mode;
    msg["pipe_prefix"] = prefix;
    msg["pipe_names"] = {"red", "yellow", "green"};
    msg["loss_tolerance"] = {0,0,1};
    msg["priority"] = {0,3,10};

    // Pipes
    for (int i=0; i<msg["pipe_names"].size(); i++){
        string filename = prefix + rmquote(msg["pipe_names"][i].dump());
        mkfifo(filename.c_str(),'w');
    }


    // ZMQ
    zmq_ipc_req = zmq_socket (zmq_context, ZMQ_REQ);
    zmq_ipc_rep = zmq_socket (zmq_context, ZMQ_REP);
    zmq_connect (zmq_ipc_req, "ipc:///tmp/ADIOS_MDTM_pipe");
    zmq_bind (zmq_ipc_rep, "ipc:///tmp/MDTM_ADIOS_pipe");

    char buffer_return[10];
    zmq_send (zmq_ipc_req, msg.dump().c_str(), msg.dump().length(), 0);
    zmq_recv (zmq_ipc_req, buffer_return, 10, 0);

    zmq_ipc_rep_thread_active = true;
    zmq_ipc_rep_thread = new thread(&MdtmMan::zmq_ipc_rep_thread_func, this);
}

MdtmMan::~MdtmMan(){
    zmq_close (zmq_ipc_rep);
    zmq_close (zmq_ipc_req);
    zmq_ipc_rep_thread_active = false;
    zmq_ipc_rep_thread->join();
    delete zmq_ipc_rep_thread;
}

int MdtmMan::put(void *data,
        string doid,
        string var,
        string dtype,
        unsigned int *putshape = 0,
        unsigned int *varshape = 0,
        unsigned int *offset = 0)
{

    json msg;
    msg["doid"] = doid;
    msg["var"] = var;
    msg["putsize"] = product(putshape) * dsize(dtype);
    msg["putshape"] = atoj(putshape);
    msg["dtype"] = dtype;

    cout << msg << endl;

    return 0;
}

void MdtmMan::zmq_ipc_rep_thread_func(){
    while (zmq_ipc_rep_thread_active){
        char msg[1024];
        zmq_recv (zmq_ipc_rep, msg, 1024, ZMQ_NOBLOCK);
        zmq_send (zmq_ipc_rep, "OK", 10, 0);
    }
}


