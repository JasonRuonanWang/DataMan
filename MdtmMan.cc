#include <sys/stat.h>
#include <unistd.h>
#include"MdtmMan.h"
#include"zmq.h"

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

    string pipename_prefix = "MdtmManPipe";
    for(int i=0; i<num_pipes; i++){
        stringstream pipename;
        pipename << pipename_prefix << i;
        if(i==0){
            msg["pipe_names"] = {pipename.str()};
            msg["loss_tolerance"] = {tolerance[i]};
            msg["priority"] = {priority[i]};
        }
        else{
            msg["pipe_names"].insert(msg["pipe_names"].end(), pipename.str());
            msg["loss_tolerance"].insert(msg["loss_tolerance"].end(), tolerance[i]);
            msg["priority"].insert(msg["priority"].end(), priority[i]);
        }
    }

    // Pipes
    for (int i=0; i<msg["pipe_names"].size(); i++){
        string filename = prefix + rmquote(msg["pipe_names"][i].dump());
        mkfifo(filename.c_str(), 0666);
    }

    // ZMQ_DataMan_MDTM

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
        unsigned int *putshape,
        unsigned int *varshape,
        unsigned int *offset,
        int priority)
{
    json msg;
    msg["doid"] = doid;
    msg["var"] = var;
    msg["putshape"] = atoj(putshape);
    msg["dtype"] = dtype;
    msg["pipe"] = "red";
    unsigned int putsize = product(putshape) * dsize(dtype);
    msg["putsize"] = product(putshape) * dsize(dtype);

    cout << msg << endl;

    char ret[10];
    zmq_send(zmq_tcp_req, msg.dump().c_str(), msg.dump().length(), 0);
    zmq_recv(zmq_tcp_req, ret, 10, 0);

    FILE *fp = fopen("/tmp/yellow", "wb");
    fwrite(data, 1, putsize, fp);
    fclose(fp);

    return 0;
}

int MdtmMan::get(void *data, json j){

    return 0;
}

void MdtmMan::zmq_ipc_rep_thread_func(){
    while (zmq_ipc_rep_thread_active){
        char msg[1024];
        zmq_recv (zmq_ipc_rep, msg, 1024, ZMQ_NOBLOCK);
        zmq_send (zmq_ipc_rep, "OK", 10, 0);
        usleep(10000);
    }
}


