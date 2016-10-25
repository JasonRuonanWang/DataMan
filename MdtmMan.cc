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
    pipe_desc["operation"] = "init";
    pipe_desc["mode"] = mode;
    pipe_desc["pipe_prefix"] = prefix;

    string pipename_prefix = "MdtmManPipe";
    for(int i=0; i<num_pipes; i++){
        stringstream pipename;
        pipename << pipename_prefix << i;
        if(i==0){
            pipe_desc["pipe_names"] = {pipename.str()};
            pipe_desc["loss_tolerance"] = {tolerance[i]};
            pipe_desc["priority"] = {priority[i]};
        }
        else{
            pipe_desc["pipe_names"].insert(pipe_desc["pipe_names"].end(), pipename.str());
            pipe_desc["loss_tolerance"].insert(pipe_desc["loss_tolerance"].end(), tolerance[i]);
            pipe_desc["priority"].insert(pipe_desc["priority"].end(), priority[i]);
        }
    }

    // Pipes
    for (int i=0; i<pipe_desc["pipe_names"].size(); i++){
        string filename = prefix + rmquote(pipe_desc["pipe_names"][i].dump());
        mkfifo(filename.c_str(), 0666);
    }

    // ZMQ_DataMan_MDTM

    zmq_ipc_req = zmq_socket (zmq_context, ZMQ_REQ);
    zmq_ipc_rep = zmq_socket (zmq_context, ZMQ_REP);
    zmq_connect (zmq_ipc_req, "ipc:///tmp/ADIOS_MDTM_pipe");
    zmq_bind (zmq_ipc_rep, "ipc:///tmp/MDTM_ADIOS_pipe");

    char buffer_return[10];
    zmq_send (zmq_ipc_req, pipe_desc.dump().c_str(), pipe_desc.dump().length(), 0);
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
        int tolerance,
        int priority)
{
    json msg;
    msg["doid"] = doid;
    msg["var"] = var;
    msg["putshape"] = atoj(putshape);
    msg["dtype"] = dtype;

    int index = closest(priority, pipe_desc["priority"], true);
    msg["pipe"] = pipe_desc["pipe_names"][index];

    unsigned int putsize = product(putshape) * dsize(dtype);
    msg["putsize"] = product(putshape) * dsize(dtype);

    cout << msg << endl;

    char ret[10];
    zmq_send(zmq_tcp_req, msg.dump().c_str(), msg.dump().length(), 0);
    zmq_recv(zmq_tcp_req, ret, 10, 0);

    FILE *fp = fopen(msg["pipe"].dump().c_str(), "wb");
    fwrite(data, 1, putsize, fp);
    fclose(fp);

    return 0;
}

void* MdtmMan::get(json j){
}

int MdtmMan::get(void *data, json j){
    int size = j["putsize"].get<int>();
    FILE *f = fopen(j["pipe"].dump().c_str(), "rb");
    fread(data, 1, size, f);
    fclose(f);
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


